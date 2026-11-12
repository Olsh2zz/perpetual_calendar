// Файл: date.hpp
// Назначение: тип даты и календарная арифметика григорианского календаря
// (високосные годы, длина месяца, день недели, разница между датами).

#pragma once

namespace calendar {

/// Первый полный год действия григорианского календаря.
inline constexpr int kMinYear = 1583;
/// Верхняя граница года, поддерживаемая программой.
inline constexpr int kMaxYear = 9999;
inline constexpr int kMonthsInYear = 12;
inline constexpr int kDaysInWeek = 7;

/// День недели. Нумерация с понедельника — так удобнее строить сетку календаря.
enum class Weekday {
    Monday = 0,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
};

/// Календарная дата. Сама структура корректность не гарантирует — см. isValidDate.
struct Date {
    int day = 1;
    int month = 1;
    int year = kMinYear;
};

bool operator==(const Date& lhs, const Date& rhs) noexcept;
bool operator!=(const Date& lhs, const Date& rhs) noexcept;

/// Возвращает true, если год високосный по правилам григорианского календаря.
bool isLeapYear(int year) noexcept;

/// Количество дней в месяце. Месяц должен быть в диапазоне 1..12.
int daysInMonth(int month, int year) noexcept;

/// Проверяет, что дата существует и год лежит в диапазоне [kMinYear, kMaxYear].
bool isValidDate(const Date& date) noexcept;

/// День недели по алгоритму Зеллера. Дата должна быть корректной.
Weekday dayOfWeek(const Date& date) noexcept;

/// Количество дней от даты from до даты to (отрицательное, если to раньше from).
long long daysBetween(const Date& from, const Date& to) noexcept;

}  // namespace calendar
