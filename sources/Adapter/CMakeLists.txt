cmake_minimum_required(VERSION 3.6)

#@+ ======================== User MODULE NAME configuration ============================
set(THIS Adapter)
#@- =========================== END OF USER CONFIGURATION ===============================

set(MODULE ${PROJECT_NAME}_${THIS})
list(APPEND THIS_INCLUDES_DIRS
            ${INCLUDES_DIR}
)

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.?pp")


add_library(${MODULE})

target_sources(${MODULE} PRIVATE
                       ${sources_list}
                       ${THIS_INCLUDES} 
                       
)


#@+ ======================== User DEPENDENCIES configuration ============================
target_link_libraries(${MODULE}
PUBLIC
                       #Private dependencies
                      Data_Consumer_Adapter_Interface::Data_Consumer_Adapter_Interface
                     ${PROJECT_NAME}_Beispiel
                        #Public dependencies
)
#@- =========================== END OF USER CONFIGURATION ===============================


target_compile_features(${MODULE} PUBLIC cxx_std_17)

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


