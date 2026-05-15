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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"_cmsLinkProfiles":[[1126,13,1126,58,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsUnregisterPluginsTHR","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsDeleteContext","cmsDetectBlackPoint","cmsGetPostScriptCRD","cmsDeltaE","cmsCIE94DeltaE","cmsCreate_sRGBProfile","cmsCreateLab4ProfileTHR","cmsCreateProofingTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsUnregisterPluginsTHR","cmsOpenProfileFromFileTHR","cmsDetectBlackPoint","cmsGetPostScriptCRD","cmsDeltaE","cmsCIE94DeltaE","cmsCreate_sRGBProfile","cmsCreateLab4ProfileTHR","cmsCreateProofingTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Assuming lcms2.h is included

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimal data for meaningful operations

    // 1. Create context and unregister plugins
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    cmsUnregisterPluginsTHR(context);  // First required API
    
    // 2. Create a profile from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Detect black point using the profile
    cmsCIEXYZ blackPoint;
    cmsBool blackDetected = cmsDetectBlackPoint(&blackPoint, hProfile, INTENT_PERCEPTUAL, 0);  // Second required API
    (void)blackDetected; // Use result to avoid unused variable warning
    
    // 4. Get PostScript CRD
    cmsUInt32Number bufferSize = 4096;
    void *buffer = malloc(bufferSize);
    if (buffer) {
        cmsUInt32Number crdSize = cmsGetPostScriptCRD(context, hProfile, INTENT_PERCEPTUAL, 0, buffer, bufferSize);  // Third required API
        (void)crdSize;
        free(buffer);
    }
    
    // 5. Create two Lab colors for delta E calculations
    cmsCIELab lab1, lab2;
    // Initialize with some values (using first bytes of input)
    lab1.L = (double)(data[0] / 255.0) * 100.0;
    lab1.a = (double)(data[1] - 128);
    lab1.b = (double)(data[2] - 128);
    
    lab2.L = (double)(data[3] / 255.0) * 100.0;
    lab2.a = (double)(data[4] - 128);
    lab2.b = (double)(data[5] - 128);
    
    // Calculate delta E using two different methods
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);  // Fourth required API
    cmsFloat64Number deltaE94 = cmsCIE94DeltaE(&lab1, &lab2);  // Fifth required API
    (void)deltaE;
    (void)deltaE94;
    
    // 6. Create proofing transform
    // Create additional profiles for proofing
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, NULL);
    
    if (sRGBProfile && labProfile) {
        cmsHTRANSFORM proofingTransform = cmsCreateProofingTransform(
            hProfile, 
            TYPE_Lab_DBL, 
            sRGBProfile, 
            TYPE_RGB_DBL, 
            labProfile, 
            INTENT_PERCEPTUAL, 
            INTENT_ABSOLUTE_COLORIMETRIC, 
            cmsFLAGS_SOFTPROOFING  // Sixth required API
        );
        
        if (proofingTransform) {
            // Transform some sample data
            cmsCIELab inputLab;
            cmsUInt8Number outputRGB[3];
            
            // Use more input bytes for sample data
            for (size_t i = 0; i < 3 && (i*3+6) < size; i++) {
                inputLab.L = (double)(data[i*3+6] / 255.0) * 100.0;
                inputLab.a = (double)(data[i*3+7] - 128);
                inputLab.b = (double)(data[i*3+8] - 128);
                
                cmsDoTransform(proofingTransform, &inputLab, outputRGB, 1);
            }
            
            cmsDeleteTransform(proofingTransform);
        }
    }
    
    // 7. Clean up all resources
    if (sRGBProfile) cmsCloseProfile(sRGBProfile);
    if (labProfile) cmsCloseProfile(labProfile);
    if (hProfile) cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}