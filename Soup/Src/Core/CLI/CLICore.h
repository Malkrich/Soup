#pragma once

namespace Soup::CLICore
{

  /**
   * Split all arguments based on spaces separation and quote for grouping argument values.
   */
  std::vector<std::string> SplitCommand(const std::string& rawString);

}
