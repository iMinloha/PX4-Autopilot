#include "../inc/library.h"

float clamp(float x, float min_val, float max_val) {
        if (x < min_val) return min_val;
        if (x > max_val) return max_val;
        return x;
}


void ESO::update(float y, float u, float dt) {
        float e = y - _z1;

        _z1 += dt * (_z2 + _beta1 * e);
        _z2 += dt * (_z3 + _beta2 * e + _b0 * u);
        _z3 += dt * (_beta3 * e);

        _z3 = clamp(_z3, -1000.0f, 1000.0f);
}


float ESO::get_state() {
	return _z1;
}

float ESO::get_derivative() {
	return _z2;
}

float ESO::get_disturbance() {
	return _z3;
}

void ESO::reset() {
	_z1 = _z2 = _z3 = 0.0f;
}

// =================================================================

float NLSEF::calculate(float e1, float e2) {
        // 非线性函数 fal
        float u1 = _kp * fal(e1, _alpha, _delta);
        float u2 = _kd * fal(e2, _alpha, _delta);

        return u1 + u2;
}

float NLSEF::fal(float x, float alpha, float delta) {
	if (fabs(x) <= delta) {
		return x / (powf(delta, 1.0f - alpha));
	} else {
		return powf(fabs(x), alpha) * sign(x);
	}
}

float NLSEF::sign(float x) {
	const float epsilon = 1e-8f;
	if (x > epsilon) return 1.0f;
	if (x < -epsilon) return -1.0f;
	return 0.0f;
}

// =================================================================


TD::TD(float r, float h0) : _r(r), _h0(h0) {
}

void TD::update(float v, float dt) {
        float fh = fhan(v1 - v, v2, _r, _h0);
        v1 += dt * v2;
        v2 += dt * fh;
}

float TD::get_state() { return v1; }

float TD::get_derivative() { return v2; }


float TD::fhan(float x1, float x2, float r, float h) {
        float d = r * h * h;
        float a0 = h * x2;
        float y = x1 + a0;

        float a1 = sqrt(d * (d + 8 * fabs(y)));
        float a2 = a0 + sign(y) * (a1 - d) / 2.0f;

        float sy = (sign(y + d) - sign(y - d)) / 2.0f;
        float a = (a0 + y - a2) * sy + a2;

        float sa = (sign(a + d) - sign(a - d)) / 2.0f;
        float f = -r * (a / d - sign(a)) * sa - r * sign(a);

        return f;
}

float TD::sign(float x) {
	return (x > 0) ? 1.0f : ((x < 0) ? -1.0f : 0.0f);
}

// =================================================================

float ADRC::update(float setpoint, float measurement, float dt) {
        _td.update(setpoint, dt);
        float v1 = _td.get_state();     // 过渡后的设定值
        float v2 = _td.get_derivative(); // 微分信号

        // 2. 扩张状态观测器：估计状态和扰动
        float u_prev_copy = _u_prev;  // 避免修改
        _eso.update(measurement, u_prev_copy, dt);

        float z1 = _eso.get_state();       // 状态估计
        float z2 = _eso.get_derivative();  // 导数估计
        float z3 = _eso.get_disturbance(); // 总扰动估计

        // 3. 误差计算
        float e1 = v1 - z1;  // 状态误差
        float e2 = v2 - z2;  // 微分误差

        // 4. 非线性状态误差反馈
        float u0 = _nlsef.calculate(e1, e2);

        // 5. 扰动补偿
        float u = (u0 - z3) / _b0;

        // 限幅
        u = clamp(u, -10.0f, 10.0f);

        // 保存上一时刻控制量（用于ESO）
        _u_prev = u;

        // 保存状态用于调试
        _last_v1 = v1;
        _last_v2 = v2;

        _last_z1 = z1;
        _last_z2 = z2;
        _last_z3 = z3;

        _last_e1 = e1;
        _last_e2 = e2;

        return u;
}
