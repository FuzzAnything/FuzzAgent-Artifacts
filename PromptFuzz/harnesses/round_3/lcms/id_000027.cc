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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 23
//<Quality> {"density":11,"unique_branches":{"Type_LUTA2B_Write":[[2964,9,2964,31,0,0,4,1],[2980,9,2980,62,0,0,4,1],[2981,9,2981,63,0,0,4,1],[2982,9,2982,38,0,0,4,1],[2988,9,2988,38,0,0,4,1],[2989,9,2989,38,0,0,4,1],[2990,9,2990,38,0,0,4,1],[2991,9,2991,38,0,0,4,1],[2992,9,2992,38,0,0,4,1],[2994,9,2994,18,0,0,4,1],[3000,9,3000,21,0,0,4,1],[3005,9,3005,18,0,0,4,1],[3011,9,3011,23,0,0,4,1],[3016,9,3016,18,0,0,4,1],[3024,9,3024,37,0,0,4,1],[3026,9,3026,44,0,0,4,1],[3027,9,3027,46,0,0,4,1],[3028,9,3028,44,0,0,4,1],[3029,9,3029,44,0,0,4,1],[3030,9,3030,44,0,0,4,1],[3032,9,3032,35,0,0,4,1]],"MemoryWrite":[[225,9,225,49,0,0,4,1]],"cmsAdaptToIlluminant":[[341,9,341,74,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFile","cmsGetHeaderProfileID","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetTagSignature","cmsAdaptToIlluminant","cmsGetTransformGamutCheckPipeline","cmsPipelineGetPtrToFirstStage","cmsGetStageContextID","cmsBuildGamma","cmsEvalToneCurve16","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateDeviceLinkFromCubeFile","cmsGetHeaderProfileID","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetTagSignature","cmsAdaptToIlluminant","cmsGetTransformGamutCheckPipeline","cmsPipelineGetPtrToFirstStage","cmsGetStageContextID","cmsBuildGamma","cmsEvalToneCurve16","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // Write input data to a temporary file for cube file parsing
    FILE *cube_file = fopen("input.cube", "wb");
    if (!cube_file) {
        cmsDeleteContext(context);
        assert_file_closed(&cube_file);
	return 0;
    }
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;

    // Create device link profile from cube file
    cmsHPROFILE deviceLinkProfile = cmsCreateDeviceLinkFromCubeFile("input.cube");
    
    // Get header profile ID
    cmsUInt8Number profileID[16];
    if (deviceLinkProfile) {
        cmsGetHeaderProfileID(deviceLinkProfile, profileID);
    }

    // Create Lab4 profile with default white point
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    cmsHPROFILE labProfile = cmsCreateLab4Profile(&whitePoint);

    // Create XYZ profile
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(context);

    // Create multiprofile transform
    cmsHPROFILE profiles[3] = {deviceLinkProfile, labProfile, xyzProfile};
    cmsUInt32Number inputFormat = TYPE_Lab_16;
    cmsUInt32Number outputFormat = TYPE_XYZ_16;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = 0;
    
    cmsHTRANSFORM transform = nullptr;
    if (deviceLinkProfile && labProfile && xyzProfile) {
        transform = cmsCreateMultiprofileTransformTHR(context, profiles, 3, 
                                                     inputFormat, outputFormat, 
                                                     intent, flags);
    }

    // Get tag signature
    cmsTagSignature tagSig = cmsTagSignature(0);
    if (deviceLinkProfile) {
        tagSig = cmsGetTagSignature(deviceLinkProfile, 0);
    }

    // Adapt to illuminant
    cmsCIEXYZ sourceWhite, illuminant, value, result;
    sourceWhite.X = 0.95047; sourceWhite.Y = 1.00000; sourceWhite.Z = 1.08883;
    illuminant.X = 0.96422; illuminant.Y = 1.00000; illuminant.Z = 0.82521;
    value.X = 0.5; value.Y = 0.5; value.Z = 0.5;
    cmsAdaptToIlluminant(&result, &sourceWhite, &illuminant, &value);

    // Get gamut check pipeline and first stage
    cmsPipeline *pipeline = nullptr;
    cmsStage *firstStage = nullptr;
    if (transform) {
        pipeline = cmsGetTransformGamutCheckPipeline(transform);
        if (pipeline) {
            firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
        }
    }

    // Get stage context ID
    cmsContext stageContext = nullptr;
    if (firstStage) {
        stageContext = cmsGetStageContextID(firstStage);
    }

    // Build gamma tone curve and evaluate
    cmsToneCurve *gammaCurve = cmsBuildGamma(context, 2.2);
    cmsUInt16Number evalResult = 0;
    if (gammaCurve) {
        evalResult = cmsEvalToneCurve16(gammaCurve, 32768); // Midpoint
    }

    // Save profile to memory
    cmsUInt32Number bytesNeeded = 0;
    void *profileBuffer = nullptr;
    if (deviceLinkProfile) {
        cmsSaveProfileToMem(deviceLinkProfile, nullptr, &bytesNeeded);
        if (bytesNeeded > 0) {
            profileBuffer = malloc(bytesNeeded);
            if (profileBuffer) {
                cmsSaveProfileToMem(deviceLinkProfile, profileBuffer, &bytesNeeded);
            }
        }
    }

    // Cleanup
    if (gammaCurve) {
        cmsFreeToneCurve(gammaCurve);
    }
    if (transform) {
        cmsDeleteTransform(transform);
    }
    if (deviceLinkProfile) {
        cmsCloseProfile(deviceLinkProfile);
    }
    if (labProfile) {
        cmsCloseProfile(labProfile);
    }
    if (xyzProfile) {
        cmsCloseProfile(xyzProfile);
    }
    if (profileBuffer) {
        free(profileBuffer);
    }
    cmsDeleteContext(context);

    // Remove temporary file
    remove("input.cube");

    assert_file_closed(&cube_file);
	return 0;
}