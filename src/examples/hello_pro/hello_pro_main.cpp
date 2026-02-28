#include "hello_pro_main.hpp"

extern "C" __EXPORT int hello_pro_main(int argc, char *argv[]);

HelloPro* HelloPro::instantiate(int argc, char *argv[]) {
	if (_object.load() != nullptr) return new HelloPro();
	else {
		_object.store(new HelloPro());
		return _object.load();
	}
}

int HelloPro::task_spawn(int argc, char *argv[]) {
    HelloPro *instance = instantiate(argc, argv);
    if (!instance) {
        return PX4_ERROR;
    }

    _object.store(instance);

    _task_id = px4_task_spawn_cmd(
        "hello_pro",                  // 任务名称
        SCHED_DEFAULT,                // 调度策略
        SCHED_PRIORITY_DEFAULT + 10,  // 优先级（适当调整）
        2000,                         // 栈大小（字节）
        (px4_main_t)&run_trampoline,  // 运行入口（ModuleBase提供的 trampoline）
        argv                          // 参数（传递给 run()）
    );

    if (_task_id < 0) {
        PX4_ERR("Task spawn failed");
        delete instance;
        _object.store(nullptr);
        return PX4_ERROR;
    }

    return PX4_OK;
}


int HelloPro::custom_command(int argc, char *argv[]) {
    return print_usage("unknown command");
}

int HelloPro::print_usage(const char *reason) {
    if (reason) {
        PX4_WARN("%s\n", reason);
    }

    PRINT_MODULE_DESCRIPTION(R"DESCR_STR(
### Description
Example module for demonstration.
)DESCR_STR");

    PRINT_MODULE_USAGE_NAME("hello_pro", "example");
    PRINT_MODULE_USAGE_COMMAND("start");
    PRINT_MODULE_USAGE_DEFAULT_COMMANDS();
    return 0;
}

void HelloPro::run() {
    while (!should_exit()) {
        PX4_INFO("Hello Pro running");
        px4_usleep(1000000); // Sleep for 1 second
    }
}

int hello_pro_main(int argc, char *argv[]) {
    return HelloPro::main(argc, argv);
}
