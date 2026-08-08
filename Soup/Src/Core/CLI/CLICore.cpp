#include "CLICore.h"

#include "Utils/StringUtils.h"

namespace Soup::CLICore
{

  namespace Utils
  {

    /**
     * Create a string with all arguments separated by a ";"
     */
    static std::string FormatLineForSplitting(const std::string& string)
    {
      std::string result;
      size_t pos = 0;
      size_t nextPos = 0;
      bool quoteOpened = false;

      while (nextPos != std::string::npos)
      {
        nextPos = string.find_first_of(" ", pos + 1);

        // Extract first element before a space
        auto lastPositionIt = nextPos == std::string::npos ? string.end() : string.begin() + nextPos;
        std::string_view word_view(string.begin() + pos, lastPositionIt);
        bool quoteBegin = word_view.starts_with('\"');
        bool quoteEnd = word_view.ends_with('\"');

        size_t cropBegin = quoteBegin ? 1 : 0;
        size_t cropCount = word_view.size() - (quoteEnd ? 1 : 0);
        cropCount -= cropBegin;
        word_view = word_view.substr(cropBegin, cropCount);
        std::string croppedString(word_view);

        if (quoteOpened)
        {
          result += " ";
        }

        quoteOpened |= quoteBegin;
        quoteOpened &= !quoteEnd;

        result += croppedString;

        pos = nextPos + 1;

        if (!quoteOpened && nextPos != std::string::npos)
        {
          result += ";";
        }
      }

      return result;
    }

  }

  std::vector<std::string> SplitCommand(const std::string& rawCommand)
  {
    std::string sanitizedCommand = StringUtils::Strip(rawCommand, ' ');

    std::string formattedArguments = Utils::FormatLineForSplitting(sanitizedCommand);
    std::vector<std::string> splittedArguments = StringUtils::Split(formattedArguments, ";");
    return splittedArguments;
  }

}
