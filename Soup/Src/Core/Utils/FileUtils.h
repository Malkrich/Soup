#pragma once

namespace Soup::FileUtils
{

  enum class SeparatorType
  {
    ForwardSlash,
    BackSlash
  };

  /**
   * Format the file path separating the folders with given separator type.
   */
  std::filesystem::path FormatFilePath(const std::string& filePath, SeparatorType separator);

}
