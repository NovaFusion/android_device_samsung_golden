/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _EXTRADATA_H_
#define _EXTRADATA_H_

#include "IFM_Types.h"
#include "IFM_Index.h"
#include "exif.h"

typedef enum
{
	Component_Camera,
	Component_ARMivProc,
	Component_Splitter,
	Component_OT_Analyzer,
	Component_AMBR_Analyzer,
	Component_FD_Analyzer,
	Component_ISPProc,
	Component_RED,
	Component_REC,
	Component_Norcos
} t_Component;

/* EXIF marker max size as specified by EXIF 2.2. Do not change */
#define EXIF_APPMARKER_SIZE (64*1024)

/* EXIF extradata container : defined as a union so that it is sized to EXIF_APPMARKER_SIZE */
typedef union EXTRADATA_EXIFMARKERCONTAINERTYPE {
    OMX_SYMBIAN_EXIFMARKERTYPE sExifMarker;
    OMX_U8 cContainer[sizeof(OMX_SYMBIAN_EXIFMARKERTYPE)- sizeof(OMX_U8) + EXIF_APPMARKER_SIZE];
} EXTRADATA_EXIFMARKERCONTAINERTYPE;

/* Structure containing Focus Status extradata */
/* Defined as a union so that actual size allows having IFM_MAX_NUMBER_OF_FOCUS_STATUS_ROIS entries */
typedef union EXTRADATA_EXTFOCUSSTATUSCONTAINERTYPE{
    OMX_SYMBIAN_EXTRADATA_EXTFOCUSSTATUSTYPE sExtFocusStatus;
    OMX_U8 cContainer[sizeof(OMX_SYMBIAN_EXTRADATA_EXTFOCUSSTATUSTYPE)- sizeof(OMX_SYMBIAN_AFROITYPE) + IFM_MAX_NUMBER_OF_FOCUS_STATUS_ROIS*sizeof(OMX_SYMBIAN_AFROITYPE)];
} EXTRADATA_EXTFOCUSSTATUSCONTAINERTYPE;

/* Structure containing analyzers feedbacks extradata */
/* Defined as a union so that it contains up to 4kb of feedbacks */
typedef union EXTRADATA_FEEDBACKSCONTAINERTYPE {
    OMX_SYMBIAN_EXTRADATA_FEEDBACKTYPE sFeedback;
    OMX_U8 cContainer[4096];
} EXTRADATA_FEEDBACKSCONTAINERTYPE;

/* Size of EXIF extradata */
/* TODO can be removed once imgfilter no more uses it */
#define SIZE_OF_EXIF_EXTRADATA  sizeof(EXTRADATA_EXIFMARKERCONTAINERTYPE)

/****************************************************IMPORTANT: ****************************************/
/* Whenever Changes are made to EXTRADATA_STILLPACKSIZE care must be taken to change STILLEXTRA_WO_EXIF if required.*/
/****************************************************IMPORTANT: ****************************************/
/* should be : Everything in EXTRADATA_STILLPACKSIZE except the Exif Data and the STE_CaptureParameter and anything else added after exif data in the buffer*/
#define STILLEXTRA_WO_EXIF (((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(IFM_BMS_CAPTURE_CONTEXT) + 3) & ~0x3) \
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_SYMBIAN_CAMERAUSERSETTINGSTYPE) + 3 ) & ~0x3) \
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(IFM_DAMPERS_DATA) + 3 ) & ~0x3) \
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_SYMBIAN_CAPTUREPARAMETERSTYPE) + 3) & ~0x3)\
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_SYMBIAN_FRAMEDESCRIPTIONTYPE) + 3) & ~0x3))

/* should be : scene characterization + user settings + raw capture context + frame description + EXIF */
#define EXTRADATA_STILLPACKSIZE ( ((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(IFM_BMS_CAPTURE_CONTEXT) + 3) & ~0x3) \
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_SYMBIAN_CAMERAUSERSETTINGSTYPE) + 3 ) & ~0x3) \
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(IFM_DAMPERS_DATA) + 3 ) & ~0x3) \
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(EXTRADATA_EXIFMARKERCONTAINERTYPE) + 3 ) & ~0x3) \
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_SYMBIAN_CAPTUREPARAMETERSTYPE) + 3) & ~0x3)\
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_SYMBIAN_FRAMEDESCRIPTIONTYPE) + 3) & ~0x3)\
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_STE_CAPTUREPARAMETERSTYPE) + 3) & ~0x3)\
                                 + sizeof(OMX_OTHER_EXTRADATATYPE))

