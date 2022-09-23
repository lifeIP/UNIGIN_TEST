## THIS FILE IS STUB FOR BUILD
## DO NOT MODIFICATE IT - IT`S PART OF ANOTHER PROJECT
##
## это файл-заглушка для сборки.
## не модифицируйте его - возможно это часть другого проекта
cmake_minimum_required(VERSION 3.20)
project(unigine_task LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
if (UNIX) 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -msse4.1")
endif()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# include directories
list(APPEND INCLUDE_DIRS "source")
list(APPEND INCLUDE_DIRS "include")

# source
list(APPEND SOURCES "source/main.cpp")
list(APPEND SOURCES "source/task.cpp")

add_executable(unigine_task ${SOURCES})
target_include_directories(unigine_task PRIVATE ${INCLUDE_DIRS})

target_link_libraries(unigine_task Threads::Threads)

set_target_properties(unigine_task PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/bin)
set_target_properties(unigine_task PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/bin)

