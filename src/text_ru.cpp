// Файл: text_ru.cpp
// Назначение: реализация русскоязычных текстов и функций форматирования.

#include "calendar/text_ru.hpp"

#include <array>
#include <cassert>
#include <iomanip>
#include <sstream>

namespace calendar {
namespace {

constexpr std::array<std::string_view, kMonthsInYear> kMonthNames = {
    "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
    "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь",
};

constexpr std::array<std::string_view, kDaysInWeek> kWeekdayNames = {
    "понедельник", "вторник", "среда", "четверг", "пятница", "суббота", "воскресенье",
};

constexpr std::array<std::string_view, kDaysInWeek> kWeekdayShortNames = {
    "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс",
};

}  // namespace

std::string_view monthName(int month) noexcept
{
    assert(month >= 1 && month <= kMonthsInYear);
    return kMonthNames[static_cast<std::size_t>(month - 1)];
}

std::string_view weekdayName(Weekday weekday) noexcept
{
    return kWeekdayNames[static_cast<std::size_t>(weekday)];
}

std::string_view weekdayShortName(Weekday weekday) noexcept
{
    return kWeekdayShortNames[static_cast<std::size_t>(weekday)];
}

std::string formatDate(const Date& date)
{
    std::ostringstream out;
    out << std::setfill('0')
        << std::setw(2) << date.day << '.'
        << std::setw(2) << date.month << '.'
        << std::setw(4) << date.year;
    return out.str();
}

std::string_view dayWord(long long count) noexcept
{
    const long long lastTwoDigits = (count < 0 ? -count : count) % 100;
    const long long lastDigit = lastTwoDigits % 10;

    // Числа 11–14 идут отдельно: «11 дней», хотя число оканчивается на 1.
    if (lastTwoDigits >= 11 && lastTwoDigits <= 14) {
        return "дней";
    }
    if (lastDigit == 1) {
        return "день";
    }
    if (lastDigit >= 2 && lastDigit <= 4) {
        return "дня";
    }
    return "дней";
}

std::size_t utf8Length(std::string_view text) noexcept
{
    // Байты-продолжения в UTF-8 имеют вид 10xxxxxx, их не считаем.
    std::size_t length = 0;
    for (const char symbol : text) {
        if ((static_cast<unsigned char>(symbol) & 0xC0U) != 0x80U) {
            ++length;
        }
    }
    return length;
}

}  // namespace calendar
