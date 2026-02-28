#include "hello_example.h"

#include <px4_platform_common/app.h>
#include <stdio.h>

int PX4_MAIN(int argc, char **argv) {
	printf("this is px4 main function\n");
	HelloExample hello;
	hello.main();
	HelloExample::appState.setRunning(false);
	printf("goodbye\n");
	return 0;
}
