// Файл: calendar_tests.cpp
// Назначение: модульные тесты календарной арифметики, разбора дат, вывода
// месяца и сценариев приложения. Сторонние библиотеки не используются.

#include <iostream>
#include <sstream>
#include <string>

#include "calendar/app.hpp"
#include "calendar/date.hpp"
#include "calendar/date_parser.hpp"
#include "calendar/month_renderer.hpp"
#include "calendar/text_ru.hpp"

namespace {

using namespace calendar;

int failureCount = 0;

/// Проверяет условие; при неудаче печатает место ошибки и продолжает тесты.
#define CHECK(condition)                                                          \
    do {                                                                          \
        if (!(condition)) {                                                       \
            ++failureCount;                                                       \
            std::cerr << __FILE__ << ':' << __LINE__ << ": не выполнено: "        \
                      << #condition << '\n';                                      \
        }                                                                         \
    } while (false)

void testLeapYears()
{
    CHECK(isLeapYear(2024));
    CHECK(!isLeapYear(2023));
    CHECK(!isLeapYear(1900));
    CHECK(isLeapYear(2000));
}

void testDaysInMonth()
{
    CHECK(daysInMonth(1, 2023) == 31);
    CHECK(daysInMonth(4, 2023) == 30);
    CHECK(daysInMonth(2, 2023) == 28);
    CHECK(daysInMonth(2, 2024) == 29);
    CHECK(daysInMonth(2, 1900) == 28);
}

void testDateValidation()
{
    CHECK(isValidDate({29, 2, 2024}));
    CHECK(!isValidDate({29, 2, 2023}));
    CHECK(!isValidDate({31, 4, 2024}));
    CHECK(!isValidDate({0, 1, 2024}));
    CHECK(!isValidDate({1, 13, 2024}));
    CHECK(!isValidDate({1, 1, kMinYear - 1}));
    CHECK(isValidDate({1, 1, kMinYear}));
}

void testDayOfWeek()
{
    CHECK(dayOfWeek({1, 2, 2024}) == Weekday::Thursday);
    CHECK(dayOfWeek({1, 1, 2000}) == Weekday::Saturday);
    CHECK(dayOfWeek({12, 4, 1961}) == Weekday::Wednesday);
    CHECK(dayOfWeek({1, 1, 1583}) == Weekday::Saturday);
    CHECK(dayOfWeek({31, 12, 9999}) == Weekday::Friday);
    CHECK(dayOfWeek({29, 2, 2000}) == Weekday::Tuesday);
}

/// Сверяет алгоритм Зеллера с независимым расчётом: каждый следующий день
/// обязан быть следующим днём недели на всём поддерживаемом диапазоне.
void testDayOfWeekIsSequential()
{
    int expected = static_cast<int>(dayOfWeek({1, 1, kMinYear}));
    bool isSequential = true;

    for (int year = kMinYear; year <= kMaxYear; ++year) {
        for (int month = 1; month <= kMonthsInYear; ++month) {
            for (int day = 1; day <= daysInMonth(month, year); ++day) {
                isSequential = isSequential
                               && static_cast<int>(dayOfWeek({day, month, year})) == expected;
                expected = (expected + 1) % kDaysInWeek;
            }
        }
    }
    CHECK(isSequential);
}

void testDaysBetween()
{
    CHECK(daysBetween({1, 1, 2024}, {1, 1, 2024}) == 0);
    CHECK(daysBetween({28, 2, 2024}, {1, 3, 2024}) == 2);
    CHECK(daysBetween({28, 2, 2023}, {1, 3, 2023}) == 1);
    CHECK(daysBetween({1, 1, 2024}, {1, 1, 2025}) == 366);
    CHECK(daysBetween({1, 1, 2025}, {1, 1, 2024}) == -366);
    CHECK(daysBetween({1, 1, 2000}, {1, 1, 2400}) == 146097);
}

void testFormatting()
{
    CHECK(formatDate({5, 3, 2024}) == "05.03.2024");
    CHECK(monthName(2) == "Февраль");
    CHECK(weekdayName(Weekday::Sunday) == "воскресенье");
    CHECK(utf8Length("Февраль 2024") == 12);

    CHECK(dayWord(1) == "день");
    CHECK(dayWord(3) == "дня");
    CHECK(dayWord(5) == "дней");
    CHECK(dayWord(11) == "дней");
    CHECK(dayWord(21) == "день");
    CHECK(dayWord(112) == "дней");
}

void testDateParsing()
{
    CHECK(parseDate("29 2 2024").date == Date({29, 2, 2024}));
    CHECK(parseDate("29.02.2024").date == Date({29, 2, 2024}));
    CHECK(parseDate("  1/1/1583 ").date == Date({1, 1, 1583}));

    CHECK(!parseDate("").date);
    CHECK(!parseDate("1 2").date);
    CHECK(!parseDate("1 2 2024 5").date);
    CHECK(!parseDate("один 2 2024").date);
    CHECK(!parseDate("29 2 2023").date);
    CHECK(!parseDate("1 13 2024").date);
    CHECK(!parseDate("1 1 1582").date);
    CHECK(!parseDate("29 2 2023").error.empty());
}

void testMonthRendering()
{
    // Эталон из постановки задачи.
    const std::string expected =
        "    Февраль 2024\n"
        "Пн Вт Ср Чт Пт Сб Вс\n"
        "          1  2  3  4\n"
        " 5  6  7  8  9 10 11\n"
        "12 13 14 15 16 17 18\n"
        "19 20 21 22 23 24 25\n"
        "26 27 28 29\n";

    std::ostringstream out;
    renderMonth(out, 2, 2024);
    CHECK(out.str() == expected);
}

void testMonthRenderingEdges()
{
    // Месяц начинается с понедельника и занимает ровно четыре недели.
    std::ostringstream compact;
    renderMonth(compact, 2, 2021);
    CHECK(compact.str() ==
          "    Февраль 2021\n"
          "Пн Вт Ср Чт Пт Сб Вс\n"
          " 1  2  3  4  5  6  7\n"
          " 8  9 10 11 12 13 14\n"
          "15 16 17 18 19 20 21\n"
          "22 23 24 25 26 27 28\n");

    // Месяц начинается с воскресенья: максимальный отступ в первой неделе.
    std::ostringstream wide;
    renderMonth(wide, 9, 2024);
    CHECK(wide.str().find("\n                   1\n 2  3") != std::string::npos);
}

void testHighlighting()
{
    RenderOptions options;
    options.today = Date{14, 2, 2024};
    options.selected = Date{20, 2, 2024};

    std::ostringstream plain;
    renderMonth(plain, 2, 2024, options);
    CHECK(plain.str().find('\033') == std::string::npos);

    options.useColor = true;
    std::ostringstream colored;
    renderMonth(colored, 2, 2024, options);
    CHECK(colored.str().find("\033[1;7m14\033[0m") != std::string::npos);
    CHECK(colored.str().find("\033[1;33m20\033[0m") != std::string::npos);
}

void testAppScenarios()
{
    // Некорректная дата, затем корректная; разница дат; выход.
    std::istringstream in("1\n31 2 2024\n29.02.2024\n3\n1 1 2024\n8 3 2024\n9\n0\n");
    std::ostringstream out;
    App app(in, out, Date{12, 11, 2026}, false);
    app.run();

    const std::string text = out.str();
    CHECK(text.find("Ошибка: День должен быть в диапазоне от 1 до 29") != std::string::npos);
    CHECK(text.find("26 27 28 29\n") != std::string::npos);
    CHECK(text.find("29.02.2024 — четверг") != std::string::npos);
    CHECK(text.find(": 67 дней") != std::string::npos);
    CHECK(text.find("Неизвестная команда") != std::string::npos);
    CHECK(text.find("До свидания!") != std::string::npos);
}

void testAppStopsOnEndOfInput()
{
    std::istringstream in("2\n");
    std::ostringstream out;
    App app(in, out, Date{12, 11, 2026}, false);
    app.run();  // Не должно зациклиться, когда ввод закончился.
    CHECK(out.str().find("До свидания!") == std::string::npos);
}

}  // namespace

int main()
{
    testLeapYears();
    testDaysInMonth();
    testDateValidation();
    testDayOfWeek();
    testDayOfWeekIsSequential();
    testDaysBetween();
    testFormatting();
    testDateParsing();
    testMonthRendering();
    testMonthRenderingEdges();
    testHighlighting();
    testAppScenarios();
    testAppStopsOnEndOfInput();

    if (failureCount > 0) {
        std::cerr << "Провалено проверок: " << failureCount << '\n';
        return 1;
    }
    std::cout << "Все тесты пройдены.\n";
    return 0;
}
