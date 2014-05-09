/*****************************************************************************/
/**
*  (c) ST-Ericsson, 2010 - All rights reserved
*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson
*
* \brief   OMX IL MM Buffer - User API definition
* \author  ST-Ericsson
*/
/*****************************************************************************/

/**
 * @file    mmhwbuffer.h
 * @defgroup MMHwBufferAPI OMX IL MMHwBuffer User API definition
 * @ingroup MMHwBufferAPI
 */
/*@{*/

#ifndef _MMHWBUFFER_H_
#define _MMHWBUFFER_H_

#if ((!defined __SYMBIAN32__) && (!defined IMPORT_C))
    #define IMPORT_C
#endif
#if ((!defined __SYMBIAN32__) && (!defined EXPORT_C))
    #define EXPORT_C
#endif

#ifdef __SYMBIAN32__
#include <openmax/il/khronos/v1_x/OMX_Core.h>
#include <openmax/il/khronos/v1_x/OMX_Types.h>
#else
#include <OMX_Core.h>
#include <OMX_Types.h>
#endif

/**
 * @mainpage MMHwBuffer class
 * 
 * MMHwBuffer class provides the low-level user API for creating and manipulating the 
 * MM HW buffers (or HW memory chunks) requested by the port of the OMX IL components. 
 * MMHwBuffer object is a handle of buffer pool which is responsible for managing the 
 * creation/deletion and access permissions to the underlying HW memory chunks.
 * 
 * - MM HW buffers designates the memory chunk objects (or shared kernel chunks) which are accessibles
 *   kernel and used side.    
 * - A buffer represents only the payload part (including possible extra data) of a OMX IL buffer. MMHwBuffer focus 
 *   on memory and buffer management, rather than managing the content or chaining the
 *   buffers. Therefore they will not be responsible for storing or transfering user metadata (e.g. offset and length).
 * - The main properties of the pool: 
 *     - all buffers are mapped inside a single memory chunk (virtually contignous) in the client process. 
 *        A given buffer is guarantee physically contignous (and implicitly logically contignous).  
 *     - the same properties (size, cache attribut, access permissions,...) are applied to all buffers in a given
 *       pool.
 *     - handle is thread/client-relative (reference count is implemented) but full process Read/Write 
 *       access permissions is allowed by default.
 * - The client are identified through its OMX_HANDLETYPE reference. This allows to track/trace the 
 *   users/clients.
 *          
 * @note This API is designed/specified for the clients which run inside a "secure/trusted" OMX IL stack: Native OS code 
 *  or portable OS code (i.e. ENS layer). 
 * 
 * @note MMHwBuffer manages also internally a dependent-OS metadata part which 
 *  can be used by the upper/client layer to exchange the buffers which a legacy/native 
 *  OMX IL component or with other client processes.
 *  
 *  
 */

/**
 * MMHwBuffer class definition
 * 
 *  Handle or/and entry point to manage a pool of MM Hw Buffers
 */

class MMHwBuffer
{
 public:
	/**
	 * Cache attribute definition 
	 */
	enum TCacheAttribute
		{
		/** 
		 * Fully cached memory type - Inner/outer cache level (L1/L2 cache) 
		 */
		ENormalCached = 1,
		/** 
		 * Uncached memory type
		 */
		ENormalUnCached = 2,
		/**
		 * Strongly ordered memory type
		 */
		EFullyBlocking = 3
		};

