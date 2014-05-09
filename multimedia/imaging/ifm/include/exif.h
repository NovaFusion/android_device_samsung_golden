/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _EXIF_H_
#define _EXIF_H_

#include "OMX_Types.h"
#include "IFM_Types.h"
#include "ImgConfig.h"

#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CApp1Header);
#endif
class CApp1Header
{
   public:
      CApp1Header();
      OMX_U32 size();
      void    setLenght(OMX_U16);
      OMX_U32 serialize(OMX_U8*);

   private:
      /* Exif 2.2 compliant packed structure ready for memory copy */
      typedef struct {
          OMX_U8  iMarkerPrefix;
          OMX_U8  iApp1Marker;
          OMX_U16 iApp1Length;
          OMX_U8  iExifId[5];
          OMX_U8  iPadding;
      } App1Header_t;

      App1Header_t iHeaderData;
};

#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CTiffHeader);
#endif
class CTiffHeader
{
   public:
      CTiffHeader();
      OMX_U32 size();
      void    setIfdOffset(OMX_U32);
      OMX_U32 serialize(OMX_U8*);

   private:
      /* Exif 2.2 compliant packed structure ready for memory copy */
      typedef struct {
          OMX_U8  iByteOrder[2];
          OMX_U16 iFourtyTwo;
          OMX_U32 iIfdOffset;
      } TiffHeader_t;

      TiffHeader_t iHeaderData;
};

/** @brief Main/EXIF field types. */
typedef enum {
   IFD_TYPE_UNKNOWN = 0,
   IFD_BYTE         = 1,
   IFD_ASCII        = 2,
   IFD_SHORT        = 3,
   IFD_LONG         = 4,
   IFD_RATIONAL     = 5,
   IFD_UNDEFINED    = 7,
   IFD_SLONG        = 9,
   IFD_SRATIONAL    = 10
} IFDtype_e;


/** @brief Main/EXIF field unique identifiers. */
typedef enum {
   IFDtag_Index			=1,
   IFDtag_Version			=2,
   IFDtag_Make                    = 271,
   IFDtag_Model                   = 272,
   IFDtag_Orientation             = 274,
   IFDtag_XResolution             = 282,
   IFDtag_YResolution             = 283,
   IFDtag_ResolutionUnit          = 296,
   IFDtag_Software          	  = 305,
   IFDtag_DateTime		          = 306,
   IFDtag_YCbCrPositioning        = 531,
   IFDtag_ExposureTime            = 33434,
   IFDtag_FNumber                 = 33437,
   IFDtag_ExifIfdPointer          = 34665,
   IFDtag_GpsInfoIfdPointer       = 34853,
   IFDtag_ISOSpeedRatings         = 34855,
   IFDtag_ExifVersion             = 36864,
   IFDtag_DateTimeOriginal        = 36867,
   IFDtag_DateTimeDigitized       = 36868,
   IFDtag_ComponentsConfiguration = 37121,
   IFDtag_ShutterSpeedValue       = 37377,
   IFDtag_ApertureValue           = 37378,
   IFDtag_BrightnessValue         = 37379,
   IFDtag_ExposureBiasValue       = 37380,
   IFDtag_SubjectDistance         = 37382,
   IFDtag_MeteringMode            = 37383,
   IFDtag_LightSource             = 37384,
   IFDtag_Flash                   = 37385,
   IFDtag_FocalLength             = 37386,
   IFDtag_SubjectArea             = 37396,
   IFDtag_MakerNote               = 37500,
   IFDtag_FlashpixVersion         = 40960,
   IFDtag_ColorSpace              = 40961,
   IFDtag_PixelXDimension         = 40962,
   IFDtag_PixelYDimension         = 40963,
   IFDtag_InteroperabilityIFDPointer = 40965,
   IFDtag_FlashEnergy             = 41483,
   IFDtag_ExposureIndex           = 41493,
   IFDtag_CustomRendered          = 41985,
   IFDtag_ExposureMode            = 41986,
   IFDtag_WhiteBalance            = 41987,
   IFDtag_DigitalZoomRatio        = 41988,
   IFDtag_SceneCaptureType        = 41990,
   IFDtag_GainControl             = 41991,
   IFDtag_Contrast                = 41992,
   IFDtag_SubjectDistanceRange	  = 41996
} IFDtag_e;


/** @brief Main/EXIF field header.
 *
 *  Contains the actual field value in valueOffset if and only if it fits in valueOffset.
 *  Otherwise, valueOffset is the offset from the tiff header start to the field value,
 *  which may be stored anywhere in the file.
 *
 */
