/*
 * Copyright (C) ST-Ericsson SA 2012. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 * Author: Julius Gustavson <julius.gustavsson@stericsson.com> for ST-Ericsson.
 */
#ifndef VSYNC_MONITOR_H
#define VSYNC_MONITOR_H (1)

#include "hardware/hwcomposer.h"

int vsync_monitor_init(int compdev_fd);

int vsync_monitor_enable(struct hwc_procs* procs);

int vsync_monitor_disable();

void vsync_monitor_destroy();

#endif        //  #ifndef VSYNC_MONITOR_H

