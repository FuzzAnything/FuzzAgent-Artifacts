// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
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

static void dummy_callback(cmsContext ContextID, cmsUInt32Number size, void* ptr) {
    // Dummy callback to satisfy compiler
    (void)ContextID;
    (void)size;
    (void)ptr;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use first byte to determine operation path
    if (Size == 0) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    uint8_t operation = Data[0] % 6;
    Data++;
    Size--;
    
    switch (operation) {
        case 0: { // Test cmsFreeProfileSequenceDescription
            cmsSEQ* seq = NULL;
            if (Size >= sizeof(cmsUInt32Number)) {
                cmsUInt32Number n = *(cmsUInt32Number*)Data;
                n = n % 256; // Limit to valid range (0-255)
                seq = cmsAllocProfileSequenceDescription(ctx, n);
            }
            cmsFreeProfileSequenceDescription(seq);
            break;
        }
        
        case 1: { // Test _cmsDupMem
            if (Size > 0) {
                cmsUInt32Number copy_size = Size > 1024 ? 1024 : (cmsUInt32Number)Size;
                void* src = _cmsMalloc(ctx, copy_size);
                if (src) {
                    memcpy(src, Data, copy_size);
                    void* dst = _cmsDupMem(ctx, src, copy_size);
                    if (dst) {
                        _cmsFree(ctx, dst);
                    }
                    _cmsFree(ctx, src);
                }
            }
            break;
        }
        
        case 2: { // Test cmsAllocProfileSequenceDescription
            cmsUInt32Number n = 0;
            if (Size >= sizeof(cmsUInt32Number)) {
                n = *(cmsUInt32Number*)Data;
                n = n % 256; // Limit to valid range (0-255)
            }
            cmsSEQ* seq = cmsAllocProfileSequenceDescription(ctx, n);
            if (seq) {
                cmsFreeProfileSequenceDescription(seq);
            }
            break;
        }
        
        case 3: { // Test _cmsMalloc
            cmsUInt32Number alloc_size = 0;
            if (Size >= sizeof(cmsUInt32Number)) {
                alloc_size = *(cmsUInt32Number*)Data;
                alloc_size = alloc_size % 4096; // Limit allocation size
            }
            void* ptr = _cmsMalloc(ctx, alloc_size);
            if (ptr) {
                _cmsFree(ctx, ptr);
            }
            break;
        }
        
        case 4: { // Test _cmsFree
            cmsUInt32Number alloc_size = 0;
            if (Size >= sizeof(cmsUInt32Number)) {
                alloc_size = *(cmsUInt32Number*)Data;
                alloc_size = alloc_size % 4096; // Limit allocation size
            }
            void* ptr = _cmsMalloc(ctx, alloc_size);
            _cmsFree(ctx, ptr);
            break;
        }
        
        case 5: { // Test cmsDupProfileSequenceDescription
            cmsSEQ* seq = NULL;
            cmsSEQ* dup_seq = NULL;
            
            if (Size >= sizeof(cmsUInt32Number)) {
                cmsUInt32Number n = *(cmsUInt32Number*)Data;
                n = n % 256; // Limit to valid range (0-255)
                seq = cmsAllocProfileSequenceDescription(ctx, n);
                if (seq) {
                    dup_seq = cmsDupProfileSequenceDescription(seq);
                    if (dup_seq) {
                        cmsFreeProfileSequenceDescription(dup_seq);
                    }
                    cmsFreeProfileSequenceDescription(seq);
                }
            }
            break;
        }
    }
    
    // Clean up
    cmsDeleteContext(ctx);
    return 0;
}