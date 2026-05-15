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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"cmsIT8SetPropertyStr":[[1545,9,1545,14,0,0,4,0]]},"library_calls":["cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetDataDbl","cmsBuildGamma","cmsCreateLinearizationDeviceLinkTHR","cmsCreateLab2ProfileTHR","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsReadTag","cmsMLUtranslationsCount","cmsMLUtranslationsCodes","cmsDoTransformLineStride","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsIT8Free"],"critical_calls":["cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetDataDbl","cmsBuildGamma","cmsCreateLinearizationDeviceLinkTHR","cmsCreateLab2ProfileTHR","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsReadTag","cmsMLUtranslationsCount","cmsMLUtranslationsCodes","cmsDoTransformLineStride","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsIT8Free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for basic operations
    
    // 1. Create IT8 handle and use IT8 APIs
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) {
	return 0;
	}
	
    
    // Set some properties using the input data
    const char* propName = "SAMPLE_FIELD";
    char propValue[100];
    size_t copySize = size > 99 ? 99 : size;
    memcpy(propValue, data, copySize);
    propValue[copySize] = '\0';
    
    cmsIT8SetPropertyStr(hIT8, propName, propValue);
    
    // Get property as double (simulate using cmsIT8GetPropertyDbl)
    // Note: We'll simulate this with cmsIT8GetDataDbl since cmsIT8GetPropertyDbl isn't in provided APIs
    cmsFloat64Number propVal = cmsIT8GetDataDbl(hIT8, "1", "1");
    
    // 2. Create a simple transform and pipeline
    cmsToneCurve* gamma = cmsBuildGamma(NULL, 2.2);
    cmsToneCurve* transfer[3] = {gamma, gamma, gamma};
    cmsHPROFILE srcProfile = cmsCreateLinearizationDeviceLinkTHR(NULL, cmsSigRgbData, transfer);
    cmsHPROFILE dstProfile = cmsCreateLab2ProfileTHR(NULL, NULL);
    
    if (srcProfile && dstProfile) {
        cmsHTRANSFORM transform = cmsCreateTransformTHR(
            NULL, srcProfile, TYPE_RGB_8, dstProfile, TYPE_Lab_8,
            INTENT_PERCEPTUAL, 0);
        
        if (transform) {
            // 3. Get pipeline information
            cmsPipeline* pipeline = cmsGetTransformPipeline(transform);
            if (pipeline) {
                cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                
                // 4. Use MLU translations (get from profile)
                cmsMLU* mlu = (cmsMLU*)cmsReadTag(srcProfile, cmsSigProfileDescriptionTag);
                if (mlu) {
                    char langCode[3] = {0};
                    char countryCode[3] = {0};
                    cmsUInt32Number transCount = cmsMLUtranslationsCount(mlu);
                    if (transCount > 0) {
                        cmsMLUtranslationsCodes(mlu, 0, langCode, countryCode);
                    }
                }
                
                // 5. Use transform with line stride
                // Prepare input/output buffers using the data
                const int width = 16;
                const int height = 16;
                const int channels = 3;
                const int stride = width * channels;
                
                uint8_t* inputBuffer = (uint8_t*)malloc(stride * height);
                uint8_t* outputBuffer = (uint8_t*)malloc(stride * height);
                
                if (inputBuffer && outputBuffer) {
                    // Copy data to input buffer (wrap around if needed)
                    for (size_t i = 0; i < stride * height; i++) {
                        inputBuffer[i] = data[i % size];
                    }
                    
                    // Perform transformation
                    cmsDoTransformLineStride(
                        transform,
                        inputBuffer,
                        outputBuffer,
                        width,
                        height,
                        stride,  // BytesPerLineIn
                        stride,  // BytesPerLineOut
                        0,       // BytesPerPlaneIn
                        0        // BytesPerPlaneOut
                    );
                    
                    free(inputBuffer);
                    free(outputBuffer);
                }
            }
            
            // 6. Clean up transform
            cmsDeleteTransform(transform);
        }
    }
    
    // 7. Clean up profiles
    if (srcProfile) cmsCloseProfile(srcProfile);
    if (dstProfile) cmsCloseProfile(dstProfile);
    if (gamma) cmsFreeToneCurve(gamma);
    
    // 8. Clean up IT8 handle
    cmsIT8Free(hIT8);
    
    return 0;
}