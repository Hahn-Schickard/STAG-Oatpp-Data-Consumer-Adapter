cmake_minimum_required(VERSION 3.6) 

set(THIS Runner)
list(APPEND THIS_INCLUDES_DIRS
            ${INCLUDES_DIR}
)

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_executable(${THIS})              

target_sources(${THIS} PRIVATE 
                       ${sources_list}
                       ${THIS_INCLUDES} 
)

target_link_libraries(${THIS} 
                      ${PROJECT_NAME}_Beispiel
                      ${PROJECT_NAME}_Adapter
)

target_compile_features(${THIS} PUBLIC cxx_std_17)
