#pragma once

namespace Soup
{

  class CLIPositionalArgRange;
  class CLIFlaggedArgRange;

  /**
   * Type associated to the argument value.
   */
  enum class ArgumentValueType
  {
    Unknown = 0,
    String,
    Date,
  };

  /**
   * Type of flag behavior for parsing.
   * - Array: contains an array of values
   * - Logical: used as a flag to do an optional action.
   */
  enum class ArgumentFlagType
  {
    Array = 0,
    Logical
  };

  /**
   * Metadata associated to an argument.
   * Used to parse the arguments in CLIArguments class.
   */
  struct CLIArgumentSpecs
  {
    std::string Name;
    std::string Help = "No help available.";
    std::optional<std::string> Flag = std::nullopt;
    ArgumentFlagType FlagType = ArgumentFlagType::Array;

    ArgumentValueType Type = ArgumentValueType::String;
    bool Required = true;
  };

  class ArgumentValue
  {
  private:
    using ArgumentValueType = std::variant<std::string, std::chrono::year_month_day>;

  public:
    ArgumentValue() = default;
    ArgumentValue(const std::string& value)
      : m_Value(value)
    {
    }
    ArgumentValue(const std::chrono::year_month_day& value)
      : m_Value(value)
    {
    }

    /**
     * Convert the value to the templated type.
     */
    template <typename T>
    T As() const
    {
      // TODO: maybe add default value?
      T retValue = T();
      std::visit(
        [&](auto&& variantValue)
        {
          using U = std::decay_t<decltype(variantValue)>;
          if constexpr (std::is_same_v<U, T>)
          {
            retValue = std::get<T>(m_Value);
          }
        },
        m_Value);

      return retValue;
    }

  private:
    ArgumentValueType m_Value;
  };

  class CLIArguments
  {
  public:
    CLIArguments() = default;
    CLIArguments(const std::vector<CLIArgumentSpecs>& argumentSpecs);

    /**
     * Parse the given command string.
     * If there is no argument specs, return true instantly.
     */
    bool Parse(const std::vector<std::string>& arguments);

    /**
     * Return the error message in case the command parsing failed.
     */
    const std::string& GetErrorMessage() const { return m_ErrorMessage; }

    /**
     * Return the argument associated to a position.
     */
    template <typename T>
    T GetArgumentValue(uint32_t position) const
    {
      SP_VERIFY(position < m_PositionalArgumentValues.size(), "Parse not successful position out of bounds!");
      T value = m_PositionalArgumentValues[position].As<T>();
      return value;
    }
    template <typename T>
    T GetArgumentValue(const std::string& key, uint32_t position = 0) const
    {
      const std::vector<ArgumentValue>& values = GetArgumentValues(key);
      if (position < values.size())
      {
        return values[position].As<T>();
      }
      else
      {
        SP_LOG_ERROR(
          "given argument position is out of scope! Argument: {} only contains {} values.", key, values.size());
        return T();
      }
    }

    /**
     * Return the arguments associated to a key.
     */
    const std::vector<ArgumentValue>& GetArgumentValues(const std::string& key) const
    {
      SP_VERIFY(m_FlaggedArgumentValues.contains(key), "Parse not successful key not found!");
      return m_FlaggedArgumentValues.at(key);
    }

    /**
     * Return the number of arguments that are associated to the given key.
     */
    size_t GetArgumentValuesCount(const std::string& key) const { return GetArgumentValues(key).size(); }

    /**
     * Return true if the given key contains some values.
     */
    bool FlagHasValues(const std::string& key) const
    {
      if (!m_FlaggedArgumentValues.contains(key))
        return false;

      return !m_FlaggedArgumentValues.at(key).empty();
    }

    /**
     * Return wether or not the given flag is contained in the command.
     */
    bool HasLogicalFlag(const std::string& flag) const { return m_LogicalFlags.contains(flag); }

    ///@{
    /**
     * Return iterable ranges for positional and flagged arguments.
     */
    CLIPositionalArgRange GetArgumentPositions() const;
    CLIFlaggedArgRange GetArgumentFlagsNames() const;
    ///@}

    ///@{
    /**
     * Return the help text associated to the given argument key or position
     * If the argument key or position is not found, return an empty string.
     */
    std::string GetHelp(uint32_t position) const;
    std::string GetName(uint32_t position) const;
    std::string GetHelp(const std::string& flag) const;
    std::string GetName(const std::string& flag) const;
    bool IsRequired(const std::string& flag) const;
    ///@}

