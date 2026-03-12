#pragma once

#include <matrix/matrix/math.hpp>
#include <mathlib/mathlib.h>

using namespace matrix;

// extended state observer (ESO) class
class ESO {
public:
	ESO(float beta1 = 30.0f, float beta2 = 300.0f, float beta3 = 1000.0f, float b0 = 1.0f)
		: _beta1(beta1), _beta2(beta2), _beta3(beta3), _b0(b0) {}

	void update(float y, float u, float dt);
	void setParameters(float beta1, float beta2, float beta3, float b0);

	// 获取状态
	float get_state();
	float get_derivative();
	float get_disturbance();

	void reset();

private:
	float _beta1, _beta2, _beta3;
	float _b0;
	float _z1 = 0.0f;
	float _z2 = 0.0f;
	float _z3 = 0.0f;
};


// nonlinear state error feedback (NLSEF) class
class NLSEF {
public:
	NLSEF(float kp = 1.5f, float kd = 0.5f, float alpha = 0.75f, float delta = 0.1f) : _kp(kp), _kd(kd), _alpha(alpha), _delta(delta) {};

	float calculate(float e1, float e2);
	void setParameters(float kp, float kd, float alpha, float delta);
private:
	float fal(float x, float alpha, float delta);
	float sign(float x);

	float _kp, _kd;       // proportional and derivative gains
	float _alpha;        // nonlinearity parameter (0 < alpha < 1)
	float _delta;        // linear region threshold
};


// tracking differentiator (TD) class
class TD {
public:
    TD(float r = 100.0f, float h0 = 0.01f);

    // update the TD state with the input signal v and time step dt
    void update(float v, float dt);
	void setParameters(float r, float h0);
	void reset();

    float get_state();
    float get_derivative();

private:
    float _r;          // speed factor
    float _h0;         // filter factor
    float v1 = 0.0f;  // tracking signal (estimate of the input signal)
    float v2 = 0.0f;  // derivative of the tracking signal

    // the fast tracking differentiator (TD) algorithm
    float fhan(float x1, float x2, float r, float h);

    float sign(float x);
};

class ADRC {
public:
	/**
	 * @param omega control bandwidth (rad/s)
	 * @param kp NLSEF proportional gain
	 * @param kd NLSEF derivative gain
	 * @param alpha NLSEF nonlinearity parameter (0 < alpha < 1)
	 * @param delta NLSEF linear region threshold
	 * @param r TD speed factor
	 * @param h0 TD filter factor
	 * @param b0 ESO control gain
	 */
	ADRC(float omega = 10, float kp = 1.5f, float kd = 0.5f, float alpha = 0.75f, float delta = 0.1f,
		float r = 100.0f, float h0 = 0.01f, float b0 = 1.0f)
		: _td(r, h0), _eso(omega, 3 * omega * omega, omega * omega * omega, b0), _nlsef(kp, kd, alpha, delta), _b0(b0), _omega(omega) {}

	float update(float setpoint, float measurement, float dt);
	void setParameters(float omega, float kp, float kd, float alpha, float delta, float r, float h0, float b0);
	void setBandwidth(float omega);
	void setNlsefParameters(float kp, float kd, float alpha, float delta);
	void setTdParameters(float r, float h0);
	void setObserverB0(float b0);
	void reset();

private:
	TD _td;
	ESO _eso;
	NLSEF _nlsef;
	float _b0;
	float _omega;

	float _u_prev = 0.0f;

	mutable float _last_v1 = 0.0f, _last_v2 = 0.0f;
	mutable float _last_z1 = 0.0f, _last_z2 = 0.0f, _last_z3 = 0.0f;
	mutable float _last_e1 = 0.0f, _last_e2 = 0.0f;
};