/* should be : scene characterization + user settings + video stab */
#define EXTRADATA_VIDEOPACKSIZE ( ((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_SYMBIAN_DIGITALVIDEOSTABTYPE) + 3) & ~0x3) \
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_SYMBIAN_CAMERAUSERSETTINGSTYPE) + 3 ) & ~0x3) \
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_SYMBIAN_CAPTUREPARAMETERSTYPE) + 3) & ~0x3)\
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_SYMBIAN_ROITYPE) + 3) & ~0x3)\
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(EXTRADATA_EXTFOCUSSTATUSCONTAINERTYPE) + 3) & ~0x3)\
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(EXTRADATA_EXIFMARKERCONTAINERTYPE) + 3 ) & ~0x3) \
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(EXTRADATA_FEEDBACKSCONTAINERTYPE) + 3) & ~0x3)\
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_STE_PRODUCTIONTESTTYPE) + 3) & ~0x3)\
                                 +((sizeof(OMX_OTHER_EXTRADATATYPE) - sizeof(OMX_U8) + sizeof(OMX_STE_CAPTUREPARAMETERSTYPE) + 3) & ~0x3)\
                                 + sizeof(OMX_OTHER_EXTRADATATYPE))

/* should be : red eye regions */
#define EXTRADATA_REDEYEPACKSIZE 0

#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(Extradata);
#endif
class Extradata
{
	public :
		/* brief : get extradata size that component wants to negociate on tunnel
		 * eComponent : component name
		 * nPort : port of component
		 * return value : extradata size
		 */
		static OMX_U32 GetExtradataSize(t_Component eComponent, OMX_U32 nPort);

