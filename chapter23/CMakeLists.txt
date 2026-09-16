get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(SUB_DIRECTORIES "drill" "exer")

add_compile_definitions(DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")

foreach(sub_dir ${SUB_DIRECTORIES})

    file(GLOB ALL_SOURCES CONFIGURE_DEPENDS "src/${sub_dir}/*.cpp")

    set(MAIN_SOURCES "")
    set(SUPPORT_SOURCES "")

    foreach(source_file ${ALL_SOURCES})
        file(READ ${source_file} FILE_CONTENT)
        if(FILE_CONTENT MATCHES "int main")
            list(APPEND MAIN_SOURCES ${source_file})
        else()
            list(APPEND SUPPORT_SOURCES ${source_file})
        endif()
    endforeach()

    if(SUPPORT_SOURCES)
        set(lib_name "${CURRENT_DIR_NAME}_${sub_dir}_lib")
        add_library(${lib_name} STATIC ${SUPPORT_SOURCES})
        target_include_directories(${lib_name} PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/include/${sub_dir}"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/drill"
        )
    endif()

    foreach(source_file ${MAIN_SOURCES})
        get_filename_component(target_name ${source_file} NAME_WE)
        add_executable(${target_name} ${source_file})
        target_include_directories(${target_name} PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/include/${sub_dir}"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/drill"
        )
        if(SUPPORT_SOURCES)
            target_link_libraries(${target_name} PRIVATE ${lib_name})
        endif()
        set(drill_lib_name "${CURRENT_DIR_NAME}_drill_lib")
        if(TARGET ${drill_lib_name})
            target_link_libraries(${target_name} PRIVATE ${drill_lib_name})
        endif()
        set_target_properties(${target_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CURRENT_DIR_NAME}/${sub_dir}"
        )
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
        list(APPEND ALL_TARGETS ${target_name})
    endforeach()

    unset(MAIN_SOURCES)
    unset(SUPPORT_SOURCES)

endforeach()

message(STATUS "[${CURRENT_DIR_NAME}] Found and configured targets: ${ALL_TARGETS}")
