cmake_minimum_required(VERSION 3.6)

#@+ ======================== User MODULE NAME configuration ============================
set(THIS Beispiel)
#@- =========================== END OF USER CONFIGURATION ===============================

set(MODULE ${PROJECT_NAME}_${THIS})



file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/src/*.?pp")
file(GLOB controller_list "${CMAKE_CURRENT_LIST_DIR}/src/controller/*.?pp")
file(GLOB dto_list "${CMAKE_CURRENT_LIST_DIR}/src/dto/*.?pp")

add_executable(${MODULE})

target_sources(${MODULE} PRIVATE
                       ${sources_list}
                       ${controller_list}
                       ${dto_list}
)
message(info,"OatppResources:${OATPP_SWAGGER_RES_PATH}")

#@+ ======================== User DEPENDENCIES configuration ============================
target_link_libraries(${MODULE}
                      PRIVATE #Private dependencies
                      oatpp::oatpp
                      oatpp::oatpp-swagger

                      PUBLIC  #Public dependencies
)
#@- =========================== END OF USER CONFIGURATION ===============================

add_definitions( 
  -DOATPP_SWAGGER_RES_PATH="/home/robin/.conan2/p/oatpp32bb770e16c3c/s/src/res"
)
target_compile_features(${MODULE} PUBLIC cxx_std_17)

if(WIN32)
    add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        -t $<TARGET_FILE_DIR:${MODULE_NAME}>
                        $<TARGET_RUNTIME_DLLS:${MODULE_NAME}>
                        COMMAND_EXPAND_LISTS
    )
    # Workaround to get conan .dll files into build
    foreach(directory ${CONAN_RUNTIME_LIB_DIRS})
        file(GLOB dir_dependencies "${directory}/*.dll")
        if(dir_dependencies)
            list(APPEND dependencies "${dir_dependencies}")
        endif()
    endforeach()
    add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        -t $<TARGET_FILE_DIR:${MODULE_NAME}>
                        ${dependencies}
                        COMMAND_EXPAND_LISTS
    )
endif()
