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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 136.5, nr_unique_branch: 40
//<Quality> {"density":21,"unique_branches":{"Type_LUTA2B_Write":[[2964,9,2964,31,0,0,4,0],[2965,13,2965,83,0,0,4,1],[3016,9,3016,18,0,0,4,0],[3019,13,3019,70,0,0,4,0]],"SaveTags":[[1482,17,1482,99,0,0,4,0]],"Type_ParametricCurve_Write":[[1492,9,1492,30,0,0,4,0]],"InkLimitingSampler":[[374,9,374,27,0,0,4,0],[374,9,374,27,0,0,4,1],[374,31,374,41,0,0,4,0],[374,31,374,41,0,0,4,1],[377,13,377,22,0,0,4,0],[377,13,377,22,0,0,4,1]],"cmsSaveProfileToIOhandler":[[1554,9,1554,30,0,0,4,0]],"GetDataFormat":[[1670,9,1670,22,0,0,4,0]],"LocateSample":[[2798,15,2798,30,0,0,4,0],[2801,13,2801,24,0,0,4,0],[2802,17,2802,49,0,0,4,1]],"cmsSaveProfileToStream":[[1610,9,1610,19,0,0,4,1]],"cmsSmoothToneCurve":[[1224,13,1224,39,0,0,4,1]],"EvalSegmentedFn":[[731,38,731,44,0,0,4,1]],"cmsCreateInkLimitingDeviceLinkTHR":[[402,9,402,37,0,0,4,1],[407,9,407,20,0,0,4,0],[410,13,410,22,0,0,4,0],[411,13,411,24,0,0,4,1],[415,9,415,14,0,0,4,1],[429,9,429,20,0,0,4,1],[435,9,435,21,0,0,4,1],[437,9,437,78,0,0,4,1],[439,9,439,102,0,0,4,1],[440,9,440,54,0,0,4,1],[441,9,441,100,0,0,4,1],[445,9,445,53,0,0,4,1],[447,9,447,56,0,0,4,1],[448,9,448,54,0,0,4,1]],"WriteSetOfCurves":[[2864,13,2864,41,0,0,4,1],[2865,14,2865,41,0,0,4,1],[2868,13,2868,45,0,0,4,1],[2875,13,2875,33,0,0,4,1],[2879,13,2879,43,0,0,4,0],[2880,21,2880,72,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsIT8GetDataRowCol","cmsBuildSegmentedToneCurve","cmsSmoothToneCurve","cmsCreateLinearizationDeviceLink","cmsCreateInkLimitingDeviceLinkTHR","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInputChannels","cmsSaveProfileToStream","cmsStageFree","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsIT8GetDataRowCol","cmsBuildSegmentedToneCurve","cmsSmoothToneCurve","cmsCreateLinearizationDeviceLink","cmsCreateInkLimitingDeviceLinkTHR","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInputChannels","cmsSaveProfileToStream","cmsStageFree","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsIT8Free","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimal data for meaningful operations
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Create input FILE* from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output FILE* for saving profile
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Load IT8 data from memory
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(context, data, size);
    if (hIT8) {
        // 2. Find data format
        int format = cmsIT8FindDataFormat(hIT8, "Sample");
        
        // 3. Get data from IT8 table
        const char *rowColData = cmsIT8GetDataRowCol(hIT8, 0, 0);
        
        // 4. Build segmented tone curve
        cmsCurveSegment segments[2];
        memset(segments, 0, sizeof(segments));
        
        // First segment: linear from 0 to 0.5
        segments[0].Type = 1;  // Linear segment type
        segments[0].x0 = 0.0f;
        segments[0].x1 = 0.5f;
        segments[0].Params[0] = 1.0;  // Gamma = 1.0 (linear)
        
        // Second segment: linear from 0.5 to 1.0
        segments[1].Type = 1;
        segments[1].x0 = 0.5f;
        segments[1].x1 = 1.0f;
        segments[1].Params[0] = 1.0;
        
        cmsToneCurve *segCurve = cmsBuildSegmentedToneCurve(context, 2, segments);
        
        if (segCurve) {
            // 5. Smooth the tone curve
            cmsSmoothToneCurve(segCurve, 0.1);
            
            // 6. Create linearization device link
            cmsToneCurve *curves[3] = {segCurve, segCurve, segCurve};
            cmsHPROFILE linProfile = cmsCreateLinearizationDeviceLink(cmsSigRgbData, curves);
            
            if (linProfile) {
                // 7. Create ink limiting device link
                cmsHPROFILE inkProfile = cmsCreateInkLimitingDeviceLinkTHR(context, cmsSigCmykData, 0.85);
                
                if (inkProfile) {
                    // 8. Create pipeline and add tone curves stage
                    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
                    if (pipeline) {
                        // 9. Allocate tone curves stage
                        cmsStage *toneCurveStage = cmsStageAllocToneCurves(context, 3, curves);
                        if (toneCurveStage) {
                            // Add stage to pipeline (simplified - would normally use cmsPipelineInsertStage)
                            // For this example, we'll just create the stage
                            
                            // 10. Get pipeline input channels
                            cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);
                            
                            // 11. Save profile to stream
                            cmsSaveProfileToStream(linProfile, out_file);
                            
                            // Clean up stage
                            cmsStageFree(toneCurveStage);
                        }
                        cmsPipelineFree(pipeline);
                    }
                    cmsCloseProfile(inkProfile);
                }
                cmsCloseProfile(linProfile);
            }
            cmsFreeToneCurve(segCurve);
        }
        
        // Clean up IT8
        cmsIT8Free(hIT8);
    }
    
    // Clean up
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}