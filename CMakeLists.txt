cmake_minimum_required(VERSION 3.22)

project(
  simple-timer
  VERSION 0.0.1
  LANGUAGES C)

set(timer_name "${PROJECT_NAME}")
set(master_name "${PROJECT_NAME}-master")

file(GLOB_RECURSE source_files_timer ${CMAKE_HOME_DIRECTORY}/src/timer/*.c)
add_executable(${timer_name} ${source_files_timer})
target_include_directories(${timer_name}
                           PUBLIC ${CMAKE_HOME_DIRECTORY}/include/timer)

file(GLOB_RECURSE source_files_master ${CMAKE_HOME_DIRECTORY}/src/master/*.c)
add_executable(${master_name} ${source_files_master})
target_include_directories(${timer_name}
                           PUBLIC ${CMAKE_HOME_DIRECTORY}/include/master)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  add_custom_target(
    copy_compile_commands
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Copying compile_commands.json to ${CMAKE_SOURCE_DIR}")

  add_dependencies(${PROJECT_NAME} copy_compile_commands)
endif()
