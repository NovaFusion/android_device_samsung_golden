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

#include "utils.h"

#include <ctime>

#include <unistd.h>
#include <fcntl.h>

int64_t get_timestamp() {
    timespec t;
    t.tv_sec=t.tv_nsec=0;
    clock_gettime(CLOCK_MONOTONIC,&t);
    return int64_t(t.tv_sec)*1000000000LL+t.tv_nsec;
}

int64_t timeval_to_nano(timeval const &t) {
    return t.tv_sec*1000000000LL+t.tv_usec*1000;
}

bool sysfs_write(const char *path,const char *value) {
    int fd=open(path,O_WRONLY);
    if(fd!=-1) {
        write(fd,value,strlen(value));
        close(fd);
        return true;
    }
    return false;
}

sensors_event_t *get_new_event(int64_t timestamp) {
    sensors_event_t *event=new sensors_event_t();
    event->version=sizeof(sensors_event_t);
    memset(event->data,0,sizeof(event->data));
    event->timestamp=timestamp;
    return event;
}