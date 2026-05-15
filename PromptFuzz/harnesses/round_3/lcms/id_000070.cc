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
//<ID> 530
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"cmsIsCLUT":[[855,9,855,46,0,0,4,0]]},"library_calls":["cmsOpenProfileFromStream","cmsGetSupportedIntentsTHR","cmsIsIntentSupported","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsPipelineUnlinkStage","cmsStageInputChannels","cmsStageFree","cmsDeleteTransform","cmsCloseProfile","cmsBuildTabulatedToneCurveFloat","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8EnumDataFormat","cmsIT8Free","cmsReadTag","cmsNamedColorIndex","cmsReadTag","cmsDupProfileSequenceDescription","cmsFreeProfileSequenceDescription","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStream","cmsGetSupportedIntentsTHR","cmsIsIntentSupported","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsPipelineUnlinkStage","cmsStageInputChannels","cmsStageFree","cmsDeleteTransform","cmsCloseProfile","cmsBuildTabulatedToneCurveFloat","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8EnumDataFormat","cmsIT8Free","cmsReadTag","cmsNamedColorIndex","cmsReadTag","cmsDupProfileSequenceDescription","cmsFreeProfileSequenceDescription","cmsCloseProfile"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to process ICC profile data:

*/


// Include lcms headers

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create a temporary file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get supported intents
    cmsUInt32Number intentCodes[10];
    char* descriptions[10];
    cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(NULL, 10, intentCodes, descriptions);
    
    // Check if specific intents are supported
    for (cmsUInt32Number i = 0; i < numIntents && i < 10; i++) {
        cmsIsIntentSupported(hProfile, intentCodes[i], 0);
    }
    
    // Create a transform for testing
    cmsHPROFILE sRGB = cmsCreate_sRGBProfile();
    if (sRGB) {
        cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, 
                                                     TYPE_RGB_8, 
                                                     sRGB, 
                                                     TYPE_RGB_8, 
                                                     INTENT_PERCEPTUAL, 
                                                     0);
        
        if (hTransform) {
            // Get pipeline from transform
            cmsPipeline* pipeline = cmsGetTransformPipeline(hTransform);
            if (pipeline) {
                // Get first stage and unlink it
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    cmsStage* unlinkedStage = NULL;
                    cmsPipelineUnlinkStage(pipeline, cmsAT_BEGIN, &unlinkedStage);
                    
                    // Get input channels of stage
                    cmsUInt32Number inputChannels = cmsStageInputChannels(firstStage);
                    
                    // Clean up unlinked stage
                    if (unlinkedStage) {
                        cmsStageFree(unlinkedStage);
                    }
                }
            }
            cmsDeleteTransform(hTransform);
        }
        cmsCloseProfile(sRGB);
    }
    
    // Create a simple tone curve from data
    std::vector<cmsFloat32Number> curveData;
    size_t curvePoints = std::min(size / 4, (size_t)256);
    for (size_t i = 0; i < curvePoints; i++) {
        float val = static_cast<float>(data[i % size]) / 255.0f;
        curveData.push_back(val);
    }
    
    if (!curveData.empty()) {
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurveFloat(NULL, 
                                                                 curveData.size(), 
                                                                 curveData.data());
        if (toneCurve) {
            cmsFreeToneCurve(toneCurve);
        }
    }
    
    // Create and use IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (hIT8) {
        char** sampleNames = NULL;
        int numFormats = cmsIT8EnumDataFormat(hIT8, &sampleNames);
        cmsIT8Free(hIT8);
    }
    
    // Try to get named color list
    cmsNAMEDCOLORLIST* namedColors = (cmsNAMEDCOLORLIST*)cmsReadTag(hProfile, cmsSigNamedColor2Tag);
    if (namedColors) {
        // Try to find a color index (use a simple name)
        cmsNamedColorIndex(namedColors, "Red");
    }
    
    // Get profile sequence description
    cmsSEQ* seq = (cmsSEQ*)cmsReadTag(hProfile, cmsSigProfileSequenceDescTag);
    if (seq) {
        cmsSEQ* dupSeq = cmsDupProfileSequenceDescription(seq);
        if (dupSeq) {
            // Free duplicated sequence
            cmsFreeProfileSequenceDescription(dupSeq);
        }
    }
    
    // Try to compute gamut boundary (simplified)
    // Note: cmsGDBCompute requires a gamut boundary descriptor which we don't have
    // We'll create a simple one if possible, but skip if not
    
    // Cleanup
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}