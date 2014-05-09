/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef __VFM_PARAMANDCONFIG_H
#define __VFM_PARAMANDCONFIG_H

#if ((!defined __SYMBIAN32__) && (!defined IMPORT_C))
    #define IMPORT_C
#endif
#if ((!defined __SYMBIAN32__) && (!defined EXPORT_C))
    #define EXPORT_C
#endif

#include "VFM_Index.h"
#include "VFM_Types.h"
#include <inc/type.h>
#include <ee/api/panic.idt>
#include "error_map.h"
#include "common_frameinfo.idt"

#ifdef ANDROID
#include "MMNativeBuffer.h"
#endif

/// forward declaration
class VFM_Component;

/* +Change start for CR333616 Delay recycling */
class RecyclingDelayType
{
    public:
        RecyclingDelayType();
        OMX_BUFFERHEADERTYPE *recycleBuffer(OMX_BUFFERHEADERTYPE *pBuffer);
        OMX_BUFFERHEADERTYPE *flushOneBuffer();
        void set(OMX_U32 value) { mRecyclingDelay=value; }
        inline OMX_U32 get() const { return mRecyclingDelay; }
        static const OMX_U32 mMaxRecycling=8;

    private:
        OMX_U32 mRecyclingDelay;        // in number of frames
        OMX_BUFFERHEADERTYPE *mBuffer[mMaxRecycling+1];
};
/* -Change end for CR333616 Delay recycling */

/* +Change start for CR330198 Revert to DTS from PTS */
class DisplayOrderType
{
public:
    DisplayOrderType();
    void set(OMX_BOOL enable);
    inline OMX_BOOL isEnabled() const { return mEnable; }
    inline void nextSaveMustReset() { mNextSaveMustReset = OMX_TRUE; }
    void save(OMX_BUFFERHEADERTYPE* pBuffer);
    OMX_TICKS get(OMX_BUFFERHEADERTYPE* pBuffer);

private:
    OMX_BOOL mEnable;
    static const OMX_U32 mMaxTsBuffers=128;
    OMX_TICKS mTimestamp[mMaxTsBuffers];
    OMX_U32 mFirst, mLast;
    OMX_BOOL mNextSaveMustReset;
};
/* -Change end for CR330198 Revert to DTS from PTS */

/* +Change start for CR332521 IOMX UseBuffer */
class MMHwBuffer;
class iOMXType
{
public:
    iOMXType();
    void set(OMX_BOOL enable);
    inline OMX_BOOL get() const { return mEnable; }
    void getiOMXBuffer(OMX_U32 nPort, OMX_BUFFERHEADERTYPE *pBuffer);
    void getApplicationBuffer(OMX_U32 nPort, OMX_BUFFERHEADERTYPE *pBuffer);
    OMX_ERRORTYPE allocate(VFM_Component *pComponent, MMHwBuffer *sharedChunk, OMX_U32 nPortIndex,
        OMX_U32 nBufferIndex, OMX_U32 nSize, OMX_BUFFERHEADERTYPE *pBuffer);
    void free(OMX_U32 nPortIndex, OMX_U32 nBufferIndex);
    void set_cache_properties(OMX_BOOL bInput, OMX_BOOL bOutput);
    IMPORT_C void getBuffer(MMHwBuffer **sharedChunk, OMX_U8 **pDataBuffer, OMX_U32 nPortIndex, OMX_U32 nBufferIndex);

private:
    OMX_BOOL mEnable;
    static const OMX_U32 mMaxiOMXBuffers=128;
    OMX_U8 *mBufferiOMX[2][mMaxiOMXBuffers], *mBufferApplication[2][mMaxiOMXBuffers];
    OMX_BOOL mResetPortPrivate[2][mMaxiOMXBuffers];     // OMX_TRUE if portprivate info should be reset on the EmptyBufferDone / FillBufferDone
    MMHwBuffer *mSharedChunk[2][mMaxiOMXBuffers];
    OMX_BOOL mCached[2];
    OMX_BOOL mIsApplied[2];     // OMX_TRUE if we need to copy for this port
};
/* -Change end for CR332521 IOMX UseBuffer */