	/**
	 * Synchronization cache operation definition 
	 */
	enum TSyncCacheOperation
		{
		/** 
		 * Synchronize before a read HW operation.  
		 * If the buffer is cached and has been written by the CPU(ARM Core), this 
		 * type of synchronisation should be used before a HW IP/peripheral process
		 * which read the data from the buffer's memory.
		 * Cache clean operation is performed. Dirty lines are written to main
		 * memory (L3). 
		 * 
		 * @note Must be called before a call of emptythisbuffer sequence.
		 */
		ESyncBeforeReadHwOperation = 1,
		/** 
		 * Synchronize before a write HW operation.  
		 * If the buffer is cached and has been written by the CPU(ARM Core), this 
		 * type of synchronisation should be used before a HW IP/peripheral process
		 * which write the data to the buffer's memory. This guarantee that no dirty
		 * lines are evicted during the HW process. 
		 * Cache invalidate operation is performed. All lines (including dirty lines)
		 * are invalidated. 
		 * 
		 * @note Must be called before a call of fillthisbuffer sequence
		 */
		ESyncBeforeWriteHwOperation = 2,
		/** 
		 * Synchronize after a write HW operation.
		 * If the buffer is cached, this type of synchronisation should be used 
		 * after a HW IP/peripheral process which has wrote the dato 
		 * to the buffer's memory.
		 * Cache invalidate operation is performed. All lines (including dirty lines)
		 * are invalidated.
		 * 
		 * @note Can be called after a fillbufferdone notification. If before the call of
		 *   fillthisbuffer, CacheSync() has been called with @ref ESyncBeforeWriteHwOperation, 
		 *   this operation is normally not necessary.
		 */
		ESyncAfterWriteHwOperation = 3
		};

	/**
	 * Memory device type definition 
	 */
	enum TMemoryDevice
		{
		/** 
		 * SDRAM memory - Designate the system OS memory pool 
		 */
		ESystemMemory, 
		/** 
		 * MPC SDRAM memory - Designate the reserved MPC SDRAM memory chunk (this resource is limited). 
		 * It is mapped on @ref ESystemMpcMemory16. Not Yet Supported.
		 */
		ESystemMpcMemory,   
		/** 
		 * MPC SDRAM memory - 16b. Not Yet Supported. 
		 */
		ESystemMpcMemory16, 
		/** 
		 * MPC SDRAM memory - 24b (mapped on 32b). Not Yet Supported. 
		 */
		ESystemMpcMemory24, 
		
		/**
		 *  MPC ESRAM memory - Designate the reserved MPC ESRAM memory chunk (this resource is limited). 
		 *  Is it mapped on @ref ELocalMpcMemory16. Not Yet Supported.
		 */
		ELocalMpcMemory, 
		/**
		 *  MPC ESRAM memory - 16b. Not Yet Supported.
		 */
		ELocalMpcMemory16, 
		/**
		 *  MPC ESRAM memory - 24b (mapped on 32b). Not Yet Supported. 
		 */
		ELocalMpcMemory24
		};

	/**
	 * Class which allows to describe the buffer pool properties 
	 */
	class TBufferPoolCreationAttributes
		{
	public:
		/** 
		 * Number of buffers in the pool. 
		 */
		OMX_U32 iBuffers;
		/** 
		 * Memory type 
		 */
		TMemoryDevice iDeviceType;
		/** 
		 * MPC Domain ID (only requested for MPC memory type)
		 */
		OMX_U32		  iDomainID;
		/** 
		 * Size (in byte) of a buffer 
		 */
		OMX_U32		  iSize;
		/** 
		 * Alignment applied to the physical base address of each buffer in the pool:
		 *    1,2,4,8,16,32,64 byte or -1 for MMU pageword size (default value - no constraint) 
		 *    iAlignment must a power of 2. 
		 *    iAlignment > MMU pageword size (4KBytes) is also supported. 
		 *    Only the physical address is considered.
		 */
		OMX_S32  iAlignment; 
		/** 
		 * Caching attribute to create the mapped chunk memory. Not applicable for the MPC memory
		 *  type.
		 */
		TCacheAttribute iCacheAttr;
		/**
		 * Indicates the boundary address which must not be crossed by the allocated physical memory chunk.
		 *    0x1000, 0x200000, 0x1000000...  or 0 (default value - no constraint)
		 *    address must be a power of 2. iSize must be inferior or equal to iNoCrossAddressBoundary.
		 *    Only the physical address is considered.
		 */
		OMX_U32  iNoCrossAddressBoundary;

