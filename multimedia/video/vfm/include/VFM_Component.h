/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef __VFM_COMPONENT_HPP
#define __VFM_COMPONENT_HPP

#if ((!defined __SYMBIAN32__) && (!defined IMPORT_C))
    #define IMPORT_C
#endif
#if ((!defined __SYMBIAN32__) && (!defined EXPORT_C))
    #define EXPORT_C
#endif

#include "ENS_Component.h"
#include "ENS_Component_Fsm.h"
#include "VFM_Utility.h"
#include "VFM_Index.h"
#include "VFM_Types.h"
#include "VFM_Port.h"
#include "VFM_ParamAndConfig.h"
#include <ee/api/panic.idt>

#ifndef GET_ENS_COMP
#define GET_NMF_EXEC_CORE(comp) NMF_CoreId(\
                                ((ENS_Component_p)((OMX_COMPONENTTYPE *)comp)->\
                                pComponentPrivate)->getExecutionCore())

#define GET_ENS_COMP(comp) ((ENS_Component_p)((OMX_COMPONENTTYPE *)comp)->\
                                    pComponentPrivate)

#define GET_ENS_PORT(comp, port_id) ((ENS_Component_p)((OMX_COMPONENTTYPE *)comp)->\
                                    pComponentPrivate)->getPort(port_id)

#define GET_ENS_TUNNELED_COMP(port) ((ENS_Component_p) ((OMX_COMPONENTTYPE *)\
                                    port->getTunneledComponent())->pComponentPrivate)

#define GET_ENS_FSM_COMP(comp) GET_ENS_COMP(comp)->getCompFsm()
#endif

void Panic(t_panic_reason reason, t_uint24 info);

// Forward definition
class VFM_ParamAndConfig;

/// @mainpage Video OpenMax IL Components
/// This is the documentation of the OpenMax IL Framework for the video.
/// The framework is decomposed in 2 general part:
/// - the VFM (Video Framework Management): it corresponds to a set of classes
///   that groups commonalities between all or part of the video encoders and decoders.
///   All the class and files belondings to this category are suffixed by VFM)
/// - For a given encoder / decoder, a set of specialization file of the proxy.
///   These files / classes are suffixed by the name of the encoder / decoder
///   (JPEGEnc for the JPEG encoder as an example).
///
/// The general way is to have an OpenMax API as thin as possible. That is the application
/// only sees what is interesting to it, and OpenMax hides more advanced features
/// of the firmware that could be either computed (for example the scale of the
/// JPEG Encoder Quantization Table by the Quality Factor, or the default huffman to
/// be used) or that are called with a constant default value (for example the
/// restart interval -- to be confirmed).
///
/// Major differences with the former VFM are:
/// - The former implementation was preserving the legacy of the HCL, and so was
///   implementing the VFM in 2 parts: the Component Mapper (the user side) and
///   the Interface Mapper (kernel side). This was complexifying both the VFM itself
///   and the application. Now, the VFM is implemented in a single part, on the
///   user side. Implementation of the VFM is then simplified: everything in C++,
///   everything based on the ENS, a single module to deliver, no more communication
///   between CMapper and IMapper
/// - Better coherency between media: audio, imaging and video
/// - Tunnelling possibilities, standard and proprietary, including between components
///   coming from different media
/// - Better reuse through the ENS due to the better coherency
/// - Better OpenMax IL fitting, to be able to use other framework for OpenMax that
///   may be delivered in OS
///
/// Dependencies with other modules are
/// - ENS (Execution Network Service), the OpneMax-like implementation. Each video component
///   is based on an ENS_Component. Refer to codex
///   https://codex.cro.st.com/file/showfiles.php?group_id=724
/// - CM (Component Manager): Manages links between the host and the SVA. Typically,
///   instantiation of components, sending buffers, getting callbacks from the SVA are
///   all part of the CM
/// - Allocation API: to be detailed once available
/// - NMF: Components on the SVA for the firware. This is not directly used by the VFM, but
///   it is used by the Firmware development of the algorithms. Refer to codex
///   https://codex.cro.st.com/projects/nmf/
///
/// @version
/// W1024 MMD Delivery

