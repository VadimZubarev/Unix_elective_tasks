project (dbus_server)
find_package(PkgConfig REQUIRED) # Include functions provided by PkgConfig module.
pkg_check_modules(DBUS REQUIRED dbus-1) # This calls pkgconfig with appropriate arguments
include_directories(${DBUS_INCLUDE_DIRS})
link_directories(${DBUS_LIBRARY_DIRS})
add_executable(dbus_server main.cpp)
target_link_libraries(dbus_server ${DBUS_LIBRARIES})
