/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef __VFM_DDEPUTILITY_HPP
#define __VFM_DDEPUTILITY_HPP

#if ((!defined __SYMBIAN32__) && (!defined IMPORT_C))
    #define IMPORT_C
#endif
#if ((!defined __SYMBIAN32__) && (!defined EXPORT_C))
    #define EXPORT_C
#endif

#include "Component.h"
#ifdef __SYMBIAN32__
// This is important to include the one from the ENS as OMX types are renamed
// with t_xyz ...
#include "osi/shared/OMX_Core.h"
#else
#include "OMX_Core.h"
#endif
#include "inc/typedef.h"
#include "common_frameinfo.idt"
#include "ENS_List.h"
#include "mmhwbuffer.h"
class Common_Error_Map;


/// @defgroup VFMDDepUtility DDep Utility classes
/// @ingroup VFM

#define VFMDDEP_TRACE(_cond, _func) { if ((_cond)) { (_func); } }

// Flagging a buffer to check it is an internal one
#define VFM_FLAG_INTERNALBUFFER  0x40000000

typedef enum {
    VFMDDEP_BUFFERMODE_FASTPATH=0x1,
    VFMDDEP_BUFFERMODE_COPY=0x2,
    VFMDDEP_BUFFERMODE_FASTANDCOPY=0x3      // = VFMDDEP_BUFFERMODE_FASTPATH | VFMDDEP_BUFFERMODE_COPY
} VFM_BufferMode;

/* + Changes for CR 334359 */
class VFM_SocCapabilityMgt {
public:
    static OMX_BOOL isSoc1080pCapable();
    static OMX_U32 getLevelCapabilities();
    /* +Change for CR 367195, 369543 */
    static OMX_U32 getOutputBufferCount();
    static OMX_U32 getMPEG4DecInputBuffSize();
    static OMX_U32 getDecoderRecyclingDelay();
    static OMX_BOOL getDecoderDisplayOrder();
    /* -Change for CR 367195, 369543 */
    /* +Change for CR 374389 */
    static OMX_U32 getH264DecoderDefaultLevel();
    /* -Change for CR 374389 */
};
/* - Changes for CR 334359 */

class VFM_PowerManagement {
public:
    /// @brief Latency, as given in the Video SAS
    typedef enum {
        VFM_ResLatency_None=1,
        VFM_ResLatency_Standard,
        VFM_ResLatency_Max
    } VFM_ResLatencyType;

    typedef enum {
        VFM_ResDVFS_Full=1,
        VFM_ResDVFS_Half,
    } VFM_ResDVFSType;

//+ ER335583
    typedef enum {
        VFM_ResDDR_Full=1,
        VFM_ResDDR_Half,
    } VFM_ResDDRType;
//- ER335583

    typedef enum {
        VFM_PowerStatus_Cut,        // Power is minimal: VFM_ResLatency_Max and VFM_ResDVFS_Half
        VFM_PowerStatus_Force,      // Power is full: VFM_ResLatency_None and VFM_ResDVFS_Full
        VFM_PowerStatus_Regular     // Power is the one requested: mRequestLatency and mRequestDVFS
    } VFM_PowerStatusType;

    IMPORT_C static void setFullDVFS(OMX_PTR omxHandle);
    IMPORT_C static void releaseFullDVFS(OMX_PTR omxHandle);

    IMPORT_C static void setDVFS(OMX_PTR omxHandle, VFM_ResDVFSType value);
    IMPORT_C static void resetDVFS(OMX_PTR omxHandle);

//+ ER335583
    IMPORT_C static void setFullDDR(OMX_PTR omxHandle);
    IMPORT_C static void releaseFullDDR(OMX_PTR omxHandle);

    IMPORT_C static void setDDR(OMX_PTR omxHandle, VFM_ResDDRType value);
    IMPORT_C static void resetDDR(OMX_PTR omxHandle);
//- ER335583

    IMPORT_C static void setLatency(OMX_PTR omxHandle, VFM_ResLatencyType value);
    IMPORT_C static void resetLatency(OMX_PTR omxHandle);

    VFM_PowerManagement(OMX_BOOL manage);
    ~VFM_PowerManagement();
    //modified for ER335583
    IMPORT_C void set(VFM_ResDVFSType dvfs, VFM_ResDDRType ddr, VFM_ResLatencyType latency);    // indicate the expected value of power
    IMPORT_C void force();      // Set power to VFM_PowerStatus_Force
    IMPORT_C void cut();        // Set power to VFM_PowerStatus_Cut
    IMPORT_C void regular();    // Set power to VFM_PowerStatus_Regular

private:
    const OMX_BOOL mManage;

    VFM_PowerStatusType mStatus;
    VFM_ResDVFSType mRequestDVFS;
//+ ER335583
	VFM_ResDDRType mRequestDDR;
//- ER335583
    VFM_ResLatencyType mRequestLatency;
};

class VFM_SetDDep;


/// @ingroup VFMDDepUtility
/// @brief Class to describe an internal buffer, in the ddep.
/// Such buffers are used to make some copies of omx buffers, for example
/// to adapt to any type of payload, or when OMX buffers are not physically
/// contiguous in memory. It is based on MMHwBuffers.
class VFM_DDepInternalBuffer {
public:
    /// @brief Constructor
    IMPORT_C VFM_DDepInternalBuffer();

