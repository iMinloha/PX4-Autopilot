#include "hello_example.h"
#include <px4_platform_common/time.h>
#include <unistd.h>
#include <stdio.h>

px4::AppState HelloExample::appState;

int HelloExample::main() {
	int i = 0;
	appState.setRunning(true);
	while (appState.isRunning() && i < 5) {
		px4_sleep(2);
		printf("  Doing work...\n");
		i++;
	}
	return 0;
}
