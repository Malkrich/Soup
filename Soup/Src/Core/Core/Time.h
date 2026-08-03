#pragma once

namespace Soup
{

  class DeltaTime
  {
  public:
    DeltaTime(float timeSeconds)
      : m_TimeSeconds(timeSeconds)
    {
    }

    inline float GetSeconds() const { return m_TimeSeconds; }
    inline float GetMilliseconds() const { return m_TimeSeconds * 1000.0f; }

  private:
    float m_TimeSeconds = 0.0f;
  };

  class Time
  {
  public:
    static float GetTime();
  };

}