    /// @brief Destructor
    IMPORT_C virtual ~VFM_DDepInternalBuffer();

    /// @brief Free memory associated with his internal buffer
    IMPORT_C void free(void);

    /// @brief Free a MHwBuffer chunk
    /// @param[in] pChunk the MMHwBuffer tchunk to free
    static void free(MMHwBuffer **pChunk);

    /// @brief Allocate an internal buffer, given the size and if cached or not.
    /// If it appears that it was alreadyy allocated with a smaller size, then
    /// there is no change. Otherwise, the old allocated chunk is freed, and reallocated.
    /// @param[in] size the mmhwbuffer to allocate
    /// @param[in] isCached OMX_TRUE to have cached memory, OMX_FALSE otherwise
    /// @return OMX_ErrorNone is no error
    OMX_ERRORTYPE alloc(t_uint32 size, OMX_BOOL isCached);

    /// @brief Get the MMHwBuffer chunk associated with this buffer
    /// @return the chunk
    inline MMHwBuffer *getChunk()                               { return mChunk; }

    /// @brief Get the status (INTERNAL_SENT, INTERNAL_FREE, INTERNAL_CURRENT or a positive number
    /// to describe when the buffer must be sent to the codec)
    /// @return The status
    inline t_sint8 getStatus(void)                              { return mStatus; };

    /// @brief Set the status of the buffer (INTERNAL_SENT, INTERNAL_FREE, INTERNAL_CURRENT,...)
    /// @param[in] status the new status
    void setStatus(t_sint8 status);

    /// @brief Get the buffer header associated with this internal buffer
    /// @return The buffer header
    inline OMX_BUFFERHEADERTYPE *getBufferHeader(void)          { return &mBufferHeader; }

    /// @brief When an internal comes back from the codec, setIsBack resets its values (lenght,
    /// status becomes free,...)
    IMPORT_C void setIsBack(void);

    /// @brief Copy pSaved into "this". pSaved becomes a free buffer. Typically, this function
    /// is used when reallocating internal buffers
    /// @param[in] pSaved the buffer to copy
    void copy(VFM_DDepInternalBuffer *pSaved);

    /// @brief Check whether or not this buffer is the last one of a frame, that is is flagged
    /// with either OMX_BUFFERFLAG_ENDOFFRAME
    /// @param[in] flags The flag to check
    /// @return OMX_TRUE is this is the last buffer of a frame
    IMPORT_C static OMX_BOOL isLastBuffer(OMX_U32 flags);

    /// @brief Cat a buffer header data in this internal buffer. Flags are updated accordingly
    /// @param[in] pHeaderToCopy the buffer header to copy
    /// @param[in@ pSet Set of internal buffers that is affected
    /// @return OMX_ErrorNone if no error occurs. If an error occurs (reallocation failed),
    /// then the internal buffer is set as corrupted
    OMX_ERRORTYPE catBufferHeader(OMX_BUFFERHEADERTYPE *pHeaderToCopy, VFM_SetDDep *pSet);

    /// @brief Get the number of bytes to relloc
    /// @return the number of bytes to relloc
    IMPORT_C virtual OMX_U32 getReallocMore();

    /// @brief Realloc an internal buffer, if neede
    /// param[in] new_size The new size to allocate
    /// @return OMX_ErrorNone if no errors
    OMX_ERRORTYPE realloc(OMX_U32 new_size, VFM_SetDDep *pSet);

    /// @brief Set this buffer corruption
    inline void setCorrupted() { mIsCorrupted = OMX_TRUE; };

    /// @brief Set this buffer corruption
    inline void resetCorrupted() { mIsCorrupted = OMX_FALSE; };

    /// @brief Return if the internal buffer is corrupted or not (because of realloc issue)
    /// @return Return if the internal buffer is corrupted or not (because of realloc issue)
    inline OMX_BOOL isCorrupted()   { return mIsCorrupted; };

private:
    /// @brief The data
    MMHwBuffer *mChunk;

    /// @brief mStatus is:
    ///      INTERNAL_FREE if it is free
    ///      INTERNAL_SENT if it has been sent to the codec
    ///      INTERNAL_CURRENT for current buffer
    ///      or 0..n to know what is the next buffer to sent
    t_sint8 mStatus;

    /// @brief Indicate whether or not the current buffer is corrupted or not
    OMX_BOOL mIsCorrupted;

    /// @brief Fake omx buffer header to know what is the internal buffer that comes back
    OMX_BUFFERHEADERTYPE mBufferHeader;
public:
    OMX_PTR ptropenmaxHandle;
};

/// @ingroup VFMDDepUtility
/// @brief Class to describe all the internal buffers associated with an
/// input port or an output port
class VFM_DDepAllInternalBuffers {
public:
    /// @brief Constructor
    IMPORT_C VFM_DDepAllInternalBuffers();

    /// @brief Get the current buffer that is copied
    /// @param[in] needANewOne OMX_TRUE if we want a new current buffer if no current buffer exists
    /// @return The current buffer, or NULL if impossible to have a current buffer
    IMPORT_C VFM_DDepInternalBuffer *getCurrent(OMX_BOOL needANewOne);

