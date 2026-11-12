// Файл: platform.hpp
// Назначение: всё, что зависит от операционной системы: настройка консоли
// (кодировка UTF-8, цвета) и получение текущей даты.

#pragma once

#include "calendar/date.hpp"

namespace calendar {

/// Настраивает консоль для вывода кириллицы в UTF-8.
/// Возвращает true, если терминал поддерживает цвета ANSI.
bool setupConsole();

/// Текущая дата по местному времени.
Date currentDate();

}  // namespace calendar
