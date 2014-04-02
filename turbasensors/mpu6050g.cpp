/*
 * Copyright (C) 2014 Marcin Chojnacki marcinch7@gmail.com
 * Copyright (C) 2014 NovaFusion https://github.com/NovaFusion
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mpu6050g.h"
#include "mpu6050.h"

#include <cstdio>

#include <unistd.h>
#include <fcntl.h>

#include "utils.h"

#define ENABLE_PATH "/sys/devices/virtual/input/input4/gyro_enable"
#define DELAY_PATH "/sys/devices/virtual/input/input4/gyro_delay"

const char *mpu6050g::get_name() { return "MPU-6050 Gyroscope"; }
const char *mpu6050g::get_vendor() { return "Invensense"; }
int mpu6050g::get_type() { return SENSOR_TYPE_GYROSCOPE; }
float mpu6050g::get_range() { return 2000.0f; }
float mpu6050g::get_resolution() { return 1.0f; }
float mpu6050g::get_power() { return 0.5f; }
int32_t mpu6050g::get_delay() { return 10000; }

bool mpu6050g::set_delay(int64_t ns) {
	//int ms=ns*0.000001;
	int ms=ns*0.001;	//according to Invensense 1ms=0.001ns, o rly?
	if(ms<10000) ms=10000;	//delay limit
	char str[10];
	sprintf(str,"%d",ms);
	return sysfs_write(DELAY_PATH,str);
}

bool mpu6050g::start() {
	if(sysfs_write(ENABLE_PATH,"1")) {
		mpu_gyro_handle=handle_;
		mpu_queue=queue_;
		start_polling(true);
		return true;
	}
	return false;
}

bool mpu6050g::stop() {
	sysfs_write(ENABLE_PATH,"0");
	stop_polling(true);
	return true;
}