/// @defgroup VFMProprietary VFM Proprietary Indexes and Structures
/// @ingroup VFM
/// @brief Contains proprietary indexes and structures, for example to get / set
/// a proprietary parameter, or to define proprietary image format

/// @defgroup VFMUtilities VFM Utilities
/// @ingroup VFM
/// @brief Set of utility functions / macros used in video proxies.

/// @defgroup VFM VFM (Video Framework Management)
/// @brief The VFM is intended to ease the development of a new proxy
/// by commonalizing functionalities that can be shared by a number
/// of video encoders / decoders.
///
/// Hence a given proxy will make use of the following VFM modules:
/// any functionalities that can be commonalized between
/// several video encoders and decoders. It is made of the following modules:
/// - The proxy itself makes use of VFM_Component
///   <b> the base class of a video component proxy </b>
///   (tunneling capabilities,
///   management, creation of video / imaging port, to give generic functions
///   for NMF instantiation / deinstantiation, for generic setting of the size of a frame,
///   for generic callback (EmptyBufferDone, FillBufferDone and EventHandler), and for
///   generic FW calls (EmptyThisBuffer, FillThisBuffer). The VFM components
///   are based on ENS component, but VFM component makes use of VFM_Port
///   instead of ENS_Port (VFM Ports are a specialization of ENS Port
///   for video proxies).
/// - Parameters and Configuration are managed by the class VFM_ParamAndConfig.
///   It contains utility functions to set and get OpenMax parameters
///   and configuration that are specified in the OpenMax Specification (no proprietary indexes).
///   This one is the parent class of VFM_EncParamAndConfig and VFM_DecParamAndConfig
///   that are used to contain specific indexes to video encoders or decoders.
///
/// Moreover, the VFM is made of several modules that are either used by the ENS,
/// or that contain utility function to help video proxy integration:
/// - @ref VFMUtilities contains a set of macros and classes useful at different places
///   in video proxies. VFM_Status is part of this module. It also contains the class
///   VFM_Status, utility class for status (true / false) management
/// - @ref VFMProprietary contains the proprietary indexes and structures required by the
///   VFM to extend OpenMax IL Specifications
/// - VFM_Component_RDB manages the resources required by a component (linked with ENS-DRE)


/// @ingroup VFM
/// @{
/// @brief Manages the resources required by a component
class VFM_Component_RDB: public ENS_ResourcesDB {
    public:
        /// @brief Creation of the required resources, that are included in the ENS_DRE
        ///
        /// Create the required resources of a video component. They are:
        /// - running exclusively on the SVA
        /// @todo to be documented once we know more about the future of the ENS_DRE
        VFM_Component_RDB(OMX_U32 nbOfDomains=1);
};
/// @}

/// @defgroup VFMOMXSUPPORT VFM OpenMax IL Suppport
/// @ingroup VFM
/// @brief VFM contains an OpenMax IL development support in order to help the
/// implementation of a video OMX component. Whereas the ENS is providing generic
/// support among all media, VFM is providing support for video specific
/// information, as for example level/profile or video buffer payload
///
/// Typically, a specific proxy to MyCodec would be made of the different classes:
/// - MyCodec_Proxy that would inherit from VFM_Component. This is intended
///   to the OMX component itself
/// - MyCodec_Port that would inherit from VFM_Port. This is intended to
///   the ports (parametrisation, buffer passing)
/// - MyCodec_ParamAndConfig that would inherit from VFM_ParamAndConfig. This is
///   intended to paramaters and configuration of the OMX component.

/// @defgroup VFMComponent VFM Component
/// @ingroup VFMOMXSUPPORT
/// @brief <b> Base class of a video component proxy </b>, specialization of the ENS_Component
/// dedicated to Video Proxies. It provides a set a functions for the creation
/// of video ports, tunneling management, generic functions for callbacks and NMF calls, ...