		TBufferPoolCreationAttributes();
		};

	/**
	 * Class Buffer descriptor 
	 */
	class TBufferInfo
		{
	public:
		OMX_U32         iLogAddr;
		OMX_U32         iPhyAddr;
		OMX_U32         iAllocatedSize;
		TCacheAttribute iCacheAttr;
		};

	
	/**
	 * Create a memory pool and returns a handle A.
	 * 
	 * @param[in]  aBuffPoolAttr Input parameter which specifies attributes of the requested pool.
	 *             (see @ref TBufferPoolCreationAttributes)
	 * @param[in]  aComponent    OMX IL client handle
	 * @param[out] apMMHwBufferHandle Valid handle if it succeeds else NULL pointer
	 * 
	 * @return OMX_ErrorNone if it succeeds
	 *     OMX_ErrorInsufficientResources if the pool/handle cannot be created due to out of memory.
	 *     OMX_ErrorBadParameter if the pool attributes are incorrect.
	 *     
	 * @note This function is used by the allocator part of the OMX IL port to reserve the buffers. 
	 *       apMMHwBufferHandle parameter must be set to NULL before to call the function.   
	 */
	IMPORT_C static OMX_ERRORTYPE Create(const TBufferPoolCreationAttributes& aBuffPoolAttrs,
			OMX_HANDLETYPE aComponent,
			MMHwBuffer*&   apMMHwBufferHandle);

	/**
	 * Destroy a memory pool
	 * 
	 * @param[in] apMMHwBufferHandle Valid handle to destroy. Is returned as 0 if no error
	 *
	 * @return OMX_ErrorNone if it succeeds
	 *     
	 * @note This function is used by the allocator part of the OMX IL port to destroy the buffers. 
	 */
	IMPORT_C static OMX_ERRORTYPE Destroy(MMHwBuffer*& apMMHwBufferHandle);

	/**
	 * Open and create a handle according the dependent-OS metadata. If the metadata are 
	 * associated to a managed pool (created by a MMHwBuffer layer), the reference count is incremented.
	 * The client must call the AddBufferInfo method to complete the informations (see @ref AddBufferInfo).
	 * 
	 * @param[in]  aComponent    OMX IL client handle
	 * @param[in]  aIndex        OMX Index type
	 * @param[in]  apConf        MetaData (OS specific) to retrieve the relevant informations 
	 * @param[out] apMMHwBufferHandle Valid handle if it succeeds else NULL pointer
	 * 
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorInsufficientResources if the handle 
	 *     cannot be created due to out of memory, OMX_ErrorBadParameter if the MetaData
	 *     are incorrect.
	 *   
	 * @note This function should be used by the allocator part of the OMX IL port to open a handle
	 *       on the buffers (provided by the tunneled component - port provider/allocator). 
	 *       apMMHwBufferHandle parameter must be set to NULL before to call the function.   
	 *               
	 */
	IMPORT_C static OMX_ERRORTYPE Open(OMX_HANDLETYPE aComponent, 
			OMX_INDEXTYPE aIndex, 
			OMX_PTR apConf,
			MMHwBuffer*& apMMHwBufferHandle);

	/**
	 * Close a memory pool
	 * 
	 * @param[in] apMMHwBufferHandle Valid handle to close. Is returned as 0 if no error.
	 *
	 * @return OMX_ErrorNone if it succeeds
	 *     
	 * @note This function is used by the allocator part of the OMX IL port to destroy the buffers. 
	 */
	IMPORT_C static OMX_ERRORTYPE Close(MMHwBuffer*& apMMHwBufferHandle);