/* +Change start for CR 399938 */
typedef struct MetadataWrapper
{
	void *pMetaDataWrapper; /* will be storing the address of MMNativeBuffer Ptr */
	void *ptrVideoMetadata;
	OMX_BOOL isValid; /* will signify if entry in table is valid or not */
}MetadataWrapper;

class MetadataEncoders
{
	private:
    static const OMX_U32 mMaxiMetadataWrapper=10;
	MetadataWrapper iMetadataWrapper[mMaxiMetadataWrapper];

    public:
    void initializeEntries();
    OMX_U32 getPhysicalAddrFromLogical(OMX_U32 logicalAddress);
	OMX_U32 getLogicalAddrFromPhysical(OMX_U32 nPhysicalAddress);
    void destroyList();
	OMX_BOOL isEntryValid(OMX_U32 type, OMX_U32 ptrData,OMX_DIRTYPE direction);
    OMX_BOOL changeEntry(OMX_U32 type, OMX_U32 ptrData, OMX_BOOL newValue, void *);
    OMX_U32 getVideoMetadata(OMX_U32 logicalAddress);
    OMX_U32 getLogicalAddressfromNativeHandle(OMX_U32 ptrData);
    OMX_BOOL isMetaDataWrapperFound(void *);
    OMX_BOOL insertMetadataWrapper(void **ptr, void ** ptrVideoMetaInfo);
	void printTable();
};

typedef struct
{
    int metadataBufferType;
    void* handle;
    int offset;
} video_metadata_t;

#ifndef ANDROID
class MMNativeBuffer {
public:
    // pBuffer is actually supposed to be a buffer_handle_t
    MMNativeBuffer(void *pBuffer) {}
    ~MMNativeBuffer() {}

    inline int init() {return 0; }
    inline OMX_U32 getBuffer() {return 0; }
    inline int getHandle() {return 0;}
    inline OMX_U32 getLogicalAddress() {return 0; }
    inline OMX_U32 getPhysicalAddress() {return 0; }
};
#endif

/* -Change start for CR 399938 */

/// @ingroup VFMUtilities
/// @{
///
/// @brief Utility class to manage the status of a given proxy, for example to know
/// if a header should be regenerate or not.
///
/// This class is made of a single attribute, a
/// boolean to indicate the status. The status can be changed through a number of methods
/// to set / reset / update and get a status, as described in the API
class VFM_Status
{
    private:
        /// @brief boolean value of the status
        OMX_BOOL status;

    public:
        /// @brief Constructor. By default, the status is set (value is true)
        IMPORT_C VFM_Status();

        /// @brief Set (to true) the status
        void set();

        /// @brief Reset (set to false) the status
        void reset();

        /// @brief Update a status depending of hasChanged
        /// @param [in] hasChanged use to update the status as status = status | hasChanged
        void update(OMX_BOOL hasChanged);

        /// @brief Get the status value
        /// @return the status
        OMX_BOOL get();
};
/// @}

/// @defgroup VFMParamAndConfig VFM Parameters and Configurations
/// @ingroup VFMOMXSUPPORT
/// @brief This is the top class to manage Parameters and Configuration of a video Proxy.

