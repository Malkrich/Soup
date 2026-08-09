#include "CLICommands.h"

#include "CLICore.h"

namespace Soup
{

  bool CLICommands::Parse(const std::string& rawCommand)
  {
    m_ParseSucess = true;
    m_ErrorMessage.clear();

    std::vector<std::string> arguments = CLICore::SplitCommand(rawCommand);
    if (arguments.empty())
    {
      m_ParseSucess = false;
      std::string message = std::format("Fatal: invalid command format for command {}", rawCommand);
      AddErrorMessage(message);
      return m_ParseSucess;
    }

    m_CurrentCommand = arguments[0];
    if (!m_Commands.contains(m_CurrentCommand))
    {
      std::string message = std::format("Unsupported command: {}", m_CurrentCommand);
      AddErrorMessage(message);
      m_ParseSucess = false;
      return m_ParseSucess;
    }

    CLIArguments& argumentParser = m_Arguments.at(m_CurrentCommand);
    if (!argumentParser.Parse(arguments))
    {
      m_ParseSucess = false;
      const std::string& message = argumentParser.GetErrorMessage();
      AddErrorMessage(message);
      return m_ParseSucess;
    }

    return m_ParseSucess;
  }

  CLICommandsRange CLICommands::GetCommandNames()
  {
    return CLICommandsRange(m_Commands);
  }

  void CLICommands::InvokeCallback() const
  {
    if (!m_ParseSucess)
    {
      return;
    }

    const CLIArguments& arguments = m_Arguments.at(m_CurrentCommand);
    m_Commands.at(m_CurrentCommand).CommandCallback(arguments);
  }

  std::string CLICommands::GetHelp(const std::string& commandName) const
  {
    if (!m_Commands.contains(commandName))
      return "";

    return m_Commands.at(commandName).Help;
  }

  const CLIArguments& CLICommands::GetArguments(const std::string& commandName) const
  {
    SP_VERIFY(m_Arguments.contains(commandName), "Command name {} does not have CLIArguments class", commandName);
    return m_Arguments.at(commandName);
  }

}
