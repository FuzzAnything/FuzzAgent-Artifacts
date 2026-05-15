// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:363:22 in lcms2.h
// cmsCIECAM02Forward at cmscam02.c:440:16 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:466:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:432:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIEXYZ) * 3 + sizeof(cmsCIELab) + sizeof(cmsJCh)) {
        return 0;
    }

    // Parse input data into required structures
    const cmsCIEXYZ *sourceWhitePt = (const cmsCIEXYZ *)Data;
    const cmsCIEXYZ *illuminant = (const cmsCIEXYZ *)(Data + sizeof(cmsCIEXYZ));
    const cmsCIEXYZ *xyzValue = (const cmsCIEXYZ *)(Data + 2 * sizeof(cmsCIEXYZ));
    const cmsCIELab *labInput = (const cmsCIELab *)(Data + 3 * sizeof(cmsCIEXYZ));
    const cmsJCh *jchInput = (const cmsJCh *)(Data + 3 * sizeof(cmsCIEXYZ) + sizeof(cmsCIELab));

    // Test cmsAdaptToIlluminant
    cmsCIEXYZ adaptedResult;
    cmsAdaptToIlluminant(&adaptedResult, sourceWhitePt, illuminant, xyzValue);

    // Test cmsXYZ2Lab with various white points
    cmsCIELab labResult1, labResult2;
    cmsXYZ2Lab(NULL, &labResult1, xyzValue);  // NULL white point (default D50)
    cmsXYZ2Lab(sourceWhitePt, &labResult2, xyzValue);  // Custom white point

    // Test cmsD50_XYZ
    const cmsCIEXYZ *d50 = cmsD50_XYZ();

    // Test cmsCIECAM02Forward and cmsCIECAM02Reverse
    cmsJCh jchResult;
    cmsCIEXYZ xyzResult;
    
    // Create a CIECAM02 model with default parameters
    cmsViewingConditions vc;
    memset(&vc, 0, sizeof(vc));
    vc.whitePoint = *d50;
    vc.Yb = 20.0;   // Relative luminance of background
    vc.surround = 1;  // Average surround (integer type)
    
    cmsHANDLE hModel = cmsCIECAM02Init(NULL, &vc);
    if (hModel) {
        cmsCIECAM02Forward(hModel, xyzValue, &jchResult);
        cmsCIECAM02Reverse(hModel, jchInput, &xyzResult);
        cmsCIECAM02Done(hModel);
    }

    // Test cmsLab2XYZ with various white points
    cmsCIEXYZ xyzFromLab1, xyzFromLab2;
    cmsLab2XYZ(NULL, &xyzFromLab1, labInput);  // NULL white point (default D50)
    cmsLab2XYZ(sourceWhitePt, &xyzFromLab2, labInput);  // Custom white point

    return 0;
}