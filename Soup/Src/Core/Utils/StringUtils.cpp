#include "StringUtils.h"

namespace Soup::StringUtils
{

  std::vector<std::string> Split(const std::string& string, const char* separator)
  {
    size_t pos = 0;
    size_t nextPos = 0;

    std::vector<std::string> splittedStrings;
    while (nextPos != std::string::npos)
    {
      nextPos = string.find_first_of(separator, pos + 1);

      std::string subString = string.substr(pos, nextPos - pos);
      splittedStrings.push_back(subString);

      pos = nextPos + 1;
    }

    return splittedStrings;
  }

  std::string Join(const std::vector<std::string>& strings, const std::string& separator)
  {
    std::string resultStr;
    for (size_t i = 0; i < strings.size(); i++)
    {
      std::string suffix = i < strings.size() - 1 ? separator : "";
      resultStr += (strings[i] + suffix);
    }
    return resultStr;
  }

  std::string Strip(std::string_view stringValue, const char character)
  {
    auto beginIt = stringValue.begin();
    for (; beginIt != stringValue.end(); beginIt++)
    {
      if (*beginIt != character)
        break;
    }

    auto endIt = stringValue.end() - 1;
    for (; endIt != stringValue.end(); endIt--)
    {
      if (*endIt != character)
        break;
    }

    size_t beginPos = beginIt - stringValue.begin();
    size_t count = (endIt - (stringValue.begin() + beginPos)) + 1;
    return std::string(stringValue.substr(beginPos, count));
  }

  std::string ToUpper(const std::string_view input)
  {
    std::string upperString;
    upperString.resize(input.size());
    size_t i = 0;
    for (const char& character : input)
      upperString[i++] = std::toupper(character);

    return upperString;
  }

  std::chrono::year_month_day StringToDate(const std::string& dateString, const std::string& separator)
  {
    bool success = true;
    std::chrono::year_month_day date = StringToDate(dateString, success, separator);
    SP_ASSERT(success, "Given year/month/day elements do not have the right amount of elements!");
    return date;
  }

  std::chrono::year_month_day StringToDate(const std::string& dateString, bool& success, const std::string& separator)
  {
    std::vector<std::string> dayMonthYear = Split(dateString, separator.c_str());
    if (dayMonthYear.size() != 3)
    {
      success = false;
      return std::chrono::year_month_day();
    }

    uint32_t dayNumber = StringUtils::ConvertFromString<uint32_t>(dayMonthYear[0]);
    uint32_t monthNumber = StringUtils::ConvertFromString<uint32_t>(dayMonthYear[1]);
    uint32_t yearNumber = StringUtils::ConvertFromString<uint32_t>(dayMonthYear[2]);
    std::chrono::day day(dayNumber);
    std::chrono::month month(monthNumber);
    std::chrono::year year(yearNumber);

    success = true;
    return std::chrono::year_month_day(year, month, day);
  }

  std::string DateToString(const std::chrono::year_month_day& date)
  {
    return std::format("{:%d}/{:%m}/{:%Y}", date.day(), date.month(), date.year());
  }

  size_t FindNthCharacter(const std::string& data, char token, size_t nth)
  {
    size_t charPos = 0;
    size_t offset = 0;

    for (uint32_t i = 0; i < nth; i++)
    {
      charPos = data.find_first_of(token, offset);

      if (charPos == std::string::npos)
        break;

      offset = charPos + 1;
    }
    return charPos;
  }

  std::string GetSubStringAt(const std::string& data, const char* separator, uint32_t index)
  {
    size_t subStrBegin = 0;

    for (uint32_t i = 0; i < index; i++)
    {
      size_t firstSep = data.find_first_of(separator, subStrBegin);

      if (firstSep == std::string::npos)
        return "";

      subStrBegin = firstSep + 1;
    }

    size_t subStrSize = data.find_first_of(separator, subStrBegin) - subStrBegin;
    return data.substr(subStrBegin, subStrSize);
  }

}
