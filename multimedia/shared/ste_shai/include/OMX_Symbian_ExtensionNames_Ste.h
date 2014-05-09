/*
  OMX_Symbian_ExtensionNames.h
  
  Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
  All rights reserved.

  This program and the accompanying materials are made available 
  under the terms of the Eclipse Public License v1.0 which accompanies 
  this distribution" and is available at 
  http://www.eclipse.org/legal/epl-v10.html

  Initial Contributors:
  Nokia Corporation - initial contribution.
*/

/** @file
@brief Symbian OpenMAX IL Extension Names.

This header file lists Symbian's extensions to OpenMAX IL.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_ExtensionNames_h
#define OMX_Symbian_ExtensionNames_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Component extensions */

#define OMX_SYMBIAN_INDEX_CONFIG_COMMITMODE_NAME            "OMX.Symbian.Index.Config.CommitMode"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMIT_NAME                "OMX.Symbian.Index.Config.Commit"
#define OMX_SYMBIAN_INDEX_CONFIG_SYMBIANTHREADPRIORITY_NAME "OMX.Symbian.Index.Config.SymbianThreadPriority"
#define OMX_SYMBIAN_INDEX_CONFIG_GPSLOCATION_NAME           "OMX.Symbian.Index.Config.GpsLocation"
#define OMX_SYMBIAN_INDEX_CONFIG_SHAREDCHUNKMETADATA_NAME   "OMX.Symbian.Index.Config.Sharedchunkmetadata"
#define OMX_SYMBIAN_INDEX_PARAM_PREEMPTIONPOLICY_NAME       "OMX.Symbian.Index.Param.PreemptionPolicy"

