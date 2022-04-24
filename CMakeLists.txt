project(Notifications)
set(CMAKE_CXX_STANDARD 20)
add_executable(main main.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET giomm-2.4 gtkmm-2.4)
target_link_libraries(main PkgConfig::deps)