/// @ingroup VFMParamAndConfig
/// @brief This is the top class to manage Parameters and Configuration of a video Proxy.
///
/// It is aimed to contain functions to get and set parameters and configuration
/// that are defined in the OpenMax Standard. The purpose is to commonalize this operation in
/// terms of cast and early check. Note that the functions do not do any processing of these
/// parameters / configurations, but only retrieve or set the values.
///
/// Current OMX Structure and Indexes that can be retrieved are:
/// - Profile and Level of a video encoder / decoder (OMX_VIDEO_PARAM_PROFILELEVELTYPE and OMX_IndexParamVideoProfileLevelCurrent)
///   - setIndexParamVideoProfileLevelCurrent()
///   - getIndexParamVideoProfileLevelCurrent()
/// - Quality Factor (OMX_IMAGE_PARAM_QFACTORTYPE and OMX_IndexParamQFactor):
///   - setIndexParamQFactor()
///   - getIndexParamQFactor()
/// - Cropping Configuration (OMX_CONFIG_RECTTYPE and OMX_IndexConfigCommonOutputCrop):
///   - setIndexConfigCommonOutputCrop()
///   - getIndexConfigCommonOutputCrop()
class VFM_ParamAndConfig
{
    public:
        /// Constructor
		IMPORT_C VFM_ParamAndConfig(VFM_Component *component);

        /// @brief Set the proxy component associated with this set of parameter
        /// @return The component associated with this set of parameter. This can
        /// casted to a specific proxy (for example MyCodec_Proxy).
        VFM_Component *getComponent() const;

        /// @brief Set a structure of type OMX_IMAGE_PARAM_QFACTORTYPE
        /// @param [in] pt_org structure of type OMX_IMAGE_PARAM_QFACTORTYPE, that contains the
        ///   quality factor to set
        /// @param [out] QFactor the quality factor that is set
        /// @param [out] has_changed true if the value of the quality factor has been modified by
        ///   this operation, false otherwise
        /// @return OMX_ErrorNone if no error occured, an error otherwise (OMX_ErrorBadParameter)
        IMPORT_C OMX_ERRORTYPE setIndexParamQFactor(OMX_PTR pt_org, OMX_U16 *QFactor, OMX_BOOL *has_changed);

        /// @brief Get a structure of type OMX_IMAGE_PARAM_QFACTORTYPE
        /// @param [in,out] pt_org structure of type OMX_IMAGE_PARAM_QFACTORTYPE, that contains the
        ///   quality factor to get
        /// @param [in] QFactor the quality factor that is get
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        IMPORT_C OMX_ERRORTYPE getIndexParamQFactor(OMX_PTR pt_org, OMX_U16 QFactor) const;

        /// @brief Set a structure of type OMX_CONFIG_RECTTYPE
        /// @param [in] pt_org structure of type OMX_CONFIG_RECTTYPE, that contains the
        ///   cropping parameters to set
        /// @return OMX_ErrorNone if no error occured, an error otherwise (OMX_ErrorBadParameter)
        IMPORT_C OMX_ERRORTYPE setIndexConfigCommonOutputCrop(OMX_PTR pt_org);

        /// @brief Get a structure of type OMX_CONFIG_RECTTYPE
        /// @param [in,out] pt_org structure of type OMX_CONFIG_RECTTYPE, that contains the
        ///   cropping parameters to get
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        IMPORT_C OMX_ERRORTYPE getIndexConfigCommonOutputCrop(OMX_PTR pt_org) const;

        /// @brief Set a structure of type OMX_VIDEO_PARAM_PROFILELEVELTYPE
        /// @param [in] pt_org structure of type OMX_VIDEO_PARAM_PROFILELEVELTYPE, that contains the
        ///   profile and the level to set
        /// @param [out] Profile is the profile that is set
        /// @param [out] Level is the level that is set
        /// @param [out] has_changed true if the values have changed, false otherwise
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        OMX_ERRORTYPE setIndexParamVideoProfileLevelCurrent(OMX_PTR pt_org, OMX_U32 *Profile, OMX_U32 *Level, OMX_BOOL *has_changed);

        /// @brief Get a structure of type OMX_VIDEO_PARAM_PROFILELEVELTYPE
        /// @param [in,out] pt_org structure of type OMX_VIDEO_PARAM_PROFILELEVELTYPE, that contains the
        ///   profile and the level to set
        /// @param [in] Profile is the profile that is get
        /// @param [in] Level is the level that is get
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        OMX_ERRORTYPE getIndexParamVideoProfileLevelCurrent(OMX_PTR pt_org, OMX_U32 Profile, OMX_U32 Level) const;

