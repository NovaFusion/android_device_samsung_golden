/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _OMX_3A_CAMERAINDEXEXT_H_
#define _OMX_3A_CAMERAINDEXEXT_H_
#define OMX_3A_CAMERA_EXTENSIONS_START_OFFSET 0x00F00000

typedef enum OMX_3A_INDEXTYPE {
  OMX_3A_IndexCameraStartUnused = OMX_IndexVendorStartUnused + OMX_3A_CAMERA_EXTENSIONS_START_OFFSET,
  OMX_3A_IndexConfigIspParameters, /**< reference: OMX_3A_CONFIG_ISPPARAMETERSTYPE */
  OMX_3A_IndexConfigExposure,      /**< reference: OMX_3A_CONFIG_EXPOSURETYPE */
  OMX_3A_IndexConfigGamma,         /**< reference: OMX_3A_CONFIG_GAMMATABLE_TYPE */
  OMX_3A_IndexConfigLsc,           /**< reference: OMX_3A_CONFIG_LSCTABLE_TYPE */
  OMX_3A_IndexConfigLinearizer,    /**< reference: OMX_3A_CONFIG_LINEARIZERTABLE_TYPE */
  OMX_3A_IndexConfigFocus,         /**< reference: OMX_3A_CONFIG_FOCUSTYPE */
  OMX_3A_IndexConfigMaxFramerate,  /**< reference: OMX_3A_CONFIG_MAXFRAMERATETYPE */
  OMX_3A_IndexConfigFlashSynchro,  /**< reference: OMX_3A_CONFIG_FLASHSYNCHROTYPE */
  OMX_3A_IndexConfigEEPROMData,    /**< reference: OMX_3A_CONFIG_EEPROMDATATYPE */
  OMX_3A_IndexConfigSensorMode,    /**< reference: OMX_3A_CONFIG_SENSORMODETYPE */
} OMX_3A_INDEXTYPE;
#endif
