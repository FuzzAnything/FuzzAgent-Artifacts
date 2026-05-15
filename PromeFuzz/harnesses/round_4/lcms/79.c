// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Use default context for memory operations
    cmsContext context = NULL;
    
    // Test 1: _cmsMalloc and _cmsFree
    if (Size >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number alloc_size = *(const cmsUInt32Number*)Data;
        void* mem = _cmsMalloc(context, alloc_size);
        if (mem) {
            // Test _cmsDupMem if we have enough data
            if (Size >= alloc_size + sizeof(cmsUInt32Number)) {
                void* dup_mem = _cmsDupMem(context, Data + sizeof(cmsUInt32Number), 
                                          alloc_size % (Size - sizeof(cmsUInt32Number)));
                if (dup_mem) {
                    _cmsFree(context, dup_mem);
                }
            }
            _cmsFree(context, mem);
        }
    }
    
    // Test 2: cmsAllocProfileSequenceDescription and cmsFreeProfileSequenceDescription
    if (Size > 0) {
        // Use first byte to determine number of entries (0-255)
        cmsUInt32Number n_entries = Data[0];
        cmsSEQ* seq = cmsAllocProfileSequenceDescription(context, n_entries);
        if (seq) {
            // Test cmsDupProfileSequenceDescription
            cmsSEQ* dup_seq = cmsDupProfileSequenceDescription(seq);
            if (dup_seq) {
                cmsFreeProfileSequenceDescription(dup_seq);
            }
            cmsFreeProfileSequenceDescription(seq);
        }
    }
    
    // Test 3: Additional test with different allocation sizes
    if (Size >= 2 * sizeof(cmsUInt32Number)) {
        cmsUInt32Number size1 = *(const cmsUInt32Number*)Data;
        cmsUInt32Number size2 = *(const cmsUInt32Number*)(Data + sizeof(cmsUInt32Number));
        
        void* mem1 = _cmsMalloc(context, size1);
        void* mem2 = _cmsMalloc(context, size2);
        
        if (mem1 && mem2 && Size >= size1 + 2 * sizeof(cmsUInt32Number)) {
            void* dup = _cmsDupMem(context, Data + 2 * sizeof(cmsUInt32Number), 
                                  size1 % (Size - 2 * sizeof(cmsUInt32Number)));
            if (dup) {
                _cmsFree(context, dup);
            }
        }
        
        if (mem1) _cmsFree(context, mem1);
        if (mem2) _cmsFree(context, mem2);
    }
    
    // Test 4: Edge case with NULL inputs
    cmsSEQ* null_seq = cmsDupProfileSequenceDescription(NULL);
    if (null_seq) {
        cmsFreeProfileSequenceDescription(null_seq);
    }
    
    _cmsFree(context, NULL);
    
    // Test 5: Test sequence with maximum entries
    if (Size > 1) {
        cmsSEQ* max_seq = cmsAllocProfileSequenceDescription(context, 255);
        if (max_seq) {
            cmsSEQ* dup_max = cmsDupProfileSequenceDescription(max_seq);
            if (dup_max) {
                cmsFreeProfileSequenceDescription(dup_max);
            }
            cmsFreeProfileSequenceDescription(max_seq);
        }
    }
    
    return 0;
}