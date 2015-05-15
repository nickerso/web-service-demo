project(web-service-demo)

cmake_minimum_required(VERSION 2.8)

# Add in the directory with the FindCellML module
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig/:$ENV{PKG_CONFIG_PATH}")

SET(CMAKE_FIND_LIBRARY_SUFFIXES_ORIGINAL ${CMAKE_FIND_LIBRARY_SUFFIXES})

SET(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX} ${CMAKE_FIND_LIBRARY_SUFFIXES})
FIND_PACKAGE(LibMicroHttpd REQUIRED)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(PLATFORM_LIBS "curl" "xml2")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(PLATFORM_LIBS "curl" "xml2" "pthread" "gcrypt" "gnutls" "ltdl" "dl" "db-5.3" "pcre" "mhash" "uuid" "m" "gmp" "xslt" "yajl")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Set build type default.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
	"Choose the build type. The options are: None (CMAKE_CXX_FLAGS or
	 CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel.")
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the build type. The options are: None (CMAKE_CXX_FLAGS or
  CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()

# Set compiler flags
if (WIN32)
else (WIN32)
	add_definitions("-std=c++11" "-Wall" "-Werror")
	set(PLATFORM_LIBS ${PLATFORM_LIBS} "m")
endif (WIN32)

# Global includes?
INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/jsoncpp
  ${libmicrohttpd_INCLUDE_DIRS}
)

# Sources
SET(SRCS
  src/demo.cpp
  src/libhttpd-utils.cpp
  src/api.cpp
  src/utils.cpp
)

set(EXECUTABLE_NAME "web-service-demo")
ADD_EXECUTABLE(${EXECUTABLE_NAME} MACOSX_BUNDLE ${SRCS})
TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
  ${libmicrohttpd_LIBRARIES}
)
# need this to get things working on Linux?
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    INSTALL_RPATH "\$ORIGIN"
)

# Install the built application and required libraries on the various platforms

set(library_dest_dir bin)
## Need to escape the CMAKE_INSTALL_PREFIX so that it is resolved appropriately when packaging
set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${EXECUTABLE_NAME}")
IF(APPLE)
  SET(library_dest_dir "${EXECUTABLE_NAME}.app/Contents/MacOS")
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/${EXECUTABLE_NAME}.app")
ENDIF(APPLE)
IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${EXECUTABLE_NAME}.exe")
ENDIF(WIN32)

# Install the application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
install(TARGETS ${EXECUTABLE_NAME}
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
)

# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# plugins are libraries that we want fixed up, but may not be found
# this is the CellML API libraries for libcsim and forcing non-standard system libraries for linux
# as new versions might not always be available
SET(PLUGINS)

# directories to look for dependencies
function(getSearchPath outDir lib)
    get_filename_component(dir ${lib} PATH)
    message(STATUS "${outDir} dir = ${dir}")
    set(${outDir} ${dir})
    message(STATUS "${outDir} dir = ${dir}")
endfunction(getSearchPath)

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
INSTALL(CODE "
    include(BundleUtilities)
    SET(BU_CHMOD_BUNDLE_ITEMS ON)
    fixup_bundle(\"${APPS}\" \"\${PLUGINS}\" \"${DIRS}\")
    " COMPONENT Runtime)

