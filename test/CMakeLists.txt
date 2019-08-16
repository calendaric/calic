add_executable(calendar_test
    calendar_test.c
)
 
target_link_libraries(calendar_test
    calendar
    Unity
)
 
add_test(calendar_test calendar_test
)