/// @ingroup VFMComponent
/// @brief <b> Base class of a video component proxy </b>, specialization of the ENS_Component
/// dedicated to Video Proxies. It provides a set a functions for the creation
/// of video ports, tunneling management, generic functions for callbacks and NMF calls, ...
///
/// This class is derived from ENS_Component, and is a specialization of a ENS_Component
/// for video and imaging proxy. In this way, the ports definition (ENS_Port) available
/// in a generic ENS Component is indeed a VFM_Port in a VFM Component.
/// The functionalities VFM_Component provides, to other proxies, are:
/// - Initialization: constructor of the proxy calls VFM_Component constructor before
///   any static initialization. Then MyCodec_Proxy::construct() must be implemented
///   as called by the ENS once the constructor is called. This specific proxy function
///   makes then use of VFM_Component::construct(nPortCount) to initialize the correct
///   number of ports. Then it makes use of VFM_Component::createPort() to create each port
///   given their specificities. Note that this latest method makes use of function
///   MyCodec_Proxy::newPort() that must be implemented
/// - resource management:
///   - VFM_Component::createResourcesDB(). This will be modified with the new RME
///     (Resource Management Engine)
/// - generic proxy function, dedicated to default behavior:
///   - VFM_Component::setParameter()
///   - VFM_Component::getParameter()
/// - generic way to get / set the size of a frame:
///   - VFM_Component::setFrameWidth()
///   - VFM_Component::setFrameHeight()
///   - VFM_Component::setFrameSize()
///   - VFM_Component::getFrameWidth()
///   - VFM_Component::getFrameHeight()
///   - VFM_Component::getFrameSize()
/// - Support for the omx functionality that query the list of all supported
///   profiles and levels: setProfileLevelSupported() that must be implemented
///   in the proxy, and setOneProfileLevelSupported() and getSupportedProfileLevel()
/// - Support to create the specific factory method: VFMFactoryMethod()
/// - reset()
class VFM_Component: public ENS_Component
{
    friend class VFM_NmfHost_ProcessingComponent;
    /* +Change start for CR330198 Revert to DTS from PTS */
    friend class VFM_NmfMpc_ProcessingComponent;
    /* -Change end for  CR330198 Revert to DTS from PTS */


    public:
        /// @brief Pointer to the Parameter and Configuration. It is aimed
        /// at simplifying parameter and configuration settings. Note that the
        /// underlying VFM_ParamAndConfig will be indeed MyProxy_ParamAndConfig type
        VFM_ParamAndConfig *pParam;
    private:
        /// @brief Maximum of supported profile
        static const int mMaxProfileLevelSupported = 8;

        /// @brief Array of supported profile
        OMX_U32 mProfileSupported[mMaxProfileLevelSupported];

        /// @brief Array of maximum supported level. For a given profile in
        /// mProfileSupported[index], mLevelSupported[index] equals the maximum
        /// supported level
        OMX_U32 mLevelSupported[mMaxProfileLevelSupported];

        /// @brief Current number of levels
        OMX_U32 mNbProfileLevelSupported;

        /// @brief Maximum of supported profile. Large number due to numerous 3D color format
        static const int mMaxVideoSettingsSupported = 20;

        /// @brief Array of supported video settings, 1 per port
        OMX_VIDEO_PARAM_PORTFORMATTYPE mVideoSettingsSupported[2][mMaxVideoSettingsSupported];

        /// @brief Number of supported video settings
        OMX_U32 mNbVideoSettingsSupported[2];

        /// @brief TRUE to use esram scratch memory
        OMX_BOOL mScratchEsram;

        /// @brief Time (nb of ticks, in micro-second) when we call the construct
        /// Reseted when 1st frame is sent out
        OMX_U32 mConstructTime;
public:
        OMX_PTR  pFwPerfDataPtr;
        OMX_BOOL mIsARMLoadComputed;

    public:
        /// @brief Constructor
        /// @param [in] Param a pointer to the parameters and configuration of the proxy
        ///
        /// This constructor only initializes generic part of a VFM component, with static
        /// parameters (no dynamic allocation for example). The real initialization is
        /// performed by the functions construct() in order to be able to perform error management.
        /// This constructor is called by MyCodec_Proxy constructor using inheritance construction
        IMPORT_C VFM_Component(VFM_ParamAndConfig *Param, OMX_BOOL scratchEsram=OMX_FALSE);

