/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _VFM_PORT_H_
#define _VFM_PORT_H_

#if ((!defined __SYMBIAN32__) && (!defined IMPORT_C))
    #define IMPORT_C
#endif
#if ((!defined __SYMBIAN32__) && (!defined EXPORT_C))
    #define EXPORT_C
#endif

// #include <cm/inc/cm.hpp>
#include "ENS_Port.h"
#include "ENS_List.h"

/// @defgroup VFMPort VFM Port
/// @ingroup VFMOMXSUPPORT
/// @brief Description of the geneirc part of a video port of a VFM component. This class
/// is derived from ENS_Port that contains that contains the generic part of a port.
/// It is part of any VFM Component due to the fact that VFM Component are also
/// ENS components that contain ports, and VFM creates its port as VFM ports.
/// Specific proxies must then have a specific port such as MyCodec_Port, derived
/// from VFM_Port


/// @ingroup VFMPort
/// @brief Description of the geneirc part of a video port of a VFM component. This class
/// is derived from ENS_Port that contains that contains the generic part of a port.
/// It is part of any VFM Component due to the fact that VFM Component are also
/// ENS components that contain ports, and VFM creates its port as VFM ports.
/// Specific proxies must then have a specific port such as MyCodec_Port, derived
/// from VFM_Port
class VFM_Port: public ENS_Port {
    public:
        /// @brief Constructor
        ///
        /// The constructor is aimed to be called by \ref VFM_Component::createPort
        /// @param [in] commonPortData contains the initialization of main parameters of the port
        ///   (direction, buffer supplier preferences, ...)
        /// @param [in] enscomp  the component that owns this port
        IMPORT_C VFM_Port(const EnsCommonPortData& commonPortData, ENS_Component &enscomp);

        /// @brief Set the width of the frame
        /// @param [in] frameWidth the width of the frame
        IMPORT_C void setFrameWidth(OMX_U32 frameWidth);

        /// @brief Set the height of the frame
        /// @param [in] frameHeight the height of the frame
        IMPORT_C void setFrameHeight(OMX_U32 frameHeight);

        /// @brief Get the width of the frame
        /// @return the width of the frame
        IMPORT_C OMX_U32 getFrameWidth() const;

        /// @brief Get the height of the frame
        /// @return the height of the frame
        IMPORT_C OMX_U32 getFrameHeight() const;

        /// @brief Get the actual buffer count of a port
        /// @return the actual buffer count on a port
        IMPORT_C OMX_U32 getBufferCountActual() const;

        /// @brief Set the height and a width of a OMX_PARAM_PORTDEFINITIONTYPE
        /// @param [in] pNewdef the port definition to set
        /// @param [in] frameWidth the new width
        /// @param [in] frameHeight the new height
        IMPORT_C void setParamWidthHeight(OMX_PARAM_PORTDEFINITIONTYPE *pNewdef, OMX_U32 frameWidth, OMX_U32 frameHeight);

