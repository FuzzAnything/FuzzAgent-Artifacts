// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
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

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsHANDLE md5Handle = cmsMD5alloc(ctx);
    if (md5Handle != NULL) {
        cmsMD5add(md5Handle, Data, Size);
        cmsProfileID profileID;
        cmsMD5finish(&profileID, md5Handle);
    }

    void *mem1 = _cmsCalloc(ctx, 10, sizeof(cmsUInt32Number));
    if (mem1 != NULL) {
        _cmsFree(ctx, mem1);
    }

    void *mem2 = _cmsMallocZero(ctx, 1024);
    if (mem2 != NULL) {
        _cmsFree(ctx, mem2);
    }

    cmsHPROFILE hProfile = NULL;
    if (Size >= 128) {
        hProfile = cmsCreateProfilePlaceholder(ctx);
        if (hProfile != NULL) {
            cmsMD5computeID(hProfile);
        }
    }

    if (hProfile != NULL) {
        cleanup_profile(hProfile);
    }

    cleanup_context(ctx);

    return 0;
}