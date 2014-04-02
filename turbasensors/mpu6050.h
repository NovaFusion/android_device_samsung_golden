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

#include <hardware/sensors.h>
#include "turbafifo.h"

extern turba_fifo<sensors_event_t *> *mpu_queue;
extern int mpu_acc_handle;
extern int mpu_gyro_handle;

void start_polling(bool gyro);
void stop_polling(bool gyro);

#endif