        /// @brief Set the profile and the level of a video encoder / decoder
        /// @param [in] profile is the profile to set
        /// @param [in] level is the level to set
        void setProfileLevel(OMX_U32 profile, OMX_U32 level);

        /// @brief Get the profile and the level of a video encoder / decoder
        /// @param [out] profile is the current profile of the encoder / decoder
        /// @param [out] level is the current level of the encoder / decoder
        void getProfileLevel(OMX_U32 *profile, OMX_U32 *level) const;

        /// @brief Get the mask of the supported extension
        /// @return The supported extension
        inline OMX_U32 getSupportedExtension() { return mSupportedExtension; }

        /// @brief Set the mask of the supported extension
        /// @param[in] nSupportedExtension The supported extension
        inline void setSupportedExtension(OMX_U32 nSupportedExtension) { mSupportedExtension = nSupportedExtension; }

        /// @brief Set a structure of type OMX_VIDEO_PARAM_PORTFORMATTYPE
        /// @param [in] pt_org structure of type OMX_VIDEO_PARAM_PORTFORMATTYPE, that contains the
        ///   video port parameters (compression format, color format and frame rate)
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        /// @note It makes use of the function MyCodec::checkIndexParamVideoPortFormat() that checks
        /// the parameters are valid
        OMX_ERRORTYPE setIndexParamVideoPortFormat(OMX_PTR pt_org);

        /// @brief Get a structure of type VFM_PARAM_THUMBNAIL_GENERATION
        /// @param [in] pt_org structure of type VFM_PARAM_THUMBNAIL_GENERATION, that contains if
        ///   the decoder is used for thumbnail generation
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        OMX_ERRORTYPE getIndexParamThumbnailGeneration(OMX_PTR pt_org) const;

        /// @brief Set a structure of type VFM_PARAM_THUMBNAIL_GENERATION
        /// @param [in] pt_org structure of type VFM_PARAM_THUMBNAIL_GENERATION, that contains the
        ///   the deocder is used for thumbnail generation
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        OMX_ERRORTYPE setIndexParamThumbnailGeneration(OMX_PTR pt_org);

		/* +Change for CR 399075 */
        OMX_ERRORTYPE getIndexConfigImmediateRelease(OMX_PTR pt_org) const;
        OMX_ERRORTYPE setIndexConfigImmediateRelease(OMX_PTR pt_org);
		/* -Change for CR 399075 */

        /// @brief Get a structure of type OMX_VIDEO_PARAM_PORTFORMATTYPE
        /// @param [in] pt_org structure of type OMX_VIDEO_PARAM_PORTFORMATTYPE, that contains the
        ///   video port parameters (compression format, color format and frame rate)
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        OMX_ERRORTYPE getIndexParamVideoPortFormat(OMX_PTR pt_org) const;

        /// @brief Set a structure of type OMX_IMAGE_PARAM_PORTFORMATTYPE
        /// @param [in] pt_org structure of type OMX_IMAGE_PARAM_PORTFORMATTYPE, that contains the
        ///   image port parameters
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        /// @note It makes use of the function MyCodec::checkIndexParamImagePortFormat() that checks
        /// the parameters are valid
        OMX_ERRORTYPE setIndexParamImagePortFormat(OMX_PTR pt_org);

        /// @brief Get a structure of type OMX_IMAGE_PARAM_PORTFORMATTYPE
        /// @param [in] pt_org structure of type OMX_IMAGE_PARAM_PORTFORMATTYPE, that contains the
        ///   image port parameters
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        OMX_ERRORTYPE getIndexParamImagePortFormat(OMX_PTR pt_org) const;

        /// @brief Set a structure of type OMX_PARAM_COLORPRIMARY
        /// @param [out] pt_org structure of type OMX_PARAM_COLORPRIMARY, that contains the color primary type
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        OMX_ERRORTYPE setIndexParamColorPrimary(OMX_PTR pt_org);

