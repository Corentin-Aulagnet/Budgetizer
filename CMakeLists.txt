cmake_minimum_required(VERSION 3.5)

project(Budgetizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui)

set(TS_FILES Budgetizer_en_US.ts)
set(APP_NAME Budgetizer)
#add_subdirectory(sqlite3)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
        src/newcategoryform.h src/newcategoryform.cpp src/newcategoryform.ui
        src/databasemanager.h src/databasemanager.cpp
        src/transactiontable.h src/transactiontable.cpp
        src/analyticsmanager.h src/analyticsmanager.cpp
        src/utils.h
        ${TS_FILES}
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${APP_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${APP_NAME} SHARED
            ${PROJECT_SOURCES}
            sqlite3
        )
    else()
        add_executable(${APP_NAME}
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
include_directories(dependencies/sqlite3/include)
add_library(sqlite3 STATIC IMPORTED)
set_property(TARGET sqlite3 PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/dependencies/sqlite3/lib/sqlite3.a)

target_link_libraries(${APP_NAME}
                                    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets 
                                    sqlite3 
                                    ${CMAKE_DL_LIBS}
                                            
                                     PUBLIC
                                             Qt::Charts
                                             Qt::Core
                                             Qt::Gui)

set_target_properties(${APP_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    WIN64_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${APP_NAME})
endif()
