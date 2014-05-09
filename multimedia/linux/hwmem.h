/*
 * Copyright (C) ST-Ericsson SA 2010
 *
 * Hardware memory driver, hwmem
 *
 * Author: Marcus Lorentzon <marcus.xm.lorentzon@stericsson.com>
 * for ST-Ericsson.
 *
 * License terms: GNU General Public License (GPL), version 2.
 */

#ifndef _HWMEM_H_
#define _HWMEM_H_

#include <linux/types.h>

#if !defined(__KERNEL__)
#include <sys/types.h>
#else
#include <linux/mm_types.h>
#endif

#define HWMEM_DEFAULT_DEVICE_NAME "hwmem"

/**
 * @brief Flags defining behavior of allocation
 */
enum hwmem_alloc_flags {
	/**
	 * @brief Buffered
	 */
	HWMEM_ALLOC_HINT_WRITE_COMBINE         = (1 << 0),
	/**
	 * @brief Non-buffered
	 */
	HWMEM_ALLOC_HINT_NO_WRITE_COMBINE      = (1 << 1),
	/**
	 * @brief Cached
	 */
	HWMEM_ALLOC_HINT_CACHED                = (1 << 2),
	/**
	 * @brief Uncached
	 */
	HWMEM_ALLOC_HINT_UNCACHED              = (1 << 3),
	/**
	 * @brief Write back
	 */
	HWMEM_ALLOC_HINT_CACHE_WB              = (1 << 4),
	/**
	 * @brief Write through
	 */
	HWMEM_ALLOC_HINT_CACHE_WT              = (1 << 5),
	/**
	 * @brief No alloc on write
	 */
	HWMEM_ALLOC_HINT_CACHE_NAOW            = (1 << 6),
	/**
	 * @brief Alloc on write
	 */
	HWMEM_ALLOC_HINT_CACHE_AOW             = (1 << 7),
	/**
	 * @brief Inner and outer cache
	 */
	HWMEM_ALLOC_HINT_INNER_AND_OUTER_CACHE = (1 << 8),
	/**
	 * @brief Inner cache only
	 */
	HWMEM_ALLOC_HINT_INNER_CACHE_ONLY      = (1 << 9),
	/**
	 * @brief Reserved for use by the cache handler integration
	 */
	HWMEM_ALLOC_RESERVED_CHI               = (1 << 31),
};

/**
 * @brief Flags defining buffer access mode.
 */
enum hwmem_access {
	/**
	 * @brief Buffer will be read from.
	 */
	HWMEM_ACCESS_READ  = (1 << 0),
	/**
	 * @brief Buffer will be written to.
	 */
	HWMEM_ACCESS_WRITE = (1 << 1),
	/**
	 * @brief Buffer will be imported.
	 */
	HWMEM_ACCESS_IMPORT = (1 << 2),
};

/**
 * @brief Values defining memory types.
 */
enum hwmem_mem_type {
	/**
	 * @brief Scattered system memory.
	 */
	HWMEM_MEM_SCATTERED_SYS,
	/**
	 * @brief Contiguous system memory.
	 */
	HWMEM_MEM_CONTIGUOUS_SYS,
	/**
	 * @brief Protected system memory.
	 */
	HWMEM_MEM_PROTECTED_SYS,

	/**
	 * @brief Static system memory.
	 */
	HWMEM_MEM_STATIC_SYS,
};

/* User space API */

/**
 * @see struct hwmem_region.
 */
struct hwmem_region_us {
	__u32 offset;
	__u32 count;
	__u32 start;
	__u32 end;
	__u32 size;
};

/**
 * @brief Alloc request data.
 */
struct hwmem_alloc_request {
	/**
	 * @brief [in] Size of requested allocation in bytes. Size will be
	 * aligned to PAGE_SIZE bytes.
	 */
	__u32 size;
	/**
	 * @brief [in] Flags describing requested allocation options.
	 */
	__u32 flags; /* enum hwmem_alloc_flags */
	/**
	 * @brief [in] Default access rights for buffer.
	 */
	__u32 default_access; /* enum hwmem_access */
	/**
	 * @brief [in] Memory type of the buffer.
	 */
	__u32 mem_type; /* enum hwmem_mem_type */
};

/**
 * @brief Set domain request data.
 */
struct hwmem_set_domain_request {
	/**
	 * @brief [in] Identifier of buffer to be prepared. If 0 is specified
	 * the buffer associated with the current file instance will be used.
	 */
	__s32 id;
	/**
	 * @brief [in] Flags specifying access mode of the operation.
	 *
	 * One of HWMEM_ACCESS_READ and HWMEM_ACCESS_WRITE is required.
	 * For details, @see enum hwmem_access.
	 */
	__u32 access; /* enum hwmem_access */
	/**
	 * @brief [in] The region of bytes to be prepared.
	 *
	 * For details, @see struct hwmem_region.
	 */
	struct hwmem_region_us region;
};

