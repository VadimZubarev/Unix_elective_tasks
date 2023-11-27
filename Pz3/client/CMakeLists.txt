project (dbus_client)

find_package(PkgConfig REQUIRED) # Include functions provided by PkgConfig module.

pkg_check_modules(DBUS REQUIRED dbus-1) # This calls pkgconfig with appropriate arguments
# Use results of pkg_check_modules() call.
include_directories(${DBUS_INCLUDE_DIRS})
link_directories(${DBUS_LIBRARY_DIRS})


add_executable(dbus_client main.cpp)



target_link_libraries(dbus_client ${DBUS_LIBRARIES})
