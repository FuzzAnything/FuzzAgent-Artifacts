// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include "lcms2.h"

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }
    
    cmsUInt32Number alloc_size = 0;
    if (Size >= 4) {
        alloc_size = *(const cmsUInt32Number*)Data;
        alloc_size = alloc_size % 65536;
    }
    
    uint8_t selector = Data[Size - 1] % 6;
    
    switch (selector) {
        case 0: {
            void* ptr = _cmsMallocZero(ctx, alloc_size);
            if (ptr) {
                _cmsFree(ctx, ptr);
            }
            break;
        }
        case 1: {
            void* ptr = _cmsMalloc(ctx, alloc_size);
            if (ptr) {
                _cmsFree(ctx, ptr);
            }
            break;
        }
        case 2: {
            if (Size > 4) {
                const void* src = Data + 4;
                cmsUInt32Number copy_size = (Size - 4) > alloc_size ? alloc_size : (Size - 4);
                void* ptr = _cmsDupMem(ctx, src, copy_size);
                if (ptr) {
                    _cmsFree(ctx, ptr);
                }
            }
            break;
        }
        case 3: {
            cmsUInt32Number num = alloc_size / 2;
            cmsUInt32Number elem_size = alloc_size - num;
            if (num > 0 && elem_size > 0) {
                void* ptr = _cmsCalloc(ctx, num, elem_size);
                if (ptr) {
                    _cmsFree(ctx, ptr);
                }
            }
            break;
        }
        case 4: {
            void* ptr = _cmsMalloc(ctx, alloc_size);
            _cmsFree(ctx, ptr);
            _cmsFree(ctx, NULL);
            break;
        }
        case 5: {
            void* ptr = _cmsMalloc(ctx, alloc_size / 2);
            if (ptr) {
                void* new_ptr = _cmsRealloc(ctx, ptr, alloc_size);
                if (new_ptr) {
                    _cmsFree(ctx, new_ptr);
                } else {
                    _cmsFree(ctx, ptr);
                }
            }
            break;
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}