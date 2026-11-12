# Файл: CMakeLists.txt
# Назначение: сценарий сборки проекта «Вечный календарь» (библиотека, приложение и тесты).

cmake_minimum_required(VERSION 3.16)
project(perpetual_calendar VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Общие флаги компиляции: строгие предупреждения и исходники в кодировке UTF-8.
add_library(calendar_options INTERFACE)
if(MSVC)
    target_compile_options(calendar_options INTERFACE /W4 /utf-8)
else()
    target_compile_options(calendar_options INTERFACE -Wall -Wextra -Wpedantic -Wconversion)
endif()

# Ядро: вся логика календаря без точки входа, чтобы её можно было тестировать.
add_library(calendar_core
    src/app.cpp
    src/date.cpp
    src/date_parser.cpp
    src/month_renderer.cpp
    src/platform.cpp
    src/text_ru.cpp
)
target_include_directories(calendar_core PUBLIC include)
target_link_libraries(calendar_core PUBLIC calendar_options)

# Исполняемый файл приложения.
add_executable(perpetual_calendar src/main.cpp)
target_link_libraries(perpetual_calendar PRIVATE calendar_core)

# Модульные тесты (запуск: ctest --test-dir build).
enable_testing()
add_executable(calendar_tests tests/calendar_tests.cpp)
target_link_libraries(calendar_tests PRIVATE calendar_core)
add_test(NAME calendar_tests COMMAND calendar_tests)
