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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"cmsSetLogErrorHandlerTHR":[[538,13,538,23,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromFileTHR","cmsCreateProfilePlaceholder","cmsGetHeaderModel","cmsAllocNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEvalReverseFloat","cmsPipelineUnlinkStage","cmsGetContextUserData","cmsGBDAlloc","cmsGDBCompute","cmsCIE2000DeltaE","cmsGBDFree","cmsStageFree","cmsStageFree","cmsFreeToneCurve","cmsFreeNamedColorList","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromFileTHR","cmsCreateProfilePlaceholder","cmsGetHeaderModel","cmsAllocNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEvalReverseFloat","cmsPipelineUnlinkStage","cmsGetContextUserData","cmsGBDAlloc","cmsGDBCompute","cmsCIE2000DeltaE","cmsGBDFree","cmsStageFree","cmsFreeToneCurve","cmsFreeNamedColorList","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Set log error handler
    cmsSetLogErrorHandlerTHR(context, NULL);
    
    // 3. Open profile from input file
    cmsHPROFILE profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!profile) {
        // Try to create a profile from the input data
        profile = cmsCreateProfilePlaceholder(context);
    }
    
    // 4. Get header model if profile exists
    cmsUInt32Number model = 0;
    if (profile) {
        model = cmsGetHeaderModel(profile);
    }
    
    // 5. Allocate named color list
    cmsNAMEDCOLORLIST* namedColors = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
    
    // 6. Allocate pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // 7. Build gamma curve
    cmsToneCurve* gammaCurve = cmsBuildGamma(context, 2.2);
    
    // 8. Create tone curves array and allocate stage
    cmsToneCurve* curves[3] = {gammaCurve, gammaCurve, gammaCurve};
    cmsStage* toneCurveStage = NULL;
    if (gammaCurve) {
        toneCurveStage = cmsStageAllocToneCurves(context, 3, curves);
    }
    
    // Add stage to pipeline if created
    if (pipeline && toneCurveStage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, toneCurveStage);
    }
    
    // 9. Evaluate pipeline in reverse (if pipeline exists)
    if (pipeline) {
        cmsFloat32Number target[3] = {0.5f, 0.5f, 0.5f};
        cmsFloat32Number result[3] = {0.0f, 0.0f, 0.0f};
        cmsFloat32Number hint[3] = {0.0f, 0.0f, 0.0f};
        cmsPipelineEvalReverseFloat(target, result, hint, pipeline);
    }
    
    // 10. Unlink stage from pipeline (if exists)
    cmsStage* unlinkedStage = NULL;
    if (pipeline && toneCurveStage) {
        cmsPipelineUnlinkStage(pipeline, cmsAT_END, &unlinkedStage);
    }
    
    // 11. Get context user data
    void* userData = cmsGetContextUserData(context);
    
    // 12. Allocate GBD handle
    cmsHANDLE gbdHandle = cmsGBDAlloc(context);
    
    // 13. Compute GBD (if handle created)
    if (gbdHandle) {
        cmsGDBCompute(gbdHandle, 0);
    }
    
    // 14. Compute CIE2000 delta E
    cmsCIELab lab1 = {50.0, 20.0, 30.0};
    cmsCIELab lab2 = {55.0, 25.0, 35.0};
    cmsFloat64Number deltaE = cmsCIE2000DeltaE(&lab1, &lab2, 1.0, 1.0, 1.0);
    
    // Cleanup in specific order
    
    // 15. Free GBD handle
    if (gbdHandle) {
        cmsGBDFree(gbdHandle);
    }
    
    // 16. Free stage (either unlinked or standalone)
    if (unlinkedStage) {
        cmsStageFree(unlinkedStage);
    } else if (toneCurveStage) {
        cmsStageFree(toneCurveStage);
    }
    
    // 17. Free tone curve
    if (gammaCurve) {
        cmsFreeToneCurve(gammaCurve);
    }
    
    // 18. Free named color list
    if (namedColors) {
        cmsFreeNamedColorList(namedColors);
    }
    
    // 19. Free pipeline
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    // 20. Close profile
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    // 21. Delete context
    cmsDeleteContext(context);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}