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

#ifndef MPU6050_H
#define MPU6050_H

#include "template.h"

#define ACCELEROMETER false
#define GYROSCOPE true

class mpu6050:public sensor_base {
SENSOR_IMPLEMENTATION

public:
    mpu6050(bool device);

private:
    static void *poller_helper(void *ctx);
    void poller();

    static int acc_handle;
    static int gyro_handle;
    static int fd;
    bool gyro;
};

#endif