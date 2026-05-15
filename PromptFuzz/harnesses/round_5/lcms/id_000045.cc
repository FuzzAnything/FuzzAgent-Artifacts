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
//<ID> 256
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 14
//<Quality> {"density":10,"unique_branches":{"_cmsEndPointsBySpace":[[727,33,727,38,0,0,4,0],[728,32,728,37,0,0,4,1]],"cmsOpenProfileFromIOhandler2THR":[[1270,9,1270,23,0,0,4,1],[1275,9,1275,14,0,0,4,1],[1281,9,1281,32,0,0,4,0],[1281,9,1281,32,0,0,4,1]],"BuildGrayOutputPipeline":[[418,9,418,24,0,0,4,1],[421,9,421,27,0,0,4,1],[424,9,424,20,0,0,4,1],[429,9,429,45,0,0,4,1],[435,13,435,110,0,0,4,1],[439,9,439,100,0,0,4,1]],"_cmsICCcolorSpace":[[769,8,769,20,0,0,4,0]],"BuildRGBOutputMatrixShaper":[[464,9,464,37,0,0,4,0]]},"library_calls":["cmsUnregisterPluginsTHR","cmsOpenProfileFromFileTHR","cmsOpenIOhandlerFromMem","cmsOpenProfileFromIOhandler2THR","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsD50_xyY","cmsFreeToneCurve","cmsDetectBlackPoint","cmsGetPostScriptCRD","cmsXYZ2xyY","cmsXYZ2Lab","cmsDeltaE","cmsCIE94DeltaE","cmsCreate_sRGBProfile","cmsCreateLab4ProfileTHR","cmsCreateProofingTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsUnregisterPluginsTHR"],"critical_calls":["cmsUnregisterPluginsTHR","cmsOpenProfileFromFileTHR","cmsOpenIOhandlerFromMem","cmsOpenProfileFromIOhandler2THR","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsD50_xyY","cmsFreeToneCurve","cmsDetectBlackPoint","cmsGetPostScriptCRD","cmsXYZ2xyY","cmsXYZ2Lab","cmsDeltaE","cmsCIE94DeltaE","cmsCreate_sRGBProfile","cmsCreateLab4ProfileTHR","cmsCreateProofingTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsUnregisterPluginsTHR"],"visited":0}
/**/


// Include lcms headers

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // 1. Unregister plugins first
    cmsUnregisterPluginsTHR(NULL);
    
    // 2. Create a profile from input data
    cmsHPROFILE hProfile = NULL;
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        hProfile = cmsOpenProfileFromFileTHR(NULL, "input_file", "r");
        assert_file_closed(&in_file);;
    }
    
    if (!hProfile) {
        // Try creating a profile from memory directly
        cmsIOHANDLER* io = cmsOpenIOhandlerFromMem(NULL, (void*)data, size, "r");
        if (io) {
            hProfile = cmsOpenProfileFromIOhandler2THR(NULL, io, 0);
            // Note: io handler is owned by the profile, no need to free separately
        }
    }
    
    if (!hProfile) {
        // If still no profile, create a simple one for testing
        cmsToneCurve* gamma = cmsBuildGamma(NULL, 2.2);
        hProfile = cmsCreateGrayProfileTHR(NULL, cmsD50_xyY(), gamma);
        cmsFreeToneCurve(gamma);
    }
    
    // 3. Detect black point
    cmsCIEXYZ blackPoint;
    cmsBool blackDetected = cmsDetectBlackPoint(&blackPoint, hProfile, INTENT_PERCEPTUAL, 0);
    
    // 4. Get PostScript CRD
    cmsUInt32Number bufferSize = 4096;
    void* buffer = malloc(bufferSize);
    if (buffer) {
        cmsUInt32Number crdSize = cmsGetPostScriptCRD(NULL, hProfile, INTENT_PERCEPTUAL, 0, buffer, bufferSize);
        // Use the result to avoid optimization
        if (crdSize > bufferSize) {
            // Buffer was too small
        }
        free(buffer);
    }
    
    // 5. Create Lab colors for DeltaE calculations
    cmsCIELab lab1, lab2;
    
    // Convert black point to Lab if detected
    if (blackDetected) {
        cmsCIEXYZ whitePoint;
        cmsCIExyY xyY;
        cmsXYZ2xyY(&xyY, &blackPoint);
        
        // Create two similar Lab colors from the black point
        cmsXYZ2Lab(NULL, &lab1, &blackPoint);
        lab2 = lab1;
        lab2.L += 1.0;  // Slight difference
        lab2.a += 0.5;
        lab2.b += 0.5;
    } else {
        // Use default Lab values
        lab1.L = 50.0; lab1.a = 0.0; lab1.b = 0.0;
        lab2.L = 51.0; lab2.a = 1.0; lab2.b = 1.0;
    }
    
    // 6. Calculate DeltaE values
    cmsFloat64Number deltaE76 = cmsDeltaE(&lab1, &lab2);
    cmsFloat64Number deltaE94 = cmsCIE94DeltaE(&lab1, &lab2);
    
    // Use the results to avoid dead code elimination
    if (deltaE76 < 0 || deltaE94 < 0) {
        // This should never happen, but prevents optimization
    }
    
    // 7. Create proofing transform
    cmsHPROFILE proofingProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE outputProfile = cmsCreateLab4ProfileTHR(NULL, NULL);
    
    if (hProfile && proofingProfile && outputProfile) {
        cmsHTRANSFORM hTransform = cmsCreateProofingTransform(
            hProfile,
            TYPE_Lab_DBL,      // Input format
            outputProfile,     // Output profile
            TYPE_Lab_DBL,      // Output format
            proofingProfile,   // Proofing profile
            INTENT_PERCEPTUAL, // Intent
            INTENT_ABSOLUTE_COLORIMETRIC, // Proofing intent
            0                  // Flags
        );
        
        if (hTransform) {
            // Test the transform with some data
            cmsCIELab inputLab, outputLab;
            inputLab.L = 50.0; inputLab.a = 0.0; inputLab.b = 0.0;
            
            cmsDoTransform(hTransform, &inputLab, &outputLab, 1);
            
            cmsDeleteTransform(hTransform);
        }
    }
    
    // 8. Clean up
    if (proofingProfile) cmsCloseProfile(proofingProfile);
    if (outputProfile) cmsCloseProfile(outputProfile);
    if (hProfile) cmsCloseProfile(hProfile);
    
    // 9. Unregister plugins again at the end
    cmsUnregisterPluginsTHR(NULL);
    
    assert_file_closed(&in_file);
	return 0;
}