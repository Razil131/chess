cmake_minimum_required(VERSION 3.10)
project(chess VERSION 0.1.0 LANGUAGES CXX)

# указываем стандарт
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ищем SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# подключаем заголовки
include_directories(
    include
    include/figures
    include/ui
)

# ищем все cpp-файлы
file(GLOB_RECURSE SOURCE_FILES
    src/*.cpp
    src/figures/*.cpp
    src/ui/*.cpp
)

# создаём исполняемый файл
add_executable(chess ${SOURCE_FILES})

# линкуем SFML
target_link_libraries(chess
    sfml-graphics
    sfml-window
    sfml-system
)