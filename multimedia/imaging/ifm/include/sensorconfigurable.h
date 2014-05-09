/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

/*
 * This is a new base class for sensors that support
 * configuration of the features through variationif
 */

#ifndef __SENSORCONFIGURABLE_H__
#define __SENSORCONFIGURABLE_H__

class CSensorConfigurable : public CSensor {
public:
  CSensorConfigurable(enumCameraSlot slot);   /* Standard Constructor*/
  virtual ~CSensorConfigurable();             /* Standard destructor */

  virtual bool getResolutions(t_sensorResolutions *resolutions);
  virtual t_uint32 getBayerWidth(void);
  virtual t_uint32 getBayerHeight(void);
  virtual void GetLensParameters(t_LensParameters *LensParameters);
private:
  bool loadResolutions();
  bool resolutionsLoaded;
  t_sensorResolutions resolutions;
};

#endif /*__SENSORCONFIGURABLE_H__*/
