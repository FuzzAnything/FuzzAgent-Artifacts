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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"cmsGetTransformGamutCheckPipeline":[[1456,9,1456,22,0,0,4,1]]},"library_calls":["cmsOpenProfileFromStreamTHR","cmsGetHeaderProfileID","cmsCreateTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineGetPtrToFirstStage","cmsGetStageContextID","cmsCreateDeviceLinkFromCubeFile","cmsBuildGamma","cmsEvalToneCurve16","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStreamTHR","cmsGetHeaderProfileID","cmsCreateTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineGetPtrToFirstStage","cmsGetStageContextID","cmsCreateDeviceLinkFromCubeFile","cmsBuildGamma","cmsEvalToneCurve16","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file.cube", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	

    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Open a profile from the input data stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(NULL, in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Get the profile ID
    cmsUInt8Number profileID[16];
    cmsGetHeaderProfileID(hProfile, profileID);

    // Create a transform using the profile
    cmsHTRANSFORM hTransform = cmsCreateTransform(
        hProfile, TYPE_RGB_8,
        hProfile, TYPE_RGB_8,
        INTENT_PERCEPTUAL, 0
    );

    if (hTransform) {
        // 2. Get the gamut check pipeline from the transform
        cmsPipeline *pipeline = cmsGetTransformGamutCheckPipeline(hTransform);
        
        if (pipeline) {
            // 3. Get stage context ID from the first stage
            cmsStage *firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (firstStage) {
                cmsContext stageContext = cmsGetStageContextID(firstStage);
                (void)stageContext; // Use to avoid unused variable warning
            }
        }
    }

    // 4. Create a cube file from input data
    FILE *cube_file = fopen("input_file.cube", "wb");
    if (cube_file) {
        fwrite(data, 1, size, cube_file);
        assert_file_closed(&cube_file);;
        
        // Create device link from cube file
        cmsHPROFILE deviceLink = cmsCreateDeviceLinkFromCubeFile("input_file.cube");
        if (deviceLink) {
            // 5. Evaluate a tone curve
            cmsToneCurve *curve = cmsBuildGamma(NULL, 2.2);
            if (curve) {
                cmsUInt16Number result = cmsEvalToneCurve16(curve, 0x8000);
                (void)result; // Use to avoid unused variable warning
                cmsFreeToneCurve(curve);
            }
            cmsCloseProfile(deviceLink);
        }
        remove("input_file.cube");
    }

    // Cleanup
    if (hTransform) cmsDeleteTransform(hTransform);
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&cube_file);
	assert_file_closed(&in_file);
	return 0;
}