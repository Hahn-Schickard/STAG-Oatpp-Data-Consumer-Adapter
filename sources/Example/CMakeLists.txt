cmake_minimum_required(VERSION 3.6) 

#@+ ======================== User MODULE NAME configuration ============================
set(THIS Example)
#@- =========================== END OF USER CONFIGURATION ===============================

set(MODULE ${PROJECT_NAME}_${THIS})

list(APPEND THIS_INCLUDES_DIRS
            ${INCLUDES_DIR}
)

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_executable(${MODULE})              

target_sources(${MODULE} PRIVATE 
                       ${sources_list}
                       ${THIS_INCLUDES} 
)

target_link_libraries(${MODULE} 
                      ${PROJECT_NAME}_Adapter
)

target_compile_features(${MODULE} PUBLIC cxx_std_17)

if(WIN32)
    add_custom_command(TARGET ${MODULE} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        -t $<TARGET_FILE_DIR:${MODULE}>
                        $<TARGET_RUNTIME_DLLS:${MODULE}>
                        COMMAND_EXPAND_LISTS
    )
    # Workaround to get conan .dll files into build
    foreach(directory ${CONAN_RUNTIME_LIB_DIRS})
        file(GLOB dir_dependencies "${directory}/*.dll")
        if(dir_dependencies)
            list(APPEND dependencies "${dir_dependencies}")
        endif()
    endforeach()
    add_custom_command(TARGET ${MODULE} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        -t $<TARGET_FILE_DIR:${MODULE}>
                        ${dependencies}
                        COMMAND_EXPAND_LISTS
    )
endif()

