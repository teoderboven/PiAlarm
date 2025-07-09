#ifndef CONSOLEUTILS_H
#define CONSOLEUTILS_H

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @file consoleUtils.h
 * @brief Utility functions for console output configuration.
 *
 * This header file provides functions to set up the console for UTF-8 output and enable virtual terminal processing on Windows.
 */

/**
 * @brief Sets up the console for UTF-8 output and enables virtual terminal processing on Windows.
 */
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

#endif //CONSOLEUTILS_H
