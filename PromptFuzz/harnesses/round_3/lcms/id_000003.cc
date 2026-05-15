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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"cmsAppendNamedColor":[[824,9,824,66,0,0,4,0],[825,13,825,48,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreateProfilePlaceholder","cmsDeleteContext","cmsGetColorSpace","cmsBuildGamma","cmsBuildGamma","cmsGetToneCurveSegment","cmsEstimateGamma","cmsStageAllocMatrix","cmsAllocNamedColorList","cmsAppendNamedColor","cmsFreeNamedColorList","cmsLabEncoded2Float","cmsLabEncoded2Float","cmsBFDdeltaE","cmsSetHeaderAttributes","cmsStageFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreateProfilePlaceholder","cmsGetColorSpace","cmsBuildGamma","cmsBuildGamma","cmsGetToneCurveSegment","cmsEstimateGamma","cmsStageAllocMatrix","cmsAllocNamedColorList","cmsAppendNamedColor","cmsFreeNamedColorList","cmsLabEncoded2Float","cmsLabEncoded2Float","cmsBFDdeltaE","cmsSetHeaderAttributes","cmsStageFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful fuzzing

    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // 1. Create profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        // If profile creation fails, create a placeholder
        hProfile = cmsCreateProfilePlaceholder(ctx);
        if (!hProfile) {
            cmsDeleteContext(ctx);
            return 0;
        }
    }

    // 2. Get color space of profile
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
    (void)colorSpace; // Use variable to avoid unused warning

    // 3. Create and use tone curves
    cmsToneCurve* gammaCurve = cmsBuildGamma(ctx, 2.2);
    cmsToneCurve* gammaCurve2 = cmsBuildGamma(ctx, 1.8);
    
    if (gammaCurve && gammaCurve2) {
        // 4. Get tone curve segment (first segment)
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(0, gammaCurve);
        (void)segment; // Use variable
        
        // 5. Estimate gamma of tone curve
        cmsFloat64Number estimatedGamma = cmsEstimateGamma(gammaCurve, 0.001);
        (void)estimatedGamma;
    }

    // 6. Create matrix stage (3x3 identity matrix)
    cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    cmsFloat64Number offset[3] = {0.0, 0.0, 0.0};
    cmsStage* matrixStage = cmsStageAllocMatrix(ctx, 3, 3, matrix, offset);
    
    // 7. Create and use named color list
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(ctx, 0, 0, "Prefix", "Suffix");
    if (namedColorList) {
        cmsUInt16Number pcs[3] = {0x8000, 0x8000, 0x8000}; // Mid-gray
        cmsUInt16Number colorant[16] = {0};
        for (int i = 0; i < 16 && i < (int)size; i++) {
            colorant[i] = (cmsUInt16Number)(data[i] << 8);
        }
        
        // 8. Append named color
        cmsAppendNamedColor(namedColorList, "FuzzColor", pcs, colorant);
        
        // Clean up named color list
        cmsFreeNamedColorList(namedColorList);
    }

    // 9. Convert encoded Lab to float
    cmsUInt16Number encodedLab[3];
    if (size >= 6) {
        encodedLab[0] = (data[0] << 8) | data[1];
        encodedLab[1] = (data[2] << 8) | data[3];
        encodedLab[2] = (data[4] << 8) | data[5];
    } else {
        encodedLab[0] = 0x8000;
        encodedLab[1] = 0x0000;
        encodedLab[2] = 0x0000;
    }
    
    cmsCIELab lab1, lab2;
    cmsLabEncoded2Float(&lab1, encodedLab);
    
    // Create second Lab color for comparison
    cmsUInt16Number encodedLab2[3] = {0x6000, 0x1000, 0x2000};
    cmsLabEncoded2Float(&lab2, encodedLab2);
    
    // 10. Compute BFD deltaE
    cmsFloat64Number deltaE = cmsBFDdeltaE(&lab1, &lab2);
    (void)deltaE;

    // 11. Set header attributes
    // Use some bits from the input data for flags
    cmsUInt64Number flags = 0;
    if (size >= 8) {
        for (int i = 0; i < 8 && i < (int)size; i++) {
            flags = (flags << 8) | data[i];
        }
    }
    cmsSetHeaderAttributes(hProfile, flags);

    // Clean up
    if (matrixStage) cmsStageFree(matrixStage);
    if (gammaCurve) cmsFreeToneCurve(gammaCurve);
    if (gammaCurve2) cmsFreeToneCurve(gammaCurve2);
    if (hProfile) cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}