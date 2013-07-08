# -*- mode: cmake; -*-

file(GLOB hdrs *.h *.hpp)
install(FILES ${hdrs} DESTINATION include/libat45tools)

configure_file(config.h.in ${CMAKE_BINARY_DIR}/libat45tools/config.h)
install(FILES ${CMAKE_BINARY_DIR}/libat45tools/config.h 
  DESTINATION include/libat45tools)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)
file(GLOB all_at45tools_src *.cpp *.hpp)
set(at45tools_src ${all_at45tools_src})
ADD_LIBRARY(at45tools ${at45tools_src})

#target_link_libraries(at45tools ${Boost_IOSTREAMS_LIBRARY})

if(BUILD_SHARED_LIBS)
  install(TARGETS at45tools LIBRARY DESTINATION lib)
else(BUILD_SHARED_LIBS)
  install(TARGETS at45tools ARCHIVE DESTINATION lib)
endif(BUILD_SHARED_LIBS)