    /// @brief Get the next buffer to send to the codec (the one with status=0).
    /// Other buffers states are updated (decreased) if willBeSent=OMX_TRUE
    /// @param[in] willBeSent OMX_TRUE if the found buffer will be sent immediatly to the
    /// codec
    /// @param[in] newStatus I case willBeSent is true, newStatus is the newStatus of the buffer
    /// that is retrieved
    /// @return Next buffer to send to the codec
    IMPORT_C VFM_DDepInternalBuffer *getNextToSend(OMX_BOOL willBeSent, OMX_S32 newStatus);

    /// @brief Get a free internal buffers. It may induce a new allocation of buffer
    /// @param[in] Force to have a free buffer returned, even by reallocating
    /// @return A free internal buffer
    VFM_DDepInternalBuffer *getFree(OMX_BOOL force);

    /// @brief Get the status of a current that is complete, so ready to be sent to the codec
    /// @return The new status of the current buffer
    int getNextStatus(void);

    /// @brief Allocate "nb" internal buffers, of size "size", with cache properties.
    /// this may induce some reallocation of  buffers
    /// @param[in] pSet the set of buffers, as given to the VFM Memory. It is used
    /// to remove the old internal buffers correspondance logical - physical
    /// @param[in] nb The number of internal buffers to allocate
    /// @param[in] size The size of each internal buffer
    /// @param[in] isCached OMX_TRUE if the buffers should be cached, OMX_FALSE otherwise
    /// @return OMX_ErrorNone when no errors
    IMPORT_C OMX_ERRORTYPE alloc(VFM_SetDDep *pSet, t_uint32 nb, t_uint32 size, OMX_BOOL isCached);

    /// @brief Add a new buffer in the already allocated ones.
    /// @return The new buffer
    VFM_DDepInternalBuffer *addOneBuffer();

    /// @brief when an internal buffer comes back because of disable, it is inserted in the 'front',
    /// that is it will be the next to be sent to the codec.
    /// @param[in] pInternal the internal buffer that comes back
    void insertFront(VFM_DDepInternalBuffer *pInternal);

    /// @brief Set all internal buffers as free ones
    void setAllFree();

public:
    /// @brief Maximum number of buffers in this set
    static const t_uint8 mNbInternalBuffer=30;

    /// @brief Array of internal buffers in this set
    VFM_DDepInternalBuffer mInternalBuffer[mNbInternalBuffer];
private:
    /// @brief Number of used internal buffers in this set
    t_uint8 mNb;
public:
    OMX_PTR ptrOpenMaxHandle;
};


/// @ingroup VFMDDepUtility
/// @brief An item in a set. This set is typically used to store a couple
/// (mSentToCodec, mBufferHeader) corresponding to the data sent to the buffer
/// (typically a nal in input and the buffer in output in the h264dec case).
typedef struct VFM_SetDDepItem {
    /// @brief Data that is sent to the codec, typically a nal
    OMX_PTR mSentToCodec;
    /// @brief The associated buffer header
    OMX_BUFFERHEADERTYPE *mBufferHeader;
    /// @brief Next item in the list
    struct VFM_SetDDepItem *mNext;
    /// @brief Signature of the buffer. Only relevant for input buffer
    OMX_U32 mSignature;
} VFM_SetDDepItem;


/// @ingroup VFMDDepUtility
/// @brief Define a set of functionalities to measure performances and starvation
/// of the ddep
class VFM_HostPerformances
{
public:
    /// @brief Constructor
    IMPORT_C VFM_HostPerformances(OMX_U32 nDisplayWindows);

    /// @brief Reset the performance structure
    IMPORT_C void reset(OMX_U32 nDisplayWindows);

    /// @brief Add a starvation state
    /// @param[in] in0 Number of buffers in the input port (including ddep and codec)
    /// @param[in] total0 Number of different buffers seen in input port
    /// @param[in] in1 Number of buffers in the output port (including ddep and codec)
    /// @param[in] total1 Number of different buffers seen in output port
    /// @param[in] new_frame OMX_TRUE if a new frame is output
    IMPORT_C void add(OMX_U32 in0, OMX_U32 total0, OMX_U32 in1, OMX_U32 total1, OMX_BOOL new_frame);

    /// @brief Display the performances every 100 output frames
    IMPORT_C void display(void);

    /// @brief Return time in micro-seconds
    /// @return Time in micro-seconds
    static OMX_U32 getTime(void);

private:
    /// @brief Return a fps (frame per second)
    /// @param[in] time1 Start time
    /// @param[in] time2 End time
    /// @param[in] nFrames Number of frames
    /// @return Number of fps
    static double getFPS(OMX_U32 time1, OMX_U32 time2, OMX_U32 nFrames);

private:
    /// @brief Number of buffers in the DDep, that is in the Codec Component
    OMX_U32 mNbInDDep[2];

    /// @brief Number of buffers out of the DDep, that is in the Tunneled Component
    OMX_U32 mNbTotal[2];

    /// @brief Number of "add" that have been performed
    OMX_U32 mNbAdd;

