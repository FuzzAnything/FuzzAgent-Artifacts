// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void* allocated_pointers[1024];
static size_t num_allocated = 0;

static void cleanup_allocations(cmsContext context) {
    for (size_t i = 0; i < num_allocated; i++) {
        if (allocated_pointers[i] != NULL) {
            _cmsFree(context, allocated_pointers[i]);
            allocated_pointers[i] = NULL;
        }
    }
    num_allocated = 0;
}

static void add_allocation(void* ptr) {
    if (num_allocated < sizeof(allocated_pointers)/sizeof(allocated_pointers[0])) {
        allocated_pointers[num_allocated++] = ptr;
    } else {
        allocated_pointers[0] = ptr;
        num_allocated = 1;
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    cmsContext context = NULL;
    
    if (Size < 1) return 0;
    
    uint8_t operation = Data[0] % 6;
    cmsUInt32Number param1 = 0, param2 = 0;
    
    if (Size > 1) param1 = Data[1];
    if (Size > 2) param1 = (param1 << 8) | Data[2];
    if (Size > 3) param1 = (param1 << 8) | Data[3];
    if (Size > 4) param1 = (param1 << 8) | Data[4];
    
    if (Size > 5) param2 = Data[5];
    if (Size > 6) param2 = (param2 << 8) | Data[6];
    if (Size > 7) param2 = (param2 << 8) | Data[7];
    if (Size > 8) param2 = (param2 << 8) | Data[8];
    
    switch (operation) {
        case 0: {
            void* ptr = _cmsMalloc(context, param1);
            if (ptr) add_allocation(ptr);
            break;
        }
        case 1: {
            void* ptr = _cmsMallocZero(context, param1);
            if (ptr) add_allocation(ptr);
            break;
        }
        case 2: {
            void* ptr = _cmsCalloc(context, param1, param2);
            if (ptr) add_allocation(ptr);
            break;
        }
        case 3: {
            if (Size > 12 && param1 > 0) {
                const void* src = Data + 9;
                size_t copy_size = param1 < (Size - 9) ? param1 : (Size - 9);
                void* ptr = _cmsDupMem(context, src, copy_size);
                if (ptr) add_allocation(ptr);
            }
            break;
        }
        case 4: {
            if (num_allocated > 0) {
                size_t idx = param1 % num_allocated;
                void* old_ptr = allocated_pointers[idx];
                void* new_ptr = _cmsRealloc(context, old_ptr, param2);
                if (new_ptr) {
                    allocated_pointers[idx] = new_ptr;
                } else if (old_ptr) {
                    _cmsFree(context, old_ptr);
                    allocated_pointers[idx] = NULL;
                }
            }
            break;
        }
        case 5: {
            if (num_allocated > 0) {
                size_t idx = param1 % num_allocated;
                if (allocated_pointers[idx]) {
                    _cmsFree(context, allocated_pointers[idx]);
                    allocated_pointers[idx] = NULL;
                }
            }
            break;
        }
    }
    
    cleanup_allocations(context);
    return 0;
}