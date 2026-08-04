#pragma once

namespace Soup::StringUtils
{

  size_t FindNthCharacter(const std::string& data, char token, size_t nth);

  std::string GetSubStringAt(const std::string& data, const char* separator, uint32_t index);

}
