// Файл: platform.cpp
// Назначение: платформенно-зависимая реализация настройки консоли и чтения
// системных часов для Windows и POSIX-систем (Linux, macOS).

#include "calendar/platform.hpp"

#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#else
#  include <unistd.h>
#endif

namespace calendar {

bool setupConsole()
{
#ifdef _WIN32
    // Без этого кириллица в консоли Windows превращается в «кракозябры».
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Включаем обработку ANSI-последовательностей; если вывод перенаправлен
    // в файл, GetConsoleMode вернёт ошибку — и цвета останутся выключенными.
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) {
        return false;
    }
    return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
#else
    // Цвета нужны только в интерактивном терминале; переменная NO_COLOR —
    // общепринятый способ их отключить (https://no-color.org).
    return isatty(STDOUT_FILENO) != 0 && std::getenv("NO_COLOR") == nullptr;
#endif
}

Date currentDate()
{
    const std::time_t now = std::time(nullptr);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif

    constexpr int kTmYearBase = 1900;
    return Date{local.tm_mday, local.tm_mon + 1, local.tm_year + kTmYearBase};
}

}  // namespace calendar
