px4_add_module(
	MODULE drivers__pwm_run
	MAIN pwm_run
	COMPILE_FLAGS
	SRCS
		pwm_run_main.cpp
	DEPENDS
		px4_work_queue
		arch_io_pins
		mixer_module
	)