        /// @brief Initialize a VFM component, to be called right after the constructor.
        /// @param [in] nPortCount number of ports (both input and output) ports
        ///    of this component. Ports are of type VFM_Port, as created
        ///    by createSpecificPort()
        ///
        /// This is not part of the constructor as C++ constructor are not able to
        /// return errors. Hence any dynamic initialization should be part of the
        /// construct().
        /// @note This construct only construct the general part of a VFM_Component.
        /// This is intended to be called from MyProxy::construct() (without arguments)
        /// which is the specific part of the construct
        IMPORT_C OMX_ERRORTYPE construct(OMX_U32 nPortCount);

        /// @brief Initialize a specific component, to be called right after the constructor
        /// of the constructor.
        /// @note This function should be overloaded by the specific proxy
        /// as it is called by the VFM function VFM_Component::VFMFactoryMethod()
        /// @todo  can it be removed  as it must be implemented in the proxy?
        IMPORT_C virtual OMX_ERRORTYPE construct() = 0;

        /// Destructor
        IMPORT_C virtual ~VFM_Component();

        IMPORT_C virtual RM_STATUS_E getResourcesEstimation (OMX_IN const OMX_PTR pCompHdl,
                        OMX_INOUT RM_EMDATA_T* pEstimationData);

        /// @brief Create a port of a VFM component, that is a VFM_Port.
        /// This port can then be retrived using the function ENS_Port::getPort()
        /// giving the index of the port to be retrieved.
        /// @param [in] nPortIndex
        ///    Index of the created port. Must be lower than the number of ports
        ///    used when creating the ENS component. Must not be the index of an
        ///    already created port. All created video ports must have incremental
        ///    indexes. This function is intended to be called by the constructor
        ///        of the component.
        /// @param [in] eDir Direction of the port
        /// @param [in] eSupplierPref
        ///    Buffer supplier preferences to use when tunneling. Use
        ///    OMX_BufferSupplyUnspecified when tunneling is not supported or to
        ///    indicate that the port has no preference
        /// @param [in] nBufferCountMin minimal number of buffers
        /// @param [in] nBufferSizeMin minimal size of buffers
        /// @param [in] domain of the port (video, imaging, audio or other)
        /// @return OMX_ErrorNone in case no error occurs, or
        ///    - OMX_ErrorBadPortIndex
        ///    - OMX_ErrorBadParameter
        /// @note This is intended to be used in MyCodec_Proxy::construct()
        ///       to dynamically allocate ports.
        /// @note This makes use of virtual function MyCodec_Proxy::newPort()
        IMPORT_C OMX_ERRORTYPE createPort(
            OMX_U32 nPortIndex,
            OMX_DIRTYPE eDir,
            OMX_BUFFERSUPPLIERTYPE eSupplierPref,
            OMX_U32 nBufferCountMin,
            OMX_U32 nBufferSizeMin,
            OMX_PORTDOMAINTYPE domain);


        /// @brief Describes the resources that are used by this component
        ///
        /// This function describes the resources that are used / required
        /// by the component in order to be implemented. Every video component
        /// is intended to run on the
        /// @return OMX_ErrorNone if no error, an error otherwise
        IMPORT_C virtual OMX_ERRORTYPE createResourcesDB();

        /// @brief Set parameters to the component, from the application. It is part
        /// of OpenMaxIL API
        /// @param [in] nParamIndex index of the structure we want to set. Supported indexes are: \n
        /// - OMX_IndexParamVideoProfileLevelCurrent
        /// - All the ones supported by ENS_Component::setParameter()
        /// @param [in] pt the structure that contains the parameters we want to set
        /// @return OMX_ErrorNone if no error, an error otherwise
        /// @todo Adopt a generic way to handle error. This is a general comment
        IMPORT_C virtual OMX_ERRORTYPE setParameter(OMX_INDEXTYPE nParamIndex, OMX_PTR pt);

        /// @brief Get parameters from the component, to the application. It is part of OpenMaxIL API
        /// @param [in] nParamIndex index of the structure we want to get. Supported indexes are: \n
        /// - OMX_IndexParamVideoProfileLevelCurrent
        /// - All the ones supported by ENS_Component::getParameter()
        /// @param [out] pt the structure that will contains the parameters we want to get
        /// @return OMX_ErrorNone if no error, an error otherwise
        IMPORT_C virtual OMX_ERRORTYPE getParameter(OMX_INDEXTYPE nParamIndex, OMX_PTR pt) const;

