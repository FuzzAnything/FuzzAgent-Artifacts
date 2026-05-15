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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":26,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDupContext","cmsDeleteContext","cmsDeleteContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsDeleteContext","cmsReadTag","cmsMLUalloc","cmsMLUsetUTF8","cmsReadTag","cmsPipelineInputChannels","cmsPipelineDup","cmsGBDAlloc","cmsGDBCompute","cmsGBDFree","cmsDeltaE","cmsCIE2000DeltaE","cmsIT8LoadFromFile","cmsIT8EnumDataFormat","cmsIT8GetProperty","cmsIT8Free","cmsMLUfree","cmsPipelineFree","cmsCloseProfile","cmsUnregisterPluginsTHR","cmsUnregisterPlugins","cmsDeleteContext","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDupContext","cmsOpenProfileFromStreamTHR","cmsReadTag","cmsMLUalloc","cmsMLUsetUTF8","cmsReadTag","cmsPipelineInputChannels","cmsPipelineDup","cmsGBDAlloc","cmsGDBCompute","cmsGBDFree","cmsDeltaE","cmsCIE2000DeltaE","cmsIT8LoadFromFile","cmsIT8EnumDataFormat","cmsIT8GetProperty","cmsIT8Free","cmsMLUfree","cmsPipelineFree","cmsCloseProfile","cmsUnregisterPluginsTHR","cmsUnregisterPlugins","cmsDeleteContext","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Insufficient data for meaningful fuzzing

    // 1. Create initial context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 2. Duplicate context
    cmsContext dupContext = cmsDupContext(context, nullptr);
    if (!dupContext) {
        cmsDeleteContext(context);
        return 0;
    }

    // 3. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Open profile from stream
    cmsHPROFILE profile = cmsOpenProfileFromStreamTHR(dupContext, in_file, "r");
    if (!profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Read a tag from profile
    cmsTagSignature tag = cmsSigCopyrightTag;  // Example tag
    void *tagData = cmsReadTag(profile, tag);
    
    // 6. Create and use MLU
    cmsMLU *mlu = cmsMLUalloc(dupContext, 1);
    if (mlu) {
        const char *lang = "EN";
        const char *country = "US";
        const char *text = "Test UTF-8 String";
        cmsMLUsetUTF8(mlu, lang, country, text);
    }

    // 7. Attempt to get pipeline from profile
    // Try reading a transform tag that might contain pipeline
    cmsTagSignature lutTag = cmsSigAToB0Tag;  // Common transform tag
    void *lutData = cmsReadTag(profile, lutTag);
    cmsPipeline *pipeline = nullptr;
    cmsPipeline *dupPipeline = nullptr;
    
    if (lutData) {
        // Assuming the tag data is a pipeline (common case)
        pipeline = (cmsPipeline *)lutData;
        // Get input channels
        cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);
        // Duplicate pipeline if valid
        if (inputChannels > 0 && inputChannels < 10) {
            dupPipeline = cmsPipelineDup(pipeline);
        }
    }

    // 8. Create and use Gamut Boundary Descriptor
    cmsHANDLE gbd = cmsGBDAlloc(dupContext);
    if (gbd) {
        // Compute with some flags
        cmsUInt32Number flags = 0;
        cmsGDBCompute(gbd, flags);
        cmsGBDFree(gbd);
    }

    // 9. Calculate DeltaE between two sample colors
    cmsCIELab lab1 = {50.0, 0.0, 0.0};
    cmsCIELab lab2 = {60.0, 10.0, 20.0};
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
    cmsFloat64Number deltaE2000 = cmsCIE2000DeltaE(&lab1, &lab2, 1.0, 1.0, 1.0);

    // 10. Create IT8 handle from temporary file
    // Write sample IT8 data to temp file
    FILE *it8_file = fopen("temp.it8", "wb");
    if (it8_file) {
        const char *it8_content = 
            "CTI1\n"
            "DESCRIPTOR \"Test Chart\"\n"
            "COLOR_REP \"CMYK\"\n"
            "NUMBER_OF_FIELDS 5\n"
            "BEGIN_DATA_FORMAT\n"
            "SAMPLE_ID C M Y K\n"
            "END_DATA_FORMAT\n"
            "NUMBER_OF_SETS 1\n"
            "BEGIN_DATA\n"
            "1 0 0 0 0\n"
            "END_DATA\n";
        fwrite(it8_content, 1, strlen(it8_content), it8_file);
        assert_file_closed(&it8_file);;
        
        // Load IT8 file
        cmsHANDLE it8 = cmsIT8LoadFromFile(dupContext, "temp.it8");
        if (it8) {
            // Enumerate data format
            char **sampleNames = nullptr;
            int formatCount = cmsIT8EnumDataFormat(it8, &sampleNames);
            
            // Get property
            const char *propValue = cmsIT8GetProperty(it8, "DESCRIPTOR");
            
            cmsIT8Free(it8);
        }
        remove("temp.it8");  // Clean up temp file
    }

    // 11. Clean up MLU
    if (mlu) {
        cmsMLUfree(mlu);
    }

    // 12. Clean up duplicated pipeline
    if (dupPipeline) {
        cmsPipelineFree(dupPipeline);
    }

    // 13. Close profile
    cmsCloseProfile(profile);

    // 14. Close file
    assert_file_closed(&in_file);;

    // 15. Unregister plugins
    cmsUnregisterPluginsTHR(dupContext);
    cmsUnregisterPlugins();

    // 16. Delete contexts
    cmsDeleteContext(dupContext);
    cmsDeleteContext(context);

    assert_file_closed(&it8_file);
	assert_file_closed(&in_file);
	return 0;
}