#pragma once

// Originaly GLFW key codes
namespace Soup
{

  enum class KeyCode
  {
    // Printable keys
    Space = 32,
    Apostrophe = 39, /* ' */
    Comma = 44,      /* , */
    Minus = 45,      /* - */
    Period = 46,     /* . */
    Slash = 47,      /* / */
    Number0 = 48,
    Number1 = 49,
    Number2 = 50,
    Number3 = 51,
    Number4 = 52,
    Number5 = 53,
    Number6 = 54,
    Number7 = 55,
    Number8 = 56,
    Number9 = 57,
    Semicolon = 59, /* ; */
    Equal = 61,     /* = */
    CharA = 65,
    CharB = 66,
    CharC = 67,
    CharD = 68,
    CharE = 69,
    CharF = 70,
    CharG = 71,
    CharH = 72,
    CharI = 73,
    CharJ = 74,
    CharK = 75,
    CharL = 76,
    CharM = 77,
    CharN = 78,
    CharO = 79,
    CharP = 80,
    CharQ = 81,
    CharR = 82,
    CharS = 83,
    CharT = 84,
    CharU = 85,
    CharV = 86,
    CharW = 87,
    CharX = 88,
    CharY = 89,
    CharZ = 90,
    LeftBracket = 91,  /* [ */
    BackSlash = 92,    /* \ */
    RightBracket = 93, /* ] */
    GraveAccent = 96,  /* ` */
    World1 = 161,      /* non-US #1 */
    World2 = 162,      /* non-US #2 */

    // Function keys
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,

    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,

    KeyPad0 = 320,
    KeyPad1 = 321,
    KeyPad2 = 322,
    KeyPad3 = 323,
    KeyPad4 = 324,
    KeyPad5 = 325,
    KeyPad6 = 326,
    KeyPad7 = 327,
    KeyPad8 = 328,
    KeyPad9 = 329,
    KeyPadDecimal = 330,
    KeyPadDivide = 331,
    KeyPadMultiply = 332,
    KeyPadSubstract = 333,
    KeyPadAdd = 334,
    KeyPadEnter = 335,
    KeyPadEqual = 336,

    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,

    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348,
    Last = Menu
  };

  enum class MouseButton
  {
    Button1 = 0,
    Button2 = 1,
    Button3 = 2,
    Button4 = 3,
    Button5 = 4,
    Button6 = 5,
    Button7 = 6,
    Button8 = 7,

    Last = Button8,
    Left = Button1,
    Right = Button2,
    Middle = Button3
  };

} // namespace Soup
