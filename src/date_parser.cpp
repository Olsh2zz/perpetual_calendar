// Файл: date_parser.cpp
// Назначение: реализация разбора даты и формирование сообщений об ошибках ввода.

#include "calendar/date_parser.hpp"

#include <algorithm>
#include <sstream>

#include "calendar/text_ru.hpp"

namespace calendar {
namespace {

bool isSeparator(char symbol) noexcept
{
    return symbol == '.' || symbol == '/';
}

/// Возвращает описание первой найденной ошибки или пустую строку, если дата корректна.
std::string describeError(const Date& date)
{
    if (date.year < kMinYear || date.year > kMaxYear) {
        return "Год должен быть в диапазоне от " + std::to_string(kMinYear)
               + " до " + std::to_string(kMaxYear) + ".";
    }
    if (date.month < 1 || date.month > kMonthsInYear) {
        return "Месяц должен быть в диапазоне от 1 до " + std::to_string(kMonthsInYear) + ".";
    }

    const int dayCount = daysInMonth(date.month, date.year);
    if (date.day < 1 || date.day > dayCount) {
        return "День должен быть в диапазоне от 1 до " + std::to_string(dayCount)
               + " (" + std::string(monthName(date.month)) + " " + std::to_string(date.year) + ").";
    }
    return {};
}

}  // namespace

ParseResult parseDate(const std::string& text)
{
    std::string normalized = text;
    std::replace_if(normalized.begin(), normalized.end(), isSeparator, ' ');

    std::istringstream input(normalized);
    Date date;
    std::string extra;
    const bool hasThreeNumbers = static_cast<bool>(input >> date.day >> date.month >> date.year);
    const bool hasExtraText = static_cast<bool>(input >> extra);

    if (!hasThreeNumbers || hasExtraText) {
        return {std::nullopt, "Нужно ввести три целых числа: день, месяц и год."};
    }

    std::string error = describeError(date);
    if (!error.empty()) {
        return {std::nullopt, std::move(error)};
    }
    return {date, {}};
}

}  // namespace calendar
