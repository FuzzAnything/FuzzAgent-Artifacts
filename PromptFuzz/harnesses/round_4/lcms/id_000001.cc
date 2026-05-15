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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateXYZProfileTHR","cmsDeleteContext","cmsCreateLab2Profile","cmsCloseProfile","cmsDeleteContext","cmsCreateExtendedTransform","cmsDoTransformStride","cmsFloat2LabEncoded","cmsXYZ2xyY","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetUTF8","cmsMLUsetUTF8","cmsDictAddEntry","cmsMLUfree","cmsMLUfree","cmsDictFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateXYZProfileTHR","cmsCreateLab2Profile","cmsCreateExtendedTransform","cmsDoTransformStride","cmsFloat2LabEncoded","cmsXYZ2xyY","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetUTF8","cmsMLUsetUTF8","cmsDictAddEntry","cmsMLUfree","cmsMLUfree","cmsDictFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<46) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeChar(fuzz_char_5, fdp);
		FDPConsumeChar(fuzz_char_6, fdp);
		FDPConsumeChar(fuzz_char_7, fdp);
		FDPConsumeChar(fuzz_char_8, fdp);
		FDPConsumeChar(fuzz_char_9, fdp);
		FDPConsumeChar(fuzz_char_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeChar(fuzz_char_12, fdp);
		FDPConsumeChar(fuzz_char_13, fdp);
		FDPConsumeChar(fuzz_char_14, fdp);
		FDPConsumeChar(fuzz_char_15, fdp);
		FDPConsumeChar(fuzz_char_16, fdp);
		FDPConsumeChar(fuzz_char_17, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_18, fuzz_str_sz_18, fdp);
	
		char fuzzer_var_cmsMLUsetUTF8_13_1[3] = {fuzz_char_5,fuzz_char_6,fuzz_char_7};
		char fuzzer_var_cmsMLUsetUTF8_13_2[3] = {fuzz_char_8,fuzz_char_9,fuzz_char_10};
		char fuzzer_var_cmsMLUsetUTF8_14_1[3] = {fuzz_char_12,fuzz_char_13,fuzz_char_14};
		char fuzzer_var_cmsMLUsetUTF8_14_2[3] = {fuzz_char_15,fuzz_char_16,fuzz_char_17};//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for processing
    
    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Create XYZ profile
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(ctx);
    if (!xyzProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create another profile (Lab) for extended transform
    cmsHPROFILE labProfile = cmsCreateLab2Profile(nullptr);
    if (!labProfile) {
        cmsCloseProfile(xyzProfile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Prepare profiles array for extended transform
    cmsHPROFILE profiles[2] = {xyzProfile, labProfile};
    cmsBool bpc[2] = {FALSE, FALSE};
    cmsUInt32Number intents[2] = {INTENT_PERCEPTUAL, INTENT_PERCEPTUAL};
    cmsFloat64Number adaptationStates[2] = {1.0, 1.0};
    
    // Create extended transform
    cmsHTRANSFORM transform = cmsCreateExtendedTransform(
        ctx, 2, profiles, bpc, intents, adaptationStates, 
        nullptr, fuzz_uint32_t_1, TYPE_Lab_DBL, TYPE_XYZ_DBL, fuzz_uint32_t_2
    );
    
    if (transform) {
        // Prepare input/output buffers
        size_t pixelCount = size / (3 * sizeof(double)) / 2;
        if (pixelCount > 0) {
            size_t inputSize = pixelCount * 3 * sizeof(double);
            size_t outputSize = pixelCount * 3 * sizeof(double);
            
            // Allocate buffers
            double* inputBuffer = static_cast<double*>(malloc(inputSize));
            double* outputBuffer = static_cast<double*>(malloc(outputSize));
            
            if (inputBuffer && outputBuffer) {
                // Copy data to input buffer (with bounds checking)
                size_t copySize = (inputSize < size) ? inputSize : size;
                memcpy(inputBuffer, data, copySize);
                
                // Perform stride transform
                cmsDoTransformStride(
                    transform, 
                    inputBuffer, 
                    outputBuffer, 
                    pixelCount, 
                    3 * sizeof(double)
                );
                
                // Convert some Lab values to encoded form
                cmsCIELab lab;
                if (size >= sizeof(cmsCIELab)) {
                    memcpy(&lab, data, sizeof(cmsCIELab));
                    cmsUInt16Number encodedLab[3];
                    cmsFloat2LabEncoded(encodedLab, &lab);
                    
                    // Convert XYZ to xyY
                    cmsCIEXYZ xyz;
                    memcpy(&xyz, data, sizeof(cmsCIEXYZ));
                    cmsCIExyY xyY;
                    cmsXYZ2xyY(&xyY, &xyz);
                }
                
                free(inputBuffer);
                free(outputBuffer);
            }
        }
        
        // Create dictionary for metadata
        cmsHANDLE dict = cmsDictAlloc(ctx);
        if (dict) {
            // Create MLU entries for dictionary
            cmsMLU* displayName = cmsMLUalloc(ctx, fuzz_uint32_t_3);
            cmsMLU* displayValue = cmsMLUalloc(ctx, fuzz_uint32_t_4);
            
            if (displayName && displayValue) {
                // Set MLU values from input data (with bounds checking)
                const char* sampleStr = "Sample";
                if (size >= 10) {
                    char langCode[3] = {'e', 'n', '\0'};
                    char countryCode[3] = {'U', 'S', '\0'};
                    
                    cmsMLUsetUTF8(displayName, fuzzer_var_cmsMLUsetUTF8_13_1, fuzzer_var_cmsMLUsetUTF8_13_2, fuzz_str_11);
                    cmsMLUsetUTF8(displayValue, fuzzer_var_cmsMLUsetUTF8_14_1, fuzzer_var_cmsMLUsetUTF8_14_2, fuzz_str_18);
                }
                
                // Add dictionary entry
                wchar_t name[50] = L"TestEntry";
                wchar_t value[50] = L"TestValue";
                cmsDictAddEntry(dict, name, value, displayName, displayValue);
                
                // Clean up MLU
                cmsMLUfree(displayName);
                cmsMLUfree(displayValue);
            }
            
            cmsDictFree(dict);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // Clean up
    cmsCloseProfile(xyzProfile);
    cmsCloseProfile(labProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}