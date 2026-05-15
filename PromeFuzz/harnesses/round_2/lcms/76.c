// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void cleanup_test_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a context for testing
    cmsContext ctx = create_test_context();
    if (!ctx) {
        return 0;
    }

    // Use first byte to determine test mode
    uint8_t mode = Data[0] % 6;
    
    switch (mode) {
        case 0: {
            // Test cmsAllocProfileSequenceDescription
            cmsUInt32Number n = (Size > 1) ? (Data[1] % 256) : 0;
            cmsSEQ* seq = cmsAllocProfileSequenceDescription(ctx, n);
            if (seq) {
                cmsFreeProfileSequenceDescription(seq);
            }
            break;
        }
        
        case 1: {
            // Test _cmsMallocZero
            cmsUInt32Number alloc_size = (Size > 1) ? (Data[1] % 1024) : 1;
            void* mem = _cmsMallocZero(ctx, alloc_size);
            if (mem) {
                _cmsFree(ctx, mem);
            }
            break;
        }
        
        case 2: {
            // Test _cmsMalloc
            cmsUInt32Number alloc_size = (Size > 1) ? (Data[1] % 1024) : 1;
            void* mem = _cmsMalloc(ctx, alloc_size);
            if (mem) {
                _cmsFree(ctx, mem);
            }
            break;
        }
        
        case 3: {
            // Test cmsDupProfileSequenceDescription with existing sequence
            cmsUInt32Number n = (Size > 1) ? (Data[1] % 256) : 0;
            cmsSEQ* seq = cmsAllocProfileSequenceDescription(ctx, n);
            if (seq) {
                cmsSEQ* dup_seq = cmsDupProfileSequenceDescription(seq);
                if (dup_seq) {
                    cmsFreeProfileSequenceDescription(dup_seq);
                }
                cmsFreeProfileSequenceDescription(seq);
            }
            break;
        }
        
        case 4: {
            // Test cmsDupProfileSequenceDescription with NULL
            cmsSEQ* dup_seq = cmsDupProfileSequenceDescription(NULL);
            if (dup_seq) {
                cmsFreeProfileSequenceDescription(dup_seq);
            }
            break;
        }
        
        case 5: {
            // Test memory allocation and free combinations
            cmsUInt32Number n = (Size > 1) ? (Data[1] % 256) : 0;
            cmsUInt32Number alloc_size = (Size > 2) ? (Data[2] % 1024) : 1;
            
            // Allocate sequence
            cmsSEQ* seq = cmsAllocProfileSequenceDescription(ctx, n);
            
            // Allocate some raw memory
            void* mem1 = _cmsMalloc(ctx, alloc_size);
            void* mem2 = _cmsMallocZero(ctx, alloc_size);
            
            // Free in different orders
            if (mem1) {
                _cmsFree(ctx, mem1);
            }
            
            if (seq) {
                cmsFreeProfileSequenceDescription(seq);
            }
            
            if (mem2) {
                _cmsFree(ctx, mem2);
            }
            break;
        }
    }

    // Test _cmsFree with NULL pointer
    _cmsFree(ctx, NULL);
    
    // Test cmsFreeProfileSequenceDescription with NULL
    cmsFreeProfileSequenceDescription(NULL);

    cleanup_test_context(ctx);
    return 0;
}