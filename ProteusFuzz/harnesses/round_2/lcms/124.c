// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:358:22 in lcms2.h
// cmsCIECAM02Forward at cmscam02.c:435:16 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:461:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:427:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void fuzz_ciecam02(cmsContext ctx, const uint8_t *Data, size_t Size) {
    if (Size < 32) return;

    cmsViewingConditions vc;
    memset(&vc, 0, sizeof(vc));
    
    // Parse white point (3 doubles = 24 bytes)
    if (Size >= 24) {
        memcpy(&vc.whitePoint, Data, 24);
        Data += 24;
        Size -= 24;
    }
    
    // Parse surround type (1 uint32 = 4 bytes)
    if (Size >= 4) {
        memcpy(&vc.surround, Data, 4);
        Data += 4;
        Size -= 4;
    }
    
    // The field 'adaptingLuminance' does not exist in this version of lcms2.
    // We will set the ambientLuminance or leave it as 0.0 (default) if the field is missing.
    // In some versions, the structure might only have whitePoint and surround.
    // We will not access the non-existent field to avoid compilation errors.
    // If the library expects a specific value, the default 0.0 might be handled internally.
    
    // Initialize CIECAM02 model
    cmsHANDLE hModel = cmsCIECAM02Init(ctx, &vc);
    if (!hModel) return;

    // Prepare XYZ input (3 doubles = 24 bytes)
    cmsCIEXYZ xyzIn;
    memset(&xyzIn, 0, sizeof(xyzIn));
    if (Size >= 24) {
        memcpy(&xyzIn, Data, 24);
        Data += 24;
        Size -= 24;
    }

    // Prepare JCh output
    cmsJCh jchOut;
    cmsCIEXYZ xyzOut;

    // Forward conversion: XYZ -> JCh
    cmsCIECAM02Forward(hModel, &xyzIn, &jchOut);

    // Reverse conversion: JCh -> XYZ
    cmsCIECAM02Reverse(hModel, &jchOut, &xyzOut);

    // Clean up model
    cmsCIECAM02Done(hModel);
}

static void fuzz_adaptation(cmsContext ctx, const uint8_t *Data, size_t Size) {
    if (Size < 72) return;

    cmsCIEXYZ sourceWhite, illuminant, value, result;
    
    // Parse source white point (24 bytes)
    memcpy(&sourceWhite, Data, 24);
    Data += 24;
    Size -= 24;
    
    // Parse target illuminant (24 bytes)
    memcpy(&illuminant, Data, 24);
    Data += 24;
    Size -= 24;
    
    // Parse input value (24 bytes)
    memcpy(&value, Data, 24);
    Data += 24;
    Size -= 24;

    cmsBool ret = cmsAdaptToIlluminant(&result, &sourceWhite, &illuminant, &value);
    (void)ret; // Suppress unused variable warning
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a new context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Fuzz CIECAM02 functions
    fuzz_ciecam02(ctx, Data, Size);

    // Fuzz adaptation function
    fuzz_adaptation(ctx, Data, Size);

    // Test with D50 constant
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    if (d50) {
        cmsCIEXYZ dummy;
        cmsAdaptToIlluminant(&dummy, d50, d50, d50);
    }

    // Clean up context
    cmsDeleteContext(ctx);
    return 0;
}