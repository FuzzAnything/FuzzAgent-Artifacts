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
//<ID> 507
//<Prompt> []
/*<Combination>: [
*/
//<score> 30.5, nr_unique_branch: 0
//<Quality> {"density":61,"unique_branches":{},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsDeleteContext","cmsIT8DefineDblFormat","cmsCreateXYZProfileTHR","cmsIT8Free","cmsDeleteContext","cmsCreateLab4Profile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsSetHeaderRenderingIntent","cmsSetHeaderRenderingIntent","cmsGetSupportedIntentsTHR","cmsPipelineAlloc","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsBuildGamma","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsIsToneCurveDescending","cmsIsToneCurveDescending","cmsReverseToneCurve","cmsReverseToneCurve","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineSetSaveAs8bitsFlag","cmsGetColorSpace","cmsGetColorSpace","cmsCreateInkLimitingDeviceLink","cmsCreateTransformTHR","cmsDoTransform","cmsFloat2XYZEncoded","cmsDeleteTransform","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsCreateXYZProfileTHR","cmsCreateLab4Profile","cmsSetHeaderRenderingIntent","cmsSetHeaderRenderingIntent","cmsGetSupportedIntentsTHR","cmsPipelineAlloc","cmsBuildGamma","cmsBuildTabulatedToneCurve16","cmsIsToneCurveDescending","cmsIsToneCurveDescending","cmsReverseToneCurve","cmsReverseToneCurve","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineSetSaveAs8bitsFlag","cmsGetColorSpace","cmsGetColorSpace","cmsCreateInkLimitingDeviceLink","cmsCreateTransformTHR","cmsDoTransform","cmsFloat2XYZEncoded","cmsDeleteTransform","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":1}
/*Here's a fuzz driver that uses all the specified LCMS APIs in a meaningful way:

*/


extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Need minimum data

    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // Allocate IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (!it8) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Define double format for IT8
    cmsIT8DefineDblFormat(it8, "%.6f");

    // Create profiles
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(context);
    if (!xyzProfile) {
        cmsIT8Free(it8);
        cmsDeleteContext(context);
        return 0;
    }

    cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0}; // D65
    cmsHPROFILE labProfile = cmsCreateLab4Profile(&whitePoint);
    if (!labProfile) {
        cmsCloseProfile(xyzProfile);
        cmsIT8Free(it8);
        cmsDeleteContext(context);
        return 0;
    }

    // Set rendering intent using data
    cmsUInt32Number intent = data[0] % 4; // 0-3 for different intents
    cmsSetHeaderRenderingIntent(xyzProfile, intent);
    cmsSetHeaderRenderingIntent(labProfile, intent);

    // Get supported intents
    cmsUInt32Number intents[10];
    char *descriptions[10];
    cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(context, 10, intents, descriptions);
    
    // Create pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    if (!pipeline) {
        cmsCloseProfile(labProfile);
        cmsCloseProfile(xyzProfile);
        cmsIT8Free(it8);
        cmsDeleteContext(context);
        return 0;
    }

    // Build gamma curve from data
    cmsFloat64Number gamma = 1.0 + (data[1] / 255.0) * 3.0; // Gamma between 1.0-4.0
    cmsToneCurve *gammaCurve = cmsBuildGamma(context, gamma);
    if (!gammaCurve) {
        cmsPipelineFree(pipeline);
        cmsCloseProfile(labProfile);
        cmsCloseProfile(xyzProfile);
        cmsIT8Free(it8);
        cmsDeleteContext(context);
        return 0;
    }

    // Build tabulated tone curve from data
    size_t entries = (size - 2) / 2;
    if (entries > 256) entries = 256;
    if (entries < 2) entries = 2;
    
    cmsUInt16Number *table = (cmsUInt16Number*)malloc(entries * sizeof(cmsUInt16Number));
    if (!table) {
        cmsFreeToneCurve(gammaCurve);
        cmsPipelineFree(pipeline);
        cmsCloseProfile(labProfile);
        cmsCloseProfile(xyzProfile);
        cmsIT8Free(it8);
        cmsDeleteContext(context);
        return 0;
    }
    
    for (size_t i = 0; i < entries && (2 + i*2 + 1) < size; i++) {
        table[i] = (data[2 + i*2] << 8) | data[2 + i*2 + 1];
    }
    
    cmsToneCurve *tabCurve = cmsBuildTabulatedToneCurve16(context, entries, table);
    free(table);
    
    if (!tabCurve) {
        cmsFreeToneCurve(gammaCurve);
        cmsPipelineFree(pipeline);
        cmsCloseProfile(labProfile);
        cmsCloseProfile(xyzProfile);
        cmsIT8Free(it8);
        cmsDeleteContext(context);
        return 0;
    }

    // Check if curves are descending
    cmsBool gammaDescending = cmsIsToneCurveDescending(gammaCurve);
    cmsBool tabDescending = cmsIsToneCurveDescending(tabCurve);

    // Reverse curves if needed
    cmsToneCurve *reversedGamma = nullptr;
    cmsToneCurve *reversedTab = nullptr;
    
    if (gammaDescending) {
        reversedGamma = cmsReverseToneCurve(gammaCurve);
    }
    
    if (tabDescending) {
        reversedTab = cmsReverseToneCurve(tabCurve);
    }

    // Create tone curves stage
    cmsToneCurve *curves[] = {
        reversedGamma ? reversedGamma : gammaCurve,
        reversedTab ? reversedTab : tabCurve,
        gammaCurve
    };
    
    cmsStage *toneStage = cmsStageAllocToneCurves(context, 3, curves);
    if (toneStage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, toneStage);
    }

    // Set pipeline to save as 8 bits
    cmsBool save8bits = (data[0] & 0x01) ? 1 : 0;
    cmsPipelineSetSaveAs8bitsFlag(pipeline, save8bits);

    // Get color spaces
    cmsColorSpaceSignature xyzSpace = cmsGetColorSpace(xyzProfile);
    cmsColorSpaceSignature labSpace = cmsGetColorSpace(labProfile);

    // Create ink limiting device link
    cmsFloat64Number limit = 200.0 + (data[2] / 255.0) * 100.0; // Limit between 200-300
    cmsHPROFILE inkLimitProfile = cmsCreateInkLimitingDeviceLink(labSpace, limit);
    
    // Create transform
    cmsUInt32Number inputFormat = TYPE_XYZ_16;
    cmsUInt32Number outputFormat = TYPE_Lab_16;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM transform = cmsCreateTransformTHR(context, 
                                                   xyzProfile, 
                                                   inputFormat, 
                                                   labProfile, 
                                                   outputFormat, 
                                                   intent, 
                                                   flags);
    
    if (transform) {
        // Prepare input data
        size_t numPixels = (size - 10) / 6; // Each XYZ pixel is 6 bytes (3 x 16-bit)
        if (numPixels > 16) numPixels = 16;
        
        cmsUInt16Number *input = (cmsUInt16Number*)malloc(numPixels * 3 * sizeof(cmsUInt16Number));
        cmsUInt16Number *output = (cmsUInt16Number*)malloc(numPixels * 3 * sizeof(cmsUInt16Number));
        
        if (input && output) {
            // Fill input with data
            for (size_t i = 0; i < numPixels * 3 && (10 + i*2 + 1) < size; i++) {
                input[i] = (data[10 + i*2] << 8) | data[10 + i*2 + 1];
            }
            
            // Apply transform
            cmsDoTransform(transform, input, output, numPixels);
            
            // Test encoded XYZ conversion
            if (numPixels > 0) {
                cmsCIEXYZ xyzFloat;
                xyzFloat.X = 0.5;
                xyzFloat.Y = 0.5;
                xyzFloat.Z = 0.5;
                cmsUInt16Number xyzEncoded[3];
                cmsFloat2XYZEncoded(xyzEncoded, &xyzFloat);
            }
            
            free(input);
            free(output);
        }
        
        cmsDeleteTransform(transform);
    }

    // Clean up curves
    if (reversedGamma) cmsFreeToneCurve(reversedGamma);
    if (reversedTab) cmsFreeToneCurve(reversedTab);
    cmsFreeToneCurve(gammaCurve);
    cmsFreeToneCurve(tabCurve);

    // Clean up pipeline
    cmsPipelineFree(pipeline);

    // Clean up profiles
    if (inkLimitProfile) cmsCloseProfile(inkLimitProfile);
    cmsCloseProfile(labProfile);
    cmsCloseProfile(xyzProfile);

    // Clean up IT8
    cmsIT8Free(it8);

    // Create dummy IO handler for file operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Use file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Suppress unused warning
        
        // Close IO handler
        assert_file_closed(&in_file);;
    }

    // Delete context
    cmsDeleteContext(context);

    assert_file_closed(&in_file);
	return 0;
}