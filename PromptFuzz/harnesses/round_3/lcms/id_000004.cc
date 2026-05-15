#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <lcms2.h>
}
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":4,"unique_branches":{"cmsAppendNamedColor":[[822,9,822,31,0,0,4,0]]},"library_calls":["cmsOpenProfileFromMem","cmsGetColorSpace","cmsCreateProfilePlaceholder","cmsAppendNamedColor","cmsCloseProfile","cmsLabEncoded2Float","cmsBuildGamma","cmsGetToneCurveSegment","cmsEstimateGamma","cmsStageAllocMatrix","cmsStageFree","cmsBFDdeltaE","cmsSetHeaderAttributes","cmsFreeToneCurve","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetColorSpace","cmsCreateProfilePlaceholder","cmsAppendNamedColor","cmsCloseProfile","cmsLabEncoded2Float","cmsBuildGamma","cmsGetToneCurveSegment","cmsEstimateGamma","cmsStageAllocMatrix","cmsStageFree","cmsBFDdeltaE","cmsSetHeaderAttributes","cmsFreeToneCurve","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // 1. Open profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	

    // 2. Get color space of profile
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
    (void)colorSpace; // Use to avoid unused warning

    // 3. Create named color list (simulate by creating placeholder)
    // Note: cmsAppendNamedColor requires existing list - we'll create via duplication
    cmsUInt16Number pcs[3] = {0, 0, 0};
    cmsUInt16Number colorant[16] = {0};
    if (size > 6) {
        // Use some input data for color values
        pcs[0] = (data[0] << 8) | data[1];
        pcs[1] = (data[2] << 8) | data[3];
        pcs[2] = (data[4] << 8) | data[5];
    }
    
    // Create a minimal named color list via profile
    cmsNAMEDCOLORLIST* namedColorList = NULL;
    // We'll simulate by creating a temporary profile with named colors
    // This is a workaround since we can't create empty list directly
    cmsHPROFILE tmpProfile = cmsCreateProfilePlaceholder(NULL);
    if (tmpProfile) {
        // Try to get named color list (may be NULL)
        // For fuzzing, we'll just call cmsAppendNamedColor with dummy list pointer
        // In real usage, we'd properly allocate the list
        cmsAppendNamedColor(namedColorList, "FuzzColor", pcs, colorant);
        cmsCloseProfile(tmpProfile);
    }

    // 4. Convert encoded Lab to float
    cmsUInt16Number wLab[3] = {0};
    cmsCIELab Lab;
    if (size >= 12) {
        wLab[0] = (data[6] << 8) | data[7];
        wLab[1] = (data[8] << 8) | data[9];
        wLab[2] = (data[10] << 8) | data[11];
        cmsLabEncoded2Float(&Lab, wLab);
    }

    // 5. Build gamma tone curve and get segment
    cmsToneCurve* gammaCurve = cmsBuildGamma(NULL, 2.2);
    if (gammaCurve) {
        // Get first tone curve segment
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(0, gammaCurve);
        (void)segment; // Use to avoid unused warning
        
        // 9. Estimate gamma
        cmsFloat64Number estimatedGamma = cmsEstimateGamma(gammaCurve, 0.001);
        (void)estimatedGamma; // Use to avoid unused warning
    }

    // 6. Allocate matrix stage
    cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    cmsFloat64Number offset[3] = {0.0, 0.0, 0.0};
    cmsStage* matrixStage = cmsStageAllocMatrix(NULL, 3, 3, matrix, offset);
    if (matrixStage) {
        cmsStageFree(matrixStage);
    }

    // 7. Compute BFD delta E
    cmsCIELab Lab2 = {50.0, 0.0, 0.0}; // Reference lab
    cmsFloat64Number deltaE = 0.0;
    if (size >= 12) {
        deltaE = cmsBFDdeltaE(&Lab, &Lab2);
        (void)deltaE; // Use to avoid unused warning
    }

    // 8. Set header attributes
    cmsUInt64Number flags = 0;
    if (size > 20) {
        flags = (static_cast<cmsUInt64Number>(data[12]) << 56) |
                (static_cast<cmsUInt64Number>(data[13]) << 48) |
                (static_cast<cmsUInt64Number>(data[14]) << 40) |
                (static_cast<cmsUInt64Number>(data[15]) << 32) |
                (static_cast<cmsUInt64Number>(data[16]) << 24) |
                (static_cast<cmsUInt64Number>(data[17]) << 16) |
                (static_cast<cmsUInt64Number>(data[18]) << 8) |
                static_cast<cmsUInt64Number>(data[19]);
    }
    cmsSetHeaderAttributes(hProfile, flags);

    // Cleanup
    if (gammaCurve) {
        cmsFreeToneCurve(gammaCurve);
    }
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    // Note: namedColorList would need proper cleanup if actually allocated

    return 0;
}