typedef struct {
   OMX_U16 tag;
   /** IFDtype_e avatar. Using explicit storage type rather than enum in order to avoid forcing
    * the structure layout at compile time. */
   OMX_U16 type;
   OMX_U32 count;
   OMX_U32 valueOffset;
} IFDfield_t;


/** @brief Main/EXIF field descriptor structure.
 *
 * A IFDfieldDesc_t static array is intended to associate the Main/EXIF tags to the field type
 * and count.
 * */
typedef struct {
   IFDtag_e tag;
   IFDtype_e type;
   OMX_U8 count;
} IFDfieldDesc_t;

/** Rational entries definitions */
/** Exif 2.2 compliant packed structure ready for memory copy */
typedef struct {
    OMX_U32 num;
    OMX_U32 den;
} exif_urational_t;

typedef struct {
    OMX_S32 num;
    OMX_S32 den;
} exif_srational_t;

typedef OMX_U8 exif_undefined_t;


/** @brief Generic Image Field Directory class. */

#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CIfd);
#endif

class CIfd
{
   public:
      CIfd(IFDfield_t*, OMX_U16, OMX_U32, OMX_U8*, OMX_U32);
      OMX_U32       size();
      OMX_ERRORTYPE setFieldFromTag(IFDtag_e aTag, const char* pData, int aLength);
      OMX_ERRORTYPE setFieldFromTag(IFDtag_e aTag, OMX_U8 aData);
      OMX_ERRORTYPE setFieldFromTag(IFDtag_e aTag, const exif_undefined_t* pData, int aCount);
      OMX_ERRORTYPE setFieldFromTag(IFDtag_e aTag, OMX_U16 aData);
      OMX_ERRORTYPE setFieldFromTag(IFDtag_e aTag, const OMX_U16* pData, int aCount);
      OMX_ERRORTYPE setFieldFromTag(IFDtag_e aTag, OMX_U32 aData);
      OMX_ERRORTYPE setFieldFromTag(IFDtag_e aTag, OMX_S32 aData);
      OMX_ERRORTYPE setFieldFromTag(IFDtag_e aTag, const exif_urational_t* pData);
      OMX_ERRORTYPE setFieldFromTag(IFDtag_e aTag, const exif_srational_t* pData);
      void          setNextIfdOffset(OMX_U16);
      bool          initialized();
      OMX_U32       serialize(OMX_U8* pData, OMX_U32 aOffsetfromTiffHeader);

   protected:
      // Functions
      OMX_ERRORTYPE initField(IFDfield_t* pField, IFDtag_e aTag);

   private:
      // Functions
      OMX_ERRORTYPE         setFieldFromTag(IFDtag_e aTag, const void* pData, IFDtype_e aType, int aCount);
      const IFDfieldDesc_t* getFieldDescFromTag(IFDtag_e aTag);
      IFDfield_t*           getFieldFromTag(IFDtag_e aTag);
      OMX_ERRORTYPE         storeField(const void* pData, OMX_U32 aSize, OMX_U32* pValueOffset);
      void                  patchLargeValuesOffsets(OMX_U32);
      int                   sizeOfType(IFDtype_e);
      // Members
      IFDfield_t* pFields;              /* Pointer to EXIF entries of the derived class */
      OMX_U16     iNumberOfFields;      /* Number of entries in the derived class */
      OMX_U32     iFieldsSize;          /* Total size of entries pointed to by pFields */
      OMX_U8*     pLargeValuesArea;     /* Points to the extended storage area of the derived class */
      OMX_U32     iLargeValuesAreaSize; /* Size of the Large Values storage area buffer */
      OMX_U32     iLargeDataSizeActual; /* Actual size of data stored in the Large Values Area */
      OMX_U32     iNextIfdOffset;       /* Offset of the next IFD */
};

/** Main picture extradata */
#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CMainIfd);
#endif

class CMainIfd: public CIfd
{
   public:
     CMainIfd();

   private:
      struct
      {
          /*
           * Main IFD definition. Packed structure ready for memcopy.
           * Contains the supported subset of possible EXIF 2.2 fields.
           * Field ordering _DOES_ matter for EXIF conformance.
           */
          IFDfield_t Make;               // 271
          IFDfield_t Model;              // 272
          IFDfield_t Orientation;        // 274
          IFDfield_t XResolution;        // 282
          IFDfield_t YResolution;        // 283
          IFDfield_t ResolutionUnit;     // 296
          IFDfield_t Software;     		 // 305
          IFDfield_t DateTime;     		 // 306
          IFDfield_t YCbCrPositioning;   // 531
          IFDfield_t ExifIfdPointer;     // 34665
          IFDfield_t GpsInfoIfdPointer;  // 34853
      } iMainIfdFields;

