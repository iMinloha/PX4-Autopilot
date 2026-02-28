#include "serial_head.h"
#include <drivers/drv_hrt.h>

SerialHello::SerialHello(const char *device) :
	ModuleParams(nullptr),
	ScheduledWorkItem(MODULE_NAME, px4::serial_port_to_wq(device)),
	_serial(device)
{
}

SerialHello::~SerialHello()
{
}

int SerialHello::print_status()
{
	return 0;
}

int SerialHello::task_spawn(int argc, char *argv[]) {
	SerialHello *instance = new SerialHello("/dev/ttyS3");

	if (instance == nullptr) {
		PX4_ERR("alloc failed");
		return PX4_ERROR;
	}

	_task_id = task_id_is_work_queue;
	instance->ScheduleNow();
	_object.store(instance);
	return PX4_OK;
}

void SerialHello::Run() {
	if (should_exit()) {
		ScheduleClear();
		if (_serial.isOpen()) _serial.close();
		exit_and_cleanup();
		return;
	}

	if (!_serial.isOpen()) {
		if (!_serial.setBaudrate(115200)) {
			PX4_ERR("Error setting baudrate");
			px4_sleep(1);
			return;
		}

		if (!_serial.open()) {
			PX4_ERR("Error opening serial device");
			px4_sleep(1);
			return;
		}
		PX4_INFO("SerialHello serial opened sucessfully");
	}

	uint8_t buf[30] = {0};
	_serial.read((uint8_t *)buf, 30);
	if (buf[0] != 0) {
		uint8_t tx_buf[50] = {0};
		snprintf((char *)tx_buf, sizeof(tx_buf), "Received: %s\n", buf);
		size_t tx_buf_len = strlen((char *)tx_buf);
		_serial.write((void *)tx_buf, tx_buf_len);
	}

	const char *msg = "New Bee\n";
	size_t len = strlen(msg);
	ssize_t written = _serial.write((void *)msg, len);

	if (written != (ssize_t)len) PX4_ERR("Error writing");

	ScheduleDelayed(50000);
}

int SerialHello::custom_command(int argc, char *argv[])
{
	return print_usage("unknown command");
}

int SerialHello::print_usage(const char *reason)
{
	if (reason) PX4_WARN("%s\n", reason);

	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Description
This is a simple example module that opens a serial port and sends "Hello from PX4!" every second.
)DESCR_STR");

	PRINT_MODULE_USAGE_NAME("serial_hello", "example");
	PRINT_MODULE_USAGE_COMMAND("start");
	PRINT_MODULE_USAGE_DEFAULT_COMMANDS();

	return 0;
}
