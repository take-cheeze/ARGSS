# RUBY_EXECUTABLE
# RUBY_LIBRARY
# RUBY_INCLUDE_DIRS
# RUBY_INCLUDE_PATH
# RUBY_DEFINITIONS
# RUBY_VERSION

find_program(RUBY_EXECUTABLE ruby)
if(RUBY_EXECUTABLE)
  message(STATUS "ruby found: ${RUBY_EXECUTABLE}")

  # library
  execute_process(COMMAND "${RUBY_EXECUTABLE}"
    -rrbconfig -e "print RbConfig::CONFIG['libdir'] + '/' + RbConfig::CONFIG['LIBRUBY']"
    OUTPUT_VARIABLE RUBY_LIBRARY)
  message(STATUS "RUBY_LIBRARY: ${RUBY_LIBRARY}")

  # version
  execute_process(COMMAND "${RUBY_EXECUTABLE}"
    -rrbconfig -e "print RbConfig::CONFIG['ruby_version']"
    OUTPUT_VARIABLE RUBY_VERSION)
  message(STATUS "RUBY_VERSION: ${RUBY_VERSION}")

  # preprocessor
  execute_process(COMMAND "${RUBY_EXECUTABLE}"
    -rrbconfig -e "print RbConfig::CONFIG['CPPFLAGS']"
    OUTPUT_VARIABLE RUBY_DEFINITIONS)
  message(STATUS "RUBY_DEFINITIONS: ${RUBY_DEFINITIONS}")

  if(APPLE)
    list(APPEND RUBY_DEFINITIONS
      "-DHAVE_STRUCT_TIMESPEC" "-DHAVE_SIGNBIT")
  endif()

  # include directory
  execute_process(COMMAND "${RUBY_EXECUTABLE}"
    -rrbconfig -e "print RbConfig::CONFIG['rubyhdrdir']"
    OUTPUT_VARIABLE RUBY_INCLUDE_DIR)
  if(RUBY_INCLUDE_DIR MATCHES "nil") # ruby 1.8
    execute_process(COMMAND "${RUBY_EXECUTABLE}"
      -rrbconfig -e "print RbConfig::CONFIG['archdir']"
      OUTPUT_VARIABLE RUBY_INCLUDE_DIRS)
  else()
    execute_process(COMMAND "${RUBY_EXECUTABLE}"
      -rrbconfig -e "print RbConfig::CONFIG['archdir']"
      OUTPUT_VARIABLE RUBY_ARCH_INCLUDE_DIR)
    set(RUBY_INCLUDE_DIRS ${RUBY_INCLUDE_DIR} ${RUBY_ARCH_INCLUDE_DIR})
  endif()
  message(STATUS "RUBY_INCLUDE_DIRS: ${RUBY_INCLUDE_DIRS}")

  set(RUBY_INCLUDE_PATH ${RUBY_INCLUDE_DIRS})

  set(Ruby_FOUND TRUE)
else()
  set(Ruby_FOUND FALSE)
endif()
