# Try to find the TCLAP librairies
# TCLAP_FOUND - system has TCLAP lib
# TCLAP_INCLUDE_DIR - the TCLAP include directory

find_path(TCLAP_INCLUDE_DIR tclap/CmdLine.h
          /usr/local/include
          /usr/include
          ~/lib/tclap/include
         )
if(TCLAP_INCLUDE_DIR)
    SET(TCLAP_FOUND TRUE)
else()
    SET(TCLAP_FOUND FALSE)
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TCLAP DEFAULT_MSG TCLAP_INCLUDE_DIR)

mark_as_advanced(TCLAP_INCLUDE_DIR)