		/* brief : get an extradata field of a buffer
		 *         if this field does not exist all struct fields are initialized to 0
		 * pOmxBufHdr : pointer to OMX buffer header
		 * pExtradataData : pointer where extradata will be stored ; pointer type defines extradata type to retrieve
		 *                  must be allocated
		 * version : not used
		 * return value : OMX_ErrorNone or OMX_ErrorInsufficientResources
		 */
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, IFM_BMS_CAPTURE_CONTEXT* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_CAMERAUSERSETTINGSTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, IFM_DAMPERS_DATA* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_DIGITALVIDEOSTABTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_CAPTUREPARAMETERSTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_FRAMEDESCRIPTIONTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_ROITYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_EXTRADATA_EXTFOCUSSTATUSTYPE* pExtradataData, OMX_U32 nExtradataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_EXTRADATA_FEEDBACKTYPE* pFeedbackExtradata, OMX_SYMBIAN_FEEDBACKTYPE nFeedbackType, OMX_U32 nFeedbackExtradataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_STE_PRODUCTIONTESTTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
	        static OMX_ERRORTYPE GetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_STE_CAPTUREPARAMETERSTYPE* pExtradataData, OMX_VERSIONTYPE version);

		/* brief : set an extradata field of a buffer
		 * pOmxBufHdr : pointer to OMX buffer header
		 * pExtraDataType : pointer to extradata ; pointer type defines extradata type to write
		 *                  must be allocated
		 * version : extradata version
		 * return value : OMX_ErrorNone or OMX_ErrorInsufficientResources
		 */
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, IFM_BMS_CAPTURE_CONTEXT* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_CAMERAUSERSETTINGSTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, IFM_DAMPERS_DATA* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_CAPTUREPARAMETERSTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_FRAMEDESCRIPTIONTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_ROITYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_EXTRADATA_EXTFOCUSSTATUSTYPE* pExtradataData, OMX_U32 nExtradataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_EXTRADATA_FEEDBACKTYPE* pFeedbackExtradata, OMX_SYMBIAN_FEEDBACKTYPE nFeedbackType, OMX_U32 nFeedbackExtradataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_STE_PRODUCTIONTESTTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		/* Caution ; this function is defined again in stab; please update it when updating this one */
		static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_SYMBIAN_DIGITALVIDEOSTABTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
	    static OMX_ERRORTYPE SetExtradataField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_STE_CAPTUREPARAMETERSTYPE* pExtradataData, OMX_VERSIONTYPE version);
		/* brief : clear extradata segment
		 * pOmxBufHdr : pointer to OMX buffer header
		 * return value : OMX_ErrorNone	or OMX_ErrorInsufficientResources
		 */
		/* Caution ; this function is defined again in stab; please update it when updating this one */
		static OMX_ERRORTYPE ClearAllExtradata(OMX_BUFFERHEADERTYPE* pOmxBufHdr);

		/* brief : write extradata terminator at start of buffer
		 * pMem : pointer to memory array
		 * return value : OMX_ErrorNone	or OMX_ErrorInsufficientResources
		 */
		static OMX_ERRORTYPE ClearAllExtradata(OMX_U8* pMem);

		/* brief : copy extradata from OMX buffer to memory
		 * pOmxBufHdr : pointer to OMX buffer header
		 * pMem : pointer to memory array where extradatas will be copied ; must be allocated
		 * nExtradataDataSize : extradata size to copy
		 * return value : OMX_ErrorNone	or OMX_ErrorInsufficientResources
		 */
		static OMX_ERRORTYPE CopyExtradata_Buffer2Mem(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_U8* pMem, OMX_U32 nExtradataDataSize);

		/* brief : copy extradata from memory to OMX buffer
		 * pOmxBufHdr : pointer to OMX buffer header
		 * pMem : pointer to memory array where extradatas are saved
		 * nExtradataDataSize : extradata size to copy
		 * return value : OMX_ErrorNone	or OMX_ErrorInsufficientResources
		 */
		static OMX_ERRORTYPE CopyExtradata_Mem2Buffer(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_U8* pMem, OMX_U32 nExtradataDataSize);

		/* brief :
		 * pMem : pointer to memory array where extradatas are saved
		 * pExtraDataType : pointer where extradata will be stored ; pointer type defines extradata type to retrieve
		 *                  must be allocated
		 * version : not used
		 * return value : OMX_ErrorNone
		 */
		static OMX_ERRORTYPE GetExtradataFieldFromMem(OMX_U8* pMem, IFM_BMS_CAPTURE_CONTEXT* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataFieldFromMem(OMX_U8* pMem, IFM_DAMPERS_DATA* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataFieldFromMem(OMX_U8* pMem, OMX_SYMBIAN_DIGITALVIDEOSTABTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataFieldFromMem(OMX_U8* pMem, OMX_SYMBIAN_CAPTUREPARAMETERSTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataFieldFromMem(OMX_U8* pMem, OMX_SYMBIAN_FRAMEDESCRIPTIONTYPE* pExtradataData, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataFieldFromMem(OMX_U8* pMem, OMX_STE_CAPTUREPARAMETERSTYPE* pExtradataData, OMX_VERSIONTYPE version);
		static OMX_ERRORTYPE GetExtradataFieldFromMem(OMX_U8* pMem, OMX_SYMBIAN_EXTRADATA_FEEDBACKTYPE* pFeedbackExtradata, OMX_SYMBIAN_FEEDBACKTYPE nFeedbackType, OMX_U32 nFeedbackExtradataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataFieldFromMem(OMX_U8* pMem, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE nVersion);

		/* brief : set an extradata field in memory
		 * pMem : pointer to memory array where extradatas awill be saved
		 * pExtraDataType : pointer where extradata are stored ; pointer type defines extradata type to retrieve
		 *                  must be allocated
		 * version : extradata version
		 * portIndex : portIndex to set in extradata header
		 * return value : OMX_ErrorNone
		 */
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_U8* pMem, IFM_BMS_CAPTURE_CONTEXT* pExtradataData, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_U8* pMem, OMX_SYMBIAN_CAMERAUSERSETTINGSTYPE* pExtradataData, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_U8* pMem, IFM_DAMPERS_DATA* pExtradataData, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_U8* pMem, OMX_SYMBIAN_CAPTUREPARAMETERSTYPE* pExtradataData, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_U8* pMem, OMX_SYMBIAN_FRAMEDESCRIPTIONTYPE* pExtradataData, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_U8* pMem, OMX_SYMBIAN_DIGITALVIDEOSTABTYPE* pExtradataData, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_U8* pMem, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_U8* pMem, OMX_STE_PRODUCTIONTESTTYPE* pExtradataData, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_U8* pMem, OMX_SYMBIAN_EXTRADATA_FEEDBACKTYPE* pFeedbackExtradata, OMX_SYMBIAN_FEEDBACKTYPE nFeedbackType, OMX_U32 nFeedbackExtradataSize, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_U8* pMem, OMX_STE_CAPTUREPARAMETERSTYPE* pExtradataData, OMX_VERSIONTYPE version, OMX_U32 portIndex);

	private :
		static OMX_ERRORTYPE SetExtradataField(OMX_EXTRADATATYPE nExtradataType, OMX_BUFFERHEADERTYPE* pOmxBufHdr, void* pExtradataData, OMX_U32 nExtradataDataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataField(OMX_EXTRADATATYPE nExtradataType, OMX_U32 nExtradataSubType, OMX_BUFFERHEADERTYPE* pOmxBufHdr, void* pExtradataData, OMX_U32 nExtradataDataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExifField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_EXTRADATATYPE nExtradataType, OMX_U8* pMem, void* pExtradataData, OMX_U32 nExtradataDataSize, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExifFieldInMem(OMX_U8* pMem, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE SetExtradataFieldInMem(OMX_EXTRADATATYPE nExtradataType,OMX_U32 nExtradataSubtype, OMX_U8* pMem, void* pExtradataData, OMX_U32 nExtradataDataSize, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);

		static OMX_ERRORTYPE GetExtradataField(OMX_EXTRADATATYPE nExtradataType, OMX_BUFFERHEADERTYPE* pOmxBufHdr, void* pExtradataData, OMX_U32 nExtradataDataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataField(OMX_EXTRADATATYPE nExtradataType, OMX_U32 nExtradataSubType, OMX_BUFFERHEADERTYPE* pOmxBufHdr, void* pExtradataData, OMX_U32 nExtradataDataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExifField(OMX_BUFFERHEADERTYPE* pOmxBufHdr, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExifFieldFromMem(OMX_U8* pMem, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataFieldFromMem(OMX_EXTRADATATYPE nExtradataType, OMX_U8* pMem, void* pExtradataData, OMX_U32 nExtradataDataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE GetExtradataFieldFromMem(OMX_EXTRADATATYPE nExtradataType, OMX_U32 nExtradataSubtype, OMX_U8* pMem, void* pExtradataData, OMX_U32 nExtradataDataSize, OMX_VERSIONTYPE nVersion);

		static OMX_ERRORTYPE GetExtradataBoundaries(OMX_BUFFERHEADERTYPE* pOmxBufHdr, OMX_OTHER_EXTRADATATYPE** ppExtradataStart, OMX_OTHER_EXTRADATATYPE** ppExtradataBoudary);
		static OMX_ERRORTYPE FormatExifData(EXTRADATA_EXIFMARKERCONTAINERTYPE* pExifMarkerContainer, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE aVersion);
		static OMX_ERRORTYPE UnFormatExifData(EXTRADATA_EXIFMARKERCONTAINERTYPE* pExifMarkerContainer, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE aVersion);
		static OMX_ERRORTYPE EnoughSpaceForExtradata(OMX_OTHER_EXTRADATATYPE* pExtradataBoundary, OMX_OTHER_EXTRADATATYPE* pExtradataField, OMX_U32 nExtradataDataSize);
		static void          FillExtradataHeader(OMX_OTHER_EXTRADATATYPE* pExtradataStart, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex, OMX_EXTRADATATYPE nExtradataType, OMX_U32 nDataSize);
		static void          FillExtradataTerminator(OMX_OTHER_EXTRADATATYPE* pExtradata );
		static OMX_OTHER_EXTRADATATYPE* NextExtradataHeader( OMX_OTHER_EXTRADATATYPE* pExtradataField);
		static OMX_ERRORTYPE SearchExtradataField(OMX_OTHER_EXTRADATATYPE*  pExtradataStart, OMX_OTHER_EXTRADATATYPE* pExtradataBoundary, OMX_EXTRADATATYPE nExtradataType, OMX_OTHER_EXTRADATATYPE** ppExtradataField, OMX_OTHER_EXTRADATATYPE** ppExtradataTerminator);

		static OMX_ERRORTYPE DoSearchExtradataField(OMX_OTHER_EXTRADATATYPE* pExtradataStart, OMX_OTHER_EXTRADATATYPE* pExtradataBoundary, OMX_EXTRADATATYPE nExtradataType, OMX_U32 nExtradataSubType, OMX_OTHER_EXTRADATATYPE** ppExtradataField, OMX_OTHER_EXTRADATATYPE** ppExtradataTerminator);
		static OMX_ERRORTYPE DoGetExtradataField(OMX_OTHER_EXTRADATATYPE* pExtradataStart, OMX_OTHER_EXTRADATATYPE* pExtradataBoundary, OMX_EXTRADATATYPE nExtradataType, OMX_U32 nExtradataSubType, void* pExtradataData, OMX_U32 nExtradataSize, OMX_VERSIONTYPE nVersion);
		static OMX_ERRORTYPE DoSetExtradataField(OMX_OTHER_EXTRADATATYPE* pExtradataStart, OMX_OTHER_EXTRADATATYPE* pExtradataBoundary, OMX_EXTRADATATYPE nExtradataType, OMX_U32 nExtradataSubType, void* pExtradataData, OMX_U32 nExtradataSize, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE DoSetExifField(OMX_OTHER_EXTRADATATYPE* pExtradataStart, OMX_OTHER_EXTRADATATYPE* pExtradataBoundary, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE nVersion, OMX_U32 nPortIndex);
		static OMX_ERRORTYPE DoGetExifField(OMX_OTHER_EXTRADATATYPE* pExtradataStart, OMX_OTHER_EXTRADATATYPE* pExtradataBoundary, IFM_EXIF_SETTINGS* pExifSettings, OMX_VERSIONTYPE nVersion);
};

#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CExtension);
#endif
class CExtension
{
    public :
        static OMX_ERRORTYPE getExtensionIndex(OMX_STRING cParameterName, OMX_INDEXTYPE* pIndexType);
};

#endif /*_EXTRADATA_H_*/
