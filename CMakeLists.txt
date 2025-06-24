cmake_minimum_required(VERSION 3.16)

project(tacsters VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(tacsters
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        splash.h splash.cpp splash.ui
        fantasybutton.h
        fantasybutton.cpp
        fantasylabel.h fantasylabel.cpp
        resources.qrc
        gallery.h gallery.cpp gallery.ui
        namesdialog.h namesdialog.cpp
        battlefield.h battlefield.cpp battlefield.ui
        crystalgridview.h crystalgridview.cpp

        mapreader.h mapreader.cpp



        Agent.h



        Agent.cpp
        gamemanager.h gamemanager.cpp
        cell.h cell.cpp
        battlefield.h battlefield.cpp
        battlefield.h battlefield.cpp
        agentdet.h agentdet.cpp
        AgentDet.h
        gamephases.h gamephases.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET tacsters APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(tacsters SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(tacsters
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(tacsters PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.tacsters)
endif()
set_target_properties(tacsters PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS tacsters
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(tacsters)
endif()
