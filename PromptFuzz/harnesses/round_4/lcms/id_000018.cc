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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsDeleteContext","cmsGetHeaderRenderingIntent","cmsGetProfileInfo","cmsPipelineAlloc","cmsStageAllocCLutFloatGranular","cmsPipelineInsertStage","cmsPipelineGetPtrToFirstStage","cmsStageNext","cmsCIE94DeltaE","cmsFloat2LabEncodedV2","cmsPlugin","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsGetHeaderRenderingIntent","cmsGetProfileInfo","cmsPipelineAlloc","cmsStageAllocCLutFloatGranular","cmsPipelineInsertStage","cmsPipelineGetPtrToFirstStage","cmsStageNext","cmsCIE94DeltaE","cmsFloat2LabEncodedV2","cmsPlugin","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Include lcms headers

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegralArray(uint32_t, fuzz_uint32_t_array_6, fuzz_uint32_t_array_size_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
	
		const cmsUInt32Number * fuzzer_var_cmsStageAllocCLutFloatGranular_7_1 = fuzz_uint32_t_array_6;
		cmsUInt32Number fuzzer_size_cmsStageAllocCLutFloatGranular_7_1 = static_cast<cmsUInt32Number>(fuzz_uint32_t_array_size_6);//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create profile from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(context, "input_file", fuzz_str_1);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 2. Get rendering intent
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
    
    // 3. Get profile info
    wchar_t buffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfo(hProfile, cmsInfoDescription, 
                                                 fuzz_str_2, fuzz_str_3, buffer, sizeof(buffer)/sizeof(wchar_t));
    
    // 4. Create and traverse pipeline stages
    cmsPipeline* pipeline = cmsPipelineAlloc(context, fuzz_uint32_t_4, fuzz_uint32_t_5);
    if (pipeline) {
        // Create CLUT stage
        cmsUInt32Number clutPoints[] = {5, 5, 5};
        cmsFloat32Number* table = new cmsFloat32Number[5*5*5*3];
        for (size_t i = 0; i < 5*5*5*3 && i < size/4; ++i) {
            table[i] = *reinterpret_cast<const float*>(data + (i % (size-3)) * 4);
        }
        
        cmsStage* clutStage = cmsStageAllocCLutFloatGranular(context, fuzzer_var_cmsStageAllocCLutFloatGranular_7_1, fuzzer_size_cmsStageAllocCLutFloatGranular_7_1, fuzz_uint32_t_7, table);
        delete[] table;
        
        if (clutStage) {
            // Add stage to pipeline
            cmsPipelineInsertStage(pipeline, cmsAT_END, clutStage);
            
            // Traverse stages
            const cmsStage* currentStage = cmsPipelineGetPtrToFirstStage(pipeline);
            while (currentStage) {
                const cmsStage* nextStage = cmsStageNext(currentStage);
                currentStage = nextStage;
            }
        }
    }
    
    // 5. Compute CIE94 Delta E
    cmsCIELab lab1, lab2;
    if (size >= 2 * sizeof(cmsCIELab)) {
        memcpy(&lab1, data, sizeof(cmsCIELab));
        memcpy(&lab2, data + sizeof(cmsCIELab), sizeof(cmsCIELab));
        cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);
        (void)deltaE; // Use result to avoid unused variable warning
    }
    
    // 6. Lab to encoded conversion
    cmsUInt16Number encodedLab[3];
    if (size >= sizeof(cmsCIELab)) {
        cmsFloat2LabEncodedV2(encodedLab, &lab1);
    }
    
    // 7. Plugin registration (simulated - use dummy plugin structure)
    if (size > 0) {
        char pluginData[100];
        size_t pluginSize = size > sizeof(pluginData) ? sizeof(pluginData) : size;
        memcpy(pluginData, data, pluginSize);
        cmsPlugin(nullptr); // Note: This would normally take a plugin struct
    }
    
    // Cleanup
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}