/**
 * @brief Pin request data.
 */
struct hwmem_pin_request {
	/**
	 * @brief [in] Identifier of buffer to be pinned. If 0 is specified,
	 * the buffer associated with the current file instance will be used.
	 */
	__s32 id;
	/**
	 * @brief [out] Physical address of first word in buffer.
	 */
	__u32 phys_addr;
};

/**
 * @brief Set access rights request data.
 */
struct hwmem_set_access_request {
	/**
	 * @brief [in] Identifier of buffer to set access rights for. If 0 is
	 * specified, the buffer associated with the current file instance will
	 * be used.
	 */
	__s32 id;
	/**
	 * @param access Access value indicating what is allowed.
	 */
	__u32 access; /* enum hwmem_access */
	/**
	 * @param pid Process ID to set rights for.
	 */
	pid_t pid;
};

/**
 * @brief Get info request data.
 */
struct hwmem_get_info_request {
	/**
	 * @brief [in] Identifier of buffer to get info about. If 0 is specified,
	 * the buffer associated with the current file instance will be used.
	 */
	__s32 id;
	/**
	 * @brief [out] Size in bytes of buffer.
	 */
	__u32 size;
	/**
	 * @brief [out] Memory type of buffer.
	 */
	__u32 mem_type; /* enum hwmem_mem_type */
	/**
	 * @brief [out] Access rights for buffer.
	 */
	__u32 access; /* enum hwmem_access */
};

/**
 * @brief Allocates <size> number of bytes and returns a buffer identifier.
 *
 * Input is a pointer to a hwmem_alloc_request struct.
 *
 * @return A buffer identifier on success, or a negative error code.
 */
#define HWMEM_ALLOC_IOC _IOW('W', 1, struct hwmem_alloc_request)

/**
 * @brief Allocates <size> number of bytes and associates the created buffer
 * with the current file instance.
 *
 * If the current file instance is already associated with a buffer the call
 * will fail. Buffers referenced through files instances shall not be released
 * with HWMEM_RELEASE_IOC, instead the file instance shall be closed.
 *
 * Input is a pointer to a hwmem_alloc_request struct.
 *
 * @return Zero on success, or a negative error code.
 */
#define HWMEM_ALLOC_FD_IOC _IOW('W', 2, struct hwmem_alloc_request)

/**
 * @brief Releases buffer.
 *
 * Buffers are reference counted and will not be destroyed until the last
 * reference is released. Buffers allocated with ALLOC_FD_IOC shall not be
 * released with this IOC, @see HWMEM_ALLOC_FD_IOC.
 *
 * Input is the buffer identifier.
 *
 * @return Zero on success, or a negative error code.
 */
#define HWMEM_RELEASE_IOC _IO('W', 3)

/**
 * Memory Mapping
 *
 * To map a hwmem buffer mmap the hwmem fd and supply the buffer identifier as
 * the offset. If the buffer is linked to the fd and thus have no buffer
 * identifier supply 0 as the offset. Note that the offset feature of mmap is
 * disabled in both cases, you can only mmap starting a position 0.
 */

/**
 * @brief Prepares the buffer for CPU access.
 *
 * Input is a pointer to a hwmem_set_domain_request struct.
 *
 * @return Zero on success, or a negative error code.
 */
#define HWMEM_SET_CPU_DOMAIN_IOC _IOW('W', 4, struct hwmem_set_domain_request)

/**
 * DEPRECATED: Set sync domain from driver instead!
 *
 * @brief Prepares the buffer for access by any DMA hardware.
 *
 * Input is a pointer to a hwmem_set_domain_request struct.
 *
 * @return Zero on success, or a negative error code.
 */
#define HWMEM_SET_SYNC_DOMAIN_IOC _IOW('W', 5, struct hwmem_set_domain_request)

/**
 * DEPRECATED: Pin from driver instead!
 *
 * @brief Pins the buffer.
 *
 * Input is a pointer to a hwmem_pin_request struct. Only contiguous buffers
 * can be pinned from user space.
 *
 * @return Zero on success, or a negative error code.
 */
#define HWMEM_PIN_IOC _IOWR('W', 6, struct hwmem_pin_request)

/**
 * DEPRECATED: Unpin from driver instead!
 *
 * @brief Unpins the buffer.
 *
 * @return Zero on success, or a negative error code.
 */
#define HWMEM_UNPIN_IOC _IO('W', 7)

/**
 * @brief Set access rights for buffer.
 *
 * Input is a pointer to a hwmem_set_access_request struct.
 *
 * @return Zero on success, or a negative error code.
 */