        /// @brief Get a structure of type OMX_PARAM_COLORPRIMARY
        /// @param [in] pt_org structure of type OMX_PARAM_COLORPRIMARY, that contains the color primary type
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        OMX_ERRORTYPE getIndexParamColorPrimary(OMX_PTR pt_org) const;

        /// @brief Set a structure of type VFM_PARAM_SUPPORTEDEXTENSION_TYPE
        /// @param [out] pt_org structure of type VFM_PARAM_SUPPORTEDEXTENSION_TYPE, that contains supported extension
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        OMX_ERRORTYPE setIndexParamSupportedExtension(OMX_PTR pt_org);

        /// @brief Get a structure of type VFM_PARAM_SUPPORTEDEXTENSION_TYPE
        /// @param [in] pt_org structure of type VFM_PARAM_SUPPORTEDEXTENSION_TYPE, that contains supported extension
        /// @return OMX_ErrorNone if no error occured, an error otherwise
        OMX_ERRORTYPE getIndexParamSupportedExtension(OMX_PTR pt_org) const;

        /// @brief Support of the OMX functionality to know the max supported level for any supported profile
        /// (on index OMX_IndexParamVideoProfileLevelQuerySupported)
        /// @param[out] pt_org Structure of type OMX_VIDEO_PARAM_PROFILELEVELTYPE
        /// @return An error if any
        OMX_ERRORTYPE getIndexParamVideoProfileLevelQuerySupported(OMX_PTR pt_org) const;

        /// @brief Support of the OMX extension to know the PAR (Pixel Aspect Ratio)
        /// (on index OMX_IndexParamPixelAspectRatio)
        /// @param[out] pt_org Structure of type OMX_PARAM_PIXELASPECTRATIO
        /// @return An error if any
        IMPORT_C OMX_ERRORTYPE getIndexParamPixelAspectRatio(OMX_PTR pt_org) const;

        /// @brief Support of the OMX index OMX_IndexConfigVideoMacroBlockErrorMap
        /// @param[out] pt_org Structure of type OMX_CONFIG_MACROBLOCKERRORMAPTYPE
        /// @return An error if any
        IMPORT_C OMX_ERRORTYPE getIndexConfigVideoMacroBlockErrorMap(OMX_PTR pt_org);

        /// @brief Support of the OMX index OMX_IndexParamVideoFastUpdate
        /// @param[out] pt_org Structure of type OMX_VIDEO_PARAM_VIDEOFASTUPDATETYPE
        /// @return An error if any
        IMPORT_C OMX_ERRORTYPE getIndexParamVideoFastUpdate(OMX_PTR pt_org);

        /// @brief Support of the OMX index VFM_IndexParamHeaderDataChanging
        /// @param[out] pt_org Structure of type VFM_PARAM_HEADERDATACHANGING_TYPE
        /// @return An error if any
        OMX_ERRORTYPE setIndexParamHeaderDataChanging(OMX_PTR pt_org);

#ifdef PACKET_VIDEO_SUPPORT
        /// @brief Support of the OMX index PV_OMX_COMPONENT_CAPABILITY_TYPE_INDEX
        /// @param[out] pt_org Structure of type PV_OMXComponentCapabilityFlagsType
        /// @return An error if any. The default implementation returns OMX_ErrorNotImplemented.
        ///     Specific proxies must overload this function to set their specific capabilities.
        IMPORT_C virtual OMX_ERRORTYPE getPVCapability(OMX_PTR pt_org);
#endif

        /* +Change start for CR333616 Delay recycling */
        /// @brief Support of the OMX index VFM_IndexParamRecyclingDelay
        /// @param[out] pt_org Structure of type VFM_PARAM_RECYCLINGDELAYTYPE
        /// @return An error if any
        OMX_ERRORTYPE setIndexParamRecyclingDelay(OMX_PTR pt_org);

