cmake_minimum_required(VERSION 3.23)

project(Notifications)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")

add_executable(main main.cpp)

add_subdirectory(alsaaudio)
add_subdirectory(notification)
add_subdirectory(socket)


find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET libnotify alsa fmt glib-2.0 gsound)
target_link_libraries(main PkgConfig::deps alsaaudio socket)

