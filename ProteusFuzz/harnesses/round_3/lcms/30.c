// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Test _cmsMallocZero
    cmsUInt32Number size1 = (Size > 0) ? Data[0] : 1;
    // Prevent excessive allocation
    if (size1 > 1024 * 1024) size1 = 1024 * 1024;
    
    void *mem1 = _cmsMallocZero(ctx, size1);
    if (mem1 != NULL) {
        // Use memory to avoid compiler warnings
        memset(mem1, 0, size1);
        _cmsFree(ctx, mem1);
    }

    // Test _cmsCalloc
    cmsUInt32Number num = (Size > 1) ? Data[1] : 1;
    cmsUInt32Number size2 = (Size > 2) ? Data[2] : 1;
    
    // Prevent overflow and excessive allocation
    if (num > 0 && size2 > 0 && num > (1024 * 1024 / size2)) {
        num = 1024 * 1024 / size2;
    }
    if (num == 0) num = 1;
    if (size2 == 0) size2 = 1;
    
    void *mem2 = _cmsCalloc(ctx, num, size2);
    if (mem2 != NULL) {
        memset(mem2, 0, num * size2);
        _cmsFree(ctx, mem2);
    }

    // Test cmsMD5alloc
    cmsHANDLE md5_handle = cmsMD5alloc(ctx);
    if (md5_handle != NULL) {
        // Test cmsMD5add
        if (Size > 0) {
            cmsMD5add(md5_handle, Data, Size);
        }
        
        // Test cmsMD5finish
        cmsProfileID profile_id;
        cmsMD5finish(&profile_id, md5_handle);
        
        // Note: MD5 handles are typically freed by the context deletion
        // but we don't have a direct public API to free them individually
        // without the context. The context deletion will handle cleanup.
    }

    cleanup_context(ctx);
    return 0;
}