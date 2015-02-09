cmake_minimum_required (VERSION 2.8.11)
project (runas C)

include (CheckFunctionExists)

check_function_exists(setresgid HAVE_SETRESGID)
check_function_exists(setregid HAVE_SETREGID)
check_function_exists(setresuid HAVE_SETRESUID)
check_function_exists(setreuid HAVE_SETREUID)
check_function_exists(initgroups HAVE_INITGROUPS)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(runas src/main.c)