    /// @brief Number of output frames
    OMX_U32 mNbFrames;

    /// @brief Init time
    OMX_U32 mInitTime;

    /// @brief Last time of the display window
    OMX_U32 mLastTime;

    /// @brief Display Window
    OMX_U32 mDisplayWindows;
};


/// @ingroup VFMDDepUtility
/// @brief Define a Set of item of type VFM_SetDDepItem. It is typically used
/// to store the buffers that have been dequeued (that is sent to the codec)
/// but not release
class VFM_SetDDep
{
public:
    /// @brief Constructor
    /// @param [in] size the size of the data mSentToCodec that are stored. In case size=0,
    /// mSentToCodec of a new item is not allocated. Otherwise, when a new item is added to
    /// the list, the allocation is performed in this class.
    /// @param[in] isInputPort 1 if this port is an input port
    IMPORT_C VFM_SetDDep(int size, int isInputPort);

    /// @brief Destructor. It deletes allocated memories, that are both lists and
    /// potentially every mSentToCodec in the list.
    IMPORT_C ~VFM_SetDDep();

    /// @brief Free the allocated memory. This is called by the destructor
    void free();

    /// @brief Add a new item in the list of data that are sent to the codec.
    /// It firsly searches in the list of item not in the codec. If it exists,
    /// this item is put in the list of item in the codec. If not, a new item
    /// is created, with an allocated mSentToCodec if mSizeItem!=0 and pSentToCodec==0
    /// @param [in] pBufferHeader the buffer header associated with the data we want
    /// to pass to the codec
    /// @param [in] pSentToCodec the data to be sent to the codec in case these data should
    /// not be allocated by this class
    /// @return The data to be sent to the codec
    IMPORT_C OMX_PTR add(OMX_BUFFERHEADERTYPE *pBufferHeader, OMX_PTR pSentToCodec=0);

    /// @brief Get the buffer header associated to data returned by the codec.
    /// The associated item is then part of the mNotInCodec list
    /// @param [in] pSentToCodec the data released by the codec
    /// @return the buffer header associated with pSentToCodec
    IMPORT_C OMX_BUFFERHEADERTYPE *getBufferHeader(OMX_PTR pSentToCodec);

    /// @brief Flush one buffer of the list mInCodec
    /// @return The first buffer header of the list
    IMPORT_C OMX_PTR FlushOneBuffer();

    /// @brief Set the VFM memory context, used to add external memory sources
    /// @param [in] pCtxtMemory the memory context
    inline void setMemoryContext(void *pCtxtMemory) { mCtxtMemory = pCtxtMemory; }

    /// @brief Return the number of buffers sent to the codec
    /// @return the number of buffers sent to the codec
    inline int getCountInCodec() const { return getCount(mInCodec); }

    /// @brief Return the number of buffers not sent to the codec
    /// @return the number of buffers not sent to the codec
    inline int getCountNotInCodec() const { return getCount(mNotInCodec); }

    /// @brief Remoev a pHeader from the mNotInCodec list
    /// @param[in] pHeader the buffer header we want to remove
    void remove(OMX_BUFFERHEADERTYPE *pHeader);

    /// @brief Check or not signatures
    /// @param[in] s equals OMX_TRUE if we want to check signatures on the input ports
    inline void setCheckSignature(OMX_BOOL s) { mCheckSignature=s; };

    /// @brief Set the boolean value mHeaderDataChanging
    inline void setHeaderDataChanging(OMX_BOOL headerDataChanging)  { mHeaderDataChanging = headerDataChanging; }

    void setVFMParam(void **ptr) {ptrParamConfig = *ptr;}

private:
    /// @brief Add VFM Memory (logical - physical address translation) related with
    /// the buffer
    /// @param [in] pBuffer the buffer header. We add the memory of pBuffer->pBuffer
    void AddMemoryExternalSource(OMX_BUFFERHEADERTYPE *pBuffer);

    /// @brief Transfer one item from the list mNotInCodec to the list mInCodec
    /// @param[in] pItem the item to transfer
    /// @param[in] pPrev the previous item (null if pItem is the 1st item of the list)
    void TransferNotInToIn(VFM_SetDDepItem *pItem, VFM_SetDDepItem *pPrev);

    /// @brief Transfer one item from the list mInCodec to the list mNotInCodec
    /// @param[in] pItem the item to transfer
    /// @param[in] pPrev the previous item (null if pItem is the 1st item of the list)
    void TransferInToNotIn(VFM_SetDDepItem *pItem, VFM_SetDDepItem *pPrev);

    /// @brief Signature computation of a buffer
    static OMX_U32 ComputeSignature(OMX_PTR pt, OMX_U32 size);

private:
    /// @brief Return the size of a given list
    /// @param [in] list the list we want to count
    /// @return the size of a given list
    IMPORT_C int getCount(VFM_SetDDepItem *list) const;

    /// @brief Size of the data to be sent to the codec. 0 if the allocation is not
    /// performed in this class
    int mSizeItem;

    /// @brief 1 if it is an input port, 0 if output port
    const int mIsInputPort;

    /// @brief List of data sent to the codec
    VFM_SetDDepItem *mInCodec;

    /// @brief List of data released by the codec
    VFM_SetDDepItem *mNotInCodec;

