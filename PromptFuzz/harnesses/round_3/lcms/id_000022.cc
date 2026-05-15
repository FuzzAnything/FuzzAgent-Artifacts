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
//<ID> 107
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"cmsCreateDeviceLinkFromCubeFileTHR":[[3286,9,3286,75,0,0,4,0]],"cmsCreateExtendedTransform":[[1155,13,1155,34,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCreateDeviceLinkFromCubeFile","cmsGetHeaderProfileID","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetTransformGamutCheckPipeline","cmsPipelineGetPtrToFirstStage","cmsGetStageContextID","cmsBuildGamma","cmsEvalToneCurve16","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateDeviceLinkFromCubeFile","cmsGetHeaderProfileID","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetTransformGamutCheckPipeline","cmsPipelineGetPtrToFirstStage","cmsGetStageContextID","cmsBuildGamma","cmsEvalToneCurve16","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create temporary files for input/output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // First API: Create device link from cube file
    // We'll write input data to a temporary cube file first
    const char *cube_filename = "input_cube.cube";
    FILE *cube_file = fopen(cube_filename, "wb");
    if (cube_file) {
        fwrite(data, 1, size, cube_file);
        assert_file_closed(&cube_file);;
        
        // Note: cmsCreateDeviceLinkFromCubeFileTHR is not in the provided list
        // Using cmsCreateDeviceLinkFromCubeFile instead
        cmsHPROFILE deviceLink = cmsCreateDeviceLinkFromCubeFile(cube_filename);
        
        if (deviceLink) {
            // Second API: Get header profile ID
            cmsUInt8Number profileID[16];
            cmsGetHeaderProfileID(deviceLink, profileID);
            
            // Create additional profiles for transform
            cmsHPROFILE labProfile = cmsCreateLab4Profile(NULL);
            cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(context);
            
            if (labProfile && xyzProfile) {
                // Create a transform with gamut checking
                cmsHPROFILE profiles[] = {labProfile, deviceLink, xyzProfile};
                cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
                    context, profiles, 3, 
                    TYPE_Lab_DBL, TYPE_XYZ_DBL,
                    INTENT_PERCEPTUAL, cmsFLAGS_GAMUTCHECK);
                
                if (transform) {
                    // Third API: Get transform gamut check pipeline
                    cmsPipeline *gamutPipeline = cmsGetTransformGamutCheckPipeline(transform);
                    
                    if (gamutPipeline) {
                        // Fourth API: Get stage context ID
                        // Find first stage in the pipeline
                        cmsStage *firstStage = cmsPipelineGetPtrToFirstStage(gamutPipeline);
                        if (firstStage) {
                            cmsContext stageContext = cmsGetStageContextID(firstStage);
                            (void)stageContext; // Use context if needed
                        }
                        
                        // Fifth API: Evaluate tone curve
                        // Create a simple gamma tone curve
                        cmsToneCurve *gammaCurve = cmsBuildGamma(context, 2.2);
                        if (gammaCurve) {
                            // Evaluate at multiple points
                            for (cmsUInt16Number i = 0; i < 10; i++) {
                                cmsUInt16Number value = i * 6553;
                                cmsUInt16Number result = cmsEvalToneCurve16(gammaCurve, value);
                                (void)result; // Use result if needed
                            }
                            cmsFreeToneCurve(gammaCurve);
                        }
                    }
                    
                    cmsDeleteTransform(transform);
                }
                
                cmsCloseProfile(labProfile);
                cmsCloseProfile(xyzProfile);
            }
            
            cmsCloseProfile(deviceLink);
        }
        
        // Clean up temporary cube file
        remove(cube_filename);
    }
    
    // Clean up
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    remove("output_file");
    
    assert_file_closed(&cube_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}