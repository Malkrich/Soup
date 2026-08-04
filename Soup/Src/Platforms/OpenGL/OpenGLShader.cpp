#include "OpenGLShader.h"

#include "OpenGLError.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Soup
{
  namespace Utils
  {

    static std::string ReadFile(const std::filesystem::path& filePath)
    {
      std::ifstream file;
      auto filePathStr = filePath.generic_string();
      file.open(filePathStr.c_str());

      if (!file)
        SP_LOG_ERROR("File named {0} not found !", filePath.generic_string());

      return std::string((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    }

    static ShaderType StringToShaderType(const std::string& token)
    {
      if (token == "vertex")
        return ShaderType::VertexShader;
      else if (token == "fragment")
        return ShaderType::FragmentShader;

      SP_ASSERT(false, "Token not known!");
      return ShaderType::None;
    }

    static GLenum ShaderTypeToGLShaderType(ShaderType shaderType)
    {
      switch (shaderType)
      {
        case ShaderType::VertexShader:
          return GL_VERTEX_SHADER;
        case ShaderType::FragmentShader:
          return GL_FRAGMENT_SHADER;
        default:
          SP_ASSERT(false, "shader type not supported!");
          return 0;
      }
    }

    static std::string PreProcessShaderSource(const std::string& source, const std::filesystem::path& sourceFilePath)
    {
      std::string includeToken = "#include";
      size_t previousPos = 0;
      size_t pos = 0;
      std::string preProcessedSource = "";

      while (pos != std::string::npos)
      {
        pos = source.find(includeToken, previousPos);

        std::string sourceSubsStr = source.substr(previousPos, pos - previousPos);
        preProcessedSource += sourceSubsStr;

        if (pos != std::string::npos)
        {
          size_t firstQuotePos = pos + includeToken.size() + 2;
          size_t lastQuotePos = source.find_first_of("\"", firstQuotePos);
          size_t lineLength = lastQuotePos - pos;

          std::filesystem::path includeFilePath = source.substr(firstQuotePos, lastQuotePos - firstQuotePos);
          std::string includeSource = ReadFile(sourceFilePath.parent_path() / includeFilePath);
          preProcessedSource += includeSource;

          previousPos = lastQuotePos + 1;
        }
      }

      return preProcessedSource;
    }

    static std::unordered_map<ShaderType, std::string> ExtractShaderSources(const std::filesystem::path& shaderFilePath)
    {
      std::string source = ReadFile(shaderFilePath);

      std::unordered_map<ShaderType, std::string> shaderSources;

      const char* typeToken = "#type";
      size_t typeTokenLength = strlen(typeToken);
      size_t pos = source.find(typeToken, 0);

      while (pos != std::string::npos)
      {
        // find the substring of the type
        size_t endLine = source.find_first_of("\r\n", pos);
        SP_ASSERT(endLine != std::string::npos, "Shader syntaxe error!");
        size_t count = endLine - (pos + typeTokenLength + 1);
        std::string type = source.substr(pos + typeTokenLength + 1, count);
        ShaderType shaderType = StringToShaderType(type); // convert to opengl enum type

        // Extract the source code
        pos = endLine + 1;
        size_t endShaderSource = source.find(typeToken, pos);
        size_t shaderSourceLength = endShaderSource - pos;
        std::string subSource = source.substr(pos, shaderSourceLength);
        shaderSources[shaderType] = subSource;

        // Goes to the next token
        pos = source.find(typeToken, shaderSourceLength + 1);
      }

      for (auto& [shaderType, shaderSource] : shaderSources)
      {
        std::string preProcessedSource = PreProcessShaderSource(shaderSource, shaderFilePath);
        shaderSources[shaderType] = preProcessedSource;
      }

      return shaderSources;
    }

  }

  OpenGLShader::OpenGLShader(const std::filesystem::path& filePath)
  {
    Invalidate(filePath);
  }

  OpenGLShader::~OpenGLShader()
  {
    glDeleteProgram(m_ShaderProgramID);
  }

  void OpenGLShader::Bind() const
  {
    glUseProgram(m_ShaderProgramID);
  }

  void OpenGLShader::Unbind() const
  {
    glUseProgram(0);
  }

  void OpenGLShader::SetBool(const std::string& name, bool data)
  {
    uint32_t loc = glGetUniformLocation(m_ShaderProgramID, name.c_str());
    glUniform1i(loc, data);
  }

  void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& data)
  {
    int32_t loc = glGetUniformLocation(m_ShaderProgramID, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(data));
  }

  void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& data)
  {
    int32_t loc = glGetUniformLocation(m_ShaderProgramID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(data));
  }

  void OpenGLShader::Invalidate(const std::filesystem::path& filePath)
  {
    if (m_ShaderProgramID)
    {
      glDeleteProgram(m_ShaderProgramID);
    }

    m_Name = filePath.stem().string();
    auto shaderSources = Utils::ExtractShaderSources(filePath);
    m_ShaderProgramID = CompileAndLink(shaderSources);
  }

  uint32_t OpenGLShader::CompileShader(const std::string& shaderContent, ShaderType shaderType)
  {
    int32_t shaderID;
    GLenum glShaderType = Utils::ShaderTypeToGLShaderType(shaderType);
    shaderID = glCreateShader(glShaderType);

    const char* rawShaderContent = shaderContent.c_str();
    glShaderSource(shaderID, 1, &rawShaderContent, nullptr);

    glCompileShader(shaderID);
#if SP_DEBUG
    CheckGlslCompileError(m_Name, shaderContent, shaderID);
#endif
    CHECK_GL_ERROR();

    return shaderID;
  }

  uint32_t OpenGLShader::CompileAndLink(const std::unordered_map<ShaderType, std::string>& shaderSources)
  {
    // We have only 2 shader possible (vertex shader ad fragment shader)
    std::array<uint32_t, 2> shaderIDs;

    uint32_t programID = 0;
    programID = glCreateProgram();

    uint8_t i = 0;
    for (const auto& [shaderType, shaderSource] : shaderSources)
    {
      uint32_t shaderID = CompileShader(shaderSource, shaderType);
      shaderIDs[i] = shaderID;
      glAttachShader(programID, shaderID);
      i++;
    }

    glLinkProgram(programID);
#if SP_DEBUG
    CheckGlslLinkError(m_Name, programID);
#endif
    CHECK_GL_ERROR();

    for (uint8_t i = 0; i < shaderSources.size(); i++)
    {
      auto shaderID = shaderIDs[i];
      glDeleteShader(shaderID);
    }

    return programID;
  }

}
