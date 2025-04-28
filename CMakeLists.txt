cmake_minimum_required(VERSION 3.16)
project(FruitBoxLocal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

file(GLOB SRC CONFIGURE_DEPENDS src/*.cpp)
add_executable(fruit_box_local ${SRC})
target_link_libraries(fruit_box_local PRIVATE sfml-graphics sfml-window sfml-system)

# -------- resources ----------------------------------------------------
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

# -------- CPACK (one-click installers) ---------------------------------
include(CPack)
set(CPACK_PACKAGE_VENDOR "YourName")
set(CPACK_PACKAGE_CONTACT "you@example.com")
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 0)

#  Windows: NSIS .exe  (needs NSIS installed)
set(CPACK_GENERATOR "NSIS")
set(CPACK_NSIS_DISPLAY_NAME "Fruit Box Local")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

#  macOS: Drag-n-Drop .dmg
if(APPLE)
  set(CPACK_GENERATOR "DragNDrop")
  set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/resources/dmg_bg.png")
endif()

#  Linux: TGZ + AppImage
if(UNIX AND NOT APPLE)
  set(CPACK_GENERATOR "DEB;TGZ")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "YourName")
endif()

