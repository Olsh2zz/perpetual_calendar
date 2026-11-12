// Файл: date.cpp
// Назначение: реализация календарной арифметики — правило високосного года,
// алгоритм Зеллера и перевод даты в сквозной номер дня.

#include "calendar/date.hpp"

#include <array>
#include <cassert>

namespace calendar {
namespace {

constexpr int kFebruary = 2;

bool isValidMonth(int month) noexcept
{
    return month >= 1 && month <= kMonthsInYear;
}

/// Сквозной номер дня (юлианский день, JDN). Разность номеров двух дат
/// равна количеству дней между ними.
long long toDayNumber(const Date& date) noexcept
{
    // Январь и февраль считаются 13-м и 14-м месяцами предыдущего года:
    // тогда «плавающий» февраль оказывается в конце года и не мешает расчёту.
    const int shift = (14 - date.month) / 12;
    const long long year = date.year + 4800LL - shift;
    const long long month = date.month + 12LL * shift - 3;

    return date.day + (153 * month + 2) / 5 + 365 * year
           + year / 4 - year / 100 + year / 400 - 32045;
}

}  // namespace

bool operator==(const Date& lhs, const Date& rhs) noexcept
{
    return lhs.day == rhs.day && lhs.month == rhs.month && lhs.year == rhs.year;
}

bool operator!=(const Date& lhs, const Date& rhs) noexcept
{
    return !(lhs == rhs);
}

bool isLeapYear(int year) noexcept
{
    // Кратен 4 — високосный, кратен 100 — нет, кратен 400 — всё же високосный.
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int daysInMonth(int month, int year) noexcept
{
    assert(isValidMonth(month));

    static constexpr std::array<int, kMonthsInYear> kDays = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
    };

    if (month == kFebruary && isLeapYear(year)) {
        return 29;
    }
    return kDays[static_cast<std::size_t>(month - 1)];
}

bool isValidDate(const Date& date) noexcept
{
    return date.year >= kMinYear && date.year <= kMaxYear
           && isValidMonth(date.month)
           && date.day >= 1 && date.day <= daysInMonth(date.month, date.year);
}

Weekday dayOfWeek(const Date& date) noexcept
{
    assert(isValidDate(date));

    // Алгоритм Зеллера: январь и февраль — это 13-й и 14-й месяцы прошлого года.
    int month = date.month;
    int year = date.year;
    if (month <= kFebruary) {
        month += kMonthsInYear;
        --year;
    }

    const int century = year / 100;
    const int yearOfCentury = year % 100;

    // Результат формулы: 0 — суббота, 1 — воскресенье, 2 — понедельник, ...
    const int zeller = (date.day + 13 * (month + 1) / 5 + yearOfCentury
                        + yearOfCentury / 4 + century / 4 + 5 * century) % kDaysInWeek;

    // Сдвигаем нумерацию так, чтобы понедельник стал нулём.
    constexpr int kSaturdayToMondayShift = 5;
    return static_cast<Weekday>((zeller + kSaturdayToMondayShift) % kDaysInWeek);
}

long long daysBetween(const Date& from, const Date& to) noexcept
{
    assert(isValidDate(from) && isValidDate(to));
    return toDayNumber(to) - toDayNumber(from);
}

}  // namespace calendar