#define HWMEM_SET_ACCESS_IOC _IOW('W', 8, struct hwmem_set_access_request)

/**
 * @brief Get buffer information.
 *
 * Input is a pointer to a hwmem_get_info_request struct.
 *
 * @return Zero on success, or a negative error code.
 */
#define HWMEM_GET_INFO_IOC _IOWR('W', 9, struct hwmem_get_info_request)

/**
 * @brief Export the buffer identifier for use in another process.
 *
 * The global name will not increase the buffers reference count and will
 * therefore not keep the buffer alive.
 *
 * Input is the buffer identifier. If 0 is specified the buffer associated with
 * the current file instance will be exported.
 *
 * @return A global buffer name on success, or a negative error code.
 */
#define HWMEM_EXPORT_IOC _IO('W', 10)

/**
 * @brief Import a buffer to allow local access to the buffer.
 *
 * Input is the buffer's global name.
 *
 * @return The imported buffer's identifier on success, or a negative error
 * code.
 */
#define HWMEM_IMPORT_IOC _IO('W', 11)

/**
 * @brief Import a buffer to allow local access to the buffer using the current
 * fd.
 *
 * Input is the buffer's global name.
 *
 * @return Zero on success, or a negative error code.
 */
#define HWMEM_IMPORT_FD_IOC _IO('W', 12)

#ifdef __KERNEL__

/* Kernel API */

/**
 * @brief Values defining memory domain.
 */
enum hwmem_domain {
	/**
	 * @brief This value specifies the neutral memory domain. Setting this
	 * domain will syncronize all supported memory domains.
	 */
	HWMEM_DOMAIN_SYNC = 0,
	/**
	 * @brief This value specifies the CPU memory domain.
	 */
	HWMEM_DOMAIN_CPU,
};

struct hwmem_alloc;

/**
 * @brief Structure defining a region of a memory buffer.
 *
 * A buffer is defined to contain a number of equally sized blocks. Each block
 * has a part of it included in the region [<start>-<end>). That is
 * <end>-<start> bytes. Each block is <size> bytes long. Total number of bytes
 * in the region is (<end> - <start>) * <count>. First byte of the region is
 * <offset> + <start> bytes into the buffer.
 *
 * Here's an example of a region in a graphics buffer (X = buffer, R = region):
 *
 * XXXXXXXXXXXXXXXXXXXX \
 * XXXXXXXXXXXXXXXXXXXX |-- offset = 60
 * XXXXXXXXXXXXXXXXXXXX /
 * XXRRRRRRRRXXXXXXXXXX \
 * XXRRRRRRRRXXXXXXXXXX |-- count = 4
 * XXRRRRRRRRXXXXXXXXXX |
 * XXRRRRRRRRXXXXXXXXXX /
 * XXXXXXXXXXXXXXXXXXXX
 * --| start = 2
 * ----------| end = 10
 * --------------------| size = 20
 */
struct hwmem_region {
	/**
	 * @brief The first block's offset from beginning of buffer.
	 */
	size_t offset;
	/**
	 * @brief The number of blocks included in this region.
	 */
	size_t count;
	/**
	 * @brief The index of the first byte included in this block.
	 */
	size_t start;
	/**
	 * @brief The index of the last byte included in this block plus one.
	 */
	size_t end;
	/**
	 * @brief The size in bytes of each block.
	 */
	size_t size;
};

struct hwmem_mem_chunk {
	phys_addr_t paddr;
	size_t size;
};

/**
 * @brief Allocates <size> number of bytes.
 *
 * @param size Number of bytes to allocate. All allocations are page aligned.
 * @param flags Allocation options.
 * @param def_access Default buffer access rights.
 * @param mem_type Memory type.
 *
 * @return Pointer to allocation, or a negative error code.
 */
struct hwmem_alloc *hwmem_alloc(size_t size, enum hwmem_alloc_flags flags,
		enum hwmem_access def_access, enum hwmem_mem_type mem_type);

/**
 * @brief Release a previously allocated buffer.
 * When last reference is released, the buffer will be freed.
 *
 * @param alloc Buffer to be released.
 */
void hwmem_release(struct hwmem_alloc *alloc);

/**
 * @brief Set the buffer domain and prepare it for access.
 *
 * @param alloc Buffer to be prepared.
 * @param access Flags defining memory access mode of the call.
 * @param domain Value specifying the memory domain.
 * @param region Structure defining the minimum area of the buffer to be
 * prepared.
 *
 * @return Zero on success, or a negative error code.
 */
int hwmem_set_domain(struct hwmem_alloc *alloc, enum hwmem_access access,
		enum hwmem_domain domain, struct hwmem_region *region);

