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
//<ID> 427
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 2
//<Quality> {"density":52,"unique_branches":{"RemoveCR":[[326,17,326,28,0,0,4,0],[326,32,326,43,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsCreateXYZProfileTHR","cmsDeleteContext","cmsCreate_OkLabProfile","cmsCloseProfile","cmsDeleteContext","cmsBuildGamma","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDupToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsPipelineAlloc","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsStageAllocIdentity","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsStageType","cmsPipelineInsertStage","cmsStageFree","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateTransform","cmsCreateTransform","cmsGetPostScriptCRD","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateXYZProfileTHR","cmsCreate_OkLabProfile","cmsBuildGamma","cmsDupToneCurve","cmsPipelineAlloc","cmsStageAllocIdentity","cmsStageType","cmsPipelineInsertStage","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateTransform","cmsGetPostScriptCRD","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // Write input data to a temporary file for cube file loading
    FILE *cube_file = fopen("input_cube.cube", "wb");
    if (!cube_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&cube_file);
	return 0;
    }
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;

    // 1. Create XYZ profile
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(ctx);
    if (!xyz_profile) {
        cmsDeleteContext(ctx);
        remove("input_cube.cube");
        assert_file_closed(&cube_file);
	return 0;
    }

    // 2. Create OkLab profile
    cmsHPROFILE oklab_profile = cmsCreate_OkLabProfile(ctx);
    if (!oklab_profile) {
        cmsCloseProfile(xyz_profile);
        cmsDeleteContext(ctx);
        remove("input_cube.cube");
        assert_file_closed(&cube_file);
	return 0;
    }

    // 3. Create gamma curve
    cmsToneCurve* gamma_curve = cmsBuildGamma(ctx, 2.2);
    if (!gamma_curve) {
        cmsCloseProfile(oklab_profile);
        cmsCloseProfile(xyz_profile);
        cmsDeleteContext(ctx);
        remove("input_cube.cube");
        assert_file_closed(&cube_file);
	return 0;
    }

    // 4. Duplicate gamma curve
    cmsToneCurve* dup_gamma_curve = cmsDupToneCurve(gamma_curve);
    if (!dup_gamma_curve) {
        cmsFreeToneCurve(gamma_curve);
        cmsCloseProfile(oklab_profile);
        cmsCloseProfile(xyz_profile);
        cmsDeleteContext(ctx);
        remove("input_cube.cube");
        assert_file_closed(&cube_file);
	return 0;
    }

    // 5. Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipeline) {
        cmsFreeToneCurve(dup_gamma_curve);
        cmsFreeToneCurve(gamma_curve);
        cmsCloseProfile(oklab_profile);
        cmsCloseProfile(xyz_profile);
        cmsDeleteContext(ctx);
        remove("input_cube.cube");
        assert_file_closed(&cube_file);
	return 0;
    }

    // 6. Create identity stage
    cmsStage* identity_stage = cmsStageAllocIdentity(ctx, 3);
    if (!identity_stage) {
        cmsPipelineFree(pipeline);
        cmsFreeToneCurve(dup_gamma_curve);
        cmsFreeToneCurve(gamma_curve);
        cmsCloseProfile(oklab_profile);
        cmsCloseProfile(xyz_profile);
        cmsDeleteContext(ctx);
        remove("input_cube.cube");
        assert_file_closed(&cube_file);
	return 0;
    }

    // 7. Get stage type (identity)
    cmsStageType(identity_stage);

    // 8. Insert stage into pipeline
    if (!cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, identity_stage)) {
        cmsStageFree(identity_stage);
        cmsPipelineFree(pipeline);
        cmsFreeToneCurve(dup_gamma_curve);
        cmsFreeToneCurve(gamma_curve);
        cmsCloseProfile(oklab_profile);
        cmsCloseProfile(xyz_profile);
        cmsDeleteContext(ctx);
        remove("input_cube.cube");
        assert_file_closed(&cube_file);
	return 0;
    }

    // 9. Create device link from cube file
    cmsHPROFILE cube_profile = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "input_cube.cube");
    
    // 10. Create transform (even if cube_profile is null, we try with other profiles)
    cmsHTRANSFORM transform = nullptr;
    if (cube_profile) {
        transform = cmsCreateTransform(xyz_profile, TYPE_XYZ_DBL, cube_profile, TYPE_RGB_FLT, INTENT_PERCEPTUAL, 0);
    } else {
        // Fallback transform
        transform = cmsCreateTransform(xyz_profile, TYPE_XYZ_DBL, oklab_profile, TYPE_Lab_DBL, INTENT_PERCEPTUAL, 0);
    }

    // 11. Get PostScript CRD
    if (cube_profile) {
        char crd_buffer[4096];
        cmsGetPostScriptCRD(ctx, cube_profile, INTENT_PERCEPTUAL, 0, crd_buffer, sizeof(crd_buffer));
    }

    // 12. Load IT8 from file
    cmsHANDLE it8_handle = cmsIT8LoadFromFile(ctx, "input_cube.cube");
    
    // 13. Set IT8 index column
    if (it8_handle) {
        cmsIT8SetIndexColumn(it8_handle, "Sample");
        
        // 14. Get IT8 table count
        cmsUInt32Number table_count = cmsIT8TableCount(it8_handle);
        (void)table_count; // Prevent unused variable warning
        
        // 15. Free IT8 handle
        cmsIT8Free(it8_handle);
    }

    // 16. Delete transform if created
    if (transform) {
        cmsDeleteTransform(transform);
    }

    // 17. Close cube profile if created
    if (cube_profile) {
        cmsCloseProfile(cube_profile);
    }

    // Cleanup
    cmsPipelineFree(pipeline);
    cmsFreeToneCurve(dup_gamma_curve);
    cmsFreeToneCurve(gamma_curve);
    cmsCloseProfile(oklab_profile);
    cmsCloseProfile(xyz_profile);
    cmsDeleteContext(ctx);
    
    // Remove temporary file
    remove("input_cube.cube");
    
    assert_file_closed(&cube_file);
	return 0;
}