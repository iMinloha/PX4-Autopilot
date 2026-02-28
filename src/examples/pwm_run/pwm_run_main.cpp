#include "pwm_run_main.hpp"

PWMDriver::PWMDriver(const uint8_t channel) :
	ModuleParams(nullptr),
	ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::hp_default),
    _channel(channel),
    _mask(1 << channel)
{
}

PWMDriver::~PWMDriver() {
    if (_initialized) {
        if (_armed) {
            up_pwm_servo_arm(false, _mask);
        }
        up_pwm_servo_deinit(_mask);
    }
}

int PWMDriver::Init() {
    int ret = up_pwm_servo_init(_mask);
    if (ret < 0) {
        PX4_ERR("PWM init failed: %d", ret);
        return ret;
    }

    if ((ret = up_pwm_servo_set_rate_group_update(_channel, 400)) != 0) {
        PX4_ERR("Set frequency failed: %d", ret);
        return ret;
    }

    up_pwm_servo_arm(true, _mask);

    _initialized = true;
    _armed = true;
    PX4_INFO("Initialized PWM channel %d @ 50Hz", _channel);
    return PX4_OK;
}


int PWMDriver::print_status()
{
	return 0;
}

int PWMDriver::task_spawn(int argc, char *argv[]) {
	PWMDriver *instance = new PWMDriver(2);

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

    if (_initialized && _armed) {
        up_pwm_servo_set(_channel, 1500); // 持续输出1500us
    }

    ScheduleDelayed(20_ms); // 标准50Hz更新
}


extern "C" __EXPORT int pwm_run_main(int argc, char *argv[])
{
	return PWMDriver::main(argc, argv);
}
