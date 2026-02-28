#include <px4_platform_common/log.h>			// 日志输出在这里
#include <px4_platform_common/posix.h>			// 收发话题设置轮询结构体在这里
#include <stdio.h>					// 标准输入输出在这里
#include <poll.h>					// 轮询相关定义在这里
#include <string.h>					// memset在这里
#include <math.h>					// 数学计算在这里
#include <uORB/uORB.h>					// uORB相关API在这里
// 话题数据结构体在这里
#include <uORB/topics/vehicle_acceleration.h>
#include <uORB/topics/vehicle_attitude.h>

__EXPORT int px4_simple_app_main(int argc, char *argv[]);

int px4_simple_app_main(int argc, char *argv[])
{
	PX4_INFO("Hello Sky!");

	// 定义一个话题的接受方
	int sensor_sub_fd = orb_subscribe(ORB_ID(vehicle_acceleration));
	// 设置话题的接受频率(200ms接受一次)
	orb_set_interval(sensor_sub_fd, 200);

	// 创建一个新的话题结构体
	struct vehicle_attitude_s att;
	memset(&att, 0, sizeof(att));
	// 定义一个话题的发布者
	orb_advert_t att_pub = orb_advertise(ORB_ID(vehicle_attitude), &att);

	// 被订阅的话题定义轮询结构体, 当有可读数据时就会出发POLLIN事件
	px4_pollfd_struct_t fds[] = {
		// .fd为话题的文件描述符, .events为关注的事件类型
		{ .fd = sensor_sub_fd,   .events = POLLIN },
	};

	int error_counter = 0;

	for (int i = 0; i < 5; i++) {
		// 等待数据更新, 间隔时间为1s, 循环5次
		int poll_ret = px4_poll(fds, 1, 1000);

		// 获取数据超时
		if (poll_ret == 0) {
			PX4_ERR("Got no data within a second");

		} else if (poll_ret < 0) {
			// poll出错, 通过error_counter避免重复打印到日志
			if (error_counter < 10 || error_counter % 50 == 0) {
				/* use a counter to prevent flooding (and slowing us down) */
				PX4_ERR("ERROR return value from poll(): %d", poll_ret);
			}

			error_counter++;

		} else {
			// 有数据更新
			if (fds[0].revents & POLLIN) {
				// 定义一个数据结构体
				struct vehicle_acceleration_s accel;
				// 将数据从文件中拷贝出来
				orb_copy(ORB_ID(vehicle_acceleration), sensor_sub_fd, &accel);
				// 输出数据到终端
				PX4_INFO("Accelerometer:\t%8.4f\t%8.4f\t%8.4f",
					 (double)accel.xyz[0],
					 (double)accel.xyz[1],
					 (double)accel.xyz[2]);

				// 设置数据到新的话题并发布
				att.timestamp = hrt_absolute_time();
				att.q[0] = accel.xyz[0];
				att.q[1] = accel.xyz[1];
				att.q[2] = accel.xyz[2];
				orb_publish(ORB_ID(vehicle_attitude), att_pub, &att);
			}
		}
	}

	PX4_INFO("exiting");

	return 0;
}
