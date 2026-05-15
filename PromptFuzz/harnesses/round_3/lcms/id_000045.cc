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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 2
//<Quality> {"density":17,"unique_branches":{"cmsReadRawTag":[[2040,17,2040,37,0,0,4,0]],"cmsOpenProfileFromIOhandlerTHR":[[1256,9,1256,32,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsSetAdaptationState","cmsDeleteContext","cmsOpenProfileFromFile","cmsDeleteContext","cmsReadRawTag","cmsOpenIOhandlerFromMem","cmsOpenProfileFromIOhandlerTHR","cmsBuildParametricToneCurve","cmsReverseToneCurveEx","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyStr","cmsIT8EnumDataFormat","cmsCreateTransform","cmsXYZ2Lab","cmsDoTransformLineStride","cmsDeleteTransform","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetAdaptationState","cmsOpenProfileFromFile","cmsReadRawTag","cmsOpenIOhandlerFromMem","cmsOpenProfileFromIOhandlerTHR","cmsBuildParametricToneCurve","cmsReverseToneCurveEx","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyStr","cmsIT8EnumDataFormat","cmsCreateTransform","cmsXYZ2Lab","cmsDoTransformLineStride","cmsDeleteTransform","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


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
	  // Need minimal data for meaningful operations

    // Create a context for thread-safe operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 1. Set adaptation state (neural adaptation factor for chromatic adaptation)
    cmsSetAdaptationState(1.0);  // Reset to default

    // 2. Write input data to a temporary file for cmsOpenProfileFromFile
    FILE* temp_file = fopen("input_file", "wb");
    if (!temp_file) {
        cmsDeleteContext(context);
        assert_file_closed(&temp_file);
	return 0;
    }
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;

    // 3. Open ICC profile from file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("input_file", "r");
    if (!hProfile) {
        cmsDeleteContext(context);
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 4. Read raw tag from profile (try reading description tag)
    char tagBuffer[256];
    cmsUInt32Number bytesRead = cmsReadRawTag(hProfile, cmsSigProfileDescriptionTag, 
                                              tagBuffer, sizeof(tagBuffer));
    
    // 5. Create I/O handler from memory and open profile
    cmsIOHANDLER* io = cmsOpenIOhandlerFromMem(context, (void*)data, 
                                                (cmsUInt32Number)size, "r");
    cmsHPROFILE memProfile = nullptr;
    if (io) {
        memProfile = cmsOpenProfileFromIOhandlerTHR(context, io);
        // Note: io is owned by the profile, don't free separately
    }

    // 6. Create a simple tone curve and reverse it
    cmsFloat64Number gammaParams[5] = {2.4, 1.0, 0.0, 0.0, 0.0};
    cmsToneCurve* toneCurve = cmsBuildParametricToneCurve(context, 
                                                          1, gammaParams);
    cmsToneCurve* reversedCurve = nullptr;
    if (toneCurve) {
        reversedCurve = cmsReverseToneCurveEx(256, toneCurve);
    }

    // 7. Create IT8 handle and enumerate data formats
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    char** sampleNames = nullptr;
    int formatCount = 0;
    if (hIT8) {
        // Set some properties to make IT8 data valid
        cmsIT8SetSheetType(hIT8, "CGATS.17");
        cmsIT8SetPropertyStr(hIT8, "ORIGINATOR", "Fuzzer");
        formatCount = cmsIT8EnumDataFormat(hIT8, &sampleNames);
    }

    // 8. Create a simple transform for testing stride operations
    cmsHTRANSFORM hTransform = nullptr;
    if (hProfile && memProfile) {
        hTransform = cmsCreateTransform(hProfile, 
                                        TYPE_RGB_8, 
                                        memProfile, 
                                        TYPE_RGB_8, 
                                        INTENT_PERCEPTUAL, 0);
    }

    // 9. Perform XYZ to Lab conversion
    cmsCIEXYZ whitePoint = {0.95047, 1.00000, 1.08883};  // D65
    cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
    cmsCIELab lab;
    cmsXYZ2Lab(&whitePoint, &lab, &xyz);

    // 10. Test transform with stride if we have a valid transform
    if (hTransform && size >= 300) {
        // Create input and output buffers for stride transform
        const int pixelsPerLine = 10;
        const int lineCount = 2;
        const int bytesPerLineIn = 30;  // 10 pixels * 3 bytes (RGB)
        const int bytesPerLineOut = 30;
        const int bytesPerPlaneIn = 30;  // Planar not used, same as bytesPerLine
        const int bytesPerPlaneOut = 30;
        
        uint8_t inputBuffer[60];  // 2 lines * 30 bytes
        uint8_t outputBuffer[60];
        
        // Use first 60 bytes of input data for the buffer
        size_t copySize = size < 60 ? size : 60;
        memcpy(inputBuffer, data, copySize);
        
        cmsDoTransformLineStride(hTransform, inputBuffer, outputBuffer,
                                 pixelsPerLine, lineCount,
                                 bytesPerLineIn, bytesPerLineOut,
                                 bytesPerPlaneIn, bytesPerPlaneOut);
    }

    // Cleanup
    if (hTransform) cmsDeleteTransform(hTransform);
    if (reversedCurve) cmsFreeToneCurve(reversedCurve);
    if (toneCurve) cmsFreeToneCurve(toneCurve);
    if (hIT8) cmsIT8Free(hIT8);
    if (memProfile) cmsCloseProfile(memProfile);
    if (hProfile) cmsCloseProfile(hProfile);
    
    // Clean up temporary file
    remove("input_file");
    
    cmsDeleteContext(context);
    assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}