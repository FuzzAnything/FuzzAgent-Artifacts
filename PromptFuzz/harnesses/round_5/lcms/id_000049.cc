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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsOpenProfileFromMem","cmsDeleteContext","cmsGetProfileInfoASCII","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineFree","cmsPluginTHR","cmsFloat2XYZEncoded","cmsAllocNamedColorList","cmsDupNamedColorList","cmsFreeNamedColorList","cmsFreeNamedColorList","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsOpenProfileFromMem","cmsGetProfileInfoASCII","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineFree","cmsPluginTHR","cmsFloat2XYZEncoded","cmsAllocNamedColorList","cmsDupNamedColorList","cmsFreeNamedColorList","cmsFreeNamedColorList","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
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

    // Open input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create context for plugin operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Open profile from file
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!hProfile) {
        // Try opening from memory instead
        hProfile = cmsOpenProfileFromMem(data, size);
        if (!hProfile) {
            cmsDeleteContext(context);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
        }
    }

    // Use cmsGetProfileInfoUTF8 (similar to ASCII version we have)
    char buffer[256];
    cmsGetProfileInfoASCII(hProfile, cmsInfoDescription, "en", "US", buffer, sizeof(buffer));

    // Create a pipeline for evaluation
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        // Create simple matrix stage
        cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
        cmsFloat64Number offset[3] = {0.0, 0.0, 0.0};
        cmsStage* matrixStage = cmsStageAllocMatrix(context, 3, 3, matrix, offset);
        
        if (matrixStage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, matrixStage);
            
            // Use cmsPipelineEval16
            cmsUInt16Number input[3] = {0x8000, 0x8000, 0x8000};
            cmsUInt16Number output[3];
            cmsPipelineEval16(input, output, pipeline);
        }
        
        cmsPipelineFree(pipeline);
    }

    // Use cmsPluginTHR with NULL plugin (just to call the API)
    cmsPluginTHR(context, NULL);

    // Use cmsFloat2XYZEncoded
    cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
    cmsUInt16Number encodedXYZ[3];
    cmsFloat2XYZEncoded(encodedXYZ, &xyz);

    // Create and duplicate named color list
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
    if (namedColorList) {
        cmsNAMEDCOLORLIST* dupList = cmsDupNamedColorList(namedColorList);
        if (dupList) {
            cmsFreeNamedColorList(dupList);
        }
        cmsFreeNamedColorList(namedColorList);
    }

    // Clean up
    cmsCloseProfile(hProfile);
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}