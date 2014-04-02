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

#include "mpu6050a.h"
#include "mpu6050.h"

#include <cstdio>

#include <unistd.h>
#include <fcntl.h>

#include "utils.h"

#define ENABLE_PATH "/sys/devices/virtual/input/input4/acc_enable"
#define DELAY_PATH "/sys/devices/virtual/input/input4/acc_delay"

const char *mpu6050a::get_name() { return "MPU-6050 Accelerometer"; }
const char *mpu6050a::get_vendor() { return "Invensense"; }
int mpu6050a::get_type() { return SENSOR_TYPE_ACCELEROMETER; }
float mpu6050a::get_range() { return 10240.0f; }
float mpu6050a::get_resolution() { return 1.0f; }
float mpu6050a::get_power() { return 0.5f; }
int32_t mpu6050a::get_delay() { return 10000; }

bool mpu6050a::set_delay(int64_t ns) {
	//int ms=ns*0.000001;
	int ms=ns*0.001;	//according to Invensense 1ms=0.001ns, o rly?
	char str[10];
	sprintf(str,"%d",ms);
	return sysfs_write(DELAY_PATH,str);
}

bool mpu6050a::start() {
	if(sysfs_write(ENABLE_PATH,"1")) {
		mpu_acc_handle=handle_;
		mpu_queue=queue_;
		start_polling(false);
		return true;
	}
	return false;
}

bool mpu6050a::stop() {
	sysfs_write(ENABLE_PATH,"0");
	stop_polling(false);
	return true;
}