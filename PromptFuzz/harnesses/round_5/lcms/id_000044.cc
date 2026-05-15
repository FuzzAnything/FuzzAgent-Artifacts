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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"RemoveCR":[[326,32,326,43,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsUnregisterPluginsTHR","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsCreateProfilePlaceholder","cmsDeleteContext","cmsDetectBlackPoint","cmsDeltaE","cmsCIE94DeltaE","cmsCreateProofingTransform","cmsDoTransform","cmsDeleteTransform","cmsGetPostScriptCRD","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsUnregisterPluginsTHR","cmsOpenProfileFromFileTHR","cmsCreateProfilePlaceholder","cmsDetectBlackPoint","cmsDeltaE","cmsCIE94DeltaE","cmsCreateProofingTransform","cmsDoTransform","cmsDeleteTransform","cmsGetPostScriptCRD","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimal data for operations

    // Create a context and unregister plugins
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    cmsUnregisterPluginsTHR(context);
    
    // Open profile from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!hProfile) {
        // If file opening fails, try creating a placeholder profile
        hProfile = cmsCreateProfilePlaceholder(context);
        if (!hProfile) {
            assert_file_closed(&in_file);;
            cmsDeleteContext(context);
            assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
        }
    }
    
    // Detect black point
    cmsCIEXYZ blackPoint;
    cmsDetectBlackPoint(&blackPoint, hProfile, INTENT_PERCEPTUAL, 0);
    
    // Create two Lab colors for Delta E calculations
    cmsCIELab lab1, lab2;
    // Initialize with some values (using data bytes for variability)
    lab1.L = (data[0] / 255.0) * 100.0;
    lab1.a = ((int8_t)data[1]) * 128.0 / 127.0;
    lab1.b = ((int8_t)data[2]) * 128.0 / 127.0;
    
    lab2.L = (data[3] / 255.0) * 100.0;
    lab2.a = ((int8_t)data[4]) * 128.0 / 127.0;
    lab2.b = ((int8_t)data[5]) * 128.0 / 127.0;
    
    // Calculate Delta E values
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
    cmsFloat64Number deltaE94 = cmsCIE94DeltaE(&lab1, &lab2);
    
    // Create proofing transform (using same profile for all for simplicity)
    cmsUInt32Number inputFormat = TYPE_Lab_DBL;
    cmsUInt32Number outputFormat = TYPE_RGB_DBL;
    
    cmsHTRANSFORM hTransform = cmsCreateProofingTransform(
        hProfile, inputFormat,
        hProfile, outputFormat,
        hProfile, INTENT_PERCEPTUAL, INTENT_ABSOLUTE_COLORIMETRIC, 0);
    
    if (hTransform) {
        // Transform some sample data
        cmsFloat64Number labInput[3] = {50.0, 0.0, 0.0};
        cmsFloat64Number rgbOutput[3];
        cmsDoTransform(hTransform, labInput, rgbOutput, 1);
        
        cmsDeleteTransform(hTransform);
    }
    
    // Get PostScript CRD
    cmsUInt32Number bufferSize = 4096;
    void *buffer = malloc(bufferSize);
    if (buffer) {
        cmsUInt32Number crdSize = cmsGetPostScriptCRD(
            context, hProfile, INTENT_PERCEPTUAL, 0, buffer, bufferSize);
        free(buffer);
    }
    
    // Clean up
    if (hProfile) cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}