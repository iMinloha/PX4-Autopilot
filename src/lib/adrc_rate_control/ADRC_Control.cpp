#include "ADRC_Control.h"
#include <px4_platform_common/defines.h>

using namespace matrix;

void ADRC_Control::setAdrcGains(const Vector3f &kp, const Vector3f &kd)
{
	_adrc_kp = kp;
	_adrc_kd = kd;

	_adrc_roll_controller.setNlsefParameters(_adrc_kp(0), _adrc_kd(0), _adrc_alpha(0), _adrc_delta(0));
	_adrc_pitch_controller.setNlsefParameters(_adrc_kp(1), _adrc_kd(1), _adrc_alpha(1), _adrc_delta(1));
	_adrc_yaw_controller.setNlsefParameters(_adrc_kp(2), _adrc_kd(2), _adrc_alpha(2), _adrc_delta(2));
}

void ADRC_Control::setAdrcBandwidth(float bandwidth)
{
	_adrc_roll_controller.setBandwidth(bandwidth);
	_adrc_pitch_controller.setBandwidth(bandwidth);
	_adrc_yaw_controller.setBandwidth(bandwidth);
}

void ADRC_Control::setAdrcNonlinearParams(const Vector3f &alpha, const Vector3f &delta)
{
	_adrc_alpha = alpha;
	_adrc_delta = delta;

	_adrc_roll_controller.setNlsefParameters(_adrc_kp(0), _adrc_kd(0), _adrc_alpha(0), _adrc_delta(0));
	_adrc_pitch_controller.setNlsefParameters(_adrc_kp(1), _adrc_kd(1), _adrc_alpha(1), _adrc_delta(1));
	_adrc_yaw_controller.setNlsefParameters(_adrc_kp(2), _adrc_kd(2), _adrc_alpha(2), _adrc_delta(2));
}

void ADRC_Control::setAdrcTdParams(const Vector3f &r, const Vector3f &h0)
{
	_adrc_roll_controller.setTdParameters(r(0), h0(0));
	_adrc_pitch_controller.setTdParameters(r(1), h0(1));
	_adrc_yaw_controller.setTdParameters(r(2), h0(2));
}

void ADRC_Control::setAdrcObserverGain(const Vector3f &b0)
{
	_adrc_roll_controller.setObserverB0(b0(0));
	_adrc_pitch_controller.setObserverB0(b0(1));
	_adrc_yaw_controller.setObserverB0(b0(2));
}

Vector3f ADRC_Control::update(const Vector3f &rate, const Vector3f &rate_sp, const float dt, const bool landed)
{
	if (dt <= 0.f || !PX4_ISFINITE(dt)) {
		return _gain_ff.emult(rate_sp);
	}

	Vector3f torque;
	torque(0) = _adrc_roll_controller.update(rate_sp(0), rate(0), dt);
	torque(1) = _adrc_pitch_controller.update(rate_sp(1), rate(1), dt);
	torque(2) = _adrc_yaw_controller.update(rate_sp(2), rate(2), dt);

	// Keep optional direct rate-to-torque feed-forward for compatibility.
	torque += _gain_ff.emult(rate_sp);

	if (landed) {
		resetAdrc();
	}

	return torque;
}

void ADRC_Control::resetAdrc()
{
	_adrc_roll_controller.reset();
	_adrc_pitch_controller.reset();
	_adrc_yaw_controller.reset();
}

void ADRC_Control::getRateControlStatus(rate_ctrl_status_s &rate_ctrl_status)
{
	rate_ctrl_status.rollspeed_integ = 0.f;
	rate_ctrl_status.pitchspeed_integ = 0.f;
	rate_ctrl_status.yawspeed_integ = 0.f;
}
