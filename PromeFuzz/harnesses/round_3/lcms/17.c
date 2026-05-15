// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
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

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void cleanup_test_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsSEQ* create_test_sequence(cmsContext ctx, cmsUInt32Number n) {
    if (n > 255) n = 255;
    return cmsAllocProfileSequenceDescription(ctx, n);
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    uint8_t selector = Data[0] % 6;
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector) {
        case 0: {
            // Test cmsFreeProfileSequenceDescription
            cmsUInt32Number n = 0;
            if (fuzz_size >= sizeof(cmsUInt32Number)) {
                n = *((const cmsUInt32Number*)fuzz_data) % 256;
            }
            cmsSEQ* seq = create_test_sequence(ctx, n);
            if (seq) {
                cmsFreeProfileSequenceDescription(seq);
            }
            break;
        }
        
        case 1: {
            // Test _cmsMalloc
            cmsUInt32Number alloc_size = 0;
            if (fuzz_size >= sizeof(cmsUInt32Number)) {
                alloc_size = *((const cmsUInt32Number*)fuzz_data);
                if (alloc_size > 65536) alloc_size = 65536;
            }
            void* ptr = _cmsMalloc(ctx, alloc_size);
            if (ptr) {
                _cmsFree(ctx, ptr);
            }
            break;
        }
        
        case 2: {
            // Test cmsAllocProfileSequenceDescription
            cmsUInt32Number n = 0;
            if (fuzz_size >= sizeof(cmsUInt32Number)) {
                n = *((const cmsUInt32Number*)fuzz_data) % 256;
            }
            cmsSEQ* seq = cmsAllocProfileSequenceDescription(ctx, n);
            if (seq) {
                cmsFreeProfileSequenceDescription(seq);
            }
            break;
        }
        
        case 3: {
            // Test _cmsFree with various allocations
            cmsUInt32Number alloc_size = 0;
            if (fuzz_size >= sizeof(cmsUInt32Number)) {
                alloc_size = *((const cmsUInt32Number*)fuzz_data) % 4096;
            }
            void* ptr = _cmsMalloc(ctx, alloc_size);
            _cmsFree(ctx, ptr);
            
            // Test with NULL pointer
            _cmsFree(ctx, NULL);
            break;
        }
        
        case 4: {
            // Test cmsDupProfileSequenceDescription
            cmsUInt32Number n = 0;
            if (fuzz_size >= sizeof(cmsUInt32Number)) {
                n = *((const cmsUInt32Number*)fuzz_data) % 256;
            }
            cmsSEQ* seq = create_test_sequence(ctx, n);
            if (seq) {
                cmsSEQ* dup_seq = cmsDupProfileSequenceDescription(seq);
                if (dup_seq) {
                    cmsFreeProfileSequenceDescription(dup_seq);
                }
                cmsFreeProfileSequenceDescription(seq);
            }
            
            // Test with NULL input
            cmsSEQ* null_dup = cmsDupProfileSequenceDescription(NULL);
            if (null_dup) {
                cmsFreeProfileSequenceDescription(null_dup);
            }
            break;
        }
        
        case 5: {
            // Test _cmsRealloc
            cmsUInt32Number initial_size = 0;
            cmsUInt32Number new_size = 0;
            
            if (fuzz_size >= 2 * sizeof(cmsUInt32Number)) {
                initial_size = *((const cmsUInt32Number*)fuzz_data) % 4096;
                fuzz_data += sizeof(cmsUInt32Number);
                new_size = *((const cmsUInt32Number*)fuzz_data) % 4096;
            }
            
            void* ptr = _cmsMalloc(ctx, initial_size);
            if (ptr) {
                void* new_ptr = _cmsRealloc(ctx, ptr, new_size);
                if (new_ptr) {
                    _cmsFree(ctx, new_ptr);
                }
                // Do not free ptr here - _cmsRealloc may have already freed it
                // even when returning NULL, and the standard realloc behavior
                // varies by implementation
            }
            
            // Test realloc with NULL pointer (should act like malloc)
            void* malloc_like = _cmsRealloc(ctx, NULL, 128);
            if (malloc_like) {
                _cmsFree(ctx, malloc_like);
            }
            break;
        }
    }
    
    cleanup_test_context(ctx);
    return 0;
}