    /// @brief VFM_Memory context
    void *mCtxtMemory;

    /// @brief Check signatures or not on input ports
    OMX_BOOL mCheckSignature;

    /// @brief Check signatures or not on input ports
    OMX_BOOL mHeaderDataChanging;

    /* +Change for CR 399938 */
    /// @brief This field contains pointer to ParanAndConfig class object
    void *ptrParamConfig;
    /* -Change for CR 399938 */

};

/// @brief Return OMX_TRUE if the expression is true
/// @hideinitializer
#define _VFM_CHECKDIFF_(_diff) { if ((_diff)) return OMX_TRUE; }


/// @ingroup VFMDDepUtility
/// @brief Check whether the common part of 2 frameinfo are the different or not. This is used by proxies to know
/// when to send a configuration has to be sent to the ddep, and by the ddep to know when
/// an auto-detected configuration has to be sent to the proxy
/// @param[in] pFrameInfo1 the first frameinfo to compare
/// @param[in] pFrameInfo2 the second frameinfo to compare
/// @return OMX_TRUE if the frameinfo are different
extern "C" IMPORT_C OMX_BOOL VFM_frameInfoDifferent(const t_common_frameinfo *pFrameInfo1, const t_common_frameinfo *pFrameInfo2);


/// @ingroup VFMDDepUtility
/// @brief Command that is to run
class VFM_DDepCommand {
public:
    /// @brief Constructor
    IMPORT_C VFM_DDepCommand();

    /// @brief Check whether a command is "valid" or not, that is the command type is not 0xdeadbeef
    /// @return OMX_TRUE if the command corresponds to a real command
    IMPORT_C OMX_BOOL isValid() const;

    /// @brief Set a new command with its parameter
    /// @param[in] cmd The command
    /// @param[in] param The parameter of the command
    inline void set(OMX_COMMANDTYPE cmd, t_uword param) { mCommand=cmd; mParam=param; }

    /// @brief Reset a command, so that it is no more valid
    IMPORT_C void reset();

    /// @brief Get the command of a command
    /// @return The command
    inline OMX_COMMANDTYPE getCommand() const { return mCommand; }

    /// @brief Get the parameters of a command
    /// @return The parameters
    inline t_uword getParam() const { return mParam; }


private:
    /// @brief the command
    OMX_COMMANDTYPE mCommand;

    /// @brief the parameter of the command
    t_uword mParam;
};

/// @ingroup VFMDDepUtility
/// @brief This class is intended to be inherited by a specific codec. It provides all the facilities
/// that are available in the ENS-FSM on arm. It adds specific capabilities such as
/// - list of the input and the ouput buffers provided to a codec
/// - flush of buffers, included the one sent to the codec, with their specific info (for example the nal in h264dec)
/// - VFM_Memory facilities (initialization and adding external memory references)
class VFM_nmf_ddep: public Component
{
    public:
        static void vfm_assert_static(int condition, int line, OMX_BOOL isFatal, int param1=0, int param2=0);

    protected:
        /// @brief Contructor
        /// @param[in] inputBufferMode the Input Buffer Mode (fastpath or/and copy)
        /// @param[in] outputBufferMode the Input Buffer Mode (fastpath or/and copy)
        /// @param[in] nSizeInput The size of the newly allocated structure to be passed (as input) to the codec
        /// (0 if no extra allocation is required).
        /// @param[in] nSizeOutput The size of the newly allocated structure to be passed (as output) to the codec
        /// (0 if no extra allocation is required).
        /// @param[in] pPorts the array of ports
        IMPORT_C VFM_nmf_ddep(
            VFM_BufferMode inputBufferMode, VFM_BufferMode outputBufferMode, int nSizeInput, int nSizeOutput,
            Port *pPorts, OMX_BOOL manageDVFS);

        /// @brief Destructor
        IMPORT_C virtual ~VFM_nmf_ddep();

        /// @brief Interface used when the proxy send a command to the ddep.
        /// This is an overloads of the FSM facilities in order to implement
        /// - flushes when going to idle or when disabling a port
        /// - cease transmitting information when going in pause
        /// - restart translmitting data when going on idle or executing or when the
        ///    port is disabled
        /// @param[in] cmd the command that is sent
        /// @param[in] param the parameters of the command
        IMPORT_C virtual void sendCommand(OMX_COMMANDTYPE cmd, t_uword param);

        /// Check if this is possible to start a process or not (no flush for example)
        /// @return TRUE if this is possible to process new buffers
        IMPORT_C bool VFM_CheckStart();

        /// Send a "flush" command to the codec. This function is a pure virtual to be
        /// implemented in the codec
        IMPORT_C virtual void flush_command_to_codec()=0;

        /// Check whether or not a flush command is completed
        /// @param [in] nPortIndex to know if this specific port is flushing. -1 if
        ///     at least 1 port is flushing
        /// @return TRUE if the codec is currently flushing buffers
        IMPORT_C bool is_codec_flushing(int nPortIndex=-1) const;

