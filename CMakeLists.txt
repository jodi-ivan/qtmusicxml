cmake_minimum_required(VERSION 3.16)

project(qtmusicxml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Svg)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS SvgWidgets)
find_package(Qt6 REQUIRED COMPONENTS Gui)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appqtmusicxml
    main.cpp
)

qt_add_qml_module(appqtmusicxml
    URI qtmusicxml
    QML_FILES
        Main.qml
        RESOURCES lib/.gitkeep
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appqtmusicxml PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqtmusicxml
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 1. Include the directory containing your Go .h file
target_include_directories(appqtmusicxml PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_link_libraries(appqtmusicxml
    PRIVATE Qt6::Quick
)

target_link_libraries(appqtmusicxml PRIVATE Qt6::Gui)
target_link_libraries(appqtmusicxml PRIVATE Qt6::Svg)
target_link_libraries(appqtmusicxml PRIVATE Qt6::Widgets)
target_link_libraries(appqtmusicxml PRIVATE Qt6::SvgWidgets)
# 2. Link the .so file directly using its path
target_link_libraries(appqtmusicxml PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhymn_renderer.so)


# 3. Configure RPATH so the executable can find the library at runtime
set(CMAKE_BUILD_RPATH "$ORIGIN/lib")
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")

install(TARGETS appqtmusicxml
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)











