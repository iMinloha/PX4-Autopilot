/****************************************************************************
 *
 *   Copyright (c) 2013-2019 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file mc_rate_control_params.c
 *
 * Parameters for multicopter rate controller
 */

/**
 * Enable ADRC rate control
 *
 * Set to 1 to use Active Disturbance Rejection Control (ADRC) for multicopter rate control.
 * When enabled, all ADRC-specific parameters (MC_ADRC_*) take effect and PID gains are ignored.
 * Disabled by default; PID is the primary controller.
 *
 * @boolean
 * @reboot_required false
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_INT32(USE_ADRC, 0);

/**
 * ADRC Extended State Observer (ESO) bandwidth
 *
 * Observer bandwidth omega_o (rad/s). The three ESO pole coefficients are placed at -omega_o:
 *   beta1 = omega_o, beta2 = 3*omega_o^2, beta3 = omega_o^3
 * A larger value yields faster disturbance estimation but amplifies sensor noise.
 * Typical range for multicopters: 5~50 rad/s.
 *
 * @unit rad/s
 * @min 1.0
 * @max 200.0
 * @decimal 2
 * @increment 0.5
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_BANDWIDTH, 10.0f);

/**
 * ADRC roll axis NLSEF proportional gain kp
 *
 * Nonlinear State Error Feedback (NLSEF) proportional gain for the roll axis.
 * Scales the fal(e1, alpha, delta) term. Increasing this value speeds up error convergence
 * but may cause overshoot. Typical range: 0.5~5.0.
 *
 * @min 0.0
 * @max 20.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_R_KP, 1.5f);

/**
 * ADRC pitch axis NLSEF proportional gain kp
 *
 * Nonlinear State Error Feedback (NLSEF) proportional gain for the pitch axis.
 * Scales the fal(e1, alpha, delta) term. Increasing this value speeds up error convergence
 * but may cause overshoot. Typical range: 0.5~5.0.
 *
 * @min 0.0
 * @max 20.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_P_KP, 1.5f);

/**
 * ADRC yaw axis NLSEF proportional gain kp
 *
 * Nonlinear State Error Feedback (NLSEF) proportional gain for the yaw axis.
 * Scales the fal(e1, alpha, delta) term. The yaw axis typically requires smaller gains
 * than roll/pitch. Typical range: 0.5~5.0.
 *
 * @min 0.0
 * @max 20.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_Y_KP, 1.5f);

/**
 * ADRC roll axis NLSEF derivative gain kd
 *
 * Nonlinear State Error Feedback (NLSEF) derivative gain for the roll axis.
 * Scales the fal(e2, alpha, delta) term where e2 is the rate error.
 * Increasing this value improves damping and reduces oscillation. Typical range: 0.1~2.0.
 *
 * @min 0.0
 * @max 20.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_R_KD, 0.5f);

/**
 * ADRC pitch axis NLSEF derivative gain kd
 *
 * Nonlinear State Error Feedback (NLSEF) derivative gain for the pitch axis.
 * Scales the fal(e2, alpha, delta) term where e2 is the rate error.
 * Increasing this value improves damping and reduces oscillation. Typical range: 0.1~2.0.
 *
 * @min 0.0
 * @max 20.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_P_KD, 0.5f);

/**
 * ADRC yaw axis NLSEF derivative gain kd
 *
 * Nonlinear State Error Feedback (NLSEF) derivative gain for the yaw axis.
 * Scales the fal(e2, alpha, delta) term where e2 is the rate error.
 * Increasing this value improves damping and reduces oscillation. Typical range: 0.1~2.0.
 *
 * @min 0.0
 * @max 20.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_Y_KD, 0.5f);

/**
 * ADRC NLSEF nonlinearity exponent alpha
 *
 * Controls the shape of the fal(x, alpha, delta) nonlinear function used in NLSEF and ESO.
 * Must be strictly between 0 and 1 (exclusive). Values closer to 0 produce stronger
 * nonlinearity (better small-error response); values closer to 1 approach linear behavior.
 * Shared across all three axes (roll, pitch, yaw). Recommended: 0.5~0.9.
 *
 * @min 0.01
 * @max 0.99
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_ALPHA, 0.75f);

/**
 * ADRC NLSEF linear zone threshold delta
 *
 * Boundary of the linear region in the fal(x, alpha, delta) function.
 * When |x| < delta, fal is linear (slope = delta^(alpha-1)); outside this zone it is
 * a power-law function |x|^alpha * sign(x). A smaller delta increases nonlinearity
 * sensitivity near zero. Shared across all three axes. Typical range: 0.01~0.5.
 *
 * @min 0.001
 * @max 1.0
 * @decimal 4
 * @increment 0.005
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_DELTA, 0.1f);

/**
 * ADRC Tracking Differentiator (TD) speed factor r
 *
 * Determines how quickly the TD tracks the setpoint and extracts its derivative.
 * A larger r allows faster tracking but amplifies high-frequency noise in the derivative
 * estimate. Must satisfy r > 1/(h0^2) to avoid numerical instability.
 * Shared across all three axes. Typical range: 50~500.
 *
 * @unit m/s^2
 * @min 1.0
 * @max 1000.0
 * @decimal 1
 * @increment 5.0
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_TD_R, 50.0f);

/**
 * ADRC Tracking Differentiator (TD) filter time step h0
 *
 * Integration step size used inside the TD fhan() algorithm. Should be set to the
 * control loop sample period or slightly larger for added filtering. A larger h0
 * provides smoother derivative estimates at the cost of tracking speed.
 * Shared across all three axes. Typical range: 0.002~0.05 s.
 *
 * @unit s
 * @min 0.001
 * @max 0.1
 * @decimal 4
 * @increment 0.001
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_TD_H0, 0.01f);

/**
 * ADRC roll axis ESO control input gain b0
 *
 * Approximate control effectiveness for the roll axis, representing the ratio of
 * control torque to angular acceleration (b0 ≈ 1/J_roll for pure inertia model).
 * Inaccuracy in b0 is compensated by the ESO as disturbance, but larger errors
 * degrade observer performance. Start with b0 = 1.0 and adjust based on vehicle response.
 *
 * @unit rad/s^2
 * @min 0.01
 * @max 100.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_R_B0, 1.0f);

/**
 * ADRC pitch axis ESO control input gain b0
 *
 * Approximate control effectiveness for the pitch axis, representing the ratio of
 * control torque to angular acceleration (b0 ≈ 1/J_pitch for pure inertia model).
 * Inaccuracy in b0 is compensated by the ESO as disturbance, but larger errors
 * degrade observer performance. Start with b0 = 1.0 and adjust based on vehicle response.
 *
 * @unit rad/s^2
 * @min 0.01
 * @max 100.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_P_B0, 1.0f);

/**
 * ADRC yaw axis ESO control input gain b0
 *
 * Approximate control effectiveness for the yaw axis, representing the ratio of
 * control torque to angular acceleration (b0 ≈ 1/J_yaw for pure inertia model).
 * The yaw axis typically has lower control authority than roll/pitch, so this value
 * may need to be tuned independently. Start with b0 = 1.0.
 *
 * @unit rad/s^2
 * @min 0.01
 * @max 100.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(ADRC_Y_B0, 1.0f);

/**
 * Roll rate P gain
 *
 * Roll rate proportional gain, i.e. control output for angular speed error 1 rad/s.
 *
 * @min 0.01
 * @max 0.5
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_ROLLRATE_P, 0.15f);

/**
 * Roll rate I gain
 *
 * Roll rate integral gain. Can be set to compensate static thrust difference or gravity center offset.
 *
 * @min 0.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_ROLLRATE_I, 0.2f);

/**
 * Roll rate integrator limit
 *
 * Roll rate integrator limit. Can be set to increase the amount of integrator available to counteract disturbances or reduced to improve settling time after large roll moment trim changes.
 *
 * @min 0.0
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_RR_INT_LIM, 0.30f);

/**
 * Roll rate D gain
 *
 * Roll rate differential gain. Small values help reduce fast oscillations. If value is too big oscillations will appear again.
 *
 * @min 0.0
 * @max 0.01
 * @decimal 4
 * @increment 0.0005
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_ROLLRATE_D, 0.003f);

/**
 * Roll rate feedforward
 *
 * Improves tracking performance.
 *
 * @min 0.0
 * @decimal 4
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_ROLLRATE_FF, 0.0f);

/**
 * Roll rate controller gain
 *
 * Global gain of the controller.
 *
 * This gain scales the P, I and D terms of the controller:
 * output = MC_ROLLRATE_K * (MC_ROLLRATE_P * error
 * 			     + MC_ROLLRATE_I * error_integral
 * 			     + MC_ROLLRATE_D * error_derivative)
 * Set MC_ROLLRATE_P=1 to implement a PID in the ideal form.
 * Set MC_ROLLRATE_K=1 to implement a PID in the parallel form.
 *
 * @min 0.01
 * @max 5.0
 * @decimal 4
 * @increment 0.0005
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_ROLLRATE_K, 1.0f);

/**
 * Pitch rate P gain
 *
 * Pitch rate proportional gain, i.e. control output for angular speed error 1 rad/s.
 *
 * @min 0.01
 * @max 0.6
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_PITCHRATE_P, 0.15f);

/**
 * Pitch rate I gain
 *
 * Pitch rate integral gain. Can be set to compensate static thrust difference or gravity center offset.
 *
 * @min 0.0
 * @decimal 3
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_PITCHRATE_I, 0.2f);

/**
 * Pitch rate integrator limit
 *
 * Pitch rate integrator limit. Can be set to increase the amount of integrator available to counteract disturbances or reduced to improve settling time after large pitch moment trim changes.
 *
 * @min 0.0
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_PR_INT_LIM, 0.30f);

/**
 * Pitch rate D gain
 *
 * Pitch rate differential gain. Small values help reduce fast oscillations. If value is too big oscillations will appear again.
 *
 * @min 0.0
 * @decimal 4
 * @increment 0.0005
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_PITCHRATE_D, 0.003f);

/**
 * Pitch rate feedforward
 *
 * Improves tracking performance.
 *
 * @min 0.0
 * @decimal 4
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_PITCHRATE_FF, 0.0f);

/**
 * Pitch rate controller gain
 *
 * Global gain of the controller.
 *
 * This gain scales the P, I and D terms of the controller:
 * output = MC_PITCHRATE_K * (MC_PITCHRATE_P * error
 * 			     + MC_PITCHRATE_I * error_integral
 * 			     + MC_PITCHRATE_D * error_derivative)
 * Set MC_PITCHRATE_P=1 to implement a PID in the ideal form.
 * Set MC_PITCHRATE_K=1 to implement a PID in the parallel form.
 *
 * @min 0.01
 * @max 5.0
 * @decimal 4
 * @increment 0.0005
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_PITCHRATE_K, 1.0f);

/**
 * Yaw rate P gain
 *
 * Yaw rate proportional gain, i.e. control output for angular speed error 1 rad/s.
 *
 * @min 0.0
 * @max 0.6
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_YAWRATE_P, 0.2f);

/**
 * Yaw rate I gain
 *
 * Yaw rate integral gain. Can be set to compensate static thrust difference or gravity center offset.
 *
 * @min 0.0
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_YAWRATE_I, 0.1f);

/**
 * Yaw rate integrator limit
 *
 * Yaw rate integrator limit. Can be set to increase the amount of integrator available to counteract disturbances or reduced to improve settling time after large yaw moment trim changes.
 *
 * @min 0.0
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_YR_INT_LIM, 0.30f);

/**
 * Yaw rate D gain
 *
 * Yaw rate differential gain. Small values help reduce fast oscillations. If value is too big oscillations will appear again.
 *
 * @min 0.0
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_YAWRATE_D, 0.0f);

/**
 * Yaw rate feedforward
 *
 * Improves tracking performance.
 *
 * @min 0.0
 * @decimal 4
 * @increment 0.01
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_YAWRATE_FF, 0.0f);

/**
 * Yaw rate controller gain
 *
 * Global gain of the controller.
 *
 * This gain scales the P, I and D terms of the controller:
 * output = MC_YAWRATE_K * (MC_YAWRATE_P * error
 * 			     + MC_YAWRATE_I * error_integral
 * 			     + MC_YAWRATE_D * error_derivative)
 * Set MC_YAWRATE_P=1 to implement a PID in the ideal form.
 * Set MC_YAWRATE_K=1 to implement a PID in the parallel form.
 *
 * @min 0.0
 * @max 5.0
 * @decimal 4
 * @increment 0.0005
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_FLOAT(MC_YAWRATE_K, 1.0f);

/**
 * Battery power level scaler
 *
 * This compensates for voltage drop of the battery over time by attempting to
 * normalize performance across the operating range of the battery. The copter
 * should constantly behave as if it was fully charged with reduced max acceleration
 * at lower battery percentages. i.e. if hover is at 0.5 throttle at 100% battery,
 * it will still be 0.5 at 60% battery.
 *
 * @boolean
 * @group Multicopter Rate Control
 */
PARAM_DEFINE_INT32(MC_BAT_SCALE_EN, 0);
