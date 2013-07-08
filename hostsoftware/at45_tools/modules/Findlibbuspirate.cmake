# -*- mode: cmake; -*-
# - Try to find libbuspirate include dirs and libraries
# Usage of this module as follows:
# This file defines:
# * LIBBUSPIRATE_FOUND if protoc was found
# * LIBBUSPIRATE_LIBRARY The lib to link to (currently only a static unix lib, not
# portable)
# * LIBBUSPIRATE_INCLUDE The include directories for libbuspirate.

cmake_policy(PUSH)
# when crosscompiling import the executable targets from a file
if(CMAKE_CROSSCOMPILING)
#  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY FIRST)
#  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE FIRST)
endif(CMAKE_CROSSCOMPILING)


message(STATUS "FindLibBuspirate check")

IF (NOT WIN32)
  include(FindPkgConfig)
  if ( PKG_CONFIG_FOUND )
    pkg_check_modules (PC_LIBBUSPIRATE libbuspirate)
    set(LIBBUSPIRATE_DEFINITIONS ${PC_LIBBUSPIRATE_CFLAGS_OTHER})
  endif(PKG_CONFIG_FOUND)
endif (NOT WIN32)

#
# set defaults
SET(_libbuspirate_HOME "/usr/local")
SET(_libbuspirate_INCLUDE_SEARCH_DIRS
  ${CMAKE_INCLUDE_PATH}
  /usr/local/include/buspirate
  /usr/include/buspirate
  )

SET(_libbuspirate_LIBRARIES_SEARCH_DIRS
  ${CMAKE_LIBRARY_PATH}
  /usr/local/lib
  /usr/lib
  )

##
if( "${LIBBUSPIRATE_HOME}" STREQUAL "")
  if("" MATCHES "$ENV{LIBBUSPIRATE_HOME}")
    message(STATUS "LIBBUSPIRATE_HOME env is not set, setting it to /usr/local")
    set (LIBBUSPIRATE_HOME ${_libbuspirate_HOME})
  else("" MATCHES "$ENV{LIBBUSPIRATE_HOME}")
    set (LIBBUSPIRATE_HOME "$ENV{LIBBUSPIRATE_HOME}")
  endif("" MATCHES "$ENV{LIBBUSPIRATE_HOME}")
endif( "${LIBBUSPIRATE_HOME}" STREQUAL "")
message(STATUS "Looking for libbuspirate in ${LIBBUSPIRATE_HOME}")

##
IF( NOT ${LIBBUSPIRATE_HOME} STREQUAL "" )
    SET(_libbuspirate_INCLUDE_SEARCH_DIRS
      ${LIBBUSPIRATE_HOME}/include/buspirate ${_libbuspirate_INCLUDE_SEARCH_DIRS})
    SET(_libbuspirate_LIBRARIES_SEARCH_DIRS ${LIBBUSPIRATE_HOME}/lib ${_libbuspirate_LIBRARIES_SEARCH_DIRS})
    SET(_libbuspirate_HOME ${LIBBUSPIRATE_HOME})
ENDIF( NOT ${LIBBUSPIRATE_HOME} STREQUAL "" )

IF( NOT $ENV{LIBBUSPIRATE_INCLUDEDIR} STREQUAL "" )
  SET(_libbuspirate_INCLUDE_SEARCH_DIRS $ENV{LIBBUSPIRATE_INCLUDEDIR} ${_libbuspirate_INCLUDE_SEARCH_DIRS})
ENDIF( NOT $ENV{LIBBUSPIRATE_INCLUDEDIR} STREQUAL "" )

IF( NOT $ENV{LIBBUSPIRATE_LIBRARYDIR} STREQUAL "" )
  SET(_libbuspirate_LIBRARIES_SEARCH_DIRS $ENV{LIBBUSPIRATE_LIBRARYDIR} ${_libbuspirate_LIBRARIES_SEARCH_DIRS})
ENDIF( NOT $ENV{LIBBUSPIRATE_LIBRARYDIR} STREQUAL "" )

IF( LIBBUSPIRATE_HOME )
  SET(_libbuspirate_INCLUDE_SEARCH_DIRS ${LIBBUSPIRATE_HOME}/include ${_libbuspirate_INCLUDE_SEARCH_DIRS})
  SET(_libbuspirate_LIBRARIES_SEARCH_DIRS ${LIBBUSPIRATE_HOME}/lib ${_libbuspirate_LIBRARIES_SEARCH_DIRS})
  SET(_libbuspirate_HOME ${LIBBUSPIRATE_HOME})
ENDIF( LIBBUSPIRATE_HOME )

message(STATUS "root_path: ${CMAKE_FIND_ROOT_PATH}")

find_path(LIBBUSPIRATE_INCLUDE_DIR buspirate.h
  HINTS
     ${_libbuspirate_INCLUDE_SEARCH_DIRS}
     ${PC_LIBBUSPIRATE_INCLUDEDIR}
     ${PC_LIBBUSPIRATE_INCLUDE_DIRS}
    ${CMAKE_INCLUDE_PATH}
)

# locate the library
SET(LIBBUSPIRATE_LIBRARY_NAMES ${LIBBUSPIRATE_LIBRARY_NAMES} libbuspirate.a)
find_library(LIBBUSPIRATE_LIBRARY NAMES ${LIBBUSPIRATE_LIBRARY_NAMES}
  HINTS
    ${_libbuspirate_LIBRARIES_SEARCH_DIRS}
    ${PC_LIBBUSPIRATE_LIBDIR}
    ${PC_LIBBUSPIRATE_LIBRARY_DIRS}
)


include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Libklio  DEFAULT_MSG LIBBUSPIRATE_LIBRARY_NAMES LIBBUSPIRATE_INCLUDE_DIR)

# if the include and the program are found then we have it
IF(LIBBUSPIRATE_INCLUDE_DIR AND LIBBUSPIRATE_LIBRARY)
  SET(LIBBUSPIRATE_FOUND "YES")
ENDIF(LIBBUSPIRATE_INCLUDE_DIR AND LIBBUSPIRATE_LIBRARY)

# if( NOT WIN32)
#   list(APPEND LIBBUSPIRATE_LIBRARY "-lrt")
# endif( NOT WIN32)

MARK_AS_ADVANCED(
  LIBBUSPIRATE_FOUND
  LIBBUSPIRATE_LIBRARY
  LIBBUSPIRATE_INCLUDE_DIR
)
if(CMAKE_CROSSCOMPILING)
#  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif(CMAKE_CROSSCOMPILING)
cmake_policy(POP)
