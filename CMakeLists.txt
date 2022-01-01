cmake_minimum_required(VERSION 3.14)

project(kijang VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Network Concurrent SerialPort Multimedia Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Network Concurrent SerialPort Multimedia Widgets REQUIRED)

set(PROJECT_SOURCES
        qml.qrc
        src/main.cpp
        src/application/kijangapp.h src/application/kijangapp.cpp
        src/application/kijanglogger.h src/application/kijanglogger.cpp
        src/kijangplugin.h src/kijangplugin.cpp # Interface
        src/application/kijangpluginmanager.h src/application/kijangpluginmanager.cpp

        src/input/kijanginputmanager.h src/input/kijanginputmanager.cpp

        src/input/audioinput.h src/input/audioinput.cpp # Interface
        src/input/videoinput.h src/input/videoinput.cpp # Interface
        src/input/motioninput.h src/input/motioninput.cpp # Interface

        src/input/pilandukaudioinput.h src/input/pilandukaudioinput.cpp
        src/input/pilandukvideoinput.h src/input/pilandukvideoinput.cpp
        src/input/pilandukmotioninput.h src/input/pilandukmotioninput.cpp

        src/input/kinect360input.h src/input/kinect360input.cpp
        src/input/kinectoneinput.h src/input/kinectoneinput.cpp
        src/input/webcaminput.h src/input/webcaminput.cpp
        src/input/microphoneinput.h src/input/microphoneinput.cpp

        src/network/kijangnetworkmanager.h src/network/kijangnetworkmanager.cpp
        src/network/kijangprotocol.h src/network/kijangprotocol.cpp

        src/network/kijangtcpserver.h src/network/kijangtcpserver.cpp
        src/network/kijangreadserver.h src/network/kijangreadserver.cpp
        src/network/kijangwriteserver.h src/network/kijangwriteserver.cpp
        src/network/kijangstatusserver.h src/network/kijangstatusserver.cpp

        src/network/readclient.h src/network/readclient.cpp
        src/network/writeclient.h src/network/writeclient.cpp
        src/network/statusclient.h src/network/statusclient.cpp

        src/network/kijangmodulehandler.h src/network/kijangmodulehandler.cpp # Interface

        src/network/udplistenerinterface.h src/network/udplistenerinterface.cpp # Interface
        src/network/udpaudiolistener.h src/network/udpaudiolistener.cpp
        src/network/udpvideolistener.h src/network/udpvideolistener.cpp
        src/network/udpmotionlistener.h src/network/udpmotionlistener.cpp
        src/network/udpfilelistener.h src/network/udpfilelistener.cpp

        src/network/udpoutputinterface.h src/network/udpoutputinterface.cpp # Interface
        src/network/udpaudiooutput.h src/network/udpaudiooutput.cpp
        src/network/udpvideooutput.h src/network/udpvideooutput.cpp
        src/network/udpmotionoutput.h src/network/udpmotionoutput.cpp

        src/network/servercontrolmodule.h src/network/servercontrolmodule.cpp
        src/network/kijangmodule.h src/network/kijangmodule.cpp
)

qt_add_executable(kijang
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# Custom libraries here
if (!WIN32)
    add_library(freenect SHARED IMPORTED)
    add_library(freenect2 SHARED IMPORTED)
    set_target_properties(freenect PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/freenect/libfreenect.so"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/inc/freenect")
    set_target_properties(freenect2 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/freenect2/libfreenect2.so"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/inc/freenect2")
endif()

target_compile_definitions(kijang
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
if (WIN32)
    target_link_libraries(kijang
      PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::SerialPort Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Widgets)
else()
    target_link_libraries(kijang
      PRIVATE freenect freenect2 Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::SerialPort Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Widgets)
endif()

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
