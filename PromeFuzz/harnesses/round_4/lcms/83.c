// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
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

static uint32_t readUInt32(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    uint32_t val = (uint32_t)((*data)[0]) << 24 |
                   (uint32_t)((*data)[1]) << 16 |
                   (uint32_t)((*data)[2]) << 8  |
                   (uint32_t)((*data)[3]);
    *data += 4;
    *size -= 4;
    return val;
}

static uint8_t readUInt8(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    uint8_t val = (*data)[0];
    *data += 1;
    *size -= 1;
    return val;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const uint8_t *p = Data;
    size_t remaining = Size;
    
    // Use first byte to decide operation
    uint8_t operation = readUInt8(&p, &remaining);
    
    // Use NULL context for global context
    cmsContext context = NULL;
    
    switch (operation % 6) {
        case 0: {
            // Test cmsDupProfileSequenceDescription
            if (remaining >= 1) {
                uint8_t n = readUInt8(&p, &remaining);
                cmsSEQ *seq = cmsAllocProfileSequenceDescription(context, n);
                if (seq) {
                    cmsSEQ *dup = cmsDupProfileSequenceDescription(seq);
                    if (dup) {
                        cmsFreeProfileSequenceDescription(dup);
                    }
                    cmsFreeProfileSequenceDescription(seq);
                }
            }
            break;
        }
        
        case 1: {
            // Test _cmsMalloc and _cmsFree
            if (remaining >= 4) {
                uint32_t alloc_size = readUInt32(&p, &remaining);
                // Limit size to avoid excessive allocation
                if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024;
                void *mem = _cmsMalloc(context, alloc_size);
                if (mem) {
                    _cmsFree(context, mem);
                }
            }
            break;
        }
        
        case 2: {
            // Test _cmsFree with NULL
            _cmsFree(context, NULL);
            break;
        }
        
        case 3: {
            // Test _cmsDupMem
            if (remaining >= 4) {
                uint32_t dup_size = readUInt32(&p, &remaining);
                // Limit size to avoid excessive allocation
                if (dup_size > 1024 * 1024) dup_size = 1024 * 1024;
                if (dup_size > remaining) dup_size = remaining;
                
                void *dup = _cmsDupMem(context, p, dup_size);
                if (dup) {
                    _cmsFree(context, dup);
                }
                p += dup_size;
                remaining -= dup_size;
            }
            break;
        }
        
        case 4: {
            // Test cmsAllocProfileSequenceDescription with various sizes
            if (remaining >= 1) {
                uint8_t n = readUInt8(&p, &remaining);
                cmsSEQ *seq = cmsAllocProfileSequenceDescription(context, n);
                if (seq) {
                    cmsFreeProfileSequenceDescription(seq);
                }
            }
            break;
        }
        
        case 5: {
            // Test cmsFreeProfileSequenceDescription with NULL
            cmsFreeProfileSequenceDescription(NULL);
            
            // Also test allocation and immediate free
            if (remaining >= 1) {
                uint8_t n = readUInt8(&p, &remaining);
                cmsSEQ *seq = cmsAllocProfileSequenceDescription(context, n);
                cmsFreeProfileSequenceDescription(seq);
            }
            break;
        }
    }
    
    return 0;
}