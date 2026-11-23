#pragma once

namespace Soup
{

  class UUID
  {
  public:
    UUID();
    UUID(uint64_t id);

    bool operator==(UUID other) const { return other.m_ID == m_ID; }
    bool operator<(UUID other) const { return m_ID < other.m_ID; }
    operator uint64_t() const { return m_ID; }
    operator bool() const { return m_ID != 0; }

  private:
    uint64_t m_ID;
  };

}

namespace std
{

  template <>
  struct hash<Soup::UUID>
  {
    std::size_t operator()(const Soup::UUID& uuid) const { return (uint64_t)uuid; }
  };

}
