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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 104, nr_unique_branch: 17
//<Quality> {"density":13,"unique_branches":{"Clamp_ab_doubleV2":[[248,9,248,32,0,0,4,1],[249,9,249,32,0,0,4,1]],"cmsStageAllocCLutFloatGranular":[[651,9,651,41,0,0,4,1],[658,9,658,23,0,0,4,1],[662,9,662,24,0,0,4,1],[673,9,673,15,0,0,4,1],[679,9,679,37,0,0,4,1],[684,9,684,22,0,0,4,0],[685,19,685,24,0,0,4,0],[685,19,685,24,0,0,4,1],[691,9,691,33,0,0,4,1]],"convert_utf16_to_utf32":[[167,42,167,63,0,0,4,0]],"cmsCIE94DeltaE":[[466,9,466,17,0,0,4,1]],"Clamp_L_doubleV2":[[238,9,238,14,0,0,4,1],[239,9,239,18,0,0,4,1]],"GetInfo":[[989,13,989,62,0,0,4,0]],"DefaultInterpolatorsFactory":[[1231,24,1231,31,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsPlugin","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsDeleteContext","cmsGetHeaderRenderingIntent","cmsGetProfileInfo","cmsPipelineAlloc","cmsStageAllocCLutFloatGranular","cmsPipelineInsertStage","cmsPipelineGetPtrToFirstStage","cmsStageNext","cmsCIE94DeltaE","cmsFloat2LabEncodedV2","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsPlugin","cmsOpenProfileFromFileTHR","cmsGetHeaderRenderingIntent","cmsGetProfileInfo","cmsPipelineAlloc","cmsStageAllocCLutFloatGranular","cmsPipelineInsertStage","cmsPipelineGetPtrToFirstStage","cmsStageNext","cmsCIE94DeltaE","cmsFloat2LabEncodedV2","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // 1. Use cmsPlugin - register a dummy plugin
    cmsPlugin(nullptr); // Plugin can be NULL for default behavior

    // 2. Create a profile from input data (simulating lcms output data)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(ctx, "input_file", fuzz_str_1);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 3. Use cmsGetHeaderRenderingIntent
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);

    // 4. Use cmsGetProfileInfo
    wchar_t infoBuffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfo(hProfile, cmsInfoDescription, 
                                                 fuzz_str_2, fuzz_str_3, infoBuffer, 256);

    // 5. Create a simple pipeline to use cmsStageNext
    cmsPipeline *pipeline = cmsPipelineAlloc(ctx, fuzz_uint32_t_4, fuzz_uint32_t_5);
    if (pipeline) {
        // Create a CLUT stage
        cmsUInt32Number clutPoints[] = {5, 5, 5};
        cmsFloat32Number table[5*5*5*3];
        for (size_t i = 0; i < sizeof(table)/sizeof(table[0]); i++) {
            table[i] = (i % 100) / 100.0f;
        }
        
        // 6. Use cmsStageAllocCLutFloatGranular
        cmsStage *clutStage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, 3, 3, table);
        if (clutStage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, clutStage);
            
            // 7. Use cmsStageNext
            cmsStage *firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (firstStage) {
                cmsStage *nextStage = cmsStageNext(firstStage);
                (void)nextStage; // May be NULL for single stage
            }
        }
    }

    // 8. Use cmsCIE94DeltaE
    cmsCIELab lab1, lab2;
    lab1.L = 50.0; lab1.a = 0.0; lab1.b = 0.0;
    lab2.L = 55.0; lab2.a = 5.0; lab2.b = 5.0;
    cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);

    // 9. Use cmsFloat2LabEncodedV2
    cmsUInt16Number encodedLab[3];
    cmsFloat2LabEncodedV2(encodedLab, &lab1);

    // Clean up
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}