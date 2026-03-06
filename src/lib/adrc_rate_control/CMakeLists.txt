px4_add_library(ADRC_RateControl
	inc/library.h
	src/library.cpp
	ADRC_Control.cpp
	ADRC_Control.h
)

target_compile_options(ADRC_RateControl PRIVATE ${MAX_CUSTOM_OPT_LEVEL})
target_include_directories(ADRC_RateControl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ADRC_RateControl PRIVATE mathlib)

px4_add_unit_gtest(SRC ADRC_Control_test.cpp LINKLIBS ADRC_RateControl)
