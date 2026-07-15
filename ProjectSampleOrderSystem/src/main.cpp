#include "ConsoleMVC/AppController.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    // Source/execution charset is UTF-8 (see /utf-8 in the vcxproj); the console
    // must be switched to the matching code page or Korean text renders as mojibake.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    cmvc::AppController app;
    app.Run();
    return 0;
}
