IF (${PROJECT_NAME}_DATAEXCHANGE)
    SET(brep_file_1 ${${PROJECT_NAME}_TEST_DATA_DIR}/test/data/brep/15_cylinder_head.brep)
    SET(brep_file_2 ${${PROJECT_NAME}_TEST_DATA_DIR}/test/data/brep/27_leverarm.brep)
    SET(brep_file_3 ${${PROJECT_NAME}_TEST_DATA_DIR}/test/data/brep/29_misc1.brep)
    SET(brep_file_4 ${${PROJECT_NAME}_TEST_DATA_DIR}/test/data/brep/31_misc2.brep)

    CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/BRepImport_test_config.h.cmake ${CMAKE_BINARY_DIR}/test_inc/BRepImport_test_config.h )
    INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/test_inc)

    ADD_OCE_TEST(BRepImport_test "TKernel;TKSTEP")
ENDIF (${PROJECT_NAME}_DATAEXCHANGE)
