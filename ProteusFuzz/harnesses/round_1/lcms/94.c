// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a context for testing
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Allocate a Global Data Block (GDB)
    cmsHANDLE hGBD = cmsGBDAlloc(ctx);
    if (hGBD == NULL) {
        cleanup_context(ctx);
        return 0;
    }

    // Parse input data to generate CIELab values
    cmsCIELab lab = {0};
    if (Size >= 12) {
        // Use first 12 bytes to populate L, a, b (4 bytes each as float)
        memcpy(&lab.L, Data, sizeof(double));
        memcpy(&lab.a, Data + 8, sizeof(double));
        memcpy(&lab.b, Data + 16, sizeof(double));
        if (Size < 24) {
            lab.L = 0;
            lab.a = 0;
            lab.b = 0;
        }
    } else {
        // Default values if input is too small
        lab.L = 50.0;
        lab.a = 0.0;
        lab.b = 0.0;
    }

    // Test cmsGDBAddPoint
    cmsBool addResult = cmsGDBAddPoint(hGBD, &lab);
    (void)addResult; // Suppress unused variable warning

    // Test cmsGDBCheckPoint
    cmsBool checkResult = cmsGDBCheckPoint(hGBD, &lab);
    (void)checkResult;

    // Test cmsDesaturateLab with varying limits
    double amax = (Size > 0) ? Data[0] / 100.0 : 0.5;
    double amin = (Size > 1) ? Data[1] / 100.0 : -0.5;
    double bmax = (Size > 2) ? Data[2] / 100.0 : 0.5;
    double bmin = (Size > 3) ? Data[3] / 100.0 : -0.5;
    cmsBool desatResult = cmsDesaturateLab(&lab, amax, amin, bmax, bmin);
    (void)desatResult;

    // Test cmsGDBCompute with different flags
    cmsUInt32Number flags = (Size > 4) ? Data[4] : 0;
    cmsBool computeResult = cmsGDBCompute(hGBD, flags);
    (void)computeResult;

    // Cleanup
    cmsGBDFree(hGBD);
    cleanup_context(ctx);

    return 0;
}