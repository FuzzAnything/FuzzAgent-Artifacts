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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.5, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cmsOpenProfileFromFile","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsReadTag","cmsEstimateGamma","cmsGetPostScriptCSA","cmsPipelineAlloc","cmsStageAllocIdentity","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineGetPtrToFirstStage","cmsStageNext","cmsPipelineFree","cmsDeleteTransform","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8Free","cmsReadTag","cmsMLUgetWide","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromFile","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsReadTag","cmsEstimateGamma","cmsGetPostScriptCSA","cmsPipelineAlloc","cmsStageAllocIdentity","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineGetPtrToFirstStage","cmsStageNext","cmsPipelineFree","cmsDeleteTransform","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8Free","cmsReadTag","cmsMLUgetWide","cmsCloseProfile","cmsCloseProfile"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified LCMS APIs to process ICC profile data:

*/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
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
	  // Need minimum data for meaningful processing

    // Write input data to a file for cmsOpenProfileFromFile
    FILE *profile_file = fopen("input_file", "wb");
    if (!profile_file) {
	assert_file_closed(&profile_file);
	return 0;
	}
	
    fwrite(data, 1, size, profile_file);
    assert_file_closed(&profile_file);;

    // 1. Open profile from file using cmsOpenProfileFromFile
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("input_file", "r");
    if (!hProfile) {
        remove("input_file");
        assert_file_closed(&profile_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 2. Create a simple transform to get output format
    cmsHPROFILE sRGB = cmsCreate_sRGBProfile();
    cmsHTRANSFORM transform = cmsCreateTransform(hProfile, TYPE_RGB_8, sRGB, TYPE_RGB_8,
                                                 INTENT_PERCEPTUAL, 0);
    
    if (transform) {
        // 3. Get transform output format using cmsGetTransformOutputFormat
        cmsUInt32Number outputFormat = cmsGetTransformOutputFormat(transform);
        (void)outputFormat; // Use result to avoid unused variable warning
        
        // 4. Try to get tone curves and estimate gamma
        cmsToneCurve* curves[3] = {NULL, NULL, NULL};
        cmsTagSignature tag = cmsSigRedTRCTag;
        
        for (int i = 0; i < 3; i++) {
            curves[i] = (cmsToneCurve*)cmsReadTag(hProfile, tag);
            if (curves[i]) {
                // 5. Estimate gamma using cmsEstimateGamma
                cmsFloat64Number gamma = cmsEstimateGamma(curves[i], 0.01);
                (void)gamma; // Use result
            }
            tag = (tag == cmsSigRedTRCTag) ? cmsSigGreenTRCTag : cmsSigBlueTRCTag;
        }
        
        // 6. Get PostScript CSA using cmsGetPostScriptCSA
        char buffer[1024];
        cmsUInt32Number bufferSize = cmsGetPostScriptCSA(NULL, hProfile, INTENT_PERCEPTUAL, 
                                                         0, buffer, sizeof(buffer));
        (void)bufferSize; // Use result
        
        // 7. Create a simple pipeline to use cmsStageNext
        cmsPipeline* pipeline = cmsPipelineAlloc(NULL, 3, 3);
        if (pipeline) {
            cmsStage* stage1 = cmsStageAllocIdentity(NULL, 3);
            cmsStage* stage2 = cmsStageAllocIdentity(NULL, 3);
            
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage1);
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage2);
            
            // 8. Use cmsStageNext to traverse stages
            cmsStage* current = cmsPipelineGetPtrToFirstStage(pipeline);
            while (current) {
                current = cmsStageNext(current);
            }
            cmsPipelineFree(pipeline);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // 9. Create and use IT8 structure with cmsIT8SetPropertyDbl
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (hIT8) {
        // Use first few bytes of data as property name/value
        if (size > 50) {
            char propName[20];
            memcpy(propName, data, 19);
            propName[19] = '\0';
            
            // Convert first 8 bytes to double for property value
            double value = 0.0;
            if (size >= 8) {
                memcpy(&value, data, 8);
            }
            
            cmsIT8SetPropertyDbl(hIT8, propName, value);
        }
        cmsIT8Free(hIT8);
    }
    
    // 10. Get MLU info using cmsMLUgetWide
    cmsMLU* mlu = (cmsMLU*)cmsReadTag(hProfile, cmsSigProfileDescriptionTag);
    if (mlu) {
        wchar_t wideBuffer[256];
        cmsUInt32Number wideSize = cmsMLUgetWide(mlu, "en", "US", 
                                                wideBuffer, sizeof(wideBuffer)/sizeof(wchar_t));
        (void)wideSize; // Use result
    }
    
    // Cleanup
    cmsCloseProfile(sRGB);
    cmsCloseProfile(hProfile);
    remove("input_file");
    
    assert_file_closed(&profile_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}