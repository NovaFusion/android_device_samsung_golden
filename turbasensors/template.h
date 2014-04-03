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

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <hardware/sensors.h>

#include "turbafifo.h"

#define SENSOR_IMPLEMENTATION \
public: \
    const char *get_name(); \
    const char *get_vendor(); \
    int get_type(); \
    float get_range(); \
    float get_resolution(); \
    float get_power(); \
    int32_t get_delay(); \
    bool set_delay(int64_t ns); \
private: \
    bool start(); \
    bool stop();

class sensor_base {
public:
    virtual ~sensor_base() { }
    virtual const char *get_name()=0;
    virtual const char *get_vendor()=0;
    virtual int get_type()=0;
    virtual float get_range()=0;
    virtual float get_resolution()=0;
    virtual float get_power()=0;
    virtual int32_t get_delay()=0;
    virtual bool set_delay(int64_t ns)=0;
    bool safe_start(turba_fifo<sensors_event_t *> *queue);
    bool safe_stop();
    turba_fifo<sensors_event_t *> *queue_;
    int handle_;

private:
    virtual bool start()=0;
    virtual bool stop()=0;
    bool enabled_;
};

#endif