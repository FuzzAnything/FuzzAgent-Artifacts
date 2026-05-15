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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 8
//<Quality> {"density":14,"unique_branches":{"cmsAppendNamedColor":[[828,15,828,49,0,0,4,0],[829,77,829,93,0,0,4,1]],"ComputeLBFD":[[486,7,486,24,0,0,4,0],[486,7,486,24,0,0,4,1]],"cmsGetToneCurveSegment":[[1511,9,1511,14,0,0,4,1],[1511,18,1511,52,0,0,4,1]],"cmsBFDdeltaE":[[517,9,517,51,0,0,4,0],[517,9,517,51,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDeleteContext","cmsGetColorSpace","cmsAllocNamedColorList","cmsAppendNamedColor","cmsFreeNamedColorList","cmsLabEncoded2Float","cmsBFDdeltaE","cmsBuildGamma","cmsGetToneCurveSegment","cmsEstimateGamma","cmsFreeToneCurve","cmsStageAllocMatrix","cmsStageFree","cmsBuildGamma","cmsFreeToneCurve","cmsSetHeaderAttributes","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsGetColorSpace","cmsAllocNamedColorList","cmsAppendNamedColor","cmsFreeNamedColorList","cmsLabEncoded2Float","cmsBFDdeltaE","cmsBuildGamma","cmsGetToneCurveSegment","cmsEstimateGamma","cmsFreeToneCurve","cmsStageAllocMatrix","cmsStageFree","cmsBuildGamma","cmsFreeToneCurve","cmsSetHeaderAttributes","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to create a color management event:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // 1. Open profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Get color space of profile
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
    
    // 3. Create and use named color list
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(context, 10, 3, "", "");
    if (namedColorList) {
        cmsUInt16Number pcs[3] = {0x8000, 0x8000, 0x8000};
        cmsUInt16Number colorant[16] = {0};
        
        // Use some input bytes for color values
        for (int i = 0; i < 3 && i * 2 + 2 < size; i++) {
            pcs[i] = (data[i * 2] << 8) | data[i * 2 + 1];
        }
        
        cmsAppendNamedColor(namedColorList, "FuzzColor", pcs, colorant);
        cmsFreeNamedColorList(namedColorList);
    }
    
    // 4. Convert encoded Lab to float
    if (size >= 6) {
        cmsUInt16Number wLab[3];
        cmsCIELab Lab;
        
        for (int i = 0; i < 3; i++) {
            size_t offset = 10 + i * 2;
            if (offset + 1 < size) {
                wLab[i] = (data[offset] << 8) | data[offset + 1];
            } else {
                wLab[i] = 0x8000;
            }
        }
        
        cmsLabEncoded2Float(&Lab, wLab);
        
        // 8. Compute BFD deltaE between two Lab colors
        cmsCIELab Lab2 = {50.0, 0.0, 0.0};
        cmsFloat64Number deltaE = cmsBFDdeltaE(&Lab, &Lab2);
        (void)deltaE; // Prevent unused warning
    }
    
    // 5. Create tone curve and get segment
    cmsToneCurve* toneCurve = cmsBuildGamma(context, 2.2);
    if (toneCurve) {
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(0, toneCurve);
        (void)segment; // Prevent unused warning
        
        // 10. Estimate gamma of tone curve
        cmsFloat64Number estimatedGamma = cmsEstimateGamma(toneCurve, 0.001);
        (void)estimatedGamma; // Prevent unused warning
        
        cmsFreeToneCurve(toneCurve);
    }
    
    // 6. Create matrix stage
    if (size >= 44) { // Need enough data for matrix (12 doubles = 96 bytes)
        cmsFloat64Number matrix[9];
        cmsFloat64Number offset[3];
        
        // Use input data for matrix values (simplified - using bytes)
        for (int i = 0; i < 9; i++) {
            size_t offset_idx = 20 + i;
            if (offset_idx < size) {
                matrix[i] = data[offset_idx] / 255.0;
            } else {
                matrix[i] = (i % 4 == 0) ? 1.0 : 0.0;
            }
        }
        
        for (int i = 0; i < 3; i++) {
            size_t offset_idx = 30 + i;
            if (offset_idx < size) {
                offset[i] = data[offset_idx] / 255.0;
            } else {
                offset[i] = 0.0;
            }
        }
        
        cmsStage* matrixStage = cmsStageAllocMatrix(context, 3, 3, matrix, offset);
        if (matrixStage) {
            cmsStageFree(matrixStage);
        }
    }
    
    // 7. Build another gamma curve
    cmsToneCurve* gammaCurve = cmsBuildGamma(context, 1.8);
    if (gammaCurve) {
        cmsFreeToneCurve(gammaCurve);
    }
    
    // 9. Set header attributes
    if (size >= 8) {
        cmsUInt64Number flags = 0;
        for (int i = 0; i < 8 && 40 + i < size; i++) {
            flags = (flags << 8) | data[40 + i];
        }
        cmsSetHeaderAttributes(hProfile, flags);
    }
    
    // Cleanup
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}