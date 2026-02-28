#ifndef _PWM_RUN_
#define _PWM_RUN_

#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/module.h>
#include <board_config.h>
#include <drivers/drv_hrt.h>
#include <drivers/drv_pwm_output.h>
#include <lib/mathlib/mathlib.h>
#include <lib/mixer_module/mixer_module.hpp>
#include <lib/perf/perf_counter.h>
#include <px4_arch/io_timer.h>
#include <px4_platform_common/getopt.h>
#include <px4_platform_common/log.h>
#include <uORB/Subscription.hpp>
#include <uORB/topics/parameter_update.h>

class PWMDriver : public ModuleBase<PWMDriver>, public ModuleParams, public px4::ScheduledWorkItem
{
public:
    explicit PWMDriver(const uint8_t channel = 0);

	~PWMDriver() override;

	/** @see ModuleBase */
	static int task_spawn(int argc, char *argv[]);

	/** @see ModuleBase */
	static int custom_command(int argc, char *argv[]);

	/** @see ModuleBase */
	static int print_usage(const char *reason = nullptr);

	/** @see ModuleBase::print_status() */
	int print_status() override;


private:
    int Init();

	void Run() override;

    uint8_t _channel;
    uint32_t _mask;
    bool _initialized{false};
    bool _armed{false};
};

#endif
