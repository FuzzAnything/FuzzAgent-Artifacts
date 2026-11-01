// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_resources(cmsContext ctx, cmsHANDLE md5_handle, void *mem_ptr) {
    if (md5_handle != NULL) {
        // MD5 handles are managed by the context, no explicit free needed here
    }
    if (mem_ptr != NULL) {
        // Memory allocated via _cmsCalloc or _cmsMallocZero is managed by the context
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHANDLE md5_handle = NULL;
    void *mem_ptr = NULL;
    cmsBool success = FALSE;

    // Fuzz _cmsCalloc
    if (Size >= 4) {
        cmsUInt32Number num = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        cmsUInt32Number size = (Size >= 8) ? ((Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7]) : 1;
        // Limit to prevent OOM
        if (num > 10000 || size > 10000) {
            num = 100;
            size = 100;
        }
        mem_ptr = _cmsCalloc(ctx, num, size);
    }

    // Fuzz _cmsMallocZero
    if (Size >= 8) {
        cmsUInt32Number size = (Data[8] << 24) | (Data[9] << 16) | (Data[10] << 8) | Data[11];
        if (size > 100000) size = 10000;
        void *ptr = _cmsMallocZero(ctx, size);
        if (ptr != NULL) {
            // Use the pointer briefly to ensure it's valid
            memset(ptr, 0xAA, 1); 
            // Note: We don't free it manually as it's context-managed
        }
    }

    // Fuzz cmsMD5alloc
    md5_handle = cmsMD5alloc(ctx);
    if (md5_handle != NULL) {
        // Fuzz cmsMD5add
        if (Size > 0) {
            cmsMD5add(md5_handle, Data, Size);
        }
        
        // Fuzz cmsMD5finish
        cmsProfileID profile_id;
        cmsMD5finish(&profile_id, md5_handle);
    }

    // Fuzz cmsMD5computeID requires a valid profile.
    // We attempt to create a dummy file and try to open it.
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        
        cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
        if (hProfile != NULL) {
            success = cmsMD5computeID(hProfile);
            cmsCloseProfile(hProfile);
        }
    }

    cleanup_resources(ctx, md5_handle, mem_ptr);
    
    return 0;
}