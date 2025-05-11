cmake_minimum_required(VERSION 3.10)
project(SeaBattle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем пути
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/freeglut/include)

# Добавляем исходники
file(GLOB_RECURSE SOURCES src/*.cpp)

# Добавляем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Линкуем напрямую .a файл без -lfreeglut
target_link_libraries(${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/freeglut/lib/libfreeglut.a
        opengl32
        glu32
)

# Устанавливаем путь к выходному .exe
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
