#pragma once

namespace Soup::StringUtils
{

  /**
   * Convert a string to the given template type.
   */
  template <typename T>
  T ConvertFromString(const std::string& stringValue) requires(std::is_fundamental<T>::value)
  {
    T retValue;
    std::from_chars(stringValue.data(), stringValue.data() + stringValue.size(), retValue);
    return retValue;
  }

  /**
   * Split the string to several sub strings based on the given separator.
   */
  std::vector<std::string> Split(const std::string& string, const char* separator);

  /**
   * Join all strings in a single string separated by the given separator.
   */
  std::string Join(const std::vector<std::string>& strings, const std::string& separator);

  /**
   * Remove the first and last series of the given characters.
   * For example, Strip("-------string-------") will return "string".
   */
  std::string Strip(const std::string_view stringValue, const char character);

  /**
   * Return a string with all letters capitalized.
   */
  std::string ToUpper(const std::string_view input);

  /**
   * Convert a string vector to a date:
   * - The first element has to be the day number
   * - The second element has to be the month number
   * - The third element has to be the year number
   */
  std::chrono::year_month_day StringToDate(const std::string& dateString, const std::string& separator = "/");
  std::chrono::year_month_day StringToDate(
    const std::string& dateString, bool& success, const std::string& separator = "/");

  std::string DateToString(const std::chrono::year_month_day& date);

  size_t FindNthCharacter(const std::string& data, char token, size_t nth);

  std::string GetSubStringAt(const std::string& data, const char* separator, uint32_t index);

}
