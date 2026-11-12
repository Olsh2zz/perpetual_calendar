// Файл: month_renderer.hpp
// Назначение: вывод календаря на месяц в виде таблицы, как в настенном календаре,
// с подсветкой сегодняшней и выбранной дат.

#pragma once

#include <iosfwd>
#include <optional>

#include "calendar/date.hpp"

namespace calendar {

/// Параметры оформления календаря.
struct RenderOptions {
    /// Сегодняшняя дата — подсвечивается, если попадает в выводимый месяц.
    std::optional<Date> today;
    /// Дата, введённая пользователем, — выделяется отдельным цветом.
    std::optional<Date> selected;
    /// Разрешает управляющие последовательности ANSI. Без них вывод — чистый текст.
    bool useColor = false;
};

/// Печатает календарь на месяц. Месяц и год должны быть корректными.
void renderMonth(std::ostream& out, int month, int year, const RenderOptions& options = {});

}  // namespace calendar
