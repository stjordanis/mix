set(exe_name test_mixal_parse)

target_collect_sources(${exe_name})

add_executable(${exe_name} ${${exe_name}_files})
target_include_directories(${exe_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_all_warnings(${exe_name} PRIVATE)

target_link_libraries(${exe_name} PRIVATE mixal_parse_lib)
target_link_libraries(${exe_name} PRIVATE GTest_Integrated)

target_install_binaries(${exe_name})

if (BUILD_SHARED_LIBS)
	target_compile_options(${exe_name} PRIVATE
		-DGTEST_LINKED_AS_SHARED_LIBRARY=1)
endif()
