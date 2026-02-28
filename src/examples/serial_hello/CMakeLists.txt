px4_add_module(
	MODULE examples__serial_hello
	MAIN serial_hello
	COMPILE_FLAGS
	SRCS
		serial_hello_main.cpp
		serial_head.cpp
	DEPENDS
		px4_work_queue
	)
