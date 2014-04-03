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

#include <cstdio>

#include <unistd.h>
#include <fcntl.h>

#include <sys/poll.h>

#include "utils.h"

#define ACC_ENABLE_PATH "/sys/devices/virtual/input/input4/acc_enable"
#define ACC_DELAY_PATH "/sys/devices/virtual/input/input4/acc_delay"

#define GYRO_ENABLE_PATH "/sys/devices/virtual/input/input4/gyro_enable"
#define GYRO_DELAY_PATH "/sys/devices/virtual/input/input4/gyro_delay"

//static members declaration
int mpu6050::acc_handle=-1;
int mpu6050::gyro_handle=-1;
int mpu6050::fd;

const char *mpu6050::get_name() { return gyro ? "MPU-6050 Gyroscope" : "MPU-6050 Accelerometer"; }
const char *mpu6050::get_vendor() { return "Invensense"; }
int mpu6050::get_type() { return gyro ? SENSOR_TYPE_GYROSCOPE : SENSOR_TYPE_ACCELEROMETER; }
float mpu6050::get_range() { return gyro ? 2000.0f : 10240.0f; }
float mpu6050::get_resolution() { return 1.0f; }
float mpu6050::get_power() { return 0.5f; }
int32_t mpu6050::get_delay() { return 10000; }

mpu6050::mpu6050(bool device) {
    this->gyro=device;
}

bool mpu6050::set_delay(int64_t ns) {
    int ms=ns*0.001;    //according to Invensense 1ms=0.001ns, o rly?
    if(gyro&&ms<10000) ms=10000;    //delay limit for gyroscope
    char str[11];
    sprintf(str,"%d",ms);
    return sysfs_write(gyro ? GYRO_DELAY_PATH : ACC_DELAY_PATH,str);
}

void mpu6050::poller() {
    pollfd rfd;
    rfd.fd=fd;
    rfd.events=POLLIN;
    input_event event;
    int ax=0; int ay=0;
    int gx=0; int gy=0;
    while(true) {
        if(poll(&rfd,1,500)==1) {
            if(rfd.revents&POLLIN) {
                if(read(fd,&event,sizeof(struct input_event))==sizeof(input_event)) {
                    if(event.type==2) {
                        if(event.code==0&&acc_handle!=-1) ax=event.value;
                        if(event.code==1&&acc_handle!=-1) ay=event.value;
                        if(event.code==2&&acc_handle!=-1) {
                            sensors_event_t *sensors_event=get_new_event(timeval_to_nano(event.time));
                            sensors_event->sensor=acc_handle;
                            sensors_event->type=SENSOR_TYPE_ACCELEROMETER;
                            sensors_event->acceleration.x=ax/1664.0f;
                            sensors_event->acceleration.y=ay/1664.0f;
                            sensors_event->acceleration.z=int(event.value)/1664.0f;
                            queue_->push(sensors_event);
                        }
                        if(event.code==3&&gyro_handle!=-1) gx=event.value;
                        if(event.code==4&&gyro_handle!=-1) gy=event.value;
                        if(event.code==5&&gyro_handle!=-1) {
                            sensors_event_t *sensors_event=get_new_event(timeval_to_nano(event.time));
                            sensors_event->sensor=gyro_handle;
                            sensors_event->type=SENSOR_TYPE_GYROSCOPE;
                            sensors_event->gyro.x=gx/4096.0f;
                            sensors_event->gyro.y=gy/4096.0f;
                            sensors_event->gyro.z=int(event.value)/4096.0f;
                            queue_->push(sensors_event);
                        }
                    }
                }
                else break;
            }
            else break;
        }
    }
}

void *mpu6050::poller_helper(void *ctx) {
    ((mpu6050 *)ctx)->poller();
    return NULL;
}

bool mpu6050::start() {
    //if thread is not running
    if(acc_handle==-1&&gyro_handle==-1) {
        fd=open("/dev/input/event4",O_RDONLY);
        if(fd!=-1) {
            pthread_t t1;
            pthread_create(&t1,NULL,&mpu6050::poller_helper,this);
        }
        else return false;
    }

    if(gyro) {
        if(!sysfs_write(GYRO_ENABLE_PATH,"1")) return false;
        gyro_handle=handle_;
    }
    else {
        if(!sysfs_write(ACC_ENABLE_PATH,"1")) return false;
        acc_handle=handle_;
    }

    return true;
}

bool mpu6050::stop() {
    if(gyro) {
        sysfs_write(GYRO_ENABLE_PATH,"0");
        gyro_handle=-1;
    }
    else {
        sysfs_write(ACC_ENABLE_PATH,"0");
        acc_handle=-1;
    }

    //kill thread if nothing is left
    if(acc_handle==-1&&gyro_handle==-1) close(fd);
    return true;
}