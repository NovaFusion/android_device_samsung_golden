/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef __VARIATIONIF_H__
#define __VARIATIONIF_H__

#include "OMX_Core.h"
#include "sensor.h"

class VariationIf {
public:
  /*
   * Will return number of resolutions.
   * @param sensor_id  Sensor Identification Data (in)
   * @param resolutions Sensor Resolution Table (out)
   * @param numberOfNonActiveLines Number of Non-Active lines fro Sensor. If not available, it is -1 (out)
   */
  virtual OMX_ERRORTYPE listOfResolutions(t_sensorDesciptor* sensor_id, t_sensorResolutions* resolutions, int *numberOfNonActiveLines = NULL);
  static VariationIf* getInstance();
  static void releaseInstance(VariationIf* instance);

protected:
  VariationIf();
  virtual ~VariationIf();
};

#endif /*__VARIATIONIF_H__*/
