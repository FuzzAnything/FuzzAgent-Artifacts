// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"

static void* safe_strdup(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    if (len == 0) return NULL;
    void *p = malloc(len + 1);
    if (p) {
        memcpy(p, s, len + 1);
    }
    return p;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        // Fallback to global context if creation fails
        ctx = NULL;
    }

    // Extract parameters from input
    cmsUInt32Number n = (Size > 0) ? Data[0] : 0;
    cmsUInt32Number colorantCount = (Size > 1) ? Data[1] : 0;
    cmsUInt32Number index = (Size > 2) ? Data[2] : 0;

    // Limit values to reasonable ranges to avoid immediate allocation failures or overflows
    if (n > 1000) n = 1000;
    if (colorantCount > cmsMAXCHANNELS) colorantCount = cmsMAXCHANNELS;

    // Prepare prefix and suffix from input data
    char prefix[33] = {0};
    char suffix[33] = {0};
    if (Size > 4) {
        size_t pLen = Data[3];
        if (pLen > 32) pLen = 32;
        if (Size > 4 + pLen) {
            memcpy(prefix, Data + 4, pLen);
            prefix[pLen] = '\0';
        }
    }
    if (Size > 5) {
        size_t sLen = Data[4];
        if (sLen > 32) sLen = 32;
        if (Size > 5 + sLen) {
            memcpy(suffix, Data + 5, sLen);
            suffix[sLen] = '\0';
        }
    }

    // 1. Allocate Named Color List
    cmsNAMEDCOLORLIST *list = cmsAllocNamedColorList(ctx, n, colorantCount, prefix, suffix);
    
    if (list) {
        // 2. Check count
        cmsUInt32Number count = cmsNamedColorCount(list);
        
        // 3. Duplicate list
        cmsNAMEDCOLORLIST *dupList = cmsDupNamedColorList(list);
        if (dupList) {
            cmsNamedColorCount(dupList);
            cmsFreeNamedColorList(dupList);
        }

        // 4. Test cmsNamedColorInfo with various indices
        char nameBuf[256] = {0};
        char preBuf[33] = {0};
        char sufBuf[33] = {0};
        cmsUInt16Number pcsBuf[4] = {0};
        cmsUInt16Number colorantBuf[cmsMAXCHANNELS] = {0};

        // Test valid index if list is not empty
        if (count > 0 && index < count) {
            cmsNamedColorInfo(list, index, nameBuf, preBuf, sufBuf, pcsBuf, colorantBuf);
        }
        // Test out of bounds index
        if (count > 0) {
            cmsNamedColorInfo(list, count, nameBuf, preBuf, sufBuf, pcsBuf, colorantBuf);
            cmsNamedColorInfo(list, count + 100, nameBuf, preBuf, sufBuf, pcsBuf, colorantBuf);
        }
        // Test with NULL buffers
        cmsNamedColorInfo(list, 0, NULL, NULL, NULL, NULL, NULL);
        cmsNamedColorInfo(list, 0, nameBuf, NULL, NULL, NULL, NULL);

        // 5. Free original list
        cmsFreeNamedColorList(list);
    } else {
        // Test functions with NULL list
        cmsNamedColorCount(NULL);
        cmsDupNamedColorList(NULL);
        cmsFreeNamedColorList(NULL);
        cmsNamedColorInfo(NULL, 0, NULL, NULL, NULL, NULL, NULL);
    }

    // 6. Test cmsGetNamedColorList with NULL transform
    cmsGetNamedColorList(NULL);

    // Cleanup context
    if (ctx) {
        cmsDeleteContext(ctx);
    }

    return 0;
}