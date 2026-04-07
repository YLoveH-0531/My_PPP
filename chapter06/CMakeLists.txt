get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 定义你的代码存放的子文件夹名字
set(SUB_DIRECTORIES "drill" "exer")

# 第一层循环：遍历每一个子文件夹 (先处理 drill，再处理 exer)
foreach(sub_dir ${SUB_DIRECTORIES})
    
    # 找当前子文件夹下的所有 .cpp 文件
    file(GLOB SOURCES CONFIGURE_DEPENDS "src/${sub_dir}/*.cpp")
    
    # 第二层循环：遍历找到的每一个 .cpp 文件
    foreach(source_file ${SOURCES})
        
        # 提取文件名作为目标名 (例如 exer_4_1)
        get_filename_component(target_name ${source_file} NAME_WE)
        
        # 创建可执行程序
        add_executable(${target_name} ${source_file})
        
        # 设置输出路径，按文件夹分类存放
        # drill 的程序会去 bin/ch04/drill/，exer 的程序会去 bin/ch04/exer/
        set_target_properties(${target_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CURRENT_DIR_NAME}/${sub_dir}"
        )

        # 把找到的目标名字加入到列表中，方便最后打印
        list(APPEND ALL_TARGETS ${target_name})
        
    endforeach()
endforeach()

# 打印一下找到的目标，方便在 Output 窗口确认
message(STATUS "[${CURRENT_DIR_NAME}] Found and configured targets: ${ALL_TARGETS}")