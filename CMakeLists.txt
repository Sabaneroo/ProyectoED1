cmake_minimum_required(VERSION 3.5)

project(ProyectoED1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        ListaProductos.h
        ListaProductos.cpp
        Alistos_Thread.cpp
        Alistos_Thread.h
        Alistador_Thread.cpp
        Alistador_Thread.h
        Alistados_thread.h
        Alistados_thread.cpp
        ListaClientes.h
        ListaClientes.cpp
        ColaFabricaciones.cpp
        ColaFabricaciones.h
        BalanceadorThread.h
        BalanceadorThread.cpp
        Fabrica.h
        Fabrica.cpp
        Empacador.cpp
        Empacador.h
        Facturar_thread.cpp
        Facturar_thread.h
        Facturador_thread.h
        Facturador_thread.cpp
        Almacen.h
        Almacen.cpp
        Cliente.h
        Cliente.cpp
        Producto.h
        Pedidos.h
        Pedidos.cpp
        Pedidos_Thread.h
        Pedidos_Thread.cpp
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ProyectoED1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ProyectoED1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ProyectoED1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ProyectoED1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ProyectoED1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(ProyectoED1 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ProyectoED1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ProyectoED1)
endif()
