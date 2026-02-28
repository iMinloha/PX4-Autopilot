#ifndef SERIAL_HEAD_H__
#define SERIAL_HEAD_H_

#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <px4_platform_common/Serial.hpp>
#include <drivers/drv_hrt.h>

using namespace device;

class SerialHello : public ModuleBase<SerialHello>, public ModuleParams, public px4::ScheduledWorkItem
{
public:
	SerialHello(const char *device);
	~SerialHello() override;

	/** @see ModuleBase */
	static int task_spawn(int argc, char *argv[]);

	/** @see ModuleBase */
	static int custom_command(int argc, char *argv[]);

	/** @see ModuleBase */
	static int print_usage(const char *reason = nullptr);

	/** @see ModuleBase::print_status() */
	int print_status() override;

private:
	void Run() override;
	Serial _serial;
};

#endif // SERIAL_HEAD_H__
