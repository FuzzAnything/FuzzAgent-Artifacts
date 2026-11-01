// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHTRANSFORM hTransform = NULL;
    cmsContext retCtx = NULL;
    void *retUser = NULL;
    void *retMutex = NULL;

    // Create a context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Test cmsCreateXYZProfileTHR
    hProfile = cmsCreateXYZProfileTHR(ctx);

    // Test cmsGetProfileContextID
    if (hProfile != NULL) {
        retCtx = cmsGetProfileContextID(hProfile);
        (void)retCtx; 
    }
    retCtx = cmsGetProfileContextID(NULL);
    (void)retCtx;

    // Create a transform if profile is valid and size is sufficient
    if (hProfile != NULL && Size > 1) {
        // Use numeric values for formats as the symbolic names might be macros or typedefs
        // cmsUInt8Number is typically 1, cmsFloat32Number is typically 3
        cmsUInt32Number inputFormat = 1; 
        cmsUInt32Number outputFormat = 3;
        
        // Intent constants: 0 = Perceptual, 1 = Saturation, 2 = Relative, 3 = Absolute
        cmsUInt32Number intent = 0; 
        cmsUInt32Number flags = 0;
        
        hTransform = cmsCreateTransform(hProfile, inputFormat, hProfile, outputFormat, intent, flags);
    }

    // Test cmsGetTransformContextID
    if (hTransform != NULL) {
        retCtx = cmsGetTransformContextID(hTransform);
        (void)retCtx;
    }
    retCtx = cmsGetTransformContextID(NULL);
    (void)retCtx;

    // Test cmsGetContextUserData
    retUser = cmsGetContextUserData(ctx);
    (void)retUser;

    // Test _cmsCreateMutex
    retMutex = _cmsCreateMutex(ctx);
    (void)retMutex;

    // Cleanup
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
    
    return 0;
}