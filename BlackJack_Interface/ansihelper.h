#pragma once
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#endif

class ANSIHelper {
public:
  static void enableANSI() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
  }
};

// Definiciones de colores que funcionan en todas partes
#ifdef _WIN32
    // Windows 10: intenta con ANSI, si falla sin colores
#define RED "\033[91m"
#define BLACK "\033[30m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#else
    // Linux/Mac: ANSI nativo
#define RED "\033[91m"
#define BLACK "\033[30m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#endif