        /// @brief Set config to the component, from the application. It is part
        /// of OpenMaxIL API
        /// @param [in] nConfigIndex index of the structure we want to set.
        /// @param [in] pt the structure that contains the parameters we want to set
        /// @return OMX_ErrorNone if no error, an error otherwise
        /// @todo Adopt a generic way to handle error. This is a general comment
        IMPORT_C virtual OMX_ERRORTYPE setConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pt);

        /// @brief Get config from the component, to the application. It is part of OpenMaxIL API
        /// @param [in] nConfigIndex index of the structure we want to get.
        /// @param [out] pt the structure that will contains the parameters we want to get
        /// @return OMX_ErrorNone if no error, an error otherwise
        IMPORT_C virtual OMX_ERRORTYPE getConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pt) const;

        /// @brief Set the width of a frame, on a given port
        /// @param [in] nPortIndex the port index to set
        /// @param [in] width the width to set
        void setFrameWidth(int nPortIndex, OMX_U32 width);

        /// @brief Set the height of a frame, on a given port
        /// @param [in] nPortIndex the port index to set
        /// @param [in] height the height to set
        void setFrameHeight(int nPortIndex, OMX_U32 height);

        /// @brief Set the size of a frame, on a given port
        /// @param [in] nPortIndex the port index to set
        /// @param [in] width the height to set
        /// @param [in] height the height to set
        void setFrameSize(int nPortIndex, OMX_U32 width, OMX_U32 height);

        /// @brief Get the width of a frame, on a given port
        /// @param [in] nPortIndex the port index to set
        /// @return the width
        OMX_U32 getFrameWidth(int nPortIndex) const;

        /// @brief Get the width of a frame, on a given port
        /// @param [in] nPortIndex the port index to set
        /// @return the width
        OMX_U32 getFrameHeight(int nPortIndex) const;

        /// @brief Get the colorformat of a frame, on a given port
        /// @param [in] nPortIndex the port index to set
        /// @return the colorformat
        OMX_U32 getColorFormat(int nPortIndex) const;


        /// @brief Get the buffer size, on a given port
        /// @param [in] nPortIndex the port index to set
        /// @return the buffer size on a port
		OMX_U32 getBufferSizeInParam(int nPortIndex) const;

        /// @brief Get the actual number of buffer count, on a given port
        /// @param [in] nPortIndex the port index to set
        /// @return the actual no of buffer count
        OMX_U32 getBufferCountActual(int nPortIndex) const;

        /// @brief Get the width of a frame, on a given port
        /// @param [in] nPortIndex the port index to set
        /// @param [out] width the width of the frame
        /// @param [out] height the height of the frame
        void getFrameSize(int nPortIndex, OMX_U32 *width, OMX_U32 *height) const;

        /// @brief Reset of the component, automatically  be called after a change of the parameters of the port
        /// @note This is mandatory to overload it inside the specific component
        virtual OMX_ERRORTYPE reset() = 0;

        /// @brief Generic factory method, to be called by any specific factory method
        /// @param [out] ppENSComponent the component that we are crating
        /// @param [in] newProxy() function to allocate the component
        /// @return an error if any, OMX_ErrorNone otherwise
        /// @note A typical factory method would be made of the single source line
        /// return VFM_Component::VFMFactoryMethod(ppENSComponent, MyCodec_Proxy::newProxy);
        IMPORT_C static OMX_ERRORTYPE VFMFactoryMethod(ENS_Component_p * ppENSComponent,
                VFM_Component *(*newProxy)(void));

        /// @brief Allocate a new video port. This function must be overloaded by the specific proxy
        /// as it is called by the VFM function VFM_Component::createPort()
        /// @param [in] commonPortData the common data of the port, provided at the initialization
        /// of the port
        /// @param [in] enscomp the omx component the port belongs to
        /// @return the port that has been newly allocated
        virtual VFM_Port *newPort(const EnsCommonPortData& commonPortData, ENS_Component &enscomp) = 0;

        /// @brief Sets all the profile / level that are supported by this codec. This function
        /// must be implemented in the proxy as called in the VFM::construct(). A typical is a list of
        /// setOneProfileLevelSupported(profile, level) command, for each supported
        /// profile (level being the maximum supported level)
        IMPORT_C virtual void setProfileLevelSupported() = 0;

        /// @brief Sets one profile that is supported, with its related maximum profile
        /// This adds an entry in mProfileSupported and mLevelSupported, and increments
        /// mNbProfileLevelSupported
        IMPORT_C void setOneProfileLevelSupported(OMX_U32 profile, OMX_U32 level);

		/// @brief Clears the mNbProfileLevelSupported value. This function is used in case the proxy
		/// wants to reset the Profile / Level that were set by calling setOneProfileLevelSupported
		IMPORT_C void clearAllProfileLevelSupported();

        /// @brief Get the supported profile and level for a given index (instance of the profile)
        /// This is used on GetParameter(OMX_IndexParamVideoProfileLevelQuerySupported)
        /// @param [in] index the index in the array of profile / level
        /// @param [out] profile the corresponding profile
        /// @param [out] level the corresponding level
        /// @return OMX_ErrorNone if no error, and OMX_ErrorNoMore if index >= mNbProfileLevelSupported
        OMX_ERRORTYPE getSupportedProfileLevel(OMX_U32 index, OMX_U32 *profile, OMX_U32 *level);

        /// @brief Sets all the profile / level that are supported by this codec. This function
        /// must be implemented in the proxy as called in the VFM::construct(). A typical is a list of
        /// setOneProfileLevelSupported(profile, level) command, for each supported
        /// profile (level being the maximum supported level)
        IMPORT_C virtual void setVideoSettingsSupported();

        /// @brief Sets one profile that is supported, with its related maximum profile
        /// This adds an entry in mVideoSettingsSupported, and increments
        /// mNbVideoSettingsSupported
        IMPORT_C void setOneVideoSettingsSupported(OMX_U32 nPortIndex, OMX_VIDEO_CODINGTYPE eCompressionFormat, OMX_COLOR_FORMATTYPE eColorFormat, OMX_U32 xFrameRate);

        /// @brief Get the supported video settings of a port for a given index (instance of the setting)
        /// This is used on GetParameter(OMX_IndexParamVideoProfileLevelQuerySupported)
        /// @param [in] nPortIndex the index of the port
        /// @param [in] index the index in the array of video settings
        /// @param [out] eCompressionFormat the compression format
        /// @param [out] eColorFormat the color format
        /// @param [out] xFrameRate the frame rate
        /// @return OMX_ErrorNone if no error, and OMX_ErrorNoMore if no more index
        OMX_ERRORTYPE getSupportedVideoSettings(OMX_U32 nPortIndex, OMX_U32 index, OMX_VIDEO_CODINGTYPE *eCompressionFormat, OMX_COLOR_FORMATTYPE *eColorFormat, OMX_U32 *xFrameRate);

        /// @brief Check if the component is a coder
        /// @return OMX_TRUE if the component is a coder
        IMPORT_C OMX_BOOL isCoder();

        /// @brief Check if the component is a coder
        /// @return OMX_TRUE if the component is a deccoder
        IMPORT_C OMX_BOOL isDecoder();

        /// @brief Check if the component is a coder
        /// @return OMX_TRUE if the component is an encoder
        IMPORT_C OMX_BOOL isEncoder();

        /// @brief Check if the component is a source or a sink
        /// @return OMX_TRUE if the component is a source or a sink
        IMPORT_C OMX_BOOL isSourceSink();

        /// @brief Function called on OMX_GetExtensionIndex for video specific indexes
        /// @param[in] cParameterName The string of the index
        /// @param[out] pIndexType The corresponding index identifier
        /// @return An error if any
        IMPORT_C virtual OMX_ERRORTYPE getExtensionIndex(OMX_STRING cParameterName, OMX_INDEXTYPE* pIndexType) const;

        /// @brief Return ParamAndConfig struture
        /// @return Pointer to the ParamAndConfig
        inline VFM_ParamAndConfig *getParamAndConfig();

        /// @brief Return TRUE if the esram scratch is used
        /// @return TRUE if the esram scratch is used
        inline OMX_BOOL getScratchEsram()   { return mScratchEsram; }

        /// @brief Get the domain used to instantiate the NMF component
        /// @return the domain used to instantiate the NMF component
        inline OMX_U32 getNmfComponentDomain()  { return getNMFDomainHandle(0); }

        /// @brief Get the domain used to allocate ddr memory
        /// @return the domain used to allocate ddr memory
        inline OMX_U32 getAllocDdrMemoryDomain()   { return getNMFDomainHandle(0); }

        /// @brief Get the domain used to allocate esram memory
        /// @return the domain used to allocate esram memory
        inline OMX_U32 getAllocEsramMemoryDomain()   { return getNMFDomainHandle((mScratchEsram==OMX_TRUE) ? 1 : 0); }

        /// @brief Check whether or not an EmptyThisBuffer should be treated or not: nFilledLen==0,
        /// without relevant flags and not marked data
        /// @param[in] pBuffer the buffer we want to empty
        /// @return OMX_TRUE if this buffer should not be treated
        static OMX_BOOL skipEmptyThisBuffer(OMX_BUFFERHEADERTYPE* pBuffer);

        /// @brief Return the maximum number of buffers accepted on a given port
        /// If this number is -1, that means no maximum is set
        /// @param[in] portIndex the port we want to check
        /// @return The maximum number of buffers this port accepts, or -1 if no maximum
        IMPORT_C virtual OMX_S32 getMaxBuffers(OMX_U32 portIndex) const;

        IMPORT_C virtual OMX_ERRORTYPE doSpecificInternalState(const ENS_FsmEvent *evt);

        static void vfm_assert_static(int condition, int line, OMX_BOOL isFatal, int param1=0, int param2=0);

        inline OMX_U32 getConstructTime()       { return mConstructTime; }
        inline void resetConstructTime()        { mConstructTime = 0; }
};