  private:
    /**
     * Clear all parsed arguments and set m_ParseSuccess to false.
     */
    void ClearParsedValues();

    /**
     * @name Parse positional or flagged values
     * Try to parse the given raw value into the expected type by looking up in the argument specs.
     * Return true the parse is successful. The converted value is also insered in the corresponding argument value
     * container. Return false otherwise.
     */
    ///@{
    /**
     * Parse positional value.
     */
    bool ParsePositionalArgValues(uint32_t position, const std::string& rawValue);
    /**
     * Parse flagged value.
     */
    bool ParseFlaggedArgValues(const std::string& flag, const std::vector<std::string>& rawValues);
    ///@}

    /**
     * Parse the given argument to its corresponding value based on the given specs.
     * Return true if the parsing was successfull, otherwise it returns false.
     */
    bool ParseArgumentValue(ArgumentValue& value, const std::string& rawValue, const CLIArgumentSpecs& specs);

    /**
     * Add an error message to the error message list.
     */
    void AddErrorMessage(const std::string& message);

  private:
    // Specifications
    std::map<std::string, std::string> m_FlagToArgNameMap;
    std::vector<CLIArgumentSpecs> m_PositionalArgumentSpecs;
    std::map<std::string, CLIArgumentSpecs> m_FlaggedArgumentSpecs;

    // Parsing data
    std::vector<ArgumentValue> m_PositionalArgumentValues;
    std::map<std::string, std::vector<ArgumentValue>> m_FlaggedArgumentValues;
    std::set<std::string> m_LogicalFlags;

    std::string m_ErrorMessage;

    friend class CLIFlaggedArgRange;
  };

  class CLIPositionalArgsIterator
  {
  public:
    using ValueType = size_t;

  public:
    const ValueType& operator*() const { return m_It; }
    const ValueType* operator->() const { return &m_It; }

    // Prefix increment
    CLIPositionalArgsIterator& operator++()
    {
      m_It++;
      return *this;
    }

    // Postfix increment
    CLIPositionalArgsIterator operator++(int)
    {
      CLIPositionalArgsIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const CLIPositionalArgsIterator& a, const CLIPositionalArgsIterator& b)
    {
      return a.m_It == b.m_It;
    }
    friend bool operator!=(const CLIPositionalArgsIterator& a, const CLIPositionalArgsIterator& b)
    {
      return a.m_It != b.m_It;
    }

  private:
    CLIPositionalArgsIterator(int itValue)
      : m_It(itValue)
    {
    }

  private:
    size_t m_It;

    friend class CLIPositionalArgRange;
  };

  class CLIPositionalArgRange
  {
  public:
    CLIPositionalArgRange(const std::vector<CLIArgumentSpecs>& args)
      : m_PositionalArgCount(args.size())
    {
    }

    CLIPositionalArgsIterator begin() { return CLIPositionalArgsIterator(0); }
    CLIPositionalArgsIterator end() { return CLIPositionalArgsIterator(m_PositionalArgCount); }

  private:
    size_t m_PositionalArgCount;
  };

  class CLIFlaggedArgIterator
  {
  public:
    using ValueType = std::string;

  private:
    using InternalIteratorType = std::map<std::string, CLIArgumentSpecs>::const_iterator;

  public:
    const ValueType operator*() const { return m_It->second.Flag.value_or(""); }

    // Prefix increment
    CLIFlaggedArgIterator& operator++()
    {
      m_It++;
      return *this;
    }

    // Postfix increment
    CLIFlaggedArgIterator operator++(int)
    {
      CLIFlaggedArgIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const CLIFlaggedArgIterator& a, const CLIFlaggedArgIterator& b) { return a.m_It == b.m_It; }
    friend bool operator!=(const CLIFlaggedArgIterator& a, const CLIFlaggedArgIterator& b) { return a.m_It != b.m_It; }

  private:
    CLIFlaggedArgIterator(const InternalIteratorType& it)
      : m_It(it)
    {
    }

  private:
    InternalIteratorType m_It;

    friend class CLIFlaggedArgRange;
  };

  class CLIFlaggedArgRange
  {
  public:
    CLIFlaggedArgRange(const std::map<std::string, CLIArgumentSpecs>& args)
      : m_Args(args)
    {
    }

    CLIFlaggedArgIterator begin() { return CLIFlaggedArgIterator(m_Args.begin()); }
    CLIFlaggedArgIterator end() { return CLIFlaggedArgIterator(m_Args.end()); }

  private:
    const std::map<std::string, CLIArgumentSpecs>& m_Args;
  };

}
