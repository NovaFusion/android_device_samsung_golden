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

#include "gp2a.h"

#include <unistd.h>
#include <fcntl.h>

#include <sys/poll.h>

#include "utils.h"

#define ENABLE_PATH "/sys/devices/virtual/input/input6/enable"

const char *gp2a::get_name() { return "GP2A Proximity Sensor"; }
const char *gp2a::get_vendor() { return "Sharp"; }
int gp2a::get_type() { return SENSOR_TYPE_PROXIMITY; }
float gp2a::get_range() { return 5.0f; }
float gp2a::get_resolution() { return 5.0f; }
float gp2a::get_power() { return 0.75f; }
int32_t gp2a::get_delay() { return 0; }

bool gp2a::set_delay(int64_t ns) {
    return true;
}

void gp2a::send_event(int64_t timestamp,float distance) {
    sensors_event_t *sensors_event=get_new_event(timestamp);
    sensors_event->sensor=handle_;
    sensors_event->type=get_type();
    sensors_event->distance=distance;
    queue_->push(sensors_event);
}

void gp2a::poller() {
    pollfd rfd;
    rfd.fd=fd;
    rfd.events=POLLIN;
    input_event event;
    send_event(get_timestamp(),5.0f);
    while(true) {
        if(poll(&rfd,1,500)==1) {
            if(rfd.revents&POLLIN) {
                if(read(fd,&event,sizeof(struct input_event))==sizeof(input_event)) {
                    if(event.type==3&&event.code==25)
                        send_event(timeval_to_nano(event.time),event.value*5.0f);
                }
                else break;
            }
            else break;
        }
    }
}

void *gp2a::poller_helper(void *ctx) {
    ((gp2a *)ctx)->poller();
    return NULL;
}

bool gp2a::start() {
    fd=open("/dev/input/event6",O_RDONLY);
    if(fd!=-1) {
        if(!sysfs_write(ENABLE_PATH,"1")) {
            close(fd);
            return false;
        }
        pthread_t t1;
        pthread_create(&t1,NULL,&gp2a::poller_helper,this);
        return true;
    }
    return false;
}

bool gp2a::stop() {
    sysfs_write(ENABLE_PATH,"0");
    close(fd);
    return true;
}