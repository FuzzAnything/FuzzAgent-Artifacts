// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext create_test_context(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return NULL;
    }
    return ctx;
}

static void destroy_test_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext ctx = create_test_context();
    if (!ctx) {
        return 0;
    }

    cmsUInt32Number alloc_size = *(cmsUInt32Number*)Data;
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }

    uint8_t operation = 0;
    if (Size >= 5) {
        operation = Data[4] % 6;
    }
    
    size_t data_offset = 5;

    switch (operation) {
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
            if (Size > data_offset + alloc_size && alloc_size > 0) {
                void* src = (void*)(Data + data_offset);
                void* dst = _cmsDupMem(ctx, src, alloc_size);
                if (dst) {
                    _cmsFree(ctx, dst);
                }
            }
            break;
        }
        case 3: {
            cmsUInt32Number num = alloc_size / 4;
            cmsUInt32Number elem_size = 4;
            if (num == 0) num = 1;
            void* ptr = _cmsCalloc(ctx, num, elem_size);
            if (ptr) {
                _cmsFree(ctx, ptr);
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

    destroy_test_context(ctx);
    return 0;
}