        /// @brief Indicate the DDep that the port nPortIndex sends the acknowledge.
        /// When all the acknowledgements have been received, flushing is done
        /// When going in idle, it also ensures that there are no more NMF activity
        /// @param[in] nPortIndex the port index that sends the acknowlegement
        IMPORT_C void codec_ack_flushing(int nPortIndex);

        /// Send a "pause" command to the codec. This function should be a pure virtual to be
        /// implemented in the codec
        IMPORT_C virtual void pause_command_to_codec();

        /// @brief Indicate the DDep that codec completed the pause command
        IMPORT_C void codec_ack_pause();

        /// Initialize the VFM Memory context. this context is sent to the codec using the
        /// pure virtual function send_vfm_memory (to be implemented in the specific ddep)
        /// @param[in] full_sw TRUE if the codec is a full software one
        /// @return TRUE if no error
        IMPORT_C bool initVFMMemory(int full_sw,OMX_PTR omxHandle);

        IMPORT_C void initParamConfig(void *ptrParam);

         IMPORT_C bool isSoC1080pCapableDdep(){return VFM_SocCapabilityMgt::isSoc1080pCapable(); }

        void initOMXHandles(OMX_PTR omxHandle);

        /// @brief Send the memory context to the codec. This pure virtual function
        /// has to be implemented in the specific ddep
        /// @param[in] memory_ctxt The memory context
        IMPORT_C virtual void send_vfm_memory(void *memory_ctxt) = 0;

        /// @brief Return a buffer to the proxy, or keep it in a list if the ddep
        /// should cease transmit data (pause or specific omx extensions)
        /// @param[in] nPortIndex The index of the port of the buffer to flush
        /// @param[in] pBuffer The buffer to return
        IMPORT_C void returnBufferDDep(int nPortIndex, OMX_BUFFERHEADERTYPE *pBuffer);

        /// @brief This interface is automatically called on PortEnable, to
        /// indicate if the port is tunneled or not
        /// @param[in] portIdx The port index, or -1 for all
        /// @param[in] isTunneled is a mask of the port index, to indicate if tunneled (1) or not (0)
        IMPORT_C void setTunnelStatus(t_sint16 portIdx, t_uint16 isTunneled);

        /// @brief Dump the number of buffers in the port
        /// @param[in] string a string to dump
        /// @param[in] nPortIndex the index of the port to dump
        IMPORT_C void dump_port_status(const char *string, int nPortIndex);

        IMPORT_C void ost_port_status_flow();
        IMPORT_C void ost_port_status_debug();
        IMPORT_C void nmflog_port_status();

        /// @brief Raise an event when a new error map is detected, if any
        /// @param [in] pErrorMap the error map that is at the origin of the event raised
        IMPORT_C void raiseEventErrorMap(Common_Error_Map *pErrorMap);

        /// @brief Send an event. It should be overloaded by the proxy
        /// @param [in] eEvent the event to send
        /// @param [in] nData1 the 1st data to send, according to OMX specifications
        /// @param [in] nData2 the 2nd data to send, according to OMX specifications
        IMPORT_C virtual void sendProxyEvent(OMX_IN OMX_EVENTTYPE eEvent,  OMX_IN OMX_U32 nData1, OMX_IN OMX_U32 nData2) = 0;

        /// @brief Process an input buffer. The implementation being specific to a codec,
        /// the default implementation is an assert(0)
        /// @param[in] pBuffer the OMX buffer to process
        /// @param[in] isInternal OMX_TRUE if it is an internal copy of a buffer
        IMPORT_C virtual void process_inputbuffer(OMX_BUFFERHEADERTYPE *pBuffer, OMX_BOOL isInternal);

        /// @brief Process a special input buffer. The implementation being specific to a codec,
        /// the default implementation is an assert(0)
        /// @param[in] pBuffer the OMX buffer to process
        IMPORT_C virtual void process_special_inputbuffer(OMX_BUFFERHEADERTYPE *pBuffer);

        /// @brief Process an output buffer. The implementation being specific to a codec,
        /// the default implementation is an assert(0)
        /// @param[in] pBuffer the OMX buffer to process
        /// @param[in] isInternal OMX_TRUE if it is an internal copy of a buffer
        IMPORT_C virtual void process_outputbuffer(OMX_BUFFERHEADERTYPE *pBuffer, OMX_BOOL isInternal);

        /// @brief Process a special output buffer. The implementation being specific to a codec,
        /// the default implementation is an assert(0)
        /// @param[in] pBuffer the OMX buffer to process
        IMPORT_C virtual void process_special_outputbuffer(OMX_BUFFERHEADERTYPE *pBuffer);

        /// @brief Sends all the output buffers (for example to the DPB), taking care of fastpath
        /// and copy of internal buffers
        IMPORT_C void sendAllOutput();

        /// @brief Check if a buffer (fastpath or copy) can be processed or not
        IMPORT_C void sendOneInput();

        /// @brief Copy input buffer in internal one if possible
        /// @return OMX_FALSE if no more copies are possible
        IMPORT_C OMX_BOOL copyOneInput();

        /// @brief Copy the input buffer in the given internal one, and then calls process_inputbuffer
        /// once a complete frame has been made
        /// @param[in] pInternal the internal buffer to copy into, and then to process if possible
        IMPORT_C void copyInputAndSend(VFM_DDepInternalBuffer *pInternal);