      // Extended storage area for fields having value size greater that 4 bytes
      OMX_U8 iMainIfdLargeValuesArea[256];
};

#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CInterOpIfd);
#endif

class CInterOpIfd: public CIfd
{
   public:
     CInterOpIfd();

   private:
      struct
      {
          /*
           * Interoperability IFD definition. Packed structure ready for memcopy.
           * Contains the supported subset of possible EXIF 2.2 fields.
           * Field ordering _DOES_ matter for EXIF conformance.
           */         
          IFDfield_t Index;               // 001
          IFDfield_t Version;              // 002
      } iInterOpIfdFields;
// Extended storage area for fields having value size greater that 4 bytes
      OMX_U8 iInterOpIfdLargeValuesArea[0];
};
#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CExifIfd);
#endif

class CExifIfd: public CIfd
{
   public:
      CExifIfd();

   private:
     struct
     {
        /*
         * EXIF IFD definition. Packed structure ready for memcopy.
         * Contains the supported subset of possible EXIF 2.2 fields.
         * Field ordering _DOES_ matter for EXIF conformance.
         */
        IFDfield_t ExposureTime;            // 33434
        IFDfield_t FNumber;                 // 33437
        IFDfield_t ISOSpeedRatings;         // 34855
        IFDfield_t ExifVersion;             // 36864
        IFDfield_t DateTimeOriginal;        // 36867
        IFDfield_t DateTimeDigitized;       // 36868
        IFDfield_t ComponentsConfiguration; // 37141
        IFDfield_t ShutterSpeedValue;       // 37377
#ifndef CUSTOMER_EXIF
 	 IFDfield_t ApertureValue;           // 37378
        IFDfield_t BrightnessValue;         // 37379
#endif
	 IFDfield_t ExposureBiasValue;    //37380
#ifndef CUSTOMER_EXIF
        IFDfield_t SubjectDistance;         // 37382
#endif        
        IFDfield_t MeteringMode;            // 37383
        IFDfield_t LightSource;             // 37384
        IFDfield_t Flash;                   // 37385
        IFDfield_t FocalLength;             // 37386
#ifndef CUSTOMER_EXIF
        IFDfield_t SubjectArea;             // 37396
#endif
#if (IMG_CONFIG_EXIF_MAKER_NOTES == IMG_CONFIG_TRUE)
        IFDfield_t MakerNote;               // 37500
#endif
        IFDfield_t FlashpixVersion;         // 40960
        IFDfield_t ColorSpace;              // 40961
        IFDfield_t PixelXDimension;         // 40962
        IFDfield_t PixelYDimension;         // 40963
	 IFDfield_t InteroperabilityIFDPointer;  //40965
#ifndef CUSTOMER_EXIF
	 IFDfield_t ExposureIndex;           // 41493
#endif
        IFDfield_t CustomRendered;          // 41985
        IFDfield_t ExposureMode;            // 41986
        IFDfield_t WhiteBalance;            // 41987
        IFDfield_t DigitalZoomRatio;        // 41988
        IFDfield_t SceneCaptureType;        // 41990
#ifndef CUSTOMER_EXIF
        IFDfield_t GainControl;             // 41991
        IFDfield_t Contrast;                // 41992
#endif
        IFDfield_t SubjectDistanceRange;    // 41996
     } iExifIfdFields;

     // Extended storage area for fields having value size greater that 4 bytes
     OMX_U8 iExifIfdLargeValuesArea[25*1024];
};

#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CExifMetadata);
#endif
class CExifMetadata
{
   public:
      CExifMetadata();
      OMX_ERRORTYPE serialize(const IFM_EXIF_SETTINGS* pExifSettings, OMX_U8* pBuffer, const OMX_U32 aSize);
      OMX_ERRORTYPE deserialize(IFM_EXIF_SETTINGS* pExifSettings, const OMX_U8* pBuffer, const OMX_U32 aSize);
      OMX_U32       size();

