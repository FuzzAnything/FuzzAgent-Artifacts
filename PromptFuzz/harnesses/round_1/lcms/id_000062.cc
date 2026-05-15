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
//<ID> 416
//<Prompt> []
/*<Combination>: [
*/
//<score> 17.5, nr_unique_branch: 0
//<Quality> {"density":35,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsIT8LoadFromFile","cmsDeleteContext","cmsIT8GetPropertyDbl","cmsIT8SetIndexColumn","cmsIT8GetDataDbl","cmsCreateProfilePlaceholder","cmsIT8Free","cmsDeleteContext","cmsMLUalloc","cmsMLUsetASCII","cmsBuildGamma","cmsMLUfree","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsCreateRGBProfile","cmsFreeToneCurve","cmsMLUfree","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsCreateTransform","cmsDoTransformLineStride","cmsPipelineAlloc","cmsBuildGamma","cmsPipelineInsertStage","cmsStageAllocIdentity","cmsPipelineEvalReverseFloat","cmsPipelineFree","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsMLUfree","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromFile","cmsIT8GetPropertyDbl","cmsIT8SetIndexColumn","cmsIT8GetDataDbl","cmsCreateProfilePlaceholder","cmsMLUalloc","cmsMLUsetASCII","cmsBuildGamma","cmsCreateRGBProfile","cmsCreateTransform","cmsDoTransformLineStride","cmsPipelineAlloc","cmsBuildGamma","cmsPipelineInsertStage","cmsStageAllocIdentity","cmsPipelineEvalReverseFloat","cmsPipelineFree","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsMLUfree","cmsIT8Free","cmsDeleteContext"],"visited":1}
/*Looking at these APIs, I can see they can work together to achieve the event of: **Loading an IT8 color measurement file, reading color data from it, creating a color transformation pipeline, and applying color transformations to image data.**

Here's the fuzz driver that uses all these APIs:

*/


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
	 // Need minimum data
    
    // Step 1: Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Step 2: Write input data to a file and load IT8
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Load IT8 file
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(context, "input_file");
    if (!hIT8) {
        remove("input_file");
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 3: Get property from IT8
    cmsFloat64Number propValue = cmsIT8GetPropertyDbl(hIT8, "SAMPLE_ID");
    
    // Step 4: Set index column
    cmsIT8SetIndexColumn(hIT8, "RGB_R");
    
    // Step 5: Get data from IT8
    cmsFloat64Number dataValue = cmsIT8GetDataDbl(hIT8, "SAMPLE_1", "RGB_R");
    
    // Step 6: Create placeholder profile
    cmsHPROFILE placeholder = cmsCreateProfilePlaceholder(context);
    if (!placeholder) {
        cmsIT8Free(hIT8);
        remove("input_file");
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 7: Allocate and set MLU (Multilingual Unicode)
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        cmsMLUsetASCII(mlu, "en", "US", "Fuzzer Generated Profile");
    }
    
    // Step 8: Create tone curves and RGB profile
    cmsToneCurve *gammaCurve = cmsBuildGamma(context, 2.2);
    if (!gammaCurve) {
        if (mlu) cmsMLUfree(mlu);
        cmsCloseProfile(placeholder);
        cmsIT8Free(hIT8);
        remove("input_file");
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    cmsToneCurve *transferFunctions[3] = {gammaCurve, gammaCurve, gammaCurve};
    
    // Create standard D50 white point
    cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0}; // D50
    
    // Create standard sRGB primaries
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},  // Red
        {0.3000, 0.6000, 1.0},  // Green
        {0.1500, 0.0600, 1.0}   // Blue
    };
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunctions);
    if (!rgbProfile) {
        cmsFreeToneCurve(gammaCurve);
        if (mlu) cmsMLUfree(mlu);
        cmsCloseProfile(placeholder);
        cmsIT8Free(hIT8);
        remove("input_file");
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 9: Create pipeline for evaluation
    // Create a simple pipeline (we'll use the RGB profile's pipeline)
    cmsPipeline *pipeline = nullptr;
    
    // Step 10: Create transform
    cmsHTRANSFORM transform = cmsCreateTransform(
        rgbProfile, TYPE_RGB_8,      // Input: RGB 8-bit
        placeholder, TYPE_RGB_8,     // Output: RGB 8-bit
        INTENT_PERCEPTUAL,           // Rendering intent
        0);                          // Flags
    
    // Step 11: Apply transformation if we have enough data
    if (transform && size >= 300) {
        const size_t pixels = 10;
        const size_t channels = 3;
        
        // Create input and output buffers
        uint8_t inputBuffer[pixels * channels];
        uint8_t outputBuffer[pixels * channels];
        
        // Use fuzz data for input (truncate to buffer size)
        size_t copySize = (size < sizeof(inputBuffer)) ? size : sizeof(inputBuffer);
        memcpy(inputBuffer, data, copySize);
        
        // Apply transformation with stride
        cmsDoTransformLineStride(
            transform,
            inputBuffer,
            outputBuffer,
            pixels,        // Pixels per line
            1,             // Line count
            pixels * channels,  // Bytes per line in
            pixels * channels,  // Bytes per line out
            pixels,        // Bytes per plane in
            pixels         // Bytes per plane out
        );
    }
    
    // Step 12: Test pipeline evaluation (if we have a pipeline)
    // We need to create a simple pipeline for this
    cmsPipeline *testPipeline = cmsPipelineAlloc(context, 3, 3);
    if (testPipeline) {
        // Create identity curve
        cmsToneCurve *identityCurve = cmsBuildGamma(context, 1.0);
        if (identityCurve) {
            // Add identity stage
            cmsPipelineInsertStage(testPipeline, cmsAT_END, 
                cmsStageAllocIdentity(context, 3));
            
            // Test reverse evaluation
            cmsFloat32Number target[3] = {0.5f, 0.5f, 0.5f};
            cmsFloat32Number result[3] = {0.0f, 0.0f, 0.0f};
            cmsFloat32Number hint[3] = {0.0f, 0.0f, 0.0f};
            
            cmsPipelineEvalReverseFloat(target, result, hint, testPipeline);
            
            // Free the pipeline
            cmsPipelineFree(testPipeline);
            cmsFreeToneCurve(identityCurve);
        }
    }
    
    // Step 13: Clean up everything
    if (transform) cmsDeleteTransform(transform);
    if (rgbProfile) cmsCloseProfile(rgbProfile);
    if (gammaCurve) cmsFreeToneCurve(gammaCurve); // Note: profile makes copy, we free our copy
    if (placeholder) cmsCloseProfile(placeholder);
    if (mlu) cmsMLUfree(mlu);
    cmsIT8Free(hIT8);
    
    // Clean up temporary file
    remove("input_file");
    
    // Step 14: Delete context
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}