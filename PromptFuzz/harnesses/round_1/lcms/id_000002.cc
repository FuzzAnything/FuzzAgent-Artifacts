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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsIT8LoadFromFile","cmsDeleteContext","cmsIT8GetPropertyDbl","cmsIT8SetIndexColumn","cmsIT8GetDataDbl","cmsCreateProfilePlaceholder","cmsMLUalloc","cmsMLUsetASCII","cmsMLUdup","cmsWriteTag","cmsMLUfree","cmsMLUfree","cmsBuildGamma","cmsBuildGamma","cmsBuildGamma","cmsCreateRGBProfile","cmsBuildGamma","cmsBuildGamma","cmsBuildGamma","cmsCreateRGBProfile","cmsCreateTransform","cmsDoTransformLineStride","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromFile","cmsIT8GetPropertyDbl","cmsIT8SetIndexColumn","cmsIT8GetDataDbl","cmsCreateProfilePlaceholder","cmsMLUalloc","cmsMLUsetASCII","cmsMLUdup","cmsWriteTag","cmsMLUfree","cmsMLUfree","cmsBuildGamma","cmsBuildGamma","cmsBuildGamma","cmsCreateRGBProfile","cmsBuildGamma","cmsBuildGamma","cmsBuildGamma","cmsCreateRGBProfile","cmsCreateTransform","cmsDoTransformLineStride","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified LCMS APIs to process IT8 calibration data and apply color transformations:

*/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data

    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	

    // 1. Write input data to a temporary file for IT8 processing
    FILE *it8_file = fopen("it8_input_file", "wb");
    if (!it8_file) {
        cmsDeleteContext(context);
        assert_file_closed(&it8_file);
	return 0;
    }
    fwrite(data, 1, size, it8_file);
    assert_file_closed(&it8_file);;

    // 2. Load IT8 data from file
    cmsHANDLE it8 = cmsIT8LoadFromFile(context, "it8_input_file");
    if (!it8) {
        remove("it8_input_file");
        cmsDeleteContext(context);
        assert_file_closed(&it8_file);
	return 0;
    }

    // Use cmsIT8GetPropertyDbl - get a property value
    cmsFloat64Number propValue = cmsIT8GetPropertyDbl(it8, "NUMBER_OF_FIELDS");
    
    // Use cmsIT8SetIndexColumn - set index column
    cmsIT8SetIndexColumn(it8, "SAMPLE_ID");
    
    // Use cmsIT8GetDataDbl - get data from specific patch and sample
    cmsFloat64Number dataValue = cmsIT8GetDataDbl(it8, "A1", "RGB_R");

    // 3. Create a dummy profile for MLU operations
    cmsHPROFILE dummyProfile = cmsCreateProfilePlaceholder(context);
    if (dummyProfile) {
        // Create an MLU object (profile description)
        cmsMLU *mlu = cmsMLUalloc(context, 1);
        if (mlu) {
            cmsMLUsetASCII(mlu, "en", "US", "Test Profile Description");
            
            // Use cmsMLUdup - duplicate the MLU
            cmsMLU *mluCopy = cmsMLUdup(mlu);
            
            // Write MLU to tag
            cmsWriteTag(dummyProfile, cmsSigProfileDescriptionTag, mlu);
            
            // Clean up MLU objects
            cmsMLUfree(mlu);
            if (mluCopy) cmsMLUfree(mluCopy);
        }
        
        // Create source profile (sRGB-like)
        cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
        cmsCIExyYTRIPLE primaries = {
            {0.64, 0.33, 1.0},
            {0.30, 0.60, 1.0},
            {0.15, 0.06, 1.0}
        };
        
        // Create tone curves
        cmsToneCurve* transferFunc[3];
        transferFunc[0] = cmsBuildGamma(context, 2.2);
        transferFunc[1] = cmsBuildGamma(context, 2.2);
        transferFunc[2] = cmsBuildGamma(context, 2.2);
        
        cmsHPROFILE srcProfile = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunc);
        
        // Create destination profile (Adobe RGB-like)
        cmsCIExyY adobeWhitePoint = {0.3127, 0.3290, 1.0};
        cmsCIExyYTRIPLE adobePrimaries = {
            {0.64, 0.33, 1.0},
            {0.21, 0.71, 1.0},
            {0.15, 0.06, 1.0}
        };
        
        cmsToneCurve* adobeTransferFunc[3];
        adobeTransferFunc[0] = cmsBuildGamma(context, 2.2);
        adobeTransferFunc[1] = cmsBuildGamma(context, 2.2);
        adobeTransferFunc[2] = cmsBuildGamma(context, 2.2);
        
        cmsHPROFILE dstProfile = cmsCreateRGBProfile(&adobeWhitePoint, &adobePrimaries, adobeTransferFunc);
        
        // 4. Create transform
        cmsHTRANSFORM transform = cmsCreateTransform(
            srcProfile, 
            TYPE_RGB_8,
            dstProfile,
            TYPE_RGB_8,
            INTENT_PERCEPTUAL,
            0
        );
        
        if (transform) {
            // 5. Create test image data using the input data
            size_t imageSize = size > 1024 ? 1024 : size;
            uint8_t *inputBuffer = (uint8_t*)malloc(imageSize);
            uint8_t *outputBuffer = (uint8_t*)malloc(imageSize);
            
            if (inputBuffer && outputBuffer) {
                memcpy(inputBuffer, data, imageSize);
                
                // Calculate stride parameters
                // Assume width of 32 pixels, 3 channels (RGB)
                cmsUInt32Number width = 32;
                cmsUInt32Number height = imageSize / (width * 3);
                if (height < 1) height = 1;
                
                cmsUInt32Number bytesPerLineIn = width * 3;
                cmsUInt32Number bytesPerLineOut = width * 3;
                
                // Use cmsDoTransformLineStride - process the image
                cmsDoTransformLineStride(
                    transform,
                    inputBuffer,
                    outputBuffer,
                    width,
                    height,
                    bytesPerLineIn,
                    bytesPerLineOut,
                    0,  // Bytes per plane in (0 for packed)
                    0   // Bytes per plane out (0 for packed)
                );
            }
            
            // Clean up transform
            cmsDeleteTransform(transform);
            
            if (inputBuffer) free(inputBuffer);
            if (outputBuffer) free(outputBuffer);
        }
        
        // Clean up profiles and tone curves
        if (srcProfile) cmsCloseProfile(srcProfile);
        if (dstProfile) cmsCloseProfile(dstProfile);
        for (int i = 0; i < 3; i++) {
            if (transferFunc[i]) cmsFreeToneCurve(transferFunc[i]);
            if (adobeTransferFunc[i]) cmsFreeToneCurve(adobeTransferFunc[i]);
        }
        
        if (dummyProfile) cmsCloseProfile(dummyProfile);
    }

    // 6. Write processed data to output file (simulating file output)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Use the property and data values from IT8 in some way
        uint8_t summary[16];
        memcpy(summary, &propValue, 8);
        memcpy(summary + 8, &dataValue, 8);
        fwrite(summary, 1, 16, out_file);
        assert_file_closed(&out_file);;
    }

    // 7. Clean up all resources
    cmsIT8Free(it8);
    remove("it8_input_file");
    cmsDeleteContext(context);
    
    assert_file_closed(&out_file);
	assert_file_closed(&it8_file);
	return 0;
}