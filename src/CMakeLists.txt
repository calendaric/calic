
file(GLOB SOURCES ./*.c)
if(TARGET_GROUP STREQUAL test)
  add_library(calendar STATIC calendar.c)
  target_include_directories(calendar PUBLIC ${CMAKE_CURRENT_LIST_DIR})  
else()
  add_executable(calic ${SOURCES})
  target_include_directories(calic PUBLIC ${CMAKE_CURRENT_LIST_DIR})  
endif()
