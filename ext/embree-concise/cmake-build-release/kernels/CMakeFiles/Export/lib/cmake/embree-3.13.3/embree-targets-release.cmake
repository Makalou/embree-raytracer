#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "embree" for configuration "Release"
set_property(TARGET embree APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(embree PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libembree3.3.13.3.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libembree3.3.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS embree )
list(APPEND _IMPORT_CHECK_FILES_FOR_embree "${_IMPORT_PREFIX}/lib/libembree3.3.13.3.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
