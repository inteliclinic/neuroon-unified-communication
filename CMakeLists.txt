cmake_minimum_required (VERSION 3.6)

# projectname is the same as the main-executable
project(nuc)

execute_process(
  COMMAND git describe --tags --abbrev=5 --dirty
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

#add_definitions(-g)
add_definitions(-Wall)
add_definitions(-Wextra)
add_definitions(-O2)
add_definitions(-std=c11)
#add_definitions(-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/exportmap)
#SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/exportmap" )
#SET( CMAKE_SHARED_LINKER_FLAGS  "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/exportmap" )
FILE (GLOB_RECURSE sources
  src/*.c
  )

#add_executable(${PROJECT_NAME} test/main.c)
add_library (${PROJECT_NAME} SHARED ${sources})
add_executable (Test test/test.c)

target_include_directories (${PROJECT_NAME} PUBLIC API)
target_include_directories (${PROJECT_NAME} PUBLIC src)

target_include_directories (Test PUBLIC API)
target_include_directories (Test PUBLIC src)
target_link_libraries(Test ${PROJECT_NAME})


#add_custom_target(${PROJECT_NAME}-symlink ALL ln --force -s ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/${PROJECT_NAME} DEPENDS ${PROJECT_NAME})
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})
