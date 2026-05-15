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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreateProfilePlaceholder","cmsCreate_sRGBProfileTHR","cmsCloseProfile","cmsDeleteContext","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsD50_XYZ","cmsPipelineDup","cmsIT8Alloc","cmsIT8SaveToFile","cmsPipelineGetPtrToFirstStage","cmsStageNext","cmsIT8EnumProperties","cmsIT8GetProperty","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsOpenIOhandlerFromFile","cmsDoTransformStride","cmsCloseIOhandler","cmsIT8Free","cmsPipelineFree","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreateProfilePlaceholder","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsD50_XYZ","cmsPipelineDup","cmsIT8Alloc","cmsIT8SaveToFile","cmsPipelineGetPtrToFirstStage","cmsStageNext","cmsIT8EnumProperties","cmsIT8GetProperty","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsOpenIOhandlerFromFile","cmsDoTransformStride","cmsCloseIOhandler","cmsIT8Free","cmsPipelineFree","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 128) {
	return 0;
	}
	 // Too small to be meaningful
    
    // Write input data to a temporary file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Open profile from input file
    cmsHPROFILE inputProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!inputProfile) {
        // If we can't open from file, create a placeholder and write the data
        fseek(in_file, 0, SEEK_SET);
        inputProfile = cmsCreateProfilePlaceholder(context);
        // Read some data to use as profile content (simplified)
    }
    
    // 3. Create sRGB profile
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(context);
    if (!srgbProfile) {
        if (inputProfile) cmsCloseProfile(inputProfile);
        cmsDeleteContext(context);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 4. Create multiprofile transform
    cmsHPROFILE profiles[2] = {inputProfile ? inputProfile : srgbProfile, srgbProfile};
    cmsUInt32Number nProfiles = inputProfile ? 2 : 1;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        context, profiles, nProfiles, 
        TYPE_RGB_8, TYPE_RGB_8, 
        intent, flags
    );
    
    // 5. Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // 6. Get D50 white point
    const cmsCIEXYZ* d50 = cmsD50_XYZ();
    
    // 7. Duplicate pipeline
    cmsPipeline* dupPipeline = pipeline ? cmsPipelineDup(pipeline) : nullptr;
    
    // 8. Create IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(context);
    
    // 9. Save IT8 to file if created
    if (it8) {
        cmsIT8SaveToFile(it8, "output_file");
    }
    
    // 10. Iterate through pipeline stages (if pipeline exists)
    if (pipeline) {
        cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
        cmsStage* nextStage = firstStage;
        while (nextStage) {
            nextStage = cmsStageNext(nextStage);
        }
    }
    
    // 11. Enumerate IT8 properties
    if (it8) {
        char** propertyNames = nullptr;
        cmsUInt32Number numProps = cmsIT8EnumProperties(it8, &propertyNames);
        // Free the property names array if allocated
        if (propertyNames) {
            for (cmsUInt32Number i = 0; i < numProps; i++) {
                // The strings themselves are owned by IT8 handle
            }
            // Free the array of pointers
            free(propertyNames);
        }
        
        // 12. Get a property
        const char* propValue = cmsIT8GetProperty(it8, "SAMPLE_FIELD");
    }
    
    // 13. Get named color list from transform
    if (transform) {
        cmsNAMEDCOLORLIST* namedColors = cmsGetNamedColorList(transform);
        if (namedColors) {
            // 14. Get count of named colors
            cmsUInt32Number colorCount = cmsNamedColorCount(namedColors);
            
            // 15. Get info for first color if exists
            if (colorCount > 0) {
                char name[256];
                char prefix[256];
                char suffix[256];
                cmsUInt16Number pcs[3];
                cmsUInt16Number colorant[16];
                
                cmsNamedColorInfo(namedColors, 0, name, prefix, suffix, pcs, colorant);
            }
        }
    }
    
    // 16. Open IO handler from file
    cmsIOHANDLER* ioHandler = cmsOpenIOhandlerFromFile(context, "output_file", "w");
    
    // 17. Perform transform on some test data
    if (transform) {
        cmsUInt8Number inputRGB[3] = {128, 128, 128};
        cmsUInt8Number outputRGB[3];
        cmsDoTransformStride(transform, inputRGB, outputRGB, 1, 0);
    }
    
    // 18. Cleanup
    if (ioHandler) cmsCloseIOhandler(ioHandler);
    if (it8) cmsIT8Free(it8);
    if (dupPipeline) cmsPipelineFree(dupPipeline);
    if (pipeline) cmsPipelineFree(pipeline);
    if (transform) cmsDeleteTransform(transform);
    if (srgbProfile) cmsCloseProfile(srgbProfile);
    if (inputProfile) cmsCloseProfile(inputProfile);
    cmsDeleteContext(context);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}