	/**
	 * Complete the buffer informations
	 * 
	 * @param[in]  aBufferIndex Index of the buffer [0..N_1]
	 * @param[in]  aLogAddr     Logical address of the buffer (user process space)
	 * @param[in]  aSize        Size of the buffer
	 * 
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorBadParameter if the aLogAddr/aSize are invalide.
	 * 
	 * @note Check that the associated address range is physically contignous and including in the pool. 
	 *       The offset between two buffers must be the same.
	 */
	virtual OMX_ERRORTYPE AddBufferInfo(OMX_U32 aBufferIndex, OMX_U32 aLogAddr, OMX_U32  aSize) = 0;

	
	/**
	 * Return the numeric extension index for the dependent-OS metadata informations.
	 * This index must be used by the IL client or tunneled component to call OMX_GetConfig()/OMX_SetConfig() 
	 * methods respectively to retrieve or to provide the dependent-OS metadata informations.
	 * 
	 * @param[in]  aParameterName The string that the component will translate into a 32-bit index.
	 * @param[out] apIndexType    A pointer to OMX_INDEXTYPE that receives the index value. 
	 * 
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorNotImplemented if not supported.
	 * 
	 * @note This method is typically used when the client/caller unknows the index descriptor.
	 */
	IMPORT_C static OMX_ERRORTYPE GetIndexExtension(OMX_STRING aParameterName, OMX_INDEXTYPE* apIndexType);
		
	/**
	 * Return the port index. aIndex and configuration structure must be valid.
	 * 
	 * @param[in]  aIndex    The index of the configuration structure. This value is from the OMX_INDEXTYPE enumeration. 
	 * @param[out] apConf    A pointer to the IL client-allocated structure. MetaData (OS specific) to retrieve 
	 *                       the relevant informations. 
	 * 
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorBadParameter parameter or configuration structure is not valid.
	 */
	IMPORT_C static OMX_ERRORTYPE GetPortIndexExtension(OMX_INDEXTYPE aIndex, OMX_PTR apConf, OMX_U32& aPortIndex);

	/**
	 * Provide to the tunneled component the dependent-OS metadata informations.
	 * Call OMX_GetExtensionIndex() and OMX_SetConfig() function.
	 * 
	 * @param[in] aTunneledComponent Handle of the tunneled component
	 * @param[in] aPortIndex         Port index of the tunneled component
	 * 
	 * @return  OMX_ErrorNone if it succeeds, OMX_ErrorBadParameter if invalid parameters,
	 *          and OMX_ErrorNotImplemented if the SetConfig() is not understood by the tunneled component
	 */
	virtual OMX_ERRORTYPE SetConfigExtension(OMX_HANDLETYPE aTunneledComponent, OMX_U32 aPortIndex) = 0;
	
	/**
	 * Complete the configuration structure - MetaData (OS specific). 
	 * aComponent handle, aIndex and configuration structure must be vaild.
	 * 
	 * @param[in]     aIndex       The index of the configuration structure. This value is from the OMX_INDEXTYPE enumeration. 
	 * @param[inout]  apConf       A pointer to the IL client-allocated structure. MetaData (OS specific) to retrieve 
	 *                             the relevant informations.
	 * @param[in]     aPortIndex   Associated Port index. The port index is a attribute of the configuration structure.
	 * 
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorBadParameter parameter or configuration structure is not valid.
	 * 
	 */
	virtual OMX_ERRORTYPE GetConfigExtension(OMX_INDEXTYPE aIndex, OMX_PTR apConf, OMX_U32 aPortIndex) const = 0;

	
	/**
	 * Return all informations of buffer.
	 * 
	 * @param  aBufferIndex Index of the buffer (0..N-1).
	 * @param  aInfo Buffer information (@see TBufferInfo).
	 * 
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorBadParameter if the buffer index is invalide.
	 */
	virtual OMX_ERRORTYPE BufferInfo(OMX_U32 aBufferIndex, TBufferInfo& aInfo) = 0;
	
