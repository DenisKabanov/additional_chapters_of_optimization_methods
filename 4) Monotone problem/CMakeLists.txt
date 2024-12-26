# файл для билда проекта
cmake_minimum_required(VERSION 3.6.0)
project(algorithms VERSION 0.1.0 LANGUAGES C CXX) # имя проекта

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_CXX_FLAGS "-Wall -Werror -Wsign-compare") # флаги для всех вариантов компиляции (warning — ошибка)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0"
    CACHE STRING "Compiler flags in debug build"
    FORCE
)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all -O2"
    CACHE STRING "Compiler flags in asan build"
    FORCE
)

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG"
    CACHE STRING "Compiler flags for the release version that executed on Yandex.Contest"
    FORCE
)


# вывод сообщений в output
message("General build compiler flags: " ${CMAKE_CXX_FLAGS})
message("DEBUG build compiler flags: " ${CMAKE_CXX_FLAGS_DEBUG})
message("ASAN build compiler flags: " ${CMAKE_CXX_FLAGS_ASAN})
message("Release build compiler flags: " ${CMAKE_CXX_FLAGS_RELEASE})

# add_executable(problem1 problem1.cpp)
# add_executable(problem2 problem2.cpp)
add_executable(monotone ./src/monotone.cpp) # первый параметр — название исполняемого файла, второй — что собирать
