# - Try to find libmicrohttpd
# Once done, this will define
#
#  libmicrohttpd_FOUND - system has libmicrohttpd
#  libmicrohttpd_INCLUDE_DIRS - the libmicrohttpd include directories
#  libmicrohttpd_LIBRARIES - link these to use libmicrohttpd

include(LibFindMacros)

# Dependencies
#libfind_package(Magick++ Magick)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(libmicrohttpd_PKGCONF libmicrohttpd)

# Include dir
find_path(libmicrohttpd_INCLUDE_DIR
  NAMES microhttpd.h
  PATHS ${libmicrohttpd_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(libmicrohttpd_LIBRARY
  NAMES microhttpd
  PATHS ${libmicrohttpd_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(libmicrohttpd_PROCESS_INCLUDES libmicrohttpd_INCLUDE_DIR)
set(libmicrohttpd_PROCESS_LIBS libmicrohttpd_LIBRARY)
libfind_process(libmicrohttpd)