	/**
	 * Synchronises cache(s) prior to HW operations. The cache operation is performed according 
	 *  the requested operation and the associated memory cache attributs.
	 * 
	 * @param  aOp      Requested Cache operation (@see TSyncCacheOperation).
	 * @param  aLogAddr Absolute logical address (not offset value) which indicates the
	 *                  the starting address of the buffer. It must be a valid address inside the address range
	 *                  of the pool.
	 * @param  aSize    Memory chunk size in bytes. [aLogAddr,aLogAddr+aSize] must be a valid address range
	 *                  inside the 
	 * @param  aPhyAddr Return the associated physical address                 
	 *    
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorBadParameter if invalide parameters (invalid range address).       
	 */
	virtual OMX_ERRORTYPE CacheSync(TSyncCacheOperation aOp,
			OMX_U32  aLogAddr,
			OMX_U32  aSize,
			OMX_U32& aPhysAddr) = 0;

	/**
	 * Synchronises cache(s) prior to HW operations. The cache operation is performed according 
	 *  the requested operation and the associated memory cache attributs.
	 * 
	 * @param  aOp Requested Cache operation (see @ref TSyncCacheOperation).
	 * @param  aBufferIndex Index of the buffer (0..N-1).
	 * 
	 * @param  aPhyAddr Return the associated physical address                 
	 *    
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorBadParameter if invalide parameters.       
	 */
	virtual OMX_ERRORTYPE CacheSync(TSyncCacheOperation aOp,
			OMX_U32  aBufferIndex,
			OMX_U32& aPhysAddr) = 0;
	
	/**
	 * Service function which returns the physical address.
	 * The logical address must be in the range of a buffer in the pool.
	 * 
	 * @param  apLogAddr Logical address
	 * @param  aSize     buffer size
	 * @param  aPhyAddr  Physical address
	 * 
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorBadParameter if [aPhyAddr,aPhysAddr+aSize] 
	 *   is not physically contignous or if [apLogAddr,apLogAddr+aSize]  is not a valid range.    
	 */
	virtual OMX_ERRORTYPE PhysAddress(OMX_U32  aLogAddr,
					  OMX_U32  aSize,
					  OMX_U32& aPhyAddr) = 0;
	
	/**
	 * Service function which returns the logical address.
	 * The physical address must be in the range of a buffer in the pool.
	 * 
	 * @param  aPhyAddr  Physical address
	 * @param  apLogAddr Logical address
	 * @param  aSize     buffer size
	 * 
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorBadParameter if [aPhyAddr,aPhysAddr+aSize] 
	 *   is not physically contignous or if [apLogAddr,apLogAddr+aSize]  is not a valid range.    
	 */
	virtual OMX_ERRORTYPE LogAddress(OMX_U32  aPhyAddr,
					 OMX_U32  aSize,
					 OMX_U32& aLogAddr) = 0;
	
#ifndef __SYMBIAN32__

	/**
	 * Service function which returns the total amount of memory created using Create with
	 * aComponent as associated handle.
	 * 
	 * @param[in]  aComponent     OMX IL client handle
	 * @param[out] aAllocatedSize Total ammount of size in bytes
	 * 
	 * @return OMX_ErrorNone if it succeeds, OMX_ErrorBadParameter if aComponent is not a 
         *         known handle.    
	 */	
	IMPORT_C static OMX_ERRORTYPE GetAllocatedSize(OMX_HANDLETYPE aComponent,
                                                       OMX_U32& aAllocatedSize);
#endif
	
	/**
	 * Return the version of the MMHwBuffer API
	 */
	IMPORT_C static OMX_VERSIONTYPE Version(void);
	
	virtual ~MMHwBuffer() { }
};


inline MMHwBuffer::TBufferPoolCreationAttributes::TBufferPoolCreationAttributes() :
 iBuffers(0),
 iDeviceType(ESystemMemory),
 iDomainID(0),
 iSize(0),
 iAlignment(-1),
 iCacheAttr(ENormalUnCached),
 iNoCrossAddressBoundary(0)
{ }

/*@}*/

#endif /* _MMHWBUFFER_H_ */