/**
 * @brief Pins the buffer.
 *
 * Notice that the number of mem chunks a buffer consists of can change at any
 * time if the buffer is not pinned. Because of this one can not assume that
 * pin will succeed if <mem_chunks> has the length specified by a previous call
 * to pin as the buffer layout may have changed between the calls. There are
 * two ways of handling this situation, keep redoing the pin procedure till it
 * succeeds or allocate enough mem chunks for the worst case ("buffer size" /
 * "page size" mem chunks). Contiguous buffers always require only one mem
 * chunk.
 *
 * @param alloc Buffer to be pinned.
 * @param mem_chunks Pointer to array of mem chunks.
 * @param mem_chunks_length Pointer to variable that contains the length of
 * <mem_chunks> array. On success the number of written mem chunks will be
 * stored in this variable. If the call fails with -ENOSPC the required length
 * of <mem_chunks> will be stored in this variable.
 *
 * @return Zero on success, or a negative error code.
 */
int hwmem_pin(struct hwmem_alloc *alloc, struct hwmem_mem_chunk *mem_chunks,
						size_t *mem_chunks_length);

/**
 * @brief Unpins the buffer.
 *
 * @param alloc Buffer to be unpinned.
 */
void hwmem_unpin(struct hwmem_alloc *alloc);

/**
 * @brief Map the buffer to user space.
 *
 * @param alloc Buffer to be mapped.
 *
 * @return Zero on success, or a negative error code.
 */
int hwmem_mmap(struct hwmem_alloc *alloc, struct vm_area_struct *vma);

/**
 * @brief Map the buffer for use in the kernel.
 *
 * This function implicitly pins the buffer.
 *
 * @param alloc Buffer to be mapped.
 *
 * @return Pointer to buffer, or a negative error code.
 */
void *hwmem_kmap(struct hwmem_alloc *alloc);

/**
 * @brief Un-map a buffer previously mapped with hwmem_kmap.
 *
 * This function implicitly unpins the buffer.
 *
 * @param alloc Buffer to be un-mapped.
 */
void hwmem_kunmap(struct hwmem_alloc *alloc);

/**
 * @brief Set access rights for buffer.
 *
 * @param alloc Buffer to set rights for.
 * @param access Access value indicating what is allowed.
 * @param pid Process ID to set rights for.
 */
int hwmem_set_access(struct hwmem_alloc *alloc, enum hwmem_access access,
								pid_t pid);

/**
 * @brief Get buffer information.
 *
 * @param alloc Buffer to get information about.
 * @param size Pointer to size output variable. Can be NULL.
 * @param size Pointer to memory type output variable. Can be NULL.
 * @param size Pointer to access rights output variable. Can be NULL.
 */
void hwmem_get_info(struct hwmem_alloc *alloc, size_t *size,
		enum hwmem_mem_type *mem_type, enum hwmem_access *access);

/**
 * @brief Allocate a global buffer name.
 * Generated buffer name is valid in all processes. Consecutive calls will get
 * the same name for the same buffer.
 *
 * @param alloc Buffer to be made public.
 *
 * @return Positive global name on success, or a negative error code.
 */
s32 hwmem_get_name(struct hwmem_alloc *alloc);

/**
 * @brief Import the global buffer name to allow local access to the buffer.
 * This call will add a buffer reference. Resulting buffer should be
 * released with a call to hwmem_release.
 *
 * @param name A valid global buffer name.
 *
 * @return Pointer to allocation, or a negative error code.
 */
struct hwmem_alloc *hwmem_resolve_by_name(s32 name);

/**
 * DEPRECATED: Temporary solution for Global Platform TEE Client API 1.0!
 *
 * @brief Resolve a hwmem allocation pointer from a vm address pointer.
 * This call will fetch the hwmem_alloc pointer based on the
 * corresponding vm address pointer, and add a buffer reference.
 * Resulting buffer should be released with a call to hwmem_release.
 *
 * @param vm_addr vm address pointer previously retrieved from hwmem_mmap().
 *
 * @return Pointer to hwmem allocation, or a negative error code
 */
struct hwmem_alloc *hwmem_resolve_by_vm_addr(void *vm_addr);

/* Integration */

struct hwmem_allocator_api {
	void *(*alloc)(void *instance, size_t size);
	void (*free)(void *instance, void *alloc);
	phys_addr_t (*get_alloc_paddr)(void *alloc);
	void *(*get_alloc_kaddr)(void *instance, void *alloc);
	size_t (*get_alloc_size)(void *alloc);
	struct page **(*get_alloc_sglist)(void *alloc);
};

struct hwmem_mem_type_struct {
	enum hwmem_mem_type id;
	struct hwmem_allocator_api allocator_api;
	void *allocator_instance;
};

extern struct hwmem_mem_type_struct *hwmem_mem_types;
extern unsigned int hwmem_num_mem_types;

#endif

#endif /* _HWMEM_H_ */
