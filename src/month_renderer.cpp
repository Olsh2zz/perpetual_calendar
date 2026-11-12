// Файл: month_renderer.cpp
// Назначение: реализация табличного вывода месяца: заголовок по центру,
// строка дней недели и сетка чисел с отступом в первой неделе.

#include "calendar/month_renderer.hpp"

#include <iomanip>
#include <ostream>
#include <string>
#include <string_view>

#include "calendar/text_ru.hpp"

namespace calendar {
namespace {

constexpr int kCellWidth = 2;
/// Ширина таблицы: семь ячеек и шесть пробелов между ними.
constexpr int kTableWidth = kDaysInWeek * kCellWidth + (kDaysInWeek - 1);

constexpr std::string_view kStyleReset = "\033[0m";
constexpr std::string_view kStyleToday = "\033[1;7m";      // жирный, инверсия
constexpr std::string_view kStyleSelected = "\033[1;33m";  // жирный жёлтый
constexpr std::string_view kStyleWeekend = "\033[31m";     // красный

bool isWeekend(int column) noexcept
{
    return column >= static_cast<int>(Weekday::Saturday);
}

/// Подбирает стиль ячейки. Пустая строка означает, что оформление не требуется.
std::string_view cellStyle(const Date& date, int column, const RenderOptions& options) noexcept
{
    if (!options.useColor) {
        return {};
    }
    if (options.today && *options.today == date) {
        return kStyleToday;
    }
    if (options.selected && *options.selected == date) {
        return kStyleSelected;
    }
    if (isWeekend(column)) {
        return kStyleWeekend;
    }
    return {};
}

void renderTitle(std::ostream& out, int month, int year)
{
    const std::string title = std::string(monthName(month)) + ' ' + std::to_string(year);
    const int titleLength = static_cast<int>(utf8Length(title));
    const int padding = titleLength < kTableWidth ? (kTableWidth - titleLength) / 2 : 0;

    out << std::string(static_cast<std::size_t>(padding), ' ') << title << '\n';
}

void renderWeekdayHeader(std::ostream& out)
{
    for (int column = 0; column < kDaysInWeek; ++column) {
        if (column > 0) {
            out << ' ';
        }
        out << weekdayShortName(static_cast<Weekday>(column));
    }
    out << '\n';
}

void renderDay(std::ostream& out, const Date& date, int column, const RenderOptions& options)
{
    const std::string_view style = cellStyle(date, column, options);

    out << style << std::setw(kCellWidth) << date.day;
    if (!style.empty()) {
        out << kStyleReset;
    }
}

void renderDays(std::ostream& out, int month, int year, const RenderOptions& options)
{
    const int firstColumn = static_cast<int>(dayOfWeek(Date{1, month, year}));
    const int dayCount = daysInMonth(month, year);

    // Первая неделя начинается с пустых ячеек до нужного дня недели.
    out << std::string(static_cast<std::size_t>(firstColumn * (kCellWidth + 1)), ' ');

    int column = firstColumn;
    for (int day = 1; day <= dayCount; ++day) {
        renderDay(out, Date{day, month, year}, column, options);

        const bool isEndOfWeek = column == kDaysInWeek - 1;
        const bool isLastDay = day == dayCount;
        if (isEndOfWeek || isLastDay) {
            out << '\n';
            column = 0;
        } else {
            out << ' ';
            ++column;
        }
    }
}

}  // namespace

void renderMonth(std::ostream& out, int month, int year, const RenderOptions& options)
{
    renderTitle(out, month, year);
    renderWeekdayHeader(out);
    renderDays(out, month, year, options);
}

}  // namespace calendar
