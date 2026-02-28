#include "serial_head.h"

extern "C" __EXPORT int serial_hello_main(int argc, char *argv[]);

int serial_hello_main(int argc, char *argv[])
{
	return SerialHello::main(argc, argv);
}
