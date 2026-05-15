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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["cmsOpenProfileFromMem","cmsGetColorSpace","cmsAllocNamedColorList","cmsAppendNamedColor","cmsFreeNamedColorList","cmsLabEncoded2Float","cmsBuildGamma","cmsGetToneCurveSegment","cmsEstimateGamma","cmsFreeToneCurve","cmsStageAllocMatrix","cmsStageFree","cmsBFDdeltaE","cmsSetHeaderAttributes","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetColorSpace","cmsAllocNamedColorList","cmsAppendNamedColor","cmsFreeNamedColorList","cmsLabEncoded2Float","cmsBuildGamma","cmsGetToneCurveSegment","cmsEstimateGamma","cmsFreeToneCurve","cmsStageAllocMatrix","cmsStageFree","cmsBFDdeltaE","cmsSetHeaderAttributes","cmsCloseProfile"],"visited":0}
/**/


// Include necessary lcms headers

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
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
    cmsColorSpaceSignature colorspace = cmsGetColorSpace(hProfile);
    (void)colorspace; // Use to avoid unused variable warning
    
    // 3. Create a named color list and append a color
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(NULL, 10, 3, "Prefix", "Suffix");
    if (namedColorList) {
        cmsUInt16Number PCS[3] = {32768, 32768, 32768};
        cmsUInt16Number Colorant[16] = {0};
        if (size > 20) {
            // Use some data to populate colorant values
            for (int i = 0; i < 16 && i*2+1 < size; i++) {
                Colorant[i] = (data[i*2] << 8) | data[i*2+1];
            }
        }
        cmsAppendNamedColor(namedColorList, "FuzzColor", PCS, Colorant);
        cmsFreeNamedColorList(namedColorList);
    }
    
    // 4. Convert encoded Lab to float
    cmsCIELab LabFloat;
    cmsUInt16Number wLab[3] = {32768, 32768, 32768};
    if (size > 6) {
        // Use some data for encoded Lab values
        for (int i = 0; i < 3 && i*2+1 < size; i++) {
            wLab[i] = (data[i*2] << 8) | data[i*2+1];
        }
    }
    cmsLabEncoded2Float(&LabFloat, wLab);
    
    // 5. Build gamma curve and get segment
    cmsFloat64Number gamma = 2.2;
    if (size > 0) {
        gamma = 1.0 + (data[0] / 255.0) * 4.0; // Gamma between 1.0 and 5.0
    }
    cmsToneCurve* toneCurve = cmsBuildGamma(NULL, gamma);
    if (toneCurve) {
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(0, toneCurve);
        (void)segment; // Use to avoid unused variable warning
        
        // 9. Estimate gamma from tone curve
        cmsFloat64Number estimatedGamma = cmsEstimateGamma(toneCurve, 0.001);
        (void)estimatedGamma; // Use to avoid unused variable warning
        
        cmsFreeToneCurve(toneCurve);
    }
    
    // 6. Create matrix stage
    cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    cmsFloat64Number offset[3] = {0.0, 0.0, 0.0};
    if (size > 50) {
        // Use data to populate matrix values
        for (int i = 0; i < 9 && i*4+3 < size; i++) {
            matrix[i] = ((double)data[i*4] / 255.0) * 2.0 - 1.0;
        }
        for (int i = 0; i < 3 && i*4+40 < size; i++) {
            offset[i] = ((double)data[i*4+40] / 255.0) * 2.0 - 1.0;
        }
    }
    cmsStage* matrixStage = cmsStageAllocMatrix(NULL, 3, 3, matrix, offset);
    if (matrixStage) {
        cmsStageFree(matrixStage);
    }
    
    // 7. Compute BFD deltaE between two Lab colors
    cmsCIELab Lab1, Lab2;
    if (size > 30) {
        // Initialize Lab1 from data
        Lab1.L = ((double)data[10] / 255.0) * 100.0;
        Lab1.a = ((double)data[11] / 255.0) * 128.0 - 64.0;
        Lab1.b = ((double)data[12] / 255.0) * 128.0 - 64.0;
        
        // Initialize Lab2 from data
        Lab2.L = ((double)data[20] / 255.0) * 100.0;
        Lab2.a = ((double)data[21] / 255.0) * 128.0 - 64.0;
        Lab2.b = ((double)data[22] / 255.0) * 128.0 - 64.0;
        
        cmsFloat64Number deltaE = cmsBFDdeltaE(&Lab1, &Lab2);
        (void)deltaE; // Use to avoid unused variable warning
    }
    
    // 8. Set header attributes
    cmsUInt64Number flags = 0;
    if (size > 10) {
        flags = (cmsUInt64Number)data[5] << 56 |
                (cmsUInt64Number)data[6] << 48 |
                (cmsUInt64Number)data[7] << 40 |
                (cmsUInt64Number)data[8] << 32 |
                (cmsUInt64Number)data[9] << 24 |
                (cmsUInt64Number)data[10] << 16 |
                (cmsUInt64Number)data[11] << 8 |
                data[12];
    }
    cmsSetHeaderAttributes(hProfile, flags);
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}