        /// @brief Support of the OMX index  VFM_IndexParamRecyclingDelay
        /// @param[out] pt_org Structure of type VFM_PARAM_RECYCLINGDELAYTYPE
        /// @return An error if any
        OMX_ERRORTYPE getIndexParamRecyclingDelay(OMX_PTR pt_org) const;
        /* -Change end for CR333616 Delay recycling */

        /* +Change start for CR330198 Revert to DTS from PTS */
        /// @brief Support of the OMX index  VFM_IndexParamTimestampDisplayOrder
        /// @param[out] pt_org Structure of type VFM_PARAM_TIMESTAMPDISPLAYORDERTYPE
        /// @return An error if any
        OMX_ERRORTYPE setIndexParamTimestampDisplayOrder(OMX_PTR pt_org);

        /// @brief Support of the OMX index  VFM_IndexParamTimestampDisplayOrder
        /// @param[out] pt_org Structure of type VFM_PARAM_TIMESTAMPDISPLAYORDERTYPE
        /// @return An error if any
        OMX_ERRORTYPE getIndexParamTimestampDisplayOrder(OMX_PTR pt_org) const;
        /* -Change end for CR330198 Revert to DTS from PTS */

        /* +Change start for CR333618 Added for IOMX renderer */
        /// @brief Support of the OMX index VFM_IndexParamSharedchunkInPlatformprivate
        /// @param[out] pt_org Structure of type VFM_PARAM_SHAREDCHUNKINPLATFORMPRIVATETYPE
        /// @return An error if any
        OMX_ERRORTYPE setIndexParamSharedchunkInPlatformprivate(OMX_PTR pt_org);

        /// @brief Support of the OMX index VFM_IndexParamSharedchunkInPlatformprivate
        /// @param[out] pt_org Structure of type VFM_PARAM_SHAREDCHUNKINPLATFORMPRIVATETYPE
        /// @return An error if any
        OMX_ERRORTYPE getIndexParamSharedchunkInPlatformprivate(OMX_PTR pt_org) const;

        inline OMX_BOOL getSharedchunkInPlatformprivate() const { return mSharedchunkInPlatformprivate; };
        /* -Change end for CR333618 Added for IOMX renderer */

        /* +Change start for CR332521 IOMX UseBuffer */
        OMX_ERRORTYPE setIndexParamiOMX(OMX_PTR pt_org);
        OMX_ERRORTYPE getIndexParamiOMX(OMX_PTR pt_org) const;

        inline void setiOMXCacheProperties(OMX_BOOL bInput, OMX_BOOL bOutput) {
            miOMX.set_cache_properties(bInput, bOutput);
        };
       /* -Change end for CR332521 IOMX UseBuffer */

        /* +Change start for CR 399938 */
        /// @brief Support of the OMX index VFM_IndexParamStoreMetadataInBuffers
        /// @param[out] pt_org Structure of type VFM_PARAM_STOREMETADATAINBUFFERS
        /// @return An error if any
        OMX_ERRORTYPE setIndexParamStoreMetadataInBuffers(OMX_PTR pt_org);

        /// @brief Support of the OMX index VFM_IndexParamStoreMetadataInBuffers
        /// @param[out] pt_org Structure of type VFM_PARAM_STOREMETADATAINBUFFERS
        /// @return An error if any
        OMX_ERRORTYPE getIndexParamStoreMetadataInBuffers(OMX_PTR pt_org) const;

        inline OMX_BOOL getStoreMetadataInBuffers() const { return mStoreMetadataInBuffers; };

        OMX_ERRORTYPE destroyMetaDataList();

        OMX_ERRORTYPE extraProcessingInputBuffers(OMX_DIRTYPE direction, OMX_BUFFERHEADERTYPE **pBuffer);
        OMX_U32 getBufferPhysicalAddress(OMX_U32 mLogicalAddress);
	OMX_U32 getBufferLogicalAddress(OMX_U32 mPhysicalAddress);

        /* -Change start for CR 399938 */

