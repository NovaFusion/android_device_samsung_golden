/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef __VFM_MEMORY_HPP
#define __VFM_MEMORY_HPP

// #define _CACHE_OPT_

#if ((!defined __SYMBIAN32__) && (!defined IMPORT_C))
    #define IMPORT_C
#endif
#if ((!defined __SYMBIAN32__) && (!defined EXPORT_C))
    #define EXPORT_C
#endif

/// @defgroup VFMMemoryAPI VFM Memory Support API
/// @ingroup VFM
/// @brief This is an API for Memory support, to have homogeneous memory allocation.
/// It allows to get the physical address given a logical adress, and vice-versa, and facilitates debug
/// as checking of freeing a wrong memory, and memory leaks, are tracked.
/// Allocation methods are given in the enum VFM_Mem_Type
///
/// This API is aimed at solving the translation from logical to physical addresses.
/// In encoders / decoders, some processing is performed at ARM level (that is use logical
/// addresses) and/or at DSP level (that uses physical addresses). This translation can
/// be performed by specific methods, depending on the allocation method that has been used.
/// Unfortunately, there is no method that return, given a logical address, the corresponding
/// physical address. That means that, without support, any pointer should also be passed along
/// a structure that enables to make the translation.
///
/// The VFM Memory API allows to make the translation logical to physical (and vice versa) directly.
/// For this, a table of correspondances is build.
/// - This table is stored as a list that is initialized
///   using VFM_CreateMemoryList(). There is no limitation in the number of list, but usal case is
///   to use one list per codec instance. Typically, the list is initialized at the
///   instantiation of the NMF network
/// - Allocation is performed using VFM_Alloc() that allows to give the allocation method to use
///   (as a VFM_Mem_Type), its size, aligned and properties
/// - In case the memory is allocated by other means (typically the OMX buffers that are not allocated
///   using the VFM Memory), then it is possible to store the correspondance using VFM_AddMemoryExternalSource()
/// - Address translation is performed using VFM_GetPhysical() and VFM_GetLogical()
/// - VFM_Free() allows the deallocation of a memory allocated through VFM_Alloc()
/// - VFM_CloseMemoryList() closes the list, and is able to return debug information such as memory leaks
///
/// @note All these methods has the context (the memory list) as first argument. This is so
/// important that encoders / decoders transmit this information from one call to another one,
/// or store this context into a NMF member.
///
/// @note The API hides how the implementation is performed. This is not useful for the user
/// to understand what is behind a context. Interested people may refer to VFMMemory

/// @ingroup VFMMemoryAPI
/// @brief Describe the kind of the allocation that is performed. This is used when allocating the
/// memory when using VFM_MemoryAlloc()
typedef enum VFM_Mem_Type {
    /// @brief No memory is allocated yet. This should never been used when allocating
    VFM_MEM_NONE=0,

    /// @brief "new" (from C++) is used. This is typically used for memory that is only
    /// used at ARM level
    VFM_MEM_NEW,

    /// @brief CM_MPCAlloc for memory that is accessible directly by the DSP.
    /// The memory is in ESRAM
    /// This memory type should be used as few as possible
    VFM_MEM_CM_MPC_ESRAM16_ALLOC,

    /// @brief CM_MPCAlloc for memory that is accessible directly by the DSP.
    /// The memory is in DDR.
    VFM_MEM_CM_MPC_SDRAM16_ALLOC,

    /// @brief Memory has been allocated externally, by an unknown means, and
    /// the correspondance logical <-> physical address is given (cf VFM_AddMemoryExternalSource())
    VFM_MEM_EXTERNALSOURCE,

    /// @brief HW Buffers are used for the allocation. Allocated buffers are not visible from the DSP
    VFM_MEM_HWBUFFER_NONDSP,


    /// @brief HW Buffers are used for the allocation. Allocated buffers are not visible from the DSP
    VFM_MEM_CACHED_HWBUFFER_NONDSP,

	/// @brief Last memory index. This is the number of memory type
    VFM_MEM_LAST
} VFM_Mem_Type;



#if !defined(TYPES_H) && !defined(NMF_TYPEDEF_H_)
typedef unsigned char t_uint8;
typedef   signed char t_int8;
typedef unsigned long  t_uint32;
typedef   signed long  t_int32;
#endif

#include "VFM_Memory.idt"

