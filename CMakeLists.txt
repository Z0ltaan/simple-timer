cmake_minimum_required(VERSION 3.22)

project(
  simple-timer
  VERSION 0.3.0
  LANGUAGES C)

set(timer_name "${PROJECT_NAME}")
set(master_name "${PROJECT_NAME}-master")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

file(GLOB_RECURSE miniaudio_files ${CMAKE_HOME_DIRECTORY}/extern/miniaudio/*.c)
add_library(miniaudio ${miniaudio_files})
target_link_libraries(
  miniaudio
  PUBLIC m
  PUBLIC pthread
  PUBLIC dl)
target_include_directories(miniaudio PUBLIC ${CMAKE_HOME_DIRECTORY}/extern)

file(GLOB_RECURSE source_files_timer ${CMAKE_HOME_DIRECTORY}/src/timer/*.c)
add_executable(${timer_name} ${source_files_timer})
target_include_directories(${timer_name}
                           PUBLIC ${CMAKE_HOME_DIRECTORY}/include/timer)
target_link_libraries(${timer_name} PRIVATE PkgConfig::GTK4 miniaudio)

file(GLOB_RECURSE source_files_master ${CMAKE_HOME_DIRECTORY}/src/master/*.c)
add_executable(${master_name} ${source_files_master})
target_include_directories(${master_name}
                           PUBLIC ${CMAKE_HOME_DIRECTORY}/include/master)
target_link_libraries(${master_name} PRIVATE PkgConfig::GTK4)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  add_custom_target(
    copy_compile_commands
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_HOME_DIRECTORY}/compile_commands.json
    COMMENT "Copying compile_commands.json to ${CMAKE_HOME_DIRECTORY}")

  add_dependencies(${timer_name} copy_compile_commands)
endif()
