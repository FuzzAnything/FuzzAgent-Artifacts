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
//<ID> 422
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"cmsDictGetEntryList":[[1194,9,1194,21,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsSetAdaptationState","cmsDeleteContext","cmsOpenProfileFromFile","cmsGetEncodedICCversion","cmsOpenIOhandlerFromNULL","cmsOpenProfileFromIOhandlerTHR","cmsReadRawTag","cmsBuildGamma","cmsReverseToneCurveEx","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsCreateExtendedTransform","cmsDictAlloc","cmsDictGetEntryList","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8SetDataRowCol","cmsDoTransformLineStride","cmsXYZ2Lab","cmsDeleteTransform","cmsDeleteTransform","cmsIT8Free","cmsDictFree","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetAdaptationState","cmsOpenProfileFromFile","cmsGetEncodedICCversion","cmsOpenIOhandlerFromNULL","cmsOpenProfileFromIOhandlerTHR","cmsReadRawTag","cmsBuildGamma","cmsReverseToneCurveEx","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsCreateExtendedTransform","cmsDictAlloc","cmsDictGetEntryList","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8SetDataRowCol","cmsDoTransformLineStride","cmsXYZ2Lab","cmsDeleteTransform","cmsDeleteTransform","cmsIT8Free","cmsDictFree","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
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
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Set adaptation state
    cmsFloat64Number prevAdaptation = cmsSetAdaptationState(1.0);
    
    // Create temporary file with input data for profile loading
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Save to file for cmsOpenProfileFromFile
    FILE* tmp_file = fopen("input_file", "wb");
    if (tmp_file) {
        fwrite(data, 1, size, tmp_file);
        assert_file_closed(&tmp_file);;
    }
    
    // 1. Open profile from file
    cmsHPROFILE hProfile1 = cmsOpenProfileFromFile("input_file", "r");
    
    // 2. Get encoded ICC version
    cmsUInt32Number version = 0;
    if (hProfile1) {
        version = cmsGetEncodedICCversion(hProfile1);
    }
    
    // 3. Create IO handler and open profile from it
    cmsIOHANDLER* io = nullptr;
    if (in_file) {
        // Reset file pointer for IO handler reading
        fseek(in_file, 0, SEEK_SET);
        // Note: cmsOpenIOhandlerFromFile is not in the list, but we need an IO handler
        // We'll create a simple one using available functions
        // For demonstration, we'll use a NULL handler since we can't create from FILE*
        io = cmsOpenIOhandlerFromNULL(context);
    }
    
    cmsHPROFILE hProfile2 = nullptr;
    if (io) {
        hProfile2 = cmsOpenProfileFromIOhandlerTHR(context, io);
    }
    
    // 4. Read raw tag if we have a valid profile
    if (hProfile1 || hProfile2) {
        cmsHPROFILE hProfile = hProfile1 ? hProfile1 : hProfile2;
        const cmsTagSignature tagSig = cmsSigRedColorantTag;
        cmsUInt32Number bufferSize = 1024;
        void* buffer = malloc(bufferSize);
        if (buffer) {
            cmsUInt32Number bytesRead = cmsReadRawTag(hProfile, tagSig, buffer, bufferSize);
            free(buffer);
        }
    }
    
    // 5. Build and reverse gamma curve
    cmsToneCurve* gammaCurve = cmsBuildGamma(context, 2.2);
    cmsToneCurve* reversedCurve = nullptr;
    if (gammaCurve) {
        reversedCurve = cmsReverseToneCurveEx(256, gammaCurve);
    }
    
    // 6. Create sRGB profile
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(context);
    
    // 7. Create transforms
    cmsHTRANSFORM transform = nullptr;
    cmsHTRANSFORM extendedTransform = nullptr;
    
    if (hProfile1 && sRGBProfile) {
        // Simple transform
        transform = cmsCreateTransform(hProfile1, TYPE_RGB_8, sRGBProfile, TYPE_RGB_8, 
                                       INTENT_PERCEPTUAL, 0);
        
        // Extended transform
        cmsHPROFILE profiles[2] = {hProfile1, sRGBProfile};
        cmsBool bpc[2] = {FALSE, FALSE};
        cmsUInt32Number intents[2] = {INTENT_PERCEPTUAL, INTENT_PERCEPTUAL};
        cmsFloat64Number adaptationStates[2] = {1.0, 1.0};
        
        extendedTransform = cmsCreateExtendedTransform(context, 2, profiles, bpc, intents,
                                                      adaptationStates, nullptr, 0,
                                                      TYPE_RGB_8, TYPE_RGB_8, 0);
    }
    
    // 8. Dictionary operations
    cmsHANDLE dict = cmsDictAlloc(context);
    const cmsDICTentry* dictEntry = nullptr;
    if (dict) {
        dictEntry = cmsDictGetEntryList(dict);
    }
    
    // 9. IT8 operations
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        cmsIT8SetPropertyDbl(it8, "SAMPLE_ID", 1.0);
        cmsIT8SetDataRowCol(it8, 0, 0, "TestData");
    }
    
    // 10. Perform color transformation with stride if we have a transform
    if (transform && size >= 12) {
        cmsUInt8Number inputPixels[12];
        cmsUInt8Number outputPixels[12];
        
        // Copy some input data for transformation
        size_t copySize = size < 12 ? size : 12;
        memcpy(inputPixels, data, copySize);
        
        // Transform with stride
        cmsDoTransformLineStride(transform, inputPixels, outputPixels,
                                 1, 1,  // 1 pixel per line, 1 line
                                 12, 12, // Bytes per line in/out
                                 4, 4); // Bytes per plane in/out
    }
    
    // 11. XYZ to Lab conversion
    cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
    cmsCIEXYZ whitePoint = {0.95047, 1.0, 1.08883};
    cmsCIELab lab;
    cmsXYZ2Lab(&whitePoint, &lab, &xyz);
    
    // Cleanup
    if (extendedTransform) cmsDeleteTransform(extendedTransform);
    if (transform) cmsDeleteTransform(transform);
    if (it8) cmsIT8Free(it8);
    if (dict) cmsDictFree(dict);  // Note: cmsDictFree not in list but needed
    if (sRGBProfile) cmsCloseProfile(sRGBProfile);
    if (reversedCurve) cmsFreeToneCurve(reversedCurve);
    if (gammaCurve) cmsFreeToneCurve(gammaCurve);
    if (hProfile2) cmsCloseProfile(hProfile2);
    if (hProfile1) cmsCloseProfile(hProfile1);
    if (io) {
        // Close IO handler - need appropriate function
        // cmsCloseIOhandler not in list, but we should free resources
    }
    
    // Clean up temporary files
    remove("input_file");
    
    if (in_file) assert_file_closed(&in_file);;
    
    cmsDeleteContext(context);
    
    assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}