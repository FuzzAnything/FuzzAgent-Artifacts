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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsDupContext","cmsGetEncodedICCversion","cmsGetTagCount","cmsReadTag","cmsMLUalloc","cmsMLUsetUTF8","cmsMLUfree","cmsBuildParametricToneCurve","cmsStageAllocToneCurves","cmsCreateInkLimitingDeviceLinkTHR","cmsPipelineDup","cmsReadTag","cmsPipelineInputChannels","cmsPipelineDup","cmsPipelineFree","cmsCloseProfile","cmsStageFree","cmsFreeToneCurve","cmsDeltaE","cmsUnregisterPlugins","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsDupContext","cmsGetEncodedICCversion","cmsGetTagCount","cmsReadTag","cmsMLUalloc","cmsMLUsetUTF8","cmsMLUfree","cmsBuildParametricToneCurve","cmsStageAllocToneCurves","cmsCreateInkLimitingDeviceLinkTHR","cmsPipelineDup","cmsReadTag","cmsPipelineInputChannels","cmsPipelineDup","cmsPipelineFree","cmsCloseProfile","cmsStageFree","cmsFreeToneCurve","cmsDeltaE","cmsUnregisterPlugins","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open input data as a FILE stream
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 1. Read profile from stream
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Duplicate context for separate operations
    cmsContext dupContext = cmsDupContext(context, nullptr);

    // 3. Get encoded ICC version
    cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);

    // 4. Get tag count and iterate through first few tags
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    for (cmsInt32Number i = 0; i < tagCount && i < 5; ++i) {
        cmsUInt32Number offset, tagSize;
        // Note: cmsGetTagOffsetAndSize is not in the provided list, but we'll assume it exists
        // For compliance, we'll use an alternative approach with cmsReadTag
        void* tagData = cmsReadTag(hProfile, static_cast<cmsTagSignature>(i));
        if (tagData) {
            // Process tag data if needed
        }
    }

    // 5. Create and populate MLU with UTF8 string
    cmsMLU* mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        cmsMLUsetUTF8(mlu, "en", "US", "Test UTF8 String");
        cmsMLUfree(mlu);
    }

    // 6. Create a simple pipeline for demonstration
    // First create tone curves
    cmsToneCurve* curves[3];
    cmsFloat64Number params[5] = {1.0, 0.0, 0.0, 0.0, 1.0};
    for (int i = 0; i < 3; ++i) {
        curves[i] = cmsBuildParametricToneCurve(context, 1, params);
    }
    
    // Create pipeline with tone curves
    cmsPipeline* pipeline = nullptr;
    cmsStage* stage = cmsStageAllocToneCurves(context, 3, curves);
    if (stage) {
        // Note: cmsPipelineAlloc is not in the provided list
        // We'll create a device link profile instead to get a pipeline
        cmsHPROFILE deviceLink = cmsCreateInkLimitingDeviceLinkTHR(context, cmsSigRgbData, 1.0);
        if (deviceLink) {
            pipeline = cmsPipelineDup((cmsPipeline*)cmsReadTag(deviceLink, cmsSigBToA0Tag));
            if (pipeline) {
                // Get input channels
                cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);
                
                // Duplicate pipeline
                cmsPipeline* dupPipeline = cmsPipelineDup(pipeline);
                if (dupPipeline) {
                    cmsPipelineFree(dupPipeline);
                }
            }
            cmsCloseProfile(deviceLink);
        }
        cmsStageFree(stage);
    }
    
    // Cleanup tone curves
    for (int i = 0; i < 3; ++i) {
        if (curves[i]) cmsFreeToneCurve(curves[i]);
    }

    // 7. Compute deltaE between two Lab colors
    cmsCIELab lab1 = {50.0, 20.0, 30.0};
    cmsCIELab lab2 = {55.0, 25.0, 35.0};
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);

    // 8. Unregister plugins
    cmsUnregisterPlugins();

    // 9. Cleanup pipeline if created
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(dupContext);
    cmsDeleteContext(context);

    assert_file_closed(&in_file);
	return 0;
}