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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsCreateLab2ProfileTHR","cmsGetHeaderModel","cmsAllocNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEvalReverseFloat","cmsPipelineUnlinkStage","cmsFreeToneCurve","cmsStageFree","cmsPipelineFree","cmsGBDAlloc","cmsGDBCompute","cmsGBDFree","cmsFreeNamedColorList","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreateLab2ProfileTHR","cmsGetHeaderModel","cmsAllocNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEvalReverseFloat","cmsPipelineUnlinkStage","cmsFreeToneCurve","cmsStageFree","cmsPipelineFree","cmsGBDAlloc","cmsGDBCompute","cmsGBDFree","cmsFreeNamedColorList","cmsCloseProfile","cmsDeleteContext"],"visited":2}
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





    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	

    // 1. Create a profile from input data to use with cmsGetHeaderModel
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(ctx, "input_file", "r");
    assert_file_closed(&in_file);;
    
    if (!hProfile) {
        // Create a simple profile if input data isn't valid
        hProfile = cmsCreateLab2ProfileTHR(ctx, NULL);
    }
    
    // 2. Use cmsGetHeaderModel
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    (void)model; // Use the result
    
    // 3. Create a named color list using cmsAllocNamedColorList
    cmsNAMEDCOLORLIST *namedColorList = cmsAllocNamedColorList(ctx, 10, 3, "Prefix", "Suffix");
    
    // 4. Create a simple pipeline for testing cmsPipelineEvalReverseFloat and cmsPipelineUnlinkStage
    cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Create tone curves for pipeline
        cmsToneCurve *toneCurves[3];
        for (int i = 0; i < 3; i++) {
            toneCurves[i] = cmsBuildGamma(ctx, 2.2);
        }
        
        // Add stage to pipeline
        cmsStage *stage = cmsStageAllocToneCurves(ctx, 3, toneCurves);
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        
        // 5. Use cmsPipelineEvalReverseFloat
        cmsFloat32Number target[3] = {0.5f, 0.5f, 0.5f};
        cmsFloat32Number result[3] = {0.0f};
        cmsFloat32Number hint[3] = {0.0f};
        cmsPipelineEvalReverseFloat(target, result, hint, pipeline);
        
        // 6. Use cmsPipelineUnlinkStage
        cmsStage *unlinkedStage = NULL;
        cmsPipelineUnlinkStage(pipeline, cmsAT_END, &unlinkedStage);
        
        // Clean up tone curves
        for (int i = 0; i < 3; i++) {
            cmsFreeToneCurve(toneCurves[i]);
        }
        
        // Clean up unlinked stage if any
        if (unlinkedStage) {
            cmsStageFree(unlinkedStage);
        }
        
        cmsPipelineFree(pipeline);
    }
    
    // 7. Create and use GDB for cmsGDBCompute
    cmsHANDLE hGDB = cmsGBDAlloc(ctx);
    if (hGDB) {
        // Set some flags based on input data
        cmsUInt32Number flags = 0;
        if (size > 10) flags = data[0] % 256;
        
        // Use cmsGDBCompute
        cmsBool computeResult = cmsGDBCompute(hGDB, flags);
        (void)computeResult; // Use the result
        
        cmsGBDFree(hGDB);
    }
    
    // 8. Clean up named color list
    if (namedColorList) {
        cmsFreeNamedColorList(namedColorList);
    }
    
    // 9. Clean up profile
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    // 10. Clean up context
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}