# 获取当前目录名作为章节标识符（例如 "chapter08"）
get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 定义需要处理的子目录列表
set(SUB_DIRECTORIES "drill" "exer")

# 第一层循环：依次处理每个子目录
foreach(sub_dir ${SUB_DIRECTORIES})

    # 收集当前子目录下所有 .cpp 源文件
    file(GLOB ALL_SOURCES CONFIGURE_DEPENDS "src/${sub_dir}/*.cpp")

    # 初始化两个列表，用于区分"有 main"和"无 main"的文件
    set(MAIN_SOURCES "")
    set(SUPPORT_SOURCES "")

    # 第二层循环：读取每个源文件内容，判断是否包含 main 函数
    foreach(source_file ${ALL_SOURCES})

        # 将文件全部内容读入变量 FILE_CONTENT
        file(READ ${source_file} FILE_CONTENT)

        # 如果文件中包含 "int main"，视为可执行程序的入口文件
        if(FILE_CONTENT MATCHES "int main")
            list(APPEND MAIN_SOURCES ${source_file})
        # 否则视为实现/支持文件（如 my.cpp），不能单独运行
        else()
            list(APPEND SUPPORT_SOURCES ${source_file})
        endif()

    endforeach()

    # 如果存在支持文件，将它们打包成一个静态库
    if(SUPPORT_SOURCES)

        # 静态库命名规则：章节名_子目录名_lib（例如 chapter08_drill_lib）
        set(lib_name "${CURRENT_DIR_NAME}_${sub_dir}_lib")

        # 创建静态库目标
        add_library(${lib_name} STATIC ${SUPPORT_SOURCES})

        # 为静态库添加对应的 include 路径（PUBLIC 使链接它的目标也能继承此路径）
        target_include_directories(${lib_name} PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/include/${sub_dir}"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/drill"
        )

    endif()

    # 第三层循环：为每个含有 main 的文件创建独立的可执行目标
    foreach(source_file ${MAIN_SOURCES})

        # 提取不含扩展名的文件名作为目标名（例如 use.cpp → use）
        get_filename_component(target_name ${source_file} NAME_WE)

        # 创建可执行文件目标
        add_executable(${target_name} ${source_file})

        # 为可执行文件添加对应子目录的 include 路径
        # PRIVATE 表示此路径只对本目标有效，不向外传递
        target_include_directories(${target_name} PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/include/${sub_dir}"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/drill"   # 额外添加
        )

        # 如果本子目录存在静态库，则将其链接到当前可执行文件
        if(SUPPORT_SOURCES)
            target_link_libraries(${target_name} PRIVATE ${lib_name})
        endif()

        # 额外链接 drill 的库（exer 里的程序可能依赖 drill 的实现）
        set(drill_lib_name "${CURRENT_DIR_NAME}_drill_lib")
        if(TARGET ${drill_lib_name})
            target_link_libraries(${target_name} PRIVATE ${drill_lib_name})
        endif()

        # 设置可执行文件的输出路径，按章节和子目录分类存放
        # 例如：bin/chapter08/drill/use
        set_target_properties(${target_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CURRENT_DIR_NAME}/${sub_dir}"
        )

        # 将当前目标名追加到汇总列表，方便最后统一打印
        list(APPEND ALL_TARGETS ${target_name})

    endforeach()

    # 重置两个列表，防止上一个子目录的内容污染下一个子目录的处理
    unset(MAIN_SOURCES)
    unset(SUPPORT_SOURCES)

endforeach()

# 打印本章节所有成功配置的目标，方便在构建输出中确认
message(STATUS "[${CURRENT_DIR_NAME}] Found and configured targets: ${ALL_TARGETS}")