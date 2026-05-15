// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Use global context (NULL) for simplicity
    cmsContext context = NULL;
    
    // Test cmsAllocProfileSequenceDescription with varying sizes
    cmsUInt32Number n = 0;
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&n, Data, sizeof(cmsUInt32Number));
        n = n % 256; // Limit to valid range (0-255)
    }
    
    cmsSEQ* seq = cmsAllocProfileSequenceDescription(context, n);
    
    // Test cmsDupProfileSequenceDescription
    if (seq) {
        cmsSEQ* dup_seq = cmsDupProfileSequenceDescription(seq);
        if (dup_seq) {
            // Test cmsFreeProfileSequenceDescription
            cmsFreeProfileSequenceDescription(dup_seq);
        }
        cmsFreeProfileSequenceDescription(seq);
    }
    
    // Test _cmsMalloc and _cmsFree with varying sizes
    cmsUInt32Number malloc_size = 0;
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&malloc_size, Data, sizeof(cmsUInt32Number));
        // Limit size to reasonable value for fuzzing
        malloc_size = malloc_size % 65536;
    }
    
    void* mem_ptr = _cmsMalloc(context, malloc_size);
    if (mem_ptr) {
        _cmsFree(context, mem_ptr);
    }
    
    // Test _cmsRealloc - fix double-free issue
    cmsUInt32Number realloc_size = 0;
    if (Size >= 2 * sizeof(cmsUInt32Number)) {
        memcpy(&realloc_size, Data + sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
        realloc_size = realloc_size % 65536;
    }
    
    void* initial_ptr = _cmsMalloc(context, malloc_size / 2);
    if (initial_ptr) {
        void* realloc_ptr = _cmsRealloc(context, initial_ptr, realloc_size);
        if (realloc_ptr) {
            // If realloc succeeded, only free the new pointer
            _cmsFree(context, realloc_ptr);
        } else {
            // If realloc failed, the original pointer is still valid
            // and needs to be freed separately
            _cmsFree(context, initial_ptr);
        }
    }
    
    return 0;
}