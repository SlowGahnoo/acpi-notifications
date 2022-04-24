project(Notifications)
set(CMAKE_CXX_STANDARD 20)
add_executable(main main.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET libnotify alsa fmt)
target_link_libraries(main PkgConfig::deps)
