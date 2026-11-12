// Файл: app.hpp
// Назначение: интерактивное консольное приложение — главное меню и сценарии
// работы с календарём.

#pragma once

#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>

#include "calendar/date.hpp"

namespace calendar {

/// Консольное приложение «Вечный календарь».
/// Потоки ввода-вывода и «сегодня» передаются снаружи, поэтому класс легко тестировать.
class App {
public:
    App(std::istream& in, std::ostream& out, const Date& today, bool useColor);

    /// Запускает цикл меню. Завершается по команде выхода или концу ввода.
    void run();

private:
    void printMenu();
    void printMonth(const Date& date, const std::optional<Date>& selected);

    // Сценарии меню. Возвращают false, если ввод закончился и пора выходить.
    bool showMonthCalendar();
    bool showWeekday();
    bool showDaysBetween();
    void showCurrentMonth();

    /// Читает строку ввода. Пустой результат — ввод закончился.
    std::optional<std::string> readLine(std::string_view prompt);
    /// Запрашивает дату, пока не будет введена корректная. Пустой результат — ввод закончился.
    std::optional<Date> readDate(std::string_view prompt);

    std::istream& in_;
    std::ostream& out_;
    Date today_;
    bool useColor_;
};

}  // namespace calendar
