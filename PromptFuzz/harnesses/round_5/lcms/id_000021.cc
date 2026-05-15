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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 7
//<Quality> {"density":19,"unique_branches":{"AllocChunk":[[1231,9,1231,18,0,0,4,0]],"cmsIT8EnumProperties":[[2687,32,2687,41,0,0,4,0],[2687,32,2687,41,0,0,4,1],[2693,9,2693,22,0,0,4,0],[2693,9,2693,22,0,0,4,1],[2697,33,2697,42,0,0,4,0],[2697,33,2697,42,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreateProfilePlaceholder","cmsDeleteContext","cmsSetHeaderAttributes","cmsSetHeaderModel","cmsSetPCS","cmsGetDeviceClass","cmsCreate_sRGBProfileTHR","cmsCloseProfile","cmsDeleteContext","cmsIT8LoadFromMem","cmsIT8SetTableByLabel","cmsIT8EnumProperties","cmsCreateTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineCheckAndRetreiveStages","cmsDeleteTransform","cmsIT8Free","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateProfilePlaceholder","cmsSetHeaderAttributes","cmsSetHeaderModel","cmsSetPCS","cmsGetDeviceClass","cmsCreate_sRGBProfileTHR","cmsIT8LoadFromMem","cmsIT8SetTableByLabel","cmsIT8EnumProperties","cmsCreateTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineCheckAndRetreiveStages","cmsDeleteTransform","cmsIT8Free","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified LCMS APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations
    
    cmsContext context = nullptr;
    cmsHPROFILE placeholder = nullptr;
    cmsHPROFILE srgb = nullptr;
    cmsHANDLE it8 = nullptr;
    cmsPipeline* pipeline = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    
    // 1. Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Create placeholder profile
    placeholder = cmsCreateProfilePlaceholder(context);
    if (!placeholder) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 3. Set header attributes using data
    cmsUInt64Number flags = 0;
    if (size >= sizeof(cmsUInt64Number)) {
        memcpy(&flags, data, sizeof(cmsUInt64Number));
    }
    cmsSetHeaderAttributes(placeholder, flags);
    
    // 4. Set header model using data
    cmsUInt32Number model = 0;
    if (size >= sizeof(cmsUInt32Number) * 2) {
        memcpy(&model, data + sizeof(cmsUInt64Number), sizeof(cmsUInt32Number));
    }
    cmsSetHeaderModel(placeholder, model);
    
    // 5. Set PCS
    cmsColorSpaceSignature pcs = cmsSigLabData;
    if (size > 0) {
        // Use first byte to select PCS type
        uint8_t pcs_selector = data[0] % 3;
        switch (pcs_selector) {
            case 0: pcs = cmsSigLabData; break;
            case 1: pcs = cmsSigXYZData; break;
            case 2: pcs = cmsSigRgbData; break;
        }
    }
    cmsSetPCS(placeholder, pcs);
    
    // 6. Get device class
    cmsProfileClassSignature deviceClass = cmsGetDeviceClass(placeholder);
    (void)deviceClass; // Use variable to avoid unused warning
    
    // 7. Create sRGB profile
    srgb = cmsCreate_sRGBProfileTHR(context);
    if (!srgb) {
        cmsCloseProfile(placeholder);
        cmsDeleteContext(context);
        return 0;
    }
    
    // 8. Create IT8 handle from input data
    if (size > 0) {
        it8 = cmsIT8LoadFromMem(context, data, (cmsUInt32Number)size);
    }
    
    if (it8) {
        // 9. Set IT8 table by label using data
        const char* set = "Data";
        const char* field = "Sample";
        const char* expectedType = "NUMBER";
        
        int tableResult = cmsIT8SetTableByLabel(it8, set, field, expectedType);
        (void)tableResult; // Use variable
        
        // 10. Enumerate IT8 properties
        char** propertyNames = nullptr;
        cmsUInt32Number propCount = cmsIT8EnumProperties(it8, &propertyNames);
        (void)propCount; // Use variable
        
        // Clean up property names if allocated
        if (propertyNames) {
            // Note: lcms allocates this array internally
        }
    }
    
    // 11. Create a simple transform to get a pipeline
    cmsHTRANSFORM transform = cmsCreateTransform(srgb, TYPE_RGB_8, 
                                                placeholder, TYPE_Lab_8, 
                                                INTENT_PERCEPTUAL, 0);
    if (transform) {
        // 12. Get pipeline from transform
        pipeline = cmsGetTransformGamutCheckPipeline(transform);
        
        if (pipeline) {
            // 13. Check and retrieve stages
            cmsBool stageResult = cmsPipelineCheckAndRetreiveStages(pipeline, 1);
            (void)stageResult; // Use variable
        }
        
        cmsDeleteTransform(transform);
    }
    
    // 14. Use FILE* operations with input data
    in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read a small amount to consume data
        char buffer[100];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
        (void)bytes_read; // Use variable
        
        // Get file descriptor
        int fd_in = fuzz_fileno(in_file);
        (void)fd_in; // Use variable
        
        assert_file_closed(&in_file);;
    }
    
    // 15. Create output file
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data
        if (size > 0) {
            fwrite(data, 1, size > 100 ? 100 : size, out_file);
        }
        
        // Get file descriptor
        int fd_out = fuzz_fileno(out_file);
        (void)fd_out; // Use variable
        
        assert_file_closed(&out_file);;
    }
    
    // 16. Clean up all resources
    if (it8) {
        cmsIT8Free(it8);
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (srgb) {
        cmsCloseProfile(srgb);
    }
    
    if (placeholder) {
        cmsCloseProfile(placeholder);
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}