        /// @brief When the IL Client calls SetParameter with OMX_IndexParamPortDefinition index,
        /// this method is called to set the "format" field in the OMX_PARAM_PORTDEFINITIONTYPE.
        /// This method must be provided by derived class.
        /// @param [in] portDef port definition to set
        /// @return OMX_ErrorNone if no error occurs, an error otherwise
        /// @note This function makes use of function getBufferSize() and getBufferCountMin()
        /// that have to be defined
        /// in the specific port MyCodec_Port
        IMPORT_C virtual OMX_ERRORTYPE setFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& portDef);

		IMPORT_C float getPixelDepth(OMX_COLOR_FORMATTYPE format);

        /// @brief Get the BufferSize field of a port parameter
        /// @return BufferSize field of a port parameter
        inline OMX_U32 getBufferSizeInParam() const;

        /// @brief Get the BufferCountMin field of a port parameter
        /// @return BufferCountMin field of a port parameter
        inline OMX_U32 getBufferCountMinInParam() const;

        /// @brief Get the color format of a port
        /// @return the color of a port
        IMPORT_C OMX_COLOR_FORMATTYPE getColorFormat() const;

        /// @brief Get the compression format of a port
        /// @return the compression of a port. Will be of type OMX_VIDEO_CODINGTYPE or OMX_IMAGE_CODINGTYPE
        IMPORT_C OMX_U32 getCompressionFormat() const;

        /// @brief Get the parameter structure of a port
        /// @return the parameter structure of a port
        OMX_PARAM_PORTDEFINITIONTYPE *getParamPortDefinition();

        /// @brief Get the video parameter structure of a port
        /// @return the video parameter structure of a port
        OMX_VIDEO_PORTDEFINITIONTYPE *getParamVideoPortDefinition();

        /// @brief Get the image parameter of a port
        /// @return the image parameter of a port
        OMX_IMAGE_PORTDEFINITIONTYPE *getParamImagePortDefinition();

        /// Check the compatibility of 2 ports
        /// @param [in] hTunneledComponent the component we are tunneled to
        /// @param [in] nTunneledPort the port in the tunneled component
        /// @return OMX_ErrorNone if they are compatible. An error otherwise
		IMPORT_C virtual OMX_ERRORTYPE checkCompatibility(OMX_HANDLETYPE hTunneledComponent,OMX_U32 nTunneledPort) const;

    public:
        /// @brief Get the size of an uncompressed frame given the color format and its size
        /// @param [in] eColor the color format
        /// @param [in] width the width of the frame
        /// @param [in] height the height of the frame
        /// @return the size, in bytes, of an uncompressed frame.
        IMPORT_C static OMX_U32 getSizeFrame(OMX_COLOR_FORMATTYPE eColor, OMX_U32 width, OMX_U32 height);

        /// @brief Get the stride of an uncompressed frame given the color format and its width. The stride
        /// has to be accurately computed as used in display. The stride corresponds to the size,
        /// in bytes, of 1 raw of the image
        /// @param [in] eColor the color format
        /// @param [in] width the width of the frame
        /// @return the stride, in bytes, of an uncompressed frame.
        IMPORT_C static OMX_U32 getStride(OMX_COLOR_FORMATTYPE eColor, OMX_U32 width);

        /// @brief Get the minimum buffer size of the port
        /// @return the minimum buffer size of the port
        /// @note this function must be overloaded as it depends on the used codec and compression format
        virtual OMX_U32 getBufferSize() const = 0;

        /// @brief Get the minimum buffer count of the port
        /// @return the minimum buffer count of the port
        /// @note this function can be overloaded if it depends on the used codec and compression format
        IMPORT_C virtual OMX_U32 getBufferCountMin() const;

        /* + Change start for ER 426886 */
        /// @brief Sets the minimum buffer count of the port
        IMPORT_C virtual void setBufferCountMin(OMX_U32 value);

        /// @brief Sets the actual buffer count of the port
        IMPORT_C virtual void setBufferCountActual(OMX_U32 value);

        /* - Change end for ER 426886 */

        /// @brief Check the the parameter of the port we want to set
        /// @param [in] portDef the port parameter we want to check
        /// @return an error if any
        /// @note This is mandatory to overload this function in the specific video proxy as this function is automatically called
        /// by the VFM when setting port parameters
        virtual OMX_ERRORTYPE checkSetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE* portDef) = 0;

        /// @brief Check the the video parameter of the port we want to set
        /// @param [in] pt the port video parameter we want to check
        /// @return an error if any
        /// @note It can be overloaded in the specific video proxy as this function is automatically called
        /// by the VFM when setting video port parameters
        IMPORT_C virtual OMX_ERRORTYPE checkIndexParamVideoPortFormat(OMX_VIDEO_PARAM_PORTFORMATTYPE *pt);

        /// @brief Check the the image parameter of the port we want to set
        /// @param [in] pt the port image parameter we want to check
        /// @return an error if any
        /// @note It can be overloaded in the specific video proxy as this function is automatically called
        /// by the VFM when setting image port parameters
        IMPORT_C virtual OMX_ERRORTYPE checkIndexParamImagePortFormat(OMX_IMAGE_PARAM_PORTFORMATTYPE *pt);

        /// @brief Get the port index of the "other" port
        /// @return The port index of the "other" port
        inline OMX_U32 getOtherPortIndex() const    { if (getPortIndex()==0) return 1; else return 0; }

        /// @brief Get the "other" port
        /// @return The "other" port
        inline VFM_Port *getOtherPort() const        { return (VFM_Port *)getENSComponent().getPort(getOtherPortIndex()); }

        /// @brief Return true is this is a master port, that is if this is an input port
        /// @return OMX_TRUE if this is a master port
        inline OMX_BOOL isMasterPort() const        { return ((getDirection() == OMX_DirInput) ? OMX_TRUE : OMX_FALSE) ; }

        /// @brief Return the master port of a given port
        /// @return the master port of a given port
        inline VFM_Port *getMasterPort()       { if (isMasterPort()) return this; else return getOtherPort(); }

        /// @brief Get the size of the slaved buffer (the output) given the size of the master one.
        /// This may depend on cropping and downsizing
        /// @param[in] nWidthMaster The width of the master port
        /// @param[in] nHeightMaster The height of the master port
        /// @param[out] nWidthSlave The resulting width of the slave port
        /// @param[out] nHeightSlave The resulting height of the slave port
        IMPORT_C virtual void getSlavedSizeFromGivenMasterSize(OMX_U32 nWidthMaster, OMX_U32 nHeightMaster, OMX_U32 *nWidthSlave, OMX_U32 *nHeightSlave) const;

        /// @brief Get the size (width, height) stroed in a given portDef
        /// @param[in] portDef The port definition
        /// @param[out] nWidth The width in the port definition
        /// @param[out] nHeight The height in the port definition
        IMPORT_C void getSizeFromParam(const OMX_PARAM_PORTDEFINITIONTYPE& portDef, OMX_U32 *nWidth, OMX_U32 *nHeight) const;

        /// @brief Check if some new "slaved" parameter settings have changed with respect to the
        /// parameters set in the master port
        /// @param [in] portDef the new set of parameters we want to set
        /// @param [in] bIAmTheMaster OMX_TRUE if this port is the master port
        /// @return OMX_TRUE if some slaved parameters have changed
        IMPORT_C OMX_BOOL isSizeChange(const OMX_PARAM_PORTDEFINITIONTYPE& portDef, const OMX_BOOL bIAmTheMaster) const;

        /// @brief Update the port settings of the slaved port when the master port
        /// has been modified. This should be called on the slaved port
        /// param [in] nWidthMaster the new width of the master port
        /// param [in] nHeightMaster the new height of the master port
        /// @return An error if any
        IMPORT_C OMX_ERRORTYPE updateParamPortDefinitionSlavedPort(const OMX_U32 nWidthMaster, const OMX_U32 nHeightMaster);

    private:
        static void vfm_assert_static(int condition, int line, OMX_BOOL isFatal, int param1=0, int param2=0);
};

