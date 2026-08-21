#include "FileUtils.h"

namespace Soup::FileUtils
{

  std::filesystem::path FormatFilePath(const std::string& filePath, SeparatorType separator)
  {
    std::regex separatorRegex;
    switch (separator)
    {
      case SeparatorType::BackSlash:
        separatorRegex = "/";
        break;
      case SeparatorType::ForwardSlash:
        separatorRegex = "\\\\";
        break;
      default:
        SP_ASSERT(false, "Unsupported separator type!");
    }

    std::string result = std::regex_replace(filePath.c_str(), separatorRegex, "/");
    return result;
  }

}
