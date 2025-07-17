cmake_minimum_required(VERSION 3.20.0)
project(InfoMonitor VERSION 0.0.1)

if (MSVC)
    add_compile_options("/utf-8")
    add_compile_options("/arch:AVX2")
    add_compile_options("/GL")
    add_compile_options("/MP")
    add_compile_options("/favor:AMD64")
    add_compile_options("/d2vzeroupper")
    add_link_options("/LTCG")
elseif (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options("-m64")
    add_compile_options("-mavx2")
    add_compile_options("-funroll-loops")
    add_compile_options("-ffunction-sections")
    add_compile_options("-fdata-sections")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core Network LinguistTools)

add_subdirectory(src)
include_directories(src)

set(TS_FILES res/translations/zh_cn.ts)
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${PROJECT_SOURCE_DIR}/res/translations)
qt_create_translation(QM_FILES src/main.cpp ${UI_FILES} ${_UI_SOURCES} ${_ELEMENT_SOURCES} ${_BASE_SOURCES} ${TS_FILES}
    OPTIONS -no-obsolete -locations none -no-ui-lines)

qt_add_executable(InfoMonitor
    MANUAL_FINALIZATION
    src/main.cpp
    ${_UI_SOURCES}
    ${_BASE_SOURCES}
    ${_ELEMENT_SOURCES}
    res/InfoMonitor.qrc
    ${QM_FILES}
)

target_link_libraries(InfoMonitor PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Network)
target_compile_definitions(InfoMonitor PUBLIC POINTERHOLDER_TRANSITION=0)

set_target_properties(InfoMonitor PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(InfoMonitor)