/// Pointer to a VFM_Port
typedef VFM_Port * VFM_Port_p;

// Implementation rules:
// - no export of inlined functions
// - no inline of virtual functions
// - inline functions defined in .inl file or at the end of the .h file
// - no export of pure virtual functions

inline OMX_U32 VFM_Port::getBufferSizeInParam() const
{ return mParamPortDefinition.nBufferSize; }

inline OMX_U32 VFM_Port::getBufferCountMinInParam() const
{ return mParamPortDefinition.nBufferCountMin; }

/// @brief Get the parameter structure of a port
/// @return the parameter structure of a port
inline OMX_PARAM_PORTDEFINITIONTYPE *VFM_Port::getParamPortDefinition()
{ return &mParamPortDefinition; };

/// @brief Get the video parameter structure of a port
/// @return the video parameter structure of a port
inline OMX_VIDEO_PORTDEFINITIONTYPE *VFM_Port::getParamVideoPortDefinition()
{ return &mParamPortDefinition.format.video; };

/// @brief Get the image parameter of a port
/// @return the image parameter of a port
inline OMX_IMAGE_PORTDEFINITIONTYPE *VFM_Port::getParamImagePortDefinition()
{ return &mParamPortDefinition.format.image; };

/* + Change start for ER 426886 */
/// @brief Sets the minimum buffer count of the port
inline void VFM_Port::setBufferCountMin(OMX_U32 value)
{
    mParamPortDefinition.nBufferCountMin = value;
}

/// @brief Sets the actual buffer count of the port
inline void VFM_Port::setBufferCountActual(OMX_U32 value)
{
    mParamPortDefinition.nBufferCountActual = value;
}
/* - Change end for ER 426886 */

#endif // _VFM_PORT_H_
