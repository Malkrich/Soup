#include "CLIArguments.h"

#include "Utils/StringUtils.h"

namespace Soup
{

  namespace Utils
  {

    /**
     * Pre process positional arguments by sorting them by position.
     */

    static std::vector<std::string> PreProcessPositionalArgs(const std::vector<std::string>& arguments)
    {
      std::vector<std::string> positionalArguments;
      // Skipping first element as it is the command name
      for (size_t i = 1; i < arguments.size(); i++)
      {
        const std::string& rawValue = arguments[i];
        if (rawValue.starts_with("-"))
        {
          break;
        }

        positionalArguments.push_back(rawValue);
      }

      return positionalArguments;
    }

    /**
     * Pre process flagged arguments by sorting them by flag tokens.
     * @param startPosition position of the first element to be read by the function.
     */
    static std::map<std::string, std::vector<std::string>> PreProcessFlaggedArgs(
      const std::vector<std::string>& arguments, size_t startPosition)
    {
      if (arguments.size() <= startPosition)
      {
        return {};
      }

      SP_ASSERT(arguments[startPosition].starts_with("-"), "Exepected first argument to be a flag!");
      std::map<std::string, std::vector<std::string>> argumentValues;

      // Parse argument values
      std::string currentFlag = "";
      for (size_t i = startPosition; i < arguments.size(); i++)
      {
        const std::string& rawValue = arguments[i];

        if (rawValue.starts_with("-"))
        {
          currentFlag = rawValue;
          argumentValues[currentFlag] = {};
          continue;
        }

        argumentValues[currentFlag].push_back(rawValue);
      }

      return argumentValues;
    }

  }

  CLIArguments::CLIArguments(const std::vector<CLIArgumentSpecs>& argumentSpecs)
  {
    for (const auto& spec : argumentSpecs)
    {
      if (spec.Flag.has_value())
      {
        m_FlaggedArgumentSpecs[spec.Name] = spec;
        m_FlagToArgNameMap[spec.Flag.value()] = spec.Name;
      }
      else
      {
        m_PositionalArgumentSpecs.push_back(spec);
      }
    }
  }

  bool CLIArguments::Parse(const std::vector<std::string>& arguments)
  {
    ClearParsedValues();

    std::vector<std::string> positionalArgRawValues = Utils::PreProcessPositionalArgs(arguments);
    std::map<std::string, std::vector<std::string>> flaggedArgumentMap =
      Utils::PreProcessFlaggedArgs(arguments, positionalArgRawValues.size() + 1);

    bool argumentValidationSuccess = true;
    // Parse positional args
    {
      if (positionalArgRawValues.size() != m_PositionalArgumentSpecs.size())
      {
        std::string message = std::format("Wrong amount of argument, expected {} but got {}.",
          m_PositionalArgumentSpecs.size(), positionalArgRawValues.size());
        AddErrorMessage(message);
        argumentValidationSuccess = false;
      }
      else
      {
        uint32_t position = 0;
        for (const std::string& rawValue : positionalArgRawValues)
        {
          argumentValidationSuccess &= ParsePositionalArgValues(position, rawValue);
        }
      }
    }

    // Parse flagged arguments
    {
      for (const auto& [argumentFlag, argumentValues] : flaggedArgumentMap)
      {
        argumentValidationSuccess &= ParseFlaggedArgValues(argumentFlag, argumentValues);
      }
    }

    // Verify that all required flagged argument are present
    for (const auto& [name, specs] : m_FlaggedArgumentSpecs)
    {
      if (m_FlaggedArgumentValues.contains(specs.Name))
        continue;

      if (!specs.Required || specs.FlagType == ArgumentFlagType::Logical)
        continue;

      std::string message = std::format("Argument {} (flag: {}) is required!", specs.Name, specs.Flag.value());
      AddErrorMessage(message);
      argumentValidationSuccess = false;
    }

    return argumentValidationSuccess;
  }

  CLIPositionalArgRange CLIArguments::GetArgumentPositions() const
  {
    return CLIPositionalArgRange(m_PositionalArgumentSpecs);
  }