// Implementation rules:
// - no export of inlined functions
// - no inline of virtual functions
// - inline functions defined in .inl file or at the end of the .h file
// - no export of pure virtual functions

inline void VFM_Component::setFrameWidth(int nPortIndex, OMX_U32 width)     { ((VFM_Port_p) getPort(nPortIndex))->setFrameWidth(width); }
inline void VFM_Component::setFrameHeight(int nPortIndex, OMX_U32 height)   { ((VFM_Port_p) getPort(nPortIndex))->setFrameHeight(height); }
inline void VFM_Component::setFrameSize(int nPortIndex, OMX_U32 width, OMX_U32 height)
{
    ((VFM_Port_p) getPort(nPortIndex))->setFrameWidth(width);
    ((VFM_Port_p) getPort(nPortIndex))->setFrameHeight(height);
}
inline OMX_U32 VFM_Component::getFrameWidth(int nPortIndex) const           { return ((VFM_Port_p) getPort(nPortIndex))->getFrameWidth(); }
inline OMX_U32 VFM_Component::getFrameHeight(int nPortIndex) const          { return ((VFM_Port_p) getPort(nPortIndex))->getFrameHeight(); }
inline OMX_U32 VFM_Component::getColorFormat(int nPortIndex) const          { return ((VFM_Port_p) getPort(nPortIndex))->getColorFormat(); }
inline void VFM_Component::getFrameSize(int nPortIndex, OMX_U32 *width, OMX_U32 *height) const
{
    *width = ((VFM_Port_p) getPort(nPortIndex))->getFrameWidth();
    *height = ((VFM_Port_p) getPort(nPortIndex))->getFrameHeight();
}
inline VFM_ParamAndConfig *VFM_Component::getParamAndConfig() { return pParam; }

inline OMX_U32 VFM_Component::getBufferCountActual(int nPortIndex) const           { return ((VFM_Port_p) getPort(nPortIndex))->getBufferCountActual(); }
inline OMX_U32 VFM_Component::getBufferSizeInParam(int nPortIndex) const           { return ((VFM_Port_p) getPort(nPortIndex))->getBufferSizeInParam(); }


#endif // __VFM_COMPONENT_HPP
