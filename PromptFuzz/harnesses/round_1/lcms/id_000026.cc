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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 5
//<Quality> {"density":24,"unique_branches":{"Type_LUT16_Read":[[2327,32,2327,63,0,0,4,0]],"cmsGetTagOffsetAndSize":[[664,9,664,26,0,0,4,1],[665,9,665,27,0,0,4,1],[667,9,667,23,0,0,4,0],[668,9,668,21,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDupContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsDeleteContext","cmsGetEncodedICCversion","cmsGetTagCount","cmsGetTagOffsetAndSize","cmsMLUalloc","cmsMLUsetUTF8","cmsReadTag","cmsPipelineInputChannels","cmsPipelineInputChannels","cmsPipelineDup","cmsGBDAlloc","cmsGDBCompute","cmsGBDFree","cmsPipelineFree","cmsDeltaE","cmsMLUfree","cmsCloseProfile","cmsUnregisterPlugins","cmsUnregisterPluginsTHR","cmsUnregisterPluginsTHR","cmsDeleteContext","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDupContext","cmsOpenProfileFromStreamTHR","cmsGetEncodedICCversion","cmsGetTagCount","cmsGetTagOffsetAndSize","cmsMLUalloc","cmsMLUsetUTF8","cmsReadTag","cmsPipelineInputChannels","cmsPipelineInputChannels","cmsPipelineDup","cmsGBDAlloc","cmsGDBCompute","cmsGBDFree","cmsPipelineFree","cmsDeltaE","cmsMLUfree","cmsCloseProfile","cmsUnregisterPlugins","cmsUnregisterPluginsTHR","cmsUnregisterPluginsTHR","cmsDeleteContext","cmsDeleteContext"],"visited":1}
/*Here's a fuzz driver that uses all the specified lcms APIs to process ICC profile data:

*/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimal data for processing
    
    // Create a FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a context and duplicate it (API #4)
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsContext dupContext = cmsDupContext(context, NULL);
    
    // Open profile from memory stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(dupContext, in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get encoded ICC version (API #5)
    cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);
    
    // Get tag information (API #1)
    cmsUInt32Number tagCount = cmsGetTagCount(hProfile);
    if (tagCount > 0) {
        cmsUInt32Number offset, tagSize;
        cmsGetTagOffsetAndSize(hProfile, 0, &offset, &tagSize);
    }
    
    // Create MLU and set UTF-8 string (API #6)
    cmsMLU *mlu = cmsMLUalloc(dupContext, 1);
    const char lang[] = "en";
    const char country[] = "US";
    const char utf8str[] = "Fuzz Test";
    cmsMLUsetUTF8(mlu, lang, country, utf8str);
    
    // Try to get a pipeline from the profile
    void *tagData = cmsReadTag(hProfile, cmsSigAToB0Tag);
    if (tagData && cmsPipelineInputChannels((cmsPipeline*)tagData) > 0) {
        // Get input channels (API #2)
        cmsUInt32Number inputChannels = cmsPipelineInputChannels((cmsPipeline*)tagData);
        
        // Duplicate pipeline (API #7)
        cmsPipeline *dupPipeline = cmsPipelineDup((cmsPipeline*)tagData);
        
        if (dupPipeline) {
            // Create and compute GDB (API #8)
            // Note: cmsGDBCompute requires a valid GDB handle
            // We'll create a simple case for demonstration
            cmsHANDLE hGDB = cmsGBDAlloc(dupContext);
            if (hGDB) {
                // Try to compute with minimal flags
                cmsGDBCompute(hGDB, 0);
                cmsGBDFree(hGDB);
            }
            cmsPipelineFree(dupPipeline);
        }
    }
    
    // Calculate DeltaE between two Lab colors (API #9)
    cmsCIELab lab1 = {50.0, 20.0, 30.0};
    cmsCIELab lab2 = {55.0, 25.0, 35.0};
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
    
    // Create output file for potential writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor if needed
        int fd = fuzz_fileno(out_file);
        // Could write processed data here
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // Cleanup
    cmsMLUfree(mlu);
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    // Unregister plugins (API #3)
    cmsUnregisterPlugins();
    
    // Also unregister for the specific contexts
    cmsUnregisterPluginsTHR(context);
    cmsUnregisterPluginsTHR(dupContext);
    
    // Delete contexts
    cmsDeleteContext(dupContext);
    cmsDeleteContext(context);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}