        /// @brief Check if fast path can be activated or not onto the given buffer.
        /// If it is ok, specificActivateInputFastPath() from the specific ddep of the codec is
        /// called if any specific conditions is required.
        /// @param[in] pBuffer the buffer we would like to do the Fast Path
        /// @return OMX_TRUE if Fast Path is possible
        IMPORT_C OMX_BOOL activateInputFastPath(OMX_BUFFERHEADERTYPE *pBuffer) const;

        /// @brief Check if fast path can be activated or not onto the given buffer.
        /// If it is ok, specificActivateOutputFastPath() from the specific ddep of the codec is
        /// called if any specific conditions is required.
        /// @param[in] pBuffer the buffer we would like to do the Fast Path
        /// @return OMX_TRUE if Fast Path is possible
        IMPORT_C OMX_BOOL activateOutputFastPath(OMX_BUFFERHEADERTYPE *pBuffer);

        /// @brief Check if this buffers is a special case, specific for this codec (for example, nFilledLen=0)
        /// @param[in] pBuffer the buffer we would like to check
        /// @return OMX_TRUE if this is a special case. Default implementation is that it is not a special case
        IMPORT_C virtual OMX_BOOL isSpecialCase(OMX_BUFFERHEADERTYPE *pBuffer) const;

        /// @brief Check if specific condition could disable fastpath (on input). Default implementation
        /// is that no specific condition occurs.
        /// @param[in] pBuffer the buffer we would like to do the Fast Path
        /// @return OMX_TRUE if Fast Path is possible
        IMPORT_C OMX_BOOL specificActivateInputFastPath(OMX_BUFFERHEADERTYPE *pBuffer) const;

        /// @brief Check if specific condition could disable fastpath (on output). Default implementation
        /// is that no specific condition occurs.
        /// @param[in] pBuffer the buffer we would like to do the Fast Path
        /// @return OMX_TRUE if Fast Path is possible
        IMPORT_C OMX_BOOL specificActivateOutputFastPath(OMX_BUFFERHEADERTYPE *pBuffer) const;

        /// @brief Copy all possible buffers of a port into the given internal buffer
        /// @param[in] pInternal the internal buffer we copy to
        /// @param[in] nPortIndex the port index we consider
        /// @param[in] isAsyncReturn OMX_TRUE if buffers are return as async
        void copyInInternalBuffer(VFM_DDepInternalBuffer *pInternal, OMX_U32 nPortIndex, OMX_BOOL isAsyncReturn);

        /// @brief Set the domain id for esram allocation
        /// @param[in] domainId the domain id
        inline void setDomainId(OMX_U32 domainDdrId, OMX_U32 domainEsramId)
        { mDomainDdrId = domainDdrId; mDomainEsramId = domainEsramId; };

        /// @brief Get the domain id for ddr allocation
        /// @return the domain id
        inline OMX_U32 getDomainDdrId()  { return mDomainDdrId; };

        /// @brief Get the domain id for esram allocation
        /// @return the domain id
        inline OMX_U32 getDomainEsramId()  { return mDomainEsramId; };

        /// @brief Compute a new performance if ok==OMX_TRUE
        /// @param[in] ok boolean to indicate if performances should be computed or not
        /// @param[in] newFrame OMX_TRUE if a new frame is output or not
        inline void addPerformance(int ok, OMX_BOOL newFrame) {
            if (ok) {
                mPerformance.add(
                        mPorts[0].queuedBufferCount()+ mInputSet.getCountInCodec(),
                        mInputSet.getCountInCodec()+mInputSet.getCountNotInCodec(),
                        mPorts[1].queuedBufferCount()+ mOutputSet.getCountInCodec(),
                        mOutputSet.getCountInCodec()+mOutputSet.getCountNotInCodec(),
                        newFrame);
            }
        }

        /// @brief Display the performance, only if ok==OMX_TRUE
        /// @param[in] ok boolean to indicate if performances should be displayed or not
        inline void displayPerformance(int ok) {
            if (ok) {
                mPerformance.display();
            }
        }

        /// @brief Sets the DVFS value is required or not
        /// @param[in] req OMX_TRUE if we'd like to have full DVFS
        IMPORT_C void setDVFSValue(OMX_BOOL req);

        /// @brief Set the power of the DVFS, based on the required value
        IMPORT_C void setDVFS();

        /// @brief Set the power of the DVFS, based on the required value
        IMPORT_C void unsetDVFS();

//+ ER335583
        /// @brief Sets the DDR value is required or not
        /// @param[in] req OMX_TRUE if we'd like to have full DDR
        IMPORT_C void setDDRValue(OMX_BOOL req);

        /// @brief Set the power of the DDR, based on the required value
        IMPORT_C void setDDR();

        /// @brief Set the power of the DDR, based on the required value
        IMPORT_C void unsetDDR();
//- ER335583

        inline VFM_PowerManagement *getPower() { return &mPower; }

        /// @brief Sets mOutputBufferSwapEndianess
        IMPORT_C void setOutputBufferSwapEndianess(t_uint32 endianess_swap);

    private:
        /// @brief The specific part of the DDep of the current command, so we can call the one of
        /// fsm, and then either execute the next delayed command, or reschedule a process
        void endCurrentCommand();

