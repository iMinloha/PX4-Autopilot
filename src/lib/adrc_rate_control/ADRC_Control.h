#pragma once

#include "inc/library.h"
#include <matrix/matrix/math.hpp>
#include <uORB/topics/rate_ctrl_status.h>
#include <mathlib/mathlib.h>

class ADRC_Control
{
public:
	ADRC_Control() = default;
	~ADRC_Control() = default;

	/**
	 * Set ADRC nonlinear feedback gains for all axes.
	 * @param kp 3D vector of ADRC proportional-like gains
	 * @param kd 3D vector of ADRC derivative-like gains
	 */
	void setAdrcGains(const matrix::Vector3f &kp, const matrix::Vector3f &kd);

	/**
	 * Set ADRC observer bandwidth for all axes.
	 * @param bandwidth observer bandwidth omega
	 */
	void setAdrcBandwidth(float bandwidth);

	/**
	 * Set ADRC nonlinear parameters for all axes.
	 */
	void setAdrcNonlinearParams(const matrix::Vector3f &alpha, const matrix::Vector3f &delta);

	/**
	 * Set ADRC tracking differentiator parameters for all axes.
	 */
	void setAdrcTdParams(const matrix::Vector3f &r, const matrix::Vector3f &h0);

	/**
	 * Set ADRC observer input gain for all axes.
	 */
	void setAdrcObserverGain(const matrix::Vector3f &b0);

	/**
	 * Set direct rate to torque feed forward gain
	 * @see _gain_ff
	 * @param FF 3D vector of feed forward gains for body x,y,z axis
	 */
	void setFeedForwardGain(const matrix::Vector3f &FF) { _gain_ff = FF; };

	/**
	 * Run one control loop cycle calculation
	 * @param rate estimation of the current vehicle angular rate
	 * @param rate_sp desired vehicle angular rate setpoint
	 * @param dt desired vehicle angular rate setpoint
	 * @return [-1,1] normalized torque vector to apply to the vehicle
	 */
	matrix::Vector3f update(const matrix::Vector3f &rate, const matrix::Vector3f &rate_sp, const float dt, const bool landed);

	void resetAdrc();

	/**
	 * Get status message of controller for logging/debugging
	 * @param rate_ctrl_status status message to fill with internal states
	 */
	void getRateControlStatus(rate_ctrl_status_s &rate_ctrl_status);

private:
	// ADRC tuning state
	matrix::Vector3f _adrc_kp{1.5f, 1.5f, 1.5f};
	matrix::Vector3f _adrc_kd{0.5f, 0.5f, 0.5f};
	matrix::Vector3f _adrc_alpha{0.75f, 0.75f, 0.75f};
	matrix::Vector3f _adrc_delta{0.1f, 0.1f, 0.1f};
	matrix::Vector3f _gain_ff; ///< direct rate to torque feed forward gain only useful for helicopters

	// ADRC
	ADRC _adrc_roll_controller, _adrc_pitch_controller, _adrc_yaw_controller;
};

