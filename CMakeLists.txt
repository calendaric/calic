project("calic" C) 
cmake_minimum_required(VERSION 3.0)

# Let's tell it a bit about our environment


# We declare a variable to select what to build
set(TARGET_GROUP release CACHE STRING "What Should I Build?")

add_subdirectory(src) 

if(TARGET_GROUP STREQUAL test)
  include(CTest)
  add_subdirectory(external) 
  add_subdirectory(test)
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}) 	
else() 
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
endif()