        /// @brief execute the current command
        void executeCurrentCommand();

        /// @brief Return the buffer to the proxy.
        /// It takes care whether or not this is an internal buffer
        /// @param[in] nPortIndex The index of the port of the buffer to flush
        /// @param[in] pBuffer The buffer to return
        /// @param[in] feedOutput is true if internal buffers can go back to the codec immediatly
        void returnBufferToProxy(int nPortIndex, OMX_BUFFERHEADERTYPE *pBuffer, OMX_BOOL feedOutput);

        /// @brief Return the buffer because of a flush command.
        /// An internal buffer is set as coming back, output buffers that are tunneled
        /// are requeued, and other buffers are sent back to the proxy
        /// It takes care whether or not this is an internal buffer
        /// @param[in] nPortIndex The index of the port of the buffer to flush
        /// @param[in] pBuffer The buffer to return
        void returnBufferForFlush(int nPortIndex, OMX_BUFFERHEADERTYPE *pBuffer);

        /// @brief Return the buffer because of a disable command.
        /// Input buffers are saved so that they can be treated again, both
        /// for internal buffers and omx buffers
        /// All buffers (input and output) are sent back to the proxy
        /// @param[in] nPortIndex The index of the port of the buffer to flush
        /// @param[in] pBuffer The buffer to return
        void returnBufferForDisable(int nPortIndex, OMX_BUFFERHEADERTYPE *pBuffer);

        /// @brief Return all the output buffers (in case of internal output)
        /// to the proxy
        /// @param[in] feedOutput is true if internal buffers can go back to the codec immediatly
        void returnOutputBuffers(OMX_BOOL feedOutput);

        /// @brief Send all the internal output buffers to the codec
        void sendInternalOutputBuffers();

        /// @brief when a buffer comes back because of the flush, it is ready to be reinjected into
        /// the codec
        /// @param[in] nPortIndex The index of the port of the buffer to flush
        /// @param[in] pBuffer The buffer to return
        void reinjectBuffer(int nPortIndex, OMX_BUFFERHEADERTYPE *pBuffer);

    private:
        /// @brief Flush the buffers of the ports
        /// @param[in] nPortMask The port mask to be flushed
        void codec_start_flushing(int nPortMask);

        /// @brief Indicate the DDep that the current flush is complete. It may start on any delayed flushes
        void codec_end_flushing();

        // @brief Change endianness of srcBuffer in dstBuffer
        void swap_endianess(t_uint8 *pDst, t_uint8 *pSrc, t_uint32 size);

    protected:
        /// @brief Set of input buffers that have been sent to the codec
        VFM_SetDDep mInputSet;

        /// @brief Set of input internal buffers. None by default.
        VFM_DDepAllInternalBuffers mInputInternal;

        /// @brief Buffer mode of the input buffers
        const VFM_BufferMode mInputBufferMode;

        /// @brief Set of output buffers that have been sent to the codec
        VFM_SetDDep mOutputSet;

        /// @brief Set of output internal buffers. None by default.
        VFM_DDepAllInternalBuffers mOutputInternal;

        /// @brief Buffer mode of the output buffers
        VFM_BufferMode mOutputBufferMode;       // is dynamically modified to get a single mode

        /// @brief Memory context
        void *mCtxtMemory;

        /// @brief Statistics to compute
        VFM_HostPerformances mPerformance;

    private:
        /// @brief Mask the current ports that are flushed
        int mIsCodecFlushing;

        /// @brief Mask of the ports that have sent the flushing acknowlegement
        int mMaskAckFlushing;

        /// @brief Array of the port of this DDep
        Port *mPorts;

        /// @brief The current command
        VFM_DDepCommand mCurrentCommand;

        /// @brief Max number of delayed command
        static const int mMaxDelayedCommands=5;

        /// @brief Number of delayed commands
        int mNbDelayedCommand;

        /// @brief The delayed command
        VFM_DDepCommand mDelayedCommand[mMaxDelayedCommands];

//+ ER335583
        /// @brief The ddr domain id, as given by the RME, through the set_config() calls
        OMX_U32 mDomainDdrId;
//- ER335583

        /// @brief The esram domain id, as given by the RME, through the set_config() calls
        OMX_U32 mDomainEsramId;

        /// @brief This component requires Full DVFS
        OMX_BOOL mRequireFullDVFS;

        /// @brief This component has requested for its DVFS to be set
        OMX_BOOL mIsDVFSSet;

//+ ER335583
        /// @brief This component requires Full DDR
        OMX_BOOL mRequireFullDDR;

        /// @brief This component has requested for its DDR to be set
        OMX_BOOL mIsDDRSet;
//- ER335583

        /// @brief Structure for the performance management: what we want, if it is has been forced,...
        VFM_PowerManagement mPower;

        OMX_PTR ptrOMXHandle;

        /// @brief Endianess of output buffer should be swapped
        OMX_BOOL mOutputBufferSwapEndianess;

        /* +Change for CR 399938 */
        /// @brief This field contains pointer to ParanAndConfig class object
        void *ptrParamConfig;
        /* -Change for CR 399938 */
};

#endif
