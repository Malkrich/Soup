#pragma once

#include "CLIArguments.h"

namespace Soup
{

  class CommandArguments;
  class CLICommandsRange;

  class CLICommands
  {
  public:
    typedef std::function<void(const CLIArguments&)> CommandCallbackFunction;

  private:
    struct CLICommandData
    {
      bool HasArguments = false;
      CommandCallbackFunction CommandCallback;
      std::string Help = "No help available for this command.";
    };

  public:
    CLICommands() = default;

    /**
     * Parse the given command line string.
     * Return true if the parse succeeded, otherwise it returns false.
     */
    bool Parse(const std::string& rawCommand);

    /**
     * Return the error message in case the command parsing failed.
     */
    const std::string& GetErrorMessage() const { return m_ErrorMessage; }

    ///@{
    /**
     * Add command with associated enum.
     */
    void AddCommand(const std::string& commandName, CommandCallbackFunction callback)
    {
      AddCommand(commandName, callback, {}, "");
    }
    void AddCommand(const std::string& commandName, CommandCallbackFunction callback, const std::string& help)
    {
      AddCommand(commandName, callback, {}, help);
    }
    void AddCommand(const std::string& commandName, CommandCallbackFunction callback,
      const std::vector<CLIArgumentSpecs>& argumentSpecs)
    {
      AddCommand(commandName, callback, argumentSpecs, "");
    }
    void AddCommand(const std::string& commandName, CommandCallbackFunction callback,
      const std::vector<CLIArgumentSpecs>& argumentSpecs, const std::string& help)
    {
      AddCommandData(commandName, !argumentSpecs.empty(), callback, help);
      m_Arguments[commandName] = CLIArguments(argumentSpecs);
    }
    ///@}

    /**
     * Return registered commands.
     */
    CLICommandsRange GetCommandNames();

    /**
     * Call the callback function associated to the currently parsed command.
     */
    void InvokeCallback() const;

    /**
     * Return the help text associated to the given command
     * If the command is not found, return an empty string.
     */
    std::string GetHelp(const std::string& commandName) const;

    /**
     * Return the arguments associated to the given command.
     */
    const CLIArguments& GetArguments(const std::string& commandName) const;

    /**
     * Return wether or not the given command names has arguments associated.
     * If the command name does not exist, it returns false.
     */
    bool HasArguments(const std::string& commandName) const
    {
      if (!m_Commands.contains(commandName))
        return false;
      return m_Commands.at(commandName).HasArguments;
    }

  private:
    /**
     * Fill command data for given name.
     */
    void AddCommandData(
      const std::string& commandName, bool hasArguments, CommandCallbackFunction callback, const std::string& help)
    {
      CLICommandData& commandData = m_Commands[commandName];
      commandData.HasArguments = hasArguments;
      commandData.CommandCallback = callback;
      commandData.Help = help;
    }

  private:
    void AddErrorMessage(const std::string& error) { m_ErrorMessage += error + "\n"; }

  private:
    bool m_ParseSucess = false;
    std::string m_CurrentCommand;
    std::string m_ErrorMessage;

    std::map<std::string, CLICommandData> m_Commands;
    std::map<std::string, CLIArguments> m_Arguments;

    friend class CLICommandsRange;
    friend class CLICommandsIterator;
  };

  class CLICommandsIterator
  {
  public:
    using ValueType = std::string;

  private:
    using InternalIteratorType = std::map<std::string, CLICommands::CLICommandData>::iterator;

  public:
    const ValueType& operator*() const { return m_It->first; }
    const ValueType* operator->() const { return &(m_It->first); }

    // Prefix increment
    CLICommandsIterator& operator++()
    {
      m_It++;
      return *this;
    }

    // Postfix increment
    CLICommandsIterator operator++(int)
    {
      CLICommandsIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const CLICommandsIterator& a, const CLICommandsIterator& b) { return a.m_It == b.m_It; }
    friend bool operator!=(const CLICommandsIterator& a, const CLICommandsIterator& b) { return a.m_It != b.m_It; }

  private:
    CLICommandsIterator(const InternalIteratorType& it)
      : m_It(it)
    {
    }

  private:
    InternalIteratorType m_It;

    friend class CLICommandsRange;
  };

  class CLICommandsRange
  {
  public:
    CLICommandsRange(std::map<std::string, CLICommands::CLICommandData>& commands)
      : m_Commands(commands)
    {
    }

    CLICommandsIterator begin() { return CLICommandsIterator(m_Commands.begin()); }
    CLICommandsIterator end() { return CLICommandsIterator(m_Commands.end()); }

  private:
    std::map<std::string, CLICommands::CLICommandData>& m_Commands;
  };

}
