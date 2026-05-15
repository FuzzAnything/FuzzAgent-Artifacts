// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    // Use default context for all operations
    cmsContext context = NULL;
    
    // Test cmsAllocProfileSequenceDescription with varying sizes
    cmsUInt32Number seq_count = 0;
    if (Size >= sizeof(cmsUInt32Number)) {
        // Use first 4 bytes for sequence count (mod 256 to stay within valid range)
        seq_count = *(const cmsUInt32Number*)Data % 256;
    }
    
    cmsSEQ* seq = cmsAllocProfileSequenceDescription(context, seq_count);
    
    // Test cmsDupProfileSequenceDescription
    cmsSEQ* dup_seq = cmsDupProfileSequenceDescription(seq);
    
    // Test _cmsMalloc with varying sizes
    cmsUInt32Number alloc_size = 0;
    if (Size >= 2 * sizeof(cmsUInt32Number)) {
        // Use next 4 bytes for allocation size
        alloc_size = *((const cmsUInt32Number*)(Data + sizeof(cmsUInt32Number)));
    }
    
    void* allocated_mem = _cmsMalloc(context, alloc_size);
    
    // Test _cmsDupMem if we have data to duplicate
    if (Size > 0 && allocated_mem != NULL) {
        // Use minimum of requested size and available data
        cmsUInt32Number copy_size = alloc_size;
        if (copy_size > Size) {
            copy_size = Size;
        }
        
        void* dup_mem = _cmsDupMem(context, Data, copy_size);
        
        // Free duplicated memory if allocation succeeded
        if (dup_mem != NULL) {
            _cmsFree(context, dup_mem);
        }
    }
    
    // Free allocated memory if allocation succeeded
    if (allocated_mem != NULL) {
        _cmsFree(context, allocated_mem);
    }
    
    // Free duplicated sequence if duplication succeeded
    if (dup_seq != NULL) {
        cmsFreeProfileSequenceDescription(dup_seq);
    }
    
    // Free original sequence if allocation succeeded
    if (seq != NULL) {
        cmsFreeProfileSequenceDescription(seq);
    }
    
    return 0;
}