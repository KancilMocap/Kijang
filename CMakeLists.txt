cmake_minimum_required(VERSION 3.14)

project(kijang VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Network Concurrent SerialPort Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Network Concurrent SerialPort Multimedia REQUIRED)

set(PROJECT_SOURCES
        qml.qrc
        src/main.cpp
        src/application/kijang.h src/application/kijang.cpp
        src/application/kijanglogger.h src/application/kijanglogger.cpp

        src/input/kijanginputmanager.h src/input/kijanginputmanager.cpp

        src/input/audioinput.h src/input/audioinput.cpp
        src/input/videoinput.h src/input/videoinput.cpp
        src/input/motioninput.h src/input/motioninput.cpp

        src/input/pilandukaudioinput.h src/input/pilandukaudioinput.cpp
        src/input/pilandukvideoinput.h src/input/pilandukvideoinput.cpp
        src/input/pilandukmotioninput.h src/input/pilandukmotioninput.cpp

        src/input/kinect360input.h src/input/kinect360input.cpp
        src/input/kinectoneinput.h src/input/kinectoneinput.cpp
        src/input/webcaminput.h src/input/webcaminput.cpp
        src/input/microphoneinput.h src/input/microphoneinput.cpp

        src/network/kijangnetworkmanager.h src/network/kijangnetworkmanager.cpp
        src/network/kijangtcpserver.h src/network/kijangtcpserver.cpp
        src/network/communicationclient.h src/network/communicationclient.cpp
        src/network/statusclient.h src/network/statusclient.cpp

        src/network/udplistenerinterface.h src/network/udplistenerinterface.cpp
        src/network/udpaudiolistener.h src/network/udpaudiolistener.cpp
        src/network/udpvideolistener.h src/network/udpvideolistener.cpp
        src/network/udpmotionlistener.h src/network/udpmotionlistener.cpp

        src/network/udpoutputinterface.h src/network/udpoutputinterface.cpp
        src/network/udpaudiooutput.h src/network/udpaudiooutput.cpp
        src/network/udpvideooutput.h src/network/udpvideooutput.cpp
        src/network/udpmotionoutput.h src/network/udpmotionoutput.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(kijang
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET pilandok APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(kijang SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(kijang
          ${PROJECT_SOURCES}
        )
    endif()
endif()

# Custom libraries here
add_library(libfreenect SHARED IMPORTED)
set_target_properties(libfreenect PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libfreenect/libfreenect.so"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/inc/libfreenect")
add_library(libfreenect2 SHARED IMPORTED)
set_target_properties(libfreenect2 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libfreenect2/libfreenect2-openni2.so"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/inc/libfreenect2")

target_compile_definitions(kijang
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(kijang
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::SerialPort Qt${QT_VERSION_MAJOR}::Multimedia)

set_target_properties(kijang PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER moe.kancil.kijang
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(kijang)
    qt_finalize_executable(kijang)
endif()

# Set version numbering
add_compile_definitions(VERSION_MAJOR=0)
add_compile_definitions(VERSION_MINOR=0)
add_compile_definitions(VERSION_MIC=1)
add_compile_definitions(VERSION_DEBUG=1)
