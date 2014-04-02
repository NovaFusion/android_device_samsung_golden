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

#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

#include <sys/time.h>

#include <hardware/sensors.h>

struct input_event {
    timeval time;
    unsigned short type;
    unsigned short code;
    unsigned int value;
};

int64_t get_timestamp();
int64_t timeval_to_nano(timeval const &t);
bool sysfs_write(const char *path,const char *value);
sensors_event_t *get_new_event(int64_t timestamp);

#endif