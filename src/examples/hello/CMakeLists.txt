px4_add_module(
	MODULE examples__hello
	MAIN hello
	COMPILE_FLAGS
	SRCS
		hello_main.cpp
		hello_start.cpp
		hello_example.cpp
	DEPENDS
	)
