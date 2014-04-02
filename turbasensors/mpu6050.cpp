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

#include "mpu6050.h"

#include <hardware/sensors.h>
#include <cstdio>

#include <unistd.h>

#include <sys/poll.h>
#include <fcntl.h>
#include <pthread.h>

#include "utils.h"

turba_fifo<sensors_event_t *> *mpu_queue;
int mpu_acc_handle;
int mpu_gyro_handle;

int mpu_fd;
bool mpu_acc_polling;
bool mpu_gyro_polling;

void *poller(void *) {
	pollfd rfd;
	rfd.fd=mpu_fd;
	rfd.events=POLLIN;
	input_event event;
	int ax=0; int ay=0;
	int gx=0; int gy=0;
	while(true) {
		if(poll(&rfd,1,500)==1) {
			if(rfd.revents&POLLIN) {
				if(read(mpu_fd,&event,sizeof(struct input_event))==sizeof(input_event)) {
					if(event.type==2) {
						if(event.code==0) ax=event.value;
						if(event.code==1) ay=event.value;
						if(event.code==2) {
							sensors_event_t *sensors_event=get_new_event(timeval_to_nano(event.time));
							sensors_event->sensor=mpu_acc_handle;
							sensors_event->type=SENSOR_TYPE_ACCELEROMETER;
							sensors_event->acceleration.x=ax/1664.0f;
							sensors_event->acceleration.y=ay/1664.0f;
							sensors_event->acceleration.z=int(event.value)/1664.0f;
							mpu_queue->push(sensors_event);
						}
						if(event.code==3) gx=event.value;
						if(event.code==4) gy=event.value;
						if(event.code==5) {
							sensors_event_t *sensors_event=get_new_event(timeval_to_nano(event.time));
							sensors_event->sensor=mpu_gyro_handle;
							sensors_event->type=SENSOR_TYPE_GYROSCOPE;
							sensors_event->gyro.x=gx/4096.0f;
							sensors_event->gyro.y=gy/4096.0f;
							sensors_event->gyro.z=int(event.value)/4096.0f;
							mpu_queue->push(sensors_event);
						}
					}
				}
				else break;
			}
			else break;
		}
	}
	return NULL;
}

void start_polling(bool gyro) {
	if(!mpu_gyro_polling&&!mpu_acc_polling) {
		mpu_fd=open("/dev/input/event4",O_RDONLY);
		if(mpu_fd!=-1) {
			pthread_t t1;
			pthread_create(&t1,NULL,poller,NULL);
		}
	}
	if(gyro) mpu_gyro_polling=true;
	else mpu_acc_polling=true;
}

void stop_polling(bool gyro) {
	if(gyro) mpu_gyro_polling=false;
	else mpu_acc_polling=false;
	if(!mpu_gyro_polling&&!mpu_acc_polling) close(mpu_fd);
}