  CLIFlaggedArgRange CLIArguments::GetArgumentFlagsNames() const
  {
    return CLIFlaggedArgRange(m_FlaggedArgumentSpecs);
  }

  std::string CLIArguments::GetHelp(uint32_t position) const
  {
    if (position < m_PositionalArgumentSpecs.size())
      return m_PositionalArgumentSpecs[position].Help;

    return "";
  }

  std::string CLIArguments::GetName(uint32_t position) const
  {
    if (position < m_PositionalArgumentSpecs.size())
      return m_PositionalArgumentSpecs[position].Name;

    return "";
  }

  std::string CLIArguments::GetHelp(const std::string& flag) const
  {
    SP_VERIFY(m_FlagToArgNameMap.contains(flag), "Flag named {} not found!", flag);
    const std::string& argName = m_FlagToArgNameMap.at(flag);
    return m_FlaggedArgumentSpecs.at(argName).Help;
  }

  std::string CLIArguments::GetName(const std::string& flag) const
  {
    SP_VERIFY(m_FlagToArgNameMap.contains(flag), "Flag named {} not found!", flag);
    return m_FlagToArgNameMap.at(flag);
  }

  bool CLIArguments::IsRequired(const std::string& flag) const
  {
    if (!m_FlagToArgNameMap.contains(flag))
      return false;

    const std::string& argName = m_FlagToArgNameMap.at(flag);
    const CLIArgumentSpecs& specs = m_FlaggedArgumentSpecs.at(argName);

    return specs.Required && (specs.FlagType != ArgumentFlagType::Logical);
  }

  void CLIArguments::ClearParsedValues()
  {
    m_PositionalArgumentValues.clear();
    m_FlaggedArgumentValues.clear();
    m_LogicalFlags.clear();

    m_ErrorMessage.clear();
  }

  bool CLIArguments::ParsePositionalArgValues(uint32_t argumentPosition, const std::string& rawValue)
  {
    if (argumentPosition < m_PositionalArgumentSpecs.size())
    {
      CLIArgumentSpecs& specs = m_PositionalArgumentSpecs[argumentPosition];
      ArgumentValue& argValue = m_PositionalArgumentValues.emplace_back();
      return ParseArgumentValue(argValue, rawValue, specs);
    }

    return false;
  }

  bool CLIArguments::ParseFlaggedArgValues(const std::string& flag, const std::vector<std::string>& rawValues)
  {
    auto it = m_FlagToArgNameMap.find(flag);
    if (it == m_FlagToArgNameMap.end())
    {
      std::string message = std::format("Unknown flag: {}", flag);
      AddErrorMessage(message);
      return false;
    }

    const CLIArgumentSpecs& specs = m_FlaggedArgumentSpecs.at(it->second);
    if (specs.FlagType == ArgumentFlagType::Logical)
    {
      if (!rawValues.empty())
      {
        std::string message = std::format("Found argument values attached to logical flag {}", flag);
        AddErrorMessage(message);
        return false;
      }

      m_LogicalFlags.insert(flag);
      return true;
    }

    bool parseSuccess = true;
    for (const std::string& rawValue : rawValues)
    {
      ArgumentValue& argValue = m_FlaggedArgumentValues[specs.Name].emplace_back();
      parseSuccess &= ParseArgumentValue(argValue, rawValue, specs);
    }

    return parseSuccess;
  }

  bool CLIArguments::ParseArgumentValue(
    ArgumentValue& value, const std::string& rawValue, const CLIArgumentSpecs& specs)
  {
    switch (specs.Type)
    {
      case ArgumentValueType::String:
        value = rawValue;
        return true;
      case ArgumentValueType::Date:
      {
        bool conversionSuccess = true;
        value = StringUtils::StringToDate(rawValue, conversionSuccess);
        if (!conversionSuccess)
        {
          std::string message = std::format("Could not convert {} to a date.", rawValue);
          AddErrorMessage(message);
        }
        return conversionSuccess;
      }
      default:
        SP_ASSERT(false, "Unsupported arguments type!");
        return false;
    }
  }

  void CLIArguments::AddErrorMessage(const std::string& message)
  {
    m_ErrorMessage += message + "\n";
  }

}
