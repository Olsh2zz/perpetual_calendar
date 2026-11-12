// Файл: main.cpp
// Назначение: точка входа. Настраивает консоль и запускает приложение.

#include <iostream>

#include "calendar/app.hpp"
#include "calendar/platform.hpp"

int main()
{
    const bool useColor = calendar::setupConsole();

    calendar::App app(std::cin, std::cout, calendar::currentDate(), useColor);
    app.run();
    return 0;
}
