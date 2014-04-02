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

#include "mpu6050a.h"
#include "mpu6050g.h"
#include "alps.h"
#include "gp2a.h"

sensor_base *sensors[]={
	new mpu6050a(),
	new mpu6050g(),
	new alps(),
	new gp2a()
};

size_t sensors_count=sizeof(sensors)/sizeof(sensor_base *);

turba_fifo<sensors_event_t *> fifo;

int sensors_getList(sensors_module_t *module,sensor_t const **list) {
	sensor_t *lista=new sensor_t[sensors_count];
	for(size_t i=0;i<sensors_count;++i) {
		lista[i].name=sensors[i]->get_name();
		lista[i].vendor=sensors[i]->get_vendor();
		lista[i].version=1;
		lista[i].handle=sensors[i]->handle_=i;
		lista[i].type=sensors[i]->get_type();
		lista[i].maxRange=sensors[i]->get_range();
		lista[i].resolution=sensors[i]->get_resolution();
		lista[i].power=sensors[i]->get_power();
		lista[i].minDelay=sensors[i]->get_delay();
		memset(lista[i].reserved,0,sizeof(lista[i].reserved));
	}
	*list=lista;
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
	sensors_event_t *dada=fifo.pop();
	memcpy(data,dada,sizeof(sensors_event_t));
	delete dada;
	return 1;
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