   private:
      // Private data
      CApp1Header   iApp1Header;
      CTiffHeader   iTiffHeader;
      CMainIfd      iMainIFD;
      CExifIfd      iExifIFD;
      CInterOpIfd iInterOpIFD;
      // Private functions
      OMX_ERRORTYPE initialize(const IFM_EXIF_SETTINGS* pExifSettings);
      OMX_ERRORTYPE setMake(const char*);
      OMX_ERRORTYPE setModel(const char*);
      OMX_ERRORTYPE setOrientation(OMX_SYMBIAN_ORIENTATIONORIGINTYPE);
      OMX_ERRORTYPE setXResolution(OMX_U32);
      OMX_ERRORTYPE setYResolution(OMX_U32);
      OMX_ERRORTYPE setResolutionUnit(IFM_RESOLUTION_UNIT);
      OMX_ERRORTYPE setYCbCrPositioning(IFM_YCbCr_POSITIONING);
      OMX_ERRORTYPE setExposureTime(OMX_U32);
      OMX_ERRORTYPE setFNumber(OMX_S32);
      OMX_ERRORTYPE setIsoSpeedRatings(OMX_U32);
      OMX_ERRORTYPE setDateTimeOriginal(const IFM_DATE_TIME*);
      OMX_ERRORTYPE setDateTimeDigitized(const IFM_DATE_TIME*);
      OMX_ERRORTYPE setComponentsConfiguration(IFM_COMPONENTS_CONFIGURATION);
      OMX_ERRORTYPE setShutterSpeedValue(OMX_U32);
      OMX_ERRORTYPE setApertureValue(OMX_U32);
      OMX_ERRORTYPE setBrightnessValue(OMX_U32);
      OMX_ERRORTYPE setSubjectDistance(OMX_U32);
      OMX_ERRORTYPE setMeteringMode(OMX_METERINGTYPE);
      OMX_ERRORTYPE setLightSource(OMX_WHITEBALCONTROLTYPE);
      OMX_ERRORTYPE setFlash(OMX_BOOL,OMX_IMAGE_FLASHCONTROLTYPE,OMX_BOOL);
      OMX_ERRORTYPE setFocalLength(float);
      OMX_ERRORTYPE setSubjectArea(OMX_SYMBIAN_RECTTYPE);
#if (IMG_CONFIG_EXIF_MAKER_NOTES == IMG_CONFIG_TRUE)
      OMX_ERRORTYPE setMakerNote(const void*, int);
#endif
      OMX_ERRORTYPE setColorSpace(IFM_COLOR_SPACE);
      OMX_ERRORTYPE setPixelXDimension(OMX_U32);
      OMX_ERRORTYPE setPixelYDimension(OMX_U32);
      OMX_ERRORTYPE setExposureIndex(OMX_S32);
      OMX_ERRORTYPE setCustomRendered(OMX_BOOL);
      OMX_ERRORTYPE setExposureMode(OMX_EXPOSURECONTROLTYPE, OMX_BOOL);
      OMX_ERRORTYPE setWhiteBalance(OMX_WHITEBALCONTROLTYPE);
      OMX_ERRORTYPE setDigitalZoomRatio(OMX_U32,OMX_U32);
      OMX_ERRORTYPE setSceneCaptureType(OMX_SYMBIAN_SCENEMODETYPE);
      OMX_ERRORTYPE setContrast(OMX_S32);
      OMX_ERRORTYPE setGainControl(OMX_S32,OMX_S32);
      OMX_ERRORTYPE setDateTime(const IFM_DATE_TIME*);
      OMX_ERRORTYPE setSoftware(const char*);
      OMX_ERRORTYPE setExposureBias(OMX_S32 );
      OMX_ERRORTYPE setInteroperabilityIFDPointer(OMX_U32);
      OMX_ERRORTYPE setSubjectDistanceRange(OMX_U32);

      OMX_ERRORTYPE setInterOperabilityIndex(const char*);
      OMX_ERRORTYPE setInterOperabilityVersion();
};

#if defined (DISPLAY_EXIF_METADATA)
#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CExifReader);
#endif
class CExifReader
{
   /* ------------------------------------------------------------------ */
   /* This class is for debug purpose: it aims at reading/printing a     */
   /* APP1/EXIF formatted buffer like a standalone application would do. */
   /* Hence, no data structure shared with actual EXIF stuff.            */
   /* ------------------------------------------------------------------ */
   public:
      static OMX_ERRORTYPE print(const OMX_U8*);
      static const char* tagId2String(OMX_U16);
      static const char* typeId2String(OMX_U16);
      static const char* fieldValueString(const OMX_U8*, const OMX_U8*);
};
#endif // DISPLAY_EXIF_METADATA

#endif /* _EXIF_H_ */
