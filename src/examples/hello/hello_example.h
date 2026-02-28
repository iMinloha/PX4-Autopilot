#pragma once

#include <px4_platform_common/app.h>

class HelloExample {
public:
	HelloExample() {}

	~HelloExample() {}

	int main();

	static px4::AppState appState;
};
