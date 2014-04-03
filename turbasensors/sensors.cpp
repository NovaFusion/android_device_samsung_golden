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

#include <hardware/sensors.h>
#include <stdlib.h>

#include "turbafifo.h"

#include "mpu6050.h"
#include "alps.h"
#include "gp2a.h"

sensor_base *sensors[]={
    new mpu6050(ACCELEROMETER),
    new mpu6050(GYROSCOPE),
    new alps(),
    new gp2a()
};

size_t sensors_count=sizeof(sensors)/sizeof(sensor_base *);

turba_fifo<sensors_event_t *> fifo;

int sensors_getList(sensors_module_t *module,sensor_t const **list) {
    sensor_t *new_list=new sensor_t[sensors_count];
    for(size_t i=0;i<sensors_count;++i) {
        new_list[i].name=sensors[i]->get_name();
        new_list[i].vendor=sensors[i]->get_vendor();
        new_list[i].version=1;
        new_list[i].handle=sensors[i]->handle_=i;
        new_list[i].type=sensors[i]->get_type();
        new_list[i].maxRange=sensors[i]->get_range();
        new_list[i].resolution=sensors[i]->get_resolution();
        new_list[i].power=sensors[i]->get_power();
        new_list[i].minDelay=sensors[i]->get_delay();
        memset(new_list[i].reserved,0,sizeof(new_list[i].reserved));
    }
    *list=new_list;
    return sensors_count;
}

int sensors_close(hw_device_t *dev) {
    return 0;
}

int sensors_activate(sensors_poll_device_t *dev,int handle,int enabled) {
    for(size_t i=0;i<sensors_count;++i) {
        if(handle==sensors[i]->handle_) {
            if(enabled) return sensors[i]->safe_start(&fifo)-1;
            else return sensors[i]->safe_stop()-1;
        }
    }
    return -1;
}

int sensors_setDelay(sensors_poll_device_t *dev,int handle,int64_t ns) {
    for(size_t i=0;i<sensors_count;++i) {
        if(handle==sensors[i]->handle_) {
            return sensors[i]->set_delay(ns)-1;
        }
    }
    return -1;
}

int sensors_poll(sensors_poll_device_t *dev,sensors_event_t *data,int count) {
#ifdef SINGLE_POLL
    sensors_event_t *event=fifo.pop();
    memcpy(&data[0],event,sizeof(sensors_event_t));
    delete event;
    return 1;
#else
    //return as many events as it is possible
    for(int i=0;i<count;i++) {
        //if some events were returned and fifo would block - return
        if(i>0&&fifo.empty()) return i;

        sensors_event_t *event=fifo.pop();
        memcpy(&data[i],event,sizeof(sensors_event_t));
        delete event;
    }

    return count;
#endif
}

int sensors_open(const hw_module_t *module,const char *id,hw_device_t **device) {
    sensors_poll_device_t *dev=new sensors_poll_device_t();
    memset(dev,0,sizeof(sensors_poll_device_t));
    
    dev->common.tag     = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module  = (hw_module_t*)module;
    dev->common.close   = sensors_close;
    dev->activate       = sensors_activate;
    dev->setDelay       = sensors_setDelay;
    dev->poll           = sensors_poll;

    *device=(hw_device_t *)dev;
    return 0;
}

struct hw_module_methods_t sensors_methods={
    open: sensors_open
};

struct sensors_module_t HAL_MODULE_INFO_SYM={
    common: {
        tag: HARDWARE_MODULE_TAG,
        version_major: 1,
        version_minor: 0,
        id: SENSORS_HARDWARE_MODULE_ID,
        name : "TurbaSensors HAL",
        author : "NovaFusion",
        methods: &sensors_methods
    },
    get_sensors_list: sensors_getList
};