        OMX_ERRORTYPE getIndexParamArmLoadComputation(OMX_PTR pt_org) const;
        OMX_ERRORTYPE setIndexParamArmLoadComputation(OMX_PTR pt_org);

        /// @brief Set the Pixel Aspect Ratio (PAR) of a video encoder / decoder
        /// @param [in] parWidth is the PAR width to set
        /// @param [in] parHeight is the PAR heith to set
        void setPAR(OMX_U32 parWidth, OMX_U32 parHeight);

        /// @brief Get the Pixel Aspect Ratio (PAR) of a video encoder / decoder
        /// @param [out] parWidth is the current PAR width of the encoder / decoder
        /// @param [out] parHeight is the current PAR heith of the encoder / decoder
        void getPAR(OMX_U32 *parWidth, OMX_U32 *parHeight) const;

        /// @brief Get the color primary
        /// @return the color primary
        OMX_COLORPRIMARYTYPE getColorPrimary() const;

        /// @brief Set the color primary
        /// @param[in] color the color primary
        void setColorPrimary(OMX_COLORPRIMARYTYPE color);

        /// @brief Return a pointer to the error map
        /// @return A pointer to the error map
        Common_Error_Map *getErrorMap();

        /// @brief Support of the custom index VFM_IndexParamVideoFwPerfData
        /// @param[in] pt_org Structure of type VFM_CONFIG_PERFDATATYPE
        /// @return An error if any
        IMPORT_C OMX_ERRORTYPE getIndexParamVideoFwPerfData(OMX_PTR pt_org);

        /// @brief Initialize the cropping information (no cropping)
        IMPORT_C void setNoCropping();

        /// @brief Set the cropping information
        void setCropping(const t_common_frameinfo *pFrameInfo);

        /// @brief Check the cropping has changed or not, and assign the new values
        OMX_BOOL isChangedCropping_And_Set(const t_common_frameinfo *pFrameInfo);

        void setCroppingInFrameinfo(t_common_frameinfo *pFrameInfo);

        /// @brief Get value of mHeaderDataChanging
        /// @return mHeaderDataChanging
        inline OMX_BOOL getHeaderDataChanging() { return mHeaderDataChanging; }

        /// @brief Get the value of mThumbnailGeneration
        inline OMX_BOOL getThumbnailGeneration()    { return mThumbnailGeneration; }

		/* +Change for CR 399075 */
        /// @brief Get the value of mImmediateRelease
        inline OMX_BOOL getImmediateRelease()    { return mImmediateRelease; }

        inline void resetDecodeImmediateFlag() { mImmediateRelease = OMX_FALSE; }
		/* -Change for CR 399075 */

        /* +Change start for CR330198 Revert to DTS from PTS */
        inline DisplayOrderType *getDisplayOrder() { return &mDisplayOrder; }
        /* -Change end for CR330198 Revert to DTS from PTS */

        /* +Change start for CR333616 Delay recycling */
        inline RecyclingDelayType *getRecyclingDelay() { return &mRecyclingDelay; }
        /* -Change end for CR333616 Delay recycling */

        /* +Change start for CR332521 IOMX UseBuffer */
        inline iOMXType *getiOMX() { return &miOMX; }
        /* -Change end for CR332521 IOMX UseBuffer */

        /* +Change start for CR 399938 */
        inline MetadataEncoders *getMetadataEncoders() { return &mMetadataEncoders; }
        /* -Change start for CR 399938 */

    private:
        /// @brief Current profile. Not applicable for jpeg
        OMX_U32    eProfile;

        /// @brief Current level
        OMX_U32    eLevel;

        /// @brief Pixel Aspect Ratio: the width
        OMX_U32    mPARWidth;

        /// @brief Pixel Aspect Ratio: the height
        OMX_U32    mPARHeight;

        /// @brief Color Primary of the codec
        OMX_COLORPRIMARYTYPE mColorPrimary;