/* Audio parameters and configurations */
#define OMX_SYMBIAN_INDEX_PARAM_AUDIO_WMAAVGBYTES_NAME                  "OMX.Symbian.Index.Param.Audio.WmaAvgBytes"
#define OMX_SYMBIAN_INDEX_PARAM_AUDIO_WMAPRO_NAME                       "OMX.Symbian.Index.Param.Audio.WmaPro"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_AMRMODE_NAME                     "OMX.Symbian.Index.Config.Audio.AmrMode"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SBCBITPOOL_NAME                  "OMX.Symbian.Index.Config.Audio.SbcBitpool"
#define OMX_SYMBIAN_INDEX_PARAM_AUDIO_G711_NAME                         "OMX.Symbian.Index.Param.Audio.G711"
#define OMX_SYMBIAN_INDEX_PARAM_AUDIO_ILBC_NAME                         "OMX.Symbian.Index.Param.Audio.Ilbc"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_ILBCFRAME_NAME                   "OMX.Symbian.Index.Config.Audio.IlbcFrame"
#define OMX_SYMBIAN_INDEX_PARAM_AUDIO_SYNCHRONIZED_NAME                 "OMX.Symbian.Index.Param.Audio.Synchronized"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_PORTPAUSE_NAME                   "OMX.Symbian.Index.Config.Audio.PortPause"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_3DOUTPUT_NAME                    "OMX.Symbian.Index.Config.Audio.3DOutput"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_3DLOCATION_NAME                  "OMX.Symbian.Index.Config.Audio.3DLocation"
#define OMX_SYMBIAN_INDEX_PARAM_AUDIO_3DDOPPLERMODE_NAME                "OMX.Symbian.Index.Param.Audio.3DDopplerMode"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_3DDOPPLERSETTINGS_NAME           "OMX.Symbian.Index.Config.Audio.3DDopplerSettings"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_3DLEVELS_NAME                    "OMX.Symbian.Index.Config.Audio.3DLevels"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_3DDISTANCEATTENUATION_NAME       "OMX.Symbian.Index.Config.Audio.3DDistanceAttenuation"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_3DDIRECTIVITYSETTINGS_NAME       "OMX.Symbian.Index.Config.Audio.3DDirectivitySettings"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_3DDIRECTIVITYORIENTATION_NAME    "OMX.Symbian.Index.Config.Audio.3DDirectivityOrientation"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_3DMACROSCOPICORIENTATION_NAME    "OMX.Symbian.Index.Config.Audio.3DMacroscopicOrientation"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_3DMACROSCOPICSIZE_NAME           "OMX.Symbian.Index.Config.Audio.3DMacroscopicSize"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_VIRTUALIZER_NAME                 "OMX.Symbian.Index.Config.Audio.Virtualizer"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_VIRTUALIZERLOUDSPEAKER_NAME      "OMX.Symbian.Index.Config.Audio.VirtualizerLoudspeaker"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_VISUALIZATION_NAME               "OMX.Symbian.Index.Config.Audio.Visualization"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_VISUALIZATIONSPECTRAL_NAME       "OMX.Symbian.Index.Config.Audio.VisualizationSpectral"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_VISUALIZATIONWAVEFORM_NAME       "OMX.Symbian.Index.Config.Audio.VisualizationWaveform"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_TRANSEQUALIZER_NAME              "OMX.Symbian.Index.Config.Audio.TransEqualizer"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_TRANSEQUALIZERIIREQ_NAME         "OMX.Symbian.Index.Config.Audio.TransEqualizerIirEq"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_TRANSEQUALIZERFIRRESPONSE_NAME   "OMX.Symbian.Index.Config.Audio.TransEqualizerFirResponse"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_MDRC_NAME                        "OMX.Symbian.Index.Config.Audio.Mdrc"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_LSPROT_NAME                      "OMX.Symbian.Index.Config.Audio.LsProt"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SPLLIMIT_NAME                    "OMX.Symbian.Index.Config.Audio.SplLimit"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_CLICKREMOVAL_NAME                "OMX.Symbian.Index.Config.Audio.ClickRemoval"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_EANC_NAME                        "OMX.Symbian.Index.Config.Audio.Eanc"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_APSSAS_NAME                      "OMX.Symbian.Index.Config.Audio.Apssas"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_BATTERYVOLTAGE_NAME              "OMX.Symbian.Index.Config.Audio.BatteryVoltage"
#define OMX_SYMBIAN_INDEX_PARAM_AUDIO_HDMI_NAME                         "OMX.Symbian.Index.Param.Audio.Hdmi"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_TONEGENERATOR_NAME               "OMX.Symbian.Index.Config.Audio.ToneGenerator"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_POWER_NAME                       "OMX.Symbian.Index.Config.Audio.Power"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_VOLUMERAMP_NAME                  "OMX.Symbian.Index.Config.Audio.VolumeRamp"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_REGISTERBANKQUERY_NAME           "OMX.Symbian.Index.Config.Audio.RegisterBankQuery"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_REGISTERBANKTABLE_NAME           "OMX.Symbian.Index.Config.Audio.RegisterBankTable"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_CUSTOMHWCONTROL_NAME             "OMX.Symbian.Index.Config.Audio.CustomHwControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_HWLOOPSUPPORT_NAME               "OMX.Symbian.Index.Config.Audio.HwLoopSupport"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_HWLOOPCONTROL_NAME               "OMX.Symbian.Index.Config.Audio.HwLoopControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_DATAAMOUNT_NAME                  "OMX.Symbian.Index.Config.Audio.DataAmount"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_DECODERSTATUS_NAME               "OMX.Symbian.Index.Config.Audio.DecoderStatus"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_ENCODERSTATUS_NAME               "OMX.Symbian.Index.Config.Audio.EncoderStatus"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_UPLINKTIMIMG_NAME                "OMX.Symbian.Index.Config.Audio.UplinkTiming"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_RFTXPOWER_NAME                   "OMX.Symbian.Index.Config.Audio.RfTxPower"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_UPLINKTIMINGINFO_NAME            "OMX.Symbian.Index.Config.Audio.UplinkTimingInfo"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_UPLINKDELAYINFO_NAME             "OMX.Symbian.Index.Config.Audio.UplinkDelayInfo"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_UPLINKALGORITHMS_NAME            "OMX.Symbian.Index.Config.Audio.UplinkAlgorithms"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_DOWNLINKALGORITHMS_NAME          "OMX.Symbian.Index.Config.Audio.DownlinkAlgorithms"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_ACOUSTICECHOCONTROL_NAME         "OMX.Symbian.Index.Config.Audio.AcousticEchoControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_BANDWIDTHEXPANSION_NAME          "OMX.Symbian.Index.Config.Audio.BandwidthExpansion"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_AUTOMATICVOLUMECONTROL_NAME      "OMX.Symbian.Index.Config.Audio.AutomaticVolumeControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_BACKGROUNDNOISECONTROL_NAME      "OMX.Symbian.Index.Config.Audio.BackgroundNoiseControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_COMFORTNOISEGENERATION_NAME      "OMX.Symbian.Index.Config.Audio.ComfortNoiseGeneration"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_DOWNLINKNOISECONTROL_NAME        "OMX.Symbian.Index.Config.Audio.DownLinkNoiseControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_MULTIMICNOISECONTROL_NAME        "OMX.Symbian.Index.Config.Audio.MultiMicNoiseControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SIDETONEHOWLINGCONTROL_NAME      "OMX.Symbian.Index.Config.Audio.SidetoneHowlingControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SIDETONEGAIN_NAME                "OMX.Symbian.Index.Config.Audio.SidetoneGain"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SIDETONEATTENUATION_NAME         "OMX.Symbian.Index.Config.Audio.SidetoneAttenuation"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_TRANSDUCERDRC_NAME               "OMX.Symbian.Index.Config.Audio.TransducerDrc"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_UPLINKLEVELNORMALIZATION_NAME    "OMX.Symbian.Index.Config.Audio.UplinkLevelNormalization"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_VOICECLARITY_NAME                "OMX.Symbian.Index.Config.Audio.VoiceClarity"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_WINDNOISECONTROL_NAME            "OMX.Symbian.Index.Config.Audio.WindNoiseControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_TUNINGDATA_NAME                  "OMX.Symbian.Index.Config.Audio.TuningData"
#define OMX_SYMBIAN_INDEX_PARAM_AUDIO_HAPTICSDRIVERPROPERTIES_NAME      "OMX.Symbian.Index.Param.Audio.HapticsDriverProperties"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SPEECHCODECENABLED_NAME          "OMX.Symbian.Index.Config.Audio.SpeechCodecEnabled"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SPEECHCODECDISABLED_NAME         "OMX.Symbian.Index.Config.Audio.SpeechCodecDisabled"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_AUTOMATICGAINCONTROL_NAME        "OMX.Symbian.Index.Config.Audio.AutomaticGainControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_MULTIMICROPHONENOISECONTROL_NAME "OMX.Symbian.Index.Config.Audio.MultiMicrophoneNoiseControl"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SPEECHLOOPS_NAME                 "OMX.Symbian.Index.Config.Audio.SpeechLoops"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SIDETONEATTENUATIONINFO_NAME     "OMX.Symbian.Index.Config.Audio.SidetoneAttenuationInfo"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_VOLUMEINFO_NAME                  "OMX.Symbian.Index.Config.Audio.VolumeInfo"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_DOWNLINKFRAMEINFO_NAME           "OMX.Symbian.Index.Config.Audio.DownlinkFrameInfo"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SAMPLERATE8KHZ_NAME              "OMX.Symbian.Index.Config.Audio.SampleRate8khz"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_SAMPLERATE16KHZ_NAME             "OMX.Symbian.Index.Config.Audio.SampleRate16khz"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_TRACINGDATA_NAME                 "OMX.Symbian.Index.Config.Audio.TracingData"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_PHYSICALTRANSPORTTYPE_NAME       "OMX.Symbian.Index.Config.Audio.PhysicalTransportType"
#define OMX_SYMBIAN_INDEX_CONFIG_AUDIO_POWERSAVE_NAME                   "OMX.Symbian.Index.Config.Audio.PowerSave"

