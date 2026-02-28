#include "hello_example.h"

#include <px4_platform_common/log.h>
#include <px4_platform_common/tasks.h>
#include <stdio.h>
#include <string.h>
#include <sched.h>

// Linux use pid as task handle
static int daemon_task;

extern "C" __EXPORT int hello_main(int argc, char *argv[]);

int hello_main(int argc, char *argv[]) {
	if (argc < 2) {
		PX4_WARN("usage: hello {start|stop|status}\n");
		return 1;
	}

	if (!strcmp(argv[1], "start")) {
		if (HelloExample::appState.isRunning()) {
			PX4_INFO("already running\n");
			return 0;
		}

		daemon_task = px4_task_spawn_cmd("hello",
						 SCHED_DEFAULT,
						 SCHED_PRIORITY_MAX - 5,
						 2000,
						 PX4_MAIN,
						 (argv) ? (char *const *)&argv[2] : (char *const *)nullptr);

		HelloExample::appState.setRunning(true);
		return 0;
	}

	if (!strcmp(argv[1], "stop")) {
		HelloExample::appState.setRunning(false);
		return 0;
	}

	if (!strcmp(argv[1], "status")) {
		if (HelloExample::appState.isRunning()) {
			PX4_INFO("is running\n");
		} else {
			PX4_INFO("not started\n");
		}

		return 0;
	}

	PX4_WARN("usage: hello_main {start|stop|status}\n");
	return 1;
}
