// Файл: app.cpp
// Назначение: реализация меню и пользовательских сценариев — календарь на месяц,
// день недели для даты, разница между датами.

#include "calendar/app.hpp"

#include <cstdlib>
#include <istream>
#include <ostream>

#include "calendar/date_parser.hpp"
#include "calendar/month_renderer.hpp"
#include "calendar/text_ru.hpp"

namespace calendar {
namespace {

constexpr std::string_view kDatePromptSuffix = " (день месяц год): ";

std::string trim(const std::string& text)
{
    constexpr std::string_view kWhitespace = " \t\r\n";
    const std::size_t first = text.find_first_not_of(kWhitespace);
    if (first == std::string::npos) {
        return {};
    }
    const std::size_t last = text.find_last_not_of(kWhitespace);
    return text.substr(first, last - first + 1);
}

/// Строка вида «29.02.2024 — четверг».
std::string describeDate(const Date& date)
{
    return formatDate(date) + " — " + std::string(weekdayName(dayOfWeek(date)));
}

}  // namespace

App::App(std::istream& in, std::ostream& out, const Date& today, bool useColor)
    : in_(in), out_(out), today_(today), useColor_(useColor)
{
}

void App::run()
{
    out_ << "=== Вечный календарь ===\n"
         << "Сегодня: " << describeDate(today_) << "\n";

    while (true) {
        printMenu();
        const std::optional<std::string> choice = readLine("Ваш выбор: ");
        if (!choice) {
            return;
        }

        bool hasMoreInput = true;
        if (*choice == "1") {
            hasMoreInput = showMonthCalendar();
        } else if (*choice == "2") {
            hasMoreInput = showWeekday();
        } else if (*choice == "3") {
            hasMoreInput = showDaysBetween();
        } else if (*choice == "4") {
            showCurrentMonth();
        } else if (*choice == "0") {
            out_ << "До свидания!\n";
            return;
        } else {
            out_ << "Неизвестная команда. Введите число от 0 до 4.\n";
        }

        if (!hasMoreInput) {
            return;
        }
    }
}

void App::printMenu()
{
    out_ << "\n"
         << "1. Календарь на месяц\n"
         << "2. День недели для даты\n"
         << "3. Разница между двумя датами\n"
         << "4. Календарь на текущий месяц\n"
         << "0. Выход\n";
}

void App::printMonth(const Date& date, const std::optional<Date>& selected)
{
    RenderOptions options;
    options.today = today_;
    options.selected = selected;
    options.useColor = useColor_;

    out_ << '\n';
    renderMonth(out_, date.month, date.year, options);
}

bool App::showMonthCalendar()
{
    const std::optional<Date> date = readDate("Введите дату");
    if (!date) {
        return false;
    }

    printMonth(*date, date);
    out_ << '\n' << describeDate(*date) << '\n';
    return true;
}

bool App::showWeekday()
{
    const std::optional<Date> date = readDate("Введите дату");
    if (!date) {
        return false;
    }

    out_ << describeDate(*date) << '\n';
    return true;
}

bool App::showDaysBetween()
{
    const std::optional<Date> first = readDate("Первая дата");
    if (!first) {
        return false;
    }
    const std::optional<Date> second = readDate("Вторая дата");
    if (!second) {
        return false;
    }

    const long long days = std::llabs(daysBetween(*first, *second));
    out_ << "Между " << formatDate(*first) << " и " << formatDate(*second) << ": "
         << days << ' ' << dayWord(days) << '\n';
    return true;
}

void App::showCurrentMonth()
{
    printMonth(today_, std::nullopt);
}

std::optional<std::string> App::readLine(std::string_view prompt)
{
    out_ << prompt << std::flush;

    std::string line;
    if (!std::getline(in_, line)) {
        out_ << '\n';
        return std::nullopt;
    }
    return trim(line);
}

std::optional<Date> App::readDate(std::string_view prompt)
{
    const std::string fullPrompt = std::string(prompt) + std::string(kDatePromptSuffix);

    while (true) {
        const std::optional<std::string> line = readLine(fullPrompt);
        if (!line) {
            return std::nullopt;
        }

        const ParseResult result = parseDate(*line);
        if (result.date) {
            return result.date;
        }
        out_ << "Ошибка: " << result.error << " Попробуйте ещё раз.\n";
    }
}

}  // namespace calendar
