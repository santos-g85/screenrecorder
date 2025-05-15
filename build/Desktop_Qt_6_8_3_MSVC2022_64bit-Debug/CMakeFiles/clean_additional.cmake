# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\screen_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\screen_autogen.dir\\ParseCache.txt"
  "screen_autogen"
  )
endif()
