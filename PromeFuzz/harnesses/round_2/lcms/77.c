// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALLOCATIONS 16
#define MAX_ALLOC_SIZE 1024

static void* allocated_ptrs[MAX_ALLOCATIONS] = {0};
static size_t num_allocations = 0;

static cmsUInt32Number read_uint32(const uint8_t** data, size_t* size) {
    if (*size < sizeof(cmsUInt32Number)) {
        return 0;
    }
    cmsUInt32Number value = 0;
    memcpy(&value, *data, sizeof(cmsUInt32Number));
    *data += sizeof(cmsUInt32Number);
    *size -= sizeof(cmsUInt32Number);
    return value;
}

static void cleanup_allocations(cmsContext context) {
    for (size_t i = 0; i < num_allocations; i++) {
        if (allocated_ptrs[i] != NULL) {
            _cmsFree(context, allocated_ptrs[i]);
            allocated_ptrs[i] = NULL;
        }
    }
    num_allocations = 0;
}

static void* allocate_and_track(cmsContext context, void* ptr) {
    if (ptr != NULL && num_allocations < MAX_ALLOCATIONS) {
        allocated_ptrs[num_allocations++] = ptr;
    }
    return ptr;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = NULL;
    const uint8_t* data_ptr = Data;
    size_t data_size = Size;
    
    while (data_size > 0) {
        uint8_t operation = data_ptr[0];
        data_ptr++;
        data_size--;
        
        switch (operation % 7) {
            case 0: {
                cmsUInt32Number num = read_uint32(&data_ptr, &data_size);
                cmsUInt32Number size = read_uint32(&data_ptr, &data_size);
                num = num % 256;
                size = size % 256;
                void* ptr = _cmsCalloc(context, num, size);
                allocate_and_track(context, ptr);
                break;
            }
            
            case 1: {
                cmsUInt32Number new_size = read_uint32(&data_ptr, &data_size) % MAX_ALLOC_SIZE;
                if (num_allocations > 0) {
                    size_t idx = operation % num_allocations;
                    void* old_ptr = allocated_ptrs[idx];
                    void* new_ptr = _cmsRealloc(context, old_ptr, new_size);
                    if (new_ptr != NULL) {
                        allocated_ptrs[idx] = new_ptr;
                    }
                }
                break;
            }
            
            case 2: {
                cmsUInt32Number size = read_uint32(&data_ptr, &data_size) % MAX_ALLOC_SIZE;
                void* ptr = _cmsMallocZero(context, size);
                allocate_and_track(context, ptr);
                break;
            }
            
            case 3: {
                cmsUInt32Number size = read_uint32(&data_ptr, &data_size) % MAX_ALLOC_SIZE;
                void* ptr = _cmsMalloc(context, size);
                allocate_and_track(context, ptr);
                break;
            }
            
            case 4: {
                if (num_allocations > 0) {
                    size_t idx = operation % num_allocations;
                    void* ptr = allocated_ptrs[idx];
                    _cmsFree(context, ptr);
                    allocated_ptrs[idx] = NULL;
                }
                break;
            }
            
            case 5: {
                cmsUInt32Number size = read_uint32(&data_ptr, &data_size) % MAX_ALLOC_SIZE;
                if (num_allocations > 0) {
                    size_t idx = operation % num_allocations;
                    const void* org = allocated_ptrs[idx];
                    if (org != NULL) {
                        void* dup = _cmsDupMem(context, org, size);
                        allocate_and_track(context, dup);
                    }
                }
                break;
            }
            
            case 6: {
                cleanup_allocations(context);
                break;
            }
        }
    }
    
    cleanup_allocations(context);
    return 0;
}