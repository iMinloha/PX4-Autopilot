#pragma once

#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>


class HelloPro : public ModuleBase<HelloPro> {
public:
	HelloPro() = default;

	~HelloPro() override = default;

	/** @see ModuleBase */
	static HelloPro *instantiate(int argc, char *argv[]);

	/** @see ModuleBase */
	static int task_spawn(int argc, char *argv[]);

	/** @see ModuleBase */
	static int custom_command(int argc, char *argv[]);

	/** @see ModuleBase */
	static int print_usage(const char *reason = nullptr);

	/** @see ModuleBase */
	void run() override;
};
