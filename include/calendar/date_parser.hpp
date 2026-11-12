// Файл: date_parser.hpp
// Назначение: разбор и проверка даты, введённой пользователем в виде строки.

#pragma once

#include <optional>
#include <string>

#include "calendar/date.hpp"

namespace calendar {

/// Результат разбора: либо дата, либо текст ошибки для пользователя.
struct ParseResult {
    std::optional<Date> date;
    std::string error;
};

/// Разбирает дату в формате «день месяц год». Разделители: пробелы, точки
/// или косые черты: подойдут и «29 2 2024», и «29.02.2024».
ParseResult parseDate(const std::string& text);

}  // namespace calendar