/* Image parameters and configurations */
#define OMX_SYMBIAN_INDEX_CONFIG_IMAGE_MAXJPEGSIZE_NAME   "OMX.Symbian.Index.Config.Image.MaxJpegSize"

/* Video parameters and configurations */
#define OMX_SYMBIAN_INDEX_CONFIG_VIDEO_SEQUENCEHEADER_NAME  "OMX.Symbian.Index.Config.Video.SequenceHeader"
/* OMX_SYMBIAN_INDEX_PARAM_VIDEO_SEQUENCEHEADER_NAME is deprecrated */
#define OMX_SYMBIAN_INDEX_PARAM_VIDEO_SEQUENCEHEADER_NAME   "OMX.Symbian.Index.Param.Video.SequenceHeader"

#define OMX_SYMBIAN_INDEX_PARAM_VIDEO_AVCSEI_NAME           "OMX.Symbian.Index.Param.Video.AvcSei"
#define OMX_SYMBIAN_INDEX_PARAM_VIDEO_AVCOUTPUTORDER_NAME   "OMX.Symbian.Index.Param.Video.AvcOutputOrder"
#define OMX_SYMBIAN_INDEX_CONFIG_VIDEO_REFRESHMODE_NAME     "OMX.Symbian.Index.Config.Video.RefreshMode"
#define OMX_SYMBIAN_INDEX_CONFIG_VIDEO_OUTPUTRECT_NAME      "OMX.Symbian.Index.Config.Video.OutputRect"
#define OMX_SYMBIAN_INDEX_PARAM_VIDEO_VC1_NAME              "OMX.Symbian.Index.Param.Video.Vc1"
#define OMX_SYMBIAN_INDEX_PARAM_VIDEO_VC6_NAME              "OMX.Symbian.Index.Param.Video.Vp6"
#define OMX_SYMBIAN_INDEX_PARAM_VIDEO_EXTENDEDPROFILELEVELQUERYSUPPORTED_NAME		"OMX.Symbian.Index.Param.Video.ExtendedProfileLevelQuerySupported"

