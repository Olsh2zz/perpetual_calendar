// Файл: text_ru.hpp
// Назначение: русскоязычные тексты и форматирование — названия месяцев и дней
// недели, запись даты, склонение слова «день».

#pragma once

#include <cstddef>
#include <string>
#include <string_view>

#include "calendar/date.hpp"

namespace calendar {

/// Название месяца с заглавной буквы («Февраль»). Месяц — в диапазоне 1..12.
std::string_view monthName(int month) noexcept;

/// Полное название дня недели в именительном падеже («четверг»).
std::string_view weekdayName(Weekday weekday) noexcept;

/// Двухбуквенное сокращение дня недели («Чт»).
std::string_view weekdayShortName(Weekday weekday) noexcept;

/// Дата в формате ДД.ММ.ГГГГ.
std::string formatDate(const Date& date);

/// Слово «день» в форме, согласованной с числом: 1 день, 2 дня, 5 дней.
std::string_view dayWord(long long count) noexcept;

/// Количество символов (а не байт) в строке UTF-8 — нужно для выравнивания.
std::size_t utf8Length(std::string_view text) noexcept;

}  // namespace calendar
