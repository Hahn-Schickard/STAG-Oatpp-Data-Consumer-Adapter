cmake_minimum_required(VERSION 3.6)

#@+ ======================== User MODULE NAME configuration ============================
set(THIS Controller)
#@- =========================== END OF USER CONFIGURATION ===============================

set(MODULE ${PROJECT_NAME}_${THIS})

list(APPEND PUBLIC_INCLUDES_DIRS
            ${PUBLIC_INCLUDES_DIR}/
)

list(APPEND PRIVATE_INCLUDES_DIRS
            ${PRIVATE_INCLUDES_DIR}/
)

INCLUDE_DIRS_AND_FILES("${PUBLIC_INCLUDES_DIRS}" THIS_INCLUDES)
INCLUDE_DIRS_AND_FILES("${PRIVATE_INCLUDES_DIRS}" PRIVATE_INCLUDES)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_library(${MODULE})

target_sources(${MODULE} PRIVATE
                       ${sources_list}
                       ${THIS_INCLUDES}
                       ${PRIVATE_INCLUDES}
)
#@+ ======================== User DEPENDENCIES configuration ============================
target_link_libraries(${MODULE}
                      PRIVATE
                      oatpp::oatpp
                      Information_Model::Information_Model
                      oatpp::oatpp-swagger
)
#@- =========================== END OF USER CONFIGURATION ===============================

target_compile_definitions(${MODULE} PUBLIC OATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIR}/../bin/oatpp-swagger/res")
target_compile_features(${MODULE} PUBLIC cxx_std_17)

target_include_directories(${MODULE} INTERFACE
                           $<BUILD_INTERFACE:${PUBLIC_INCLUDES_DIR}>
                           $<BUILD_INTERFACE:${PRIVATE_INCLUDES_DIR}>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)

install(
    TARGETS ${MODULE}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    FILES ${THIS_INCLUDES}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)


