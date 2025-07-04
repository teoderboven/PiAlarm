#ifdef _WIN32
#include <windows.h>
#endif

void setup_console() {
#ifdef _WIN32
    // Set windows console to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Get the handle to the standard output device.
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hOut == INVALID_HANDLE_VALUE) {
        exit(GetLastError());
    }

    DWORD dwMode = 0;

    // Get the current output mode.
    if (!GetConsoleMode(hOut, &dwMode)) {
        exit(GetLastError());
    }

    // Enable the virtual terminal processing mode.
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hOut, dwMode)) {
        exit(GetLastError());
    }
#endif
}

using namespace PiAlarm;

int main(int argc, char *argv[]) {
    setup_console();

}
