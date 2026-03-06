#include "pwm_run_main.hpp"

PWMDriver::PWMDriver(const uint8_t channel) :
	ModuleParams(nullptr),
	ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::hp_default),
    _channel(channel),
    _mask(1 << channel)
{
}

PWMDriver::~PWMDriver() {
    up_pwm_servo_arm(false, _mask);
    up_pwm_servo_deinit(_mask);
}

int PWMDriver::Init() {
    int ret = up_pwm_servo_init(_mask);
    if (ret < 0) {
        PX4_ERR("PWM init failed: %d", ret);
        return ret;
    }

    // if the channel is in multiple groups, we need to set frequency for all groups
    uint32_t group_mask = 0;
    for (unsigned group = 0; group < MAX_IO_TIMERS; group++) {
        uint32_t channel_mask = up_pwm_servo_get_rate_group(group);
        if (channel_mask & _mask) {
            group_mask |= (1 << group);
        }
    }

    if (group_mask == 0) {
        PX4_ERR("Channel %d not found in any group", _channel);
        return -1;
    }

    // use 100Hz for testing, the default is 50Hz
    if ((ret = up_pwm_servo_set_rate_group_update(group_mask, 100)) != 0) {
        PX4_ERR("Set frequency failed: %d", ret);
        return ret;
    }

    up_pwm_servo_arm(true, _mask);

    up_pwm_servo_set(_channel, 2000);
    up_pwm_update(_mask);

    // read now frequency and CCR for debugging
    for (unsigned group = 0; group < MAX_IO_TIMERS; group++) {
        uint32_t channel_mask = up_pwm_servo_get_rate_group(group);
        if (channel_mask & _mask) {
            unsigned ccr = up_pwm_servo_get(_channel);
            PX4_INFO("Group %d current CCR: %u", group, ccr);
        }
    }

    PX4_INFO("PWM channel %d @ 100Hz, group_mask=0x%x", _channel, (unsigned) group_mask);

    return PX4_OK;
}


int PWMDriver::print_status()
{
	return 0;
}

int PWMDriver::task_spawn(int argc, char *argv[]) {
	PWMDriver *instance = new PWMDriver(4);

	if (instance == nullptr) {
		PX4_ERR("alloc failed");
		return PX4_ERROR;
	}

    int ret = instance->Init();
    if (ret != PX4_OK) {
        delete instance;
        return ret;
    }

	_task_id = task_id_is_work_queue;
	instance->ScheduleNow();
	_object.store(instance);
	return PX4_OK;
}


int PWMDriver::custom_command(int argc, char *argv[])
{
	return print_usage("unknown command");
}

int PWMDriver::print_usage(const char *reason)
{
	if (reason) PX4_WARN("%s\n", reason);

	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Description
This is a simple example module.
The pwm_run driver drives a specific PWM output channel (currently channel 4) by periodically setting it to a fixed pulse width.
)DESCR_STR");

	PRINT_MODULE_USAGE_NAME("pwm_run", "example");
	PRINT_MODULE_USAGE_COMMAND("start");
	PRINT_MODULE_USAGE_DEFAULT_COMMANDS();

	return 0;
}

void PWMDriver::Run() {
    if (should_exit()) {
        ScheduleClear();
        exit_and_cleanup();
        return;
    }

    up_pwm_servo_set(_channel, 2000);

    up_pwm_update(_mask);

    ScheduleDelayed(2500_us);
}




extern "C" __EXPORT int pwm_run_main(int argc, char *argv[])
{
	return PWMDriver::main(argc, argv);
}