/* Image & Video common configurations */
#define OMX_SYMBIAN_INDEX_PARAM_COMMON_PIXELASPECTRATIO_NAME              "OMX.Symbian.Index.Param.Common.PixelAspectRatio"
#define OMX_SYMBIAN_INDEX_PARAM_COMMON_COLORPRIMARY_NAME                  "OMX.Symbian.Index.Param.Common.ColorPrimary"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_CAMERAFUNCTESTSELECT_NAME         "OMX.Symbian.Index.Config.Common.CameraFuncTestSelect"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_CAMERASELFTESTSELECT_NAME         "OMX.Symbian.Index.Config.Common.CameraSelfTestSelect" 
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_CAMERATESTING_NAME                "OMX.Symbian.Index.Config.Common.CameraTesting" 
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_SCENEMODE_NAME                    "OMX.Symbian.Index.Config.Common.SceneMode"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_RGBHISTOGRAM_NAME                 "OMX.Symbian.Index.Config.Common.RgbHistogram" 
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_HISTOGRAMCONTROL_NAME             "OMX.Symbian.Index.Config.Common.HistogramControl" 
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_XENONLIFECOUNTER_NAME             "OMX.Symbian.Index.Config.Common.XenonLifeCounter" 
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_ROISELECTION_NAME                 "OMX.Symbian.Index.Config.Common.RoiSelection"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_REDCOMPLEXITY_NAME                "OMX.Symbian.Index.Config.Common.RedComplexity"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_REDEYEREMOVAL_NAME                "OMX.Symbian.Index.Config.Common.RedEyeRemoval"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_ORIENTATIONSCENE_NAME             "OMX.Symbian.Index.Config.Common.OrientationScene"
#define OMX_SYMBIAN_INDEX_PARAM_COMMON_REVERTASPECTRATIO_NAME             "OMX.Symbian.Index.Param.Common.RevertAspectRatio"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_NDFILTERCONTROL_NAME              "OMX.Symbian.Index.Config.Common.NdFilterControl"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_EXPOSURELOCK_NAME                 "OMX.Symbian.Index.Config.Common.ExposureLock"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_WHITEBALANCELOCK_NAME             "OMX.Symbian.Index.Config.Common.WhiteBalanceLock"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_FOCUSLOCK_NAME                    "OMX.Symbian.Index.Config.Common.FocusLock"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_ALLLOCK_NAME                      "OMX.Symbian.Index.Config.Common.AllLock"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_CENTERFIELDOFVIEW_NAME            "OMX.Symbian.Index.Config.Common.CenterFieldOfView"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_EXTDIGITALZOOM_NAME               "OMX.Symbian.Index.Config.Common.ExtDigitalZoom"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_EXTOPTICALZOOM_NAME               "OMX.Symbian.Index.Config.Common.ExtOpticalZoom"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_FOCUSRANGE_NAME                   "OMX.Symbian.Index.Config.Common.FocusRange"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_EXTFOCUSSTATUS_NAME               "OMX.Symbian.Index.Config.Common.ExtFocusStatus"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_FLICKERREMOVAL_NAME               "OMX.Symbian.Index.Config.Common.FlickerRemoval"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_FLASHCONTROL_NAME                 "OMX.Symbian.Index.Config.Common.FlashControl"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_AFASSISTANTLIGHT_NAME             "OMX.Symbian.Index.Config.Common.AfAssistantLight"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_HINTPOWERVSQUALITY_NAME           "OMX.Symbian.Index.Config.Common.HintPowerVsQuality"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_HINTDEPTHOFFIELD_NAME             "OMX.Symbian.Index.Config.Common.HintDepthOfField"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_FOCUSREGION_NAME                  "OMX.Symbian.Index.Config.Common.FocusRegion"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_ROI_NAME                          "OMX.Symbian.Index.Config.Common.Roi"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_CAMERASENSORINFO_NAME             "OMX.Symbian.Index.Config.Common.CameraSensorInfo"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_FLASHGUNINFO_NAME                 "OMX.Symbian.Index.Config.Common.FlashGunInfo"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_MOTIONLEVEL_NAME                  "OMX.Symbian.Index.Config.Common.MotionLevel"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_SHARPNESS_NAME                    "OMX.Symbian.Index.Config.Common.Sharpness"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_EXTCAPTURING_NAME                 "OMX.Symbian.Index.Config.Common.ExtCapturing"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_EXTCAPTUREMODE_NAME               "OMX.Symbian.Index.Config.Common.ExtCaptureMode"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_XENONFLASHSTATUS_NAME             "OMX.Symbian.Index.Config.Common.XenonFlashStatus"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_PRECAPTUREEXPOSURETIME_NAME       "OMX.Symbian.Index.Config.Common.PreCaptureExposureTime" 
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_EXPOSUREINITIATED_NAME            "OMX.Symbian.Index.Config.Common.ExposureInitiated"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_BRACKETING_NAME                   "OMX.Symbian.Index.Config.Common.Bracketing"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_RAWPRESET_NAME                    "OMX.Symbian.Index.Config.Common.RawPreset"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_APPLYUSERSETTINGS_NAME            "OMX.Symbian.Index.Config.Common.ApplyUserSettings"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_VIDEOLIGHT_NAME                   "OMX.Symbian.Index.Config.Common.VideoLight"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_AUTOROTATIONCONTROL_NAME          "OMX.Symbian.Index.Config.Common.AutoRotationControl"  
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_SUPPORTEDRESOLUTIONS_NAME         "OMX.Symbian.Index.Config.Common.SupportedResolutions"
#define OMX_SYMBIAN_INDEX_PARAM_COMMON_DEVICEMAKEMODEL_NAME               "OMX.Symbian.Index.Param.Common.DeviceMakeModel"  
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_LOWLIGHTNOISEREDUCTION_NAME       "OMX.Symbian.Index.Config.Common.LowLightNoiseReduction"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_SENSORNVMDATA_NAME                "OMX.Symbian.Index.Config.Common.SensorNvmData"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_ANALYZERFEEDBACK_NAME             "OMX.Symbian.Index.Config.Common.AnalyzerFeedback"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_EXTRADATA_NAME                    "OMX.Symbian.Index.Config.Common.Extradata"
#define OMX_SYMBIAN_INDEX_CONFIG_COMMON_DEVICESOFTWARE_NAME               "OMX.Symbian.Index.Param.Common.Software"
#define OMX_SYMBIAN_INDEX_CONFIG_ZOOM_SUPPORTED_RESOLUTIONS               "OMX.Symbian.Index.Param.Zoom.Supported.Resolutions"
/* Other configurations */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_ExtensionNames_h */