        /// @brief Cropping information
        OMX_CONFIG_RECTTYPE mOutputCrop;

        /// @brief Mask of the supported extension. Cf. VFM_SUPPORTEDEXTENSION_MASK and VFM_PARAM_SUPPORTEDEXTENSION_TYPE
        OMX_U32    mSupportedExtension;

        /// @brief Boolean to indicate pHeader->pBuffer can changed during a run
        /// @note To be implemented in the codec
        OMX_BOOL mHeaderDataChanging;

        /// @brief Boolean to indicate the decoder is used for thumbnail generation
        OMX_BOOL mThumbnailGeneration;

		/* +Change for CR 399075 */
         /// @brief Boolean to indicate the decoder is used for immediate release I frame
        OMX_BOOL mImmediateRelease;
        /* -Change for CR 399075 */

        /* +Change start for CR330198 Revert to DTS from PTS */
        /// @brief OMX_TRUE to use display order in timestamp in input of decoders
        DisplayOrderType mDisplayOrder;
        /* -Change end for CR330198 Revert to DTS from PTS */

        /* +Change start for CR333616 Delay recycling */
        /// @brief Recycling delay in output
        RecyclingDelayType mRecyclingDelay;
        /* -Change end for CR333616 Delay recycling */

        /* +Change start for CR332521 IOMX UseBuffer */
        /// @brief To have iOMX (UseBufer without Shared Chunk) working
        iOMXType miOMX;
        /* -Change end for CR332521 IOMX UseBuffer */

        /* +Change start for CR333618 Added for IOMX renderer */
        /// @brief Whether or not PlatformPrivate should contain the Shared Chunk Metadata on FillBufferDone
        OMX_BOOL mSharedchunkInPlatformprivate;
        /* -Change end for CR333618 Added for IOMX renderer */

        /* +Change start for CR 399938 */
        OMX_BOOL mStoreMetadataInBuffers;
        /* -Change start for CR 399938 */

        /// @brief OpenMax Component this class is related to (for example JPEGEnc_Proxy)
        VFM_Component   *pComponent;

        /// @brief Error map struture
        Common_Error_Map mErrorMap;

        /* +Change start for CR 399938 */
        MetadataEncoders mMetadataEncoders;
        /* -Change start for CR 399938 */
};

// Symbian rules:
// - no export of inlined functions
// - no inline of virtual functions
// - inline functions defined in .inl file or at the end of the .h file
// - no export of pure virtual functions
inline void VFM_Status::set()                       { status = OMX_TRUE; };
inline void VFM_Status::reset()                     { status = OMX_FALSE; };
inline void VFM_Status::update(OMX_BOOL hasChanged) { if (hasChanged)   status = OMX_TRUE; };
inline OMX_BOOL VFM_Status::get()                   { return status; };
inline VFM_Component *VFM_ParamAndConfig::getComponent() const                          { return pComponent; }
inline void VFM_ParamAndConfig::setProfileLevel(OMX_U32 profile, OMX_U32 level)         { eProfile = profile;   eLevel = level; }
inline void VFM_ParamAndConfig::getProfileLevel(OMX_U32 *profile, OMX_U32 *level) const { *profile = eProfile ; *level = eLevel; }
inline void VFM_ParamAndConfig::setPAR(OMX_U32 parWidth, OMX_U32 parHeight)             { mPARWidth = parWidth; mPARHeight = parHeight; }
inline void VFM_ParamAndConfig::getPAR(OMX_U32 *parWidth, OMX_U32 *parHeight) const
{ *parWidth = mPARWidth ; *parHeight = mPARHeight; }
inline Common_Error_Map *VFM_ParamAndConfig::getErrorMap()     { return &mErrorMap; }
inline OMX_COLORPRIMARYTYPE VFM_ParamAndConfig::getColorPrimary() const     { return mColorPrimary; }
inline void VFM_ParamAndConfig::setColorPrimary(OMX_COLORPRIMARYTYPE color) { mColorPrimary = color; }
#endif