#ifdef __cplusplus
extern "C" {
#endif


/// @ingroup VFMMemoryAPI
/// @brief Create a memory list, i.e. a context, to make the translation physical <-> logical possible
/// @param [in] domainDdrId the domain Id of the allocation, as given by the RME, for Ddr
/// @param [in] domainEsramId the domain Id of the allocation, as given by the RME, for Esram
/// @param [in] fullsw indicates if the application is a full software application or not.
///    If so, the translation logical <-> physical has no effect
/// @return a context, i.e. a list of memory. This context is  required argument for all the VFM Memory function
IMPORT_C void *VFM_CreateMemoryList(t_uint32 domainDdrId, t_uint32 domainEsramId, t_uint8 fullsw);

/// @ingroup VFMMemoryAPI
/// @brief To update the openmax handle for memory context
/// @param [in] ctxt the context of the allocation. The context is updated with a new memory member
/// @param[in] pOMXHandle the handle of the omx component
IMPORT_C void VFM_MemoryInit(void *ctxt,void *pOMXHandle);

IMPORT_C void VFM_InitParamConfig(void *ctxt,void *ptrParam);


/// @ingroup VFMMemoryAPI
/// @brief Allocates a memory in a given context, using the given allocation method
/// @param [in] ctxt the context of the allocation. The context is updated with a new memory member
/// @param [in] size the size of the allocation memory
/// @param [in] type the type of the allocation, as given in the enum VFM_Mem_Type
/// @param [in] align refers to alignement property. This is passed directly to the corresponding method, so the user may
///    refer to guidelines of the allocation that is used. Note that memory of type VFM_MEM_NEW only allows
///    0 for alignement
/// @param [in] prop is the memory property (cachable,...). This is passed directly to the corresponding method, so the user may
///    refer to guidelines of the allocation that is used. Note that memory of type VFM_MEM_NEW only allows
///    0 for properties
/// @param [in] line is the line of the source code where the allocation has been performed (used in debug)
/// @param [in] file is the file name where the allocation is performed. Typically, we are calling
///    VFM_Alloc(ctxt, ..., __LINE__, __FILE__)
IMPORT_C t_uint8 *VFM_Alloc(void *ctxt, t_uint32 size, VFM_Mem_Type type, t_uint32 align, t_uint32 prop, t_uint32 line, t_uint8 *file);

/// @ingroup VFMMemoryAPI
/// @brief Add a correspondance between a logical address and a physical address that have been allocated
/// outside the scope of the VFM_Memory
/// @param [in] ctxt the context of the allocation. The context is updated with a new memory member
/// @param [in] LogicalAddress, PhysicalAddress the logical address and the corresponding physical address
/// @param [in] size the size of this allocation
/// @param [in] pMetaData The metadata associated with a HWBuffer, null if not from an HWBuffer
IMPORT_C void VFM_AddMemoryExternalSource(void *ctxt, t_uint8 *LogicalAddress, t_uint8 *PhysicalAddress, t_uint32 size, void *pMetaData);

/// @ingroup VFMMemoryAPI
/// @brief Return the physical address given the logical ones
/// @param [in] ctxt the context of the allocation
/// @param [in] LogicalAddress the logical address
/// @return the corresponding physical address
IMPORT_C t_uint8 *VFM_GetPhysical(void *ctxt, t_uint8 *LogicalAddress);

/// @ingroup VFMMemoryAPI
/// @brief Return the logical address given the physical ones
/// @param [in] ctxt the context of the allocation
/// @param [in] PhysicalAddress the physical address
/// @return the corresponding logical address
IMPORT_C t_uint8 *VFM_GetLogical(void *ctxt, t_uint8 *PhysicalAddress);

/// @ingroup VFMMemoryAPI
/// @brief Return the mpc address given the logical ones
/// @param [in] ctxt the context of the allocation
/// @param [in] LogicalAddress the logical address
/// @return the corresponding physical address
IMPORT_C t_uint8 *VFM_GetMpc(void *ctxt, t_uint8 *LogicalAddress);

/// @ingroup VFMMemoryAPI
/// @brief Free a memory from the VFM Memory. This free the memory using the dedicated
/// free method (not application to memory that have been added using VFM_AddMemoryExternalSource())
/// and removes the address from the context
/// @param [in] ctxt the context of the allocation. The context is updated with a new memory member
/// @param [in] pt the pointer to free
IMPORT_C void VFM_Free(void *ctxt, t_uint8 *pt);

/// @ingroup VFMMemoryAPI
/// @brief Free a context. Check are perform for memory leaks in debug mode
/// @param [in] ctxt the context to free
/// @note All the memory should have free using VFM_Free() before this call.
IMPORT_C t_uint32 VFM_CloseMemoryList(void *ctxt);


/// @ingroup VFMMemoryAPI
/// @brief Get the status of a memory context
/// @param [in] ctxt the memory context
/// @param [out] pMemoryStatus the returned meomry status
/// @note All the memory should have free using VFM_Free() before this call.
IMPORT_C void VFM_GetMemoryListStatus(void *ctxt, VFM_MemoryStatus *pMemoryStatus);

/// @ingroup VFMMemoryAPI
/// @brief Return a string according to the enum of the memory type
/// @param [in] type the memory type we want to dump
/// @return The string of the given memory type
IMPORT_C const char* VFM_MemTypeStringify(VFM_Mem_Type type);

#ifdef _CACHE_OPT_
/// @ingroup VFMMemoryAPI
/// @brief Invalidate the cache
/// @param [in] ctxt The memory context
/// @param [in] LogicalAddress The logical address to clean / invalidate.
///     The operation is made on the whole HW Buffer
/// @param [in] size The size of the invalidate (0 to invalidate the whole buffer that contains
///     the address
/// @return OMX_ErrorNone if no error, an error otherwise
/// @note Memory should have been allocated using HW buffers
IMPORT_C void VFM_CacheInvalidate(void *ctxt, t_uint8 *LogicalAddress, t_uint32 size);

/// @ingroup VFMMemoryAPI
/// @brief Clean the cache
/// @param [in] ctxt The memory context
/// @param [in] LogicalAddress The logical address to clean / invalidate.
///     The operation is made on the whole HW Buffer
/// @param [in] size The size of the clean (0 to invalidate the whole buffer that contains
///     the address
/// @return OMX_ErrorNone if no error, an error otherwise
/// @note Memory should have been allocated using HW buffers
IMPORT_C void VFM_CacheClean(void *ctxt, t_uint8 *LogicalAddress, t_uint32 size);
#endif


#ifdef __cplusplus
}
#endif


#endif  // __VFM_MEMORY_HPP
