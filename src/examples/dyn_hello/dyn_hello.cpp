#include <px4_platform_common/log.h>

extern "C" __EXPORT int dyn_hello_main(int argc, char *argv[]);
int dyn_hello_main(int argc, char *argv[]) {
	PX4_INFO("Hello, I am a example module.");

	PX4_INFO("Argv:");

	for (int i = 0; i < argc; ++i)
		PX4_INFO("  %d: %s", i, argv[i]);

	return 0;
}
