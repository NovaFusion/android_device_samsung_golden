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

#include "alps.h"

#include <cstdio>

#include <unistd.h>
#include <fcntl.h>

#include <sys/poll.h>
#include <sys/ioctl.h>

#include "utils.h"

#define ALPSIO_SET_MAGACTIVATE _IOW(0xAF,0,int)
#define ALPSIO_SET_DELAY _IOW(0xAF,2,int)

const char *alps::get_name() { return "ALPS 3-axis Compass"; }
const char *alps::get_vendor() { return "ALPS Electric"; }
int alps::get_type() { return SENSOR_TYPE_MAGNETIC_FIELD; }
float alps::get_range() { return 10240.0f; }
float alps::get_resolution() { return 1.0f; }
float alps::get_power() { return 0.5f; }
int32_t alps::get_delay() { return 10000; }

bool alps::set_delay(int64_t ns) {
	int ctlfd=open("/dev/alps_compass_io",O_WRONLY);
	if(ctlfd!=-1) {
		//int ms=ns*0.000001;
		int ms=ns*0.000001;
		ioctl(ctlfd,ALPSIO_SET_DELAY,&ms);
		close(ctlfd);
		return true;
	}
	return false;
}

void alps::poller() {
	pollfd rfd;
	rfd.fd=fd;
	rfd.events=POLLIN;
	input_event event;
	int x=0; int y=0;
	while(true) {
		if(poll(&rfd,1,500)==1) {
			if(rfd.revents&POLLIN) {
				if(read(fd,&event,sizeof(struct input_event))==sizeof(input_event)) {
					if(event.type==2) {
						if(event.code==0) x=event.value;
						if(event.code==1) y=event.value;
						if(event.code==2) {
							sensors_event_t *sensors_event=get_new_event(timeval_to_nano(event.time));
							sensors_event->sensor=handle_;
							sensors_event->type=get_type();
							sensors_event->magnetic.x=x/7.0f+19.0f;
							sensors_event->magnetic.y=y/7.0f+11.5f;
							sensors_event->magnetic.z=int(event.value)/7.0f-24.5f;
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

void *alps::poller_helper(void *ctx) {
	((alps *)ctx)->poller();
	return NULL;
}

bool alps::start() {
	fd=open("/dev/input/event5",O_RDONLY);
	if(fd!=-1) {
		int ctlfd=open("/dev/alps_compass_io",O_WRONLY);
		if(ctlfd!=-1) {
			int arg=1;
			ioctl(ctlfd,ALPSIO_SET_MAGACTIVATE,&arg);
			close(ctlfd);
			
			pthread_t t1;
			pthread_create(&t1,NULL,&alps::poller_helper,this);
			return true;
		}
		else {
			close(fd);
			return false;
		}
	}
	return false;
}

bool alps::stop() {
	int ctlfd=open("/dev/alps_compass_io",O_WRONLY);
	if(ctlfd!=-1) {
		int arg=0;
		ioctl(ctlfd,ALPSIO_SET_MAGACTIVATE,&arg);
		close(ctlfd);
	}

	close(fd);
	return true;
}