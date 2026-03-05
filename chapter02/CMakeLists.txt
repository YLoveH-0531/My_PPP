add_executable(Hello_world src/drill/Hello_world.cpp)
add_executable(exer_2_1 src/exercises/exer_2_1.cpp)

set_target_properties(Hello_world 
                     PROPERTIES 
                     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ch02)
set_target_properties(exer_2_1 
                     PROPERTIES 
                     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ch02)