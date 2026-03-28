#include <windows.h>

// enum for ANSI Color Codes (256-colors in total, first 16 are used here)
enum ConsoleColor {
    Black        = 0,
    Red          = 1,
    Green        = 2,
    Yellow       = 3,
    Blue         = 4,
    Magenta      = 5,
    Cyan         = 6,
    White        = 7,
    BrightBlack  = 8,
    BrightRed    = 9,
    BrightGreen  = 10,
    BrightYellow = 11,
    BrightBlue   = 12,
    BrightMagenta= 13,
    BrightCyan   = 14,
    BrightWhite  = 15
};

class ConsoleUtils {
public:
    static void enableVirtualTerminal();
    static void setColor(int foreground, int background);
    static void setForegroundColor(int foreground);
    static void setBackgroundColor(int background);
    static void setDefaultColor();
    static void setConsoleBackgroundColor(int backgroundColor, int foregroundColor = 7);
    static void clearConsole();
    static void moveCursor(int x, int y);
    static void temp();
    static void hideCursor();
    static void showCursor();
    static void DrawRectangle(int x, int y, int lenX, int lenY);
    static void DrawDoubleLineRectangle(int x, int y, int lenX, int lenY);
    void virtual showCredits();
};