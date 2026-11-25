cmake_minimum_required(VERSION 3.21)
project(scrawl VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(KF6SyntaxHighlighting QUIET)

qt_add_executable(scrawl
    resources/resources.qrc
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/DocumentView.cpp
    src/DocumentView.h
)

target_link_libraries(scrawl PRIVATE Qt6::Widgets)

if(KF6SyntaxHighlighting_FOUND)
    target_link_libraries(scrawl PRIVATE KF6::SyntaxHighlighting)
    target_compile_definitions(scrawl PRIVATE SCRAWL_HAS_SYNTAX)
    message(STATUS "KSyntaxHighlighting found, code highlighting enabled")
else()
    message(STATUS "KSyntaxHighlighting not found, plain editor")
endif()

if(WIN32)
    target_sources(scrawl PRIVATE resources/scrawl.rc)
    set_target_properties(scrawl PROPERTIES WIN32_EXECUTABLE ON)
endif()
