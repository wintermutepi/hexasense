# -*- mode: cmake; -*-

file(GLOB_RECURSE h_files "*.hpp,*.h")
file(GLOB_RECURSE cpp_files "*.cpp")
file(GLOB_RECURSE c_files "*.c")
set(at45_imguploader_src "at45_imguploader.cpp")

include_directories(
  ${CMAKE_SOURCE_DIR} 
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_BINARY_DIR}/libat45tools
  ${Boost_INCLUDE_DIRS}
  ${LIBBUSPIRATE_INCLUDE_DIR}
)
#link_directories(
#  #  ${SQLITE3_STATIC_LIBRARY_DIRS}
#  )

add_executable(at45_imguploader ${at45_imguploader_src})

# Link the executables
target_link_libraries(at45_imguploader
  at45tools
  pthread
  ${Boost_LIBRARIES}
  ${LIBBUSPIRATE_LIBRARY}
)
# add programs to the install target 
INSTALL(PROGRAMS 
  ${CMAKE_CURRENT_BINARY_DIR}/at45_imguploader
  DESTINATION bin)

