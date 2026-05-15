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
//<ID> 257
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 8
//<Quality> {"density":13,"unique_branches":{"AddConversion":[[440,17,440,38,0,0,4,0],[441,17,441,128,0,0,4,1]],"BuildGrayInputMatrixPipeline":[[163,9,163,24,0,0,4,0]],"cmsCreateExtendedTransform":[[1154,9,1154,38,0,0,4,0],[1155,13,1155,34,0,0,4,1],[1158,9,1158,40,0,0,4,0],[1158,45,1158,67,0,0,4,1],[1158,71,1158,105,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsCreate_sRGBProfile","cmsDeleteContext","cmsCreate_sRGBProfile","cmsCreateLab4ProfileTHR","cmsCreateNULLProfileTHR","cmsCreateProofingTransform","cmsDeltaE","cmsCIE94DeltaE","cmsDetectBlackPoint","cmsGetPostScriptCRD","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsUnregisterPluginsTHR","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreate_sRGBProfile","cmsCreate_sRGBProfile","cmsCreateLab4ProfileTHR","cmsCreateNULLProfileTHR","cmsCreateProofingTransform","cmsDeltaE","cmsCIE94DeltaE","cmsDetectBlackPoint","cmsGetPostScriptCRD","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsUnregisterPluginsTHR","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data for meaningful processing
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Create a temporary file to simulate profile data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open profile from file (simulating reading ICC profile from fuzzed data)
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    
    // If we couldn't open from file, create a synthetic profile from memory data
    if (!hProfile) {
        // Create a simple sRGB profile as fallback
        hProfile = cmsCreate_sRGBProfile();
    }
    
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Create additional profiles for proofing transform
    cmsHPROFILE hInput = cmsCreate_sRGBProfile();
    cmsHPROFILE hOutput = cmsCreateLab4ProfileTHR(context, nullptr);
    cmsHPROFILE hProofing = cmsCreateNULLProfileTHR(context);
    
    if (hInput && hOutput && hProofing) {
        // Create proofing transform
        cmsHTRANSFORM hTransform = cmsCreateProofingTransform(
            hInput,
            TYPE_RGB_8,
            hOutput,
            TYPE_Lab_8,
            hProofing,
            INTENT_PERCEPTUAL,
            INTENT_RELATIVE_COLORIMETRIC,
            cmsFLAGS_GAMUTCHECK | cmsFLAGS_SOFTPROOFING
        );
        
        if (hTransform) {
            // Prepare Lab colors for DeltaE calculations
            cmsCIELab lab1, lab2;
            
            // Extract some values from fuzzed data for Lab colors
            if (size >= sizeof(cmsCIELab) * 2) {
                memcpy(&lab1, data, sizeof(cmsCIELab));
                memcpy(&lab2, data + sizeof(cmsCIELab), sizeof(cmsCIELab));
            } else {
                // Use default values if not enough data
                lab1.L = 50.0; lab1.a = 0.0; lab1.b = 0.0;
                lab2.L = 55.0; lab2.a = 10.0; lab2.b = 10.0;
            }
            
            // Calculate DeltaE metrics
            cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
            cmsFloat64Number cie94DeltaE = cmsCIE94DeltaE(&lab1, &lab2);
            
            // Detect black point
            cmsCIEXYZ blackPoint;
            cmsBool blackPointDetected = cmsDetectBlackPoint(&blackPoint, hProfile, 
                                                           INTENT_PERCEPTUAL, 0);
            
            // Get PostScript CRD
            cmsUInt32Number bufferSize = 4096;
            uint8_t* buffer = new uint8_t[bufferSize];
            cmsUInt32Number crdSize = cmsGetPostScriptCRD(context, hProfile, 
                                                         INTENT_PERCEPTUAL, 0, 
                                                         buffer, bufferSize);
            
            // Clean up PostScript buffer
            delete[] buffer;
            
            // Delete transform
            cmsDeleteTransform(hTransform);
        }
    }
    
    // Clean up profiles
    if (hInput) cmsCloseProfile(hInput);
    if (hOutput) cmsCloseProfile(hOutput);
    if (hProofing) cmsCloseProfile(hProofing);
    if (hProfile) cmsCloseProfile(hProfile);
    
    // Unregister plugins
    cmsUnregisterPluginsTHR(context);
    
    // Clean up file and context
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}