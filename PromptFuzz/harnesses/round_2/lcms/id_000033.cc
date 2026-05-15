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
//<ID> 231
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 0
//<Quality> {"density":26,"unique_branches":{},"library_calls":["cmsOpenProfileFromFile","cmsGetProfileContextID","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateXYZProfile","cmsCloseProfile","cmsCreateTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsGetTransformOutputFormat","cmsReadTag","cmsBuildSegmentedToneCurve","cmsEstimateGamma","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsSliceSpace16","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsMLUalloc","cmsMLUgetWide","cmsCreateXYZProfile","cmsSetColorSpace","cmsCloseProfile","cmsDictAlloc","cmsDictAddEntry","cmsDictFree","cmsMLUfree","cmsIT8Free","cmsPipelineFree","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromFile","cmsGetProfileContextID","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateXYZProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsReadTag","cmsBuildSegmentedToneCurve","cmsEstimateGamma","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsSliceSpace16","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsMLUalloc","cmsMLUgetWide","cmsCreateXYZProfile","cmsSetColorSpace","cmsCloseProfile","cmsDictAlloc","cmsDictAddEntry","cmsDictFree","cmsMLUfree","cmsIT8Free","cmsPipelineFree","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile"],"visited":0}
/**/


extern "C" {
}

// Dummy sampler function for cmsSliceSpace16
static int  id_000033_dummySampler(const cmsUInt16Number* in, cmsUInt16Number* out, void* cargo) {
    // Just copy input to output
    for (int i = 0; i < 3; i++) {
        out[i] = in[i];
    }
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    cmsHPROFILE hInputProfile = NULL;
    cmsHPROFILE hOutputProfile = NULL;
    cmsHPROFILE hCubeProfile = NULL;
    cmsHTRANSFORM hTransform = NULL;
    cmsContext context = NULL;
    cmsPipeline* pipeline = NULL;
    cmsToneCurve* toneCurve = NULL;
    cmsStage* stage = NULL;
    cmsHANDLE hIT8 = NULL;
    cmsHANDLE hDict = NULL;
    cmsMLU* mlu = NULL;
    void* tagData = NULL;
    
    // 1. Create temporary files for input
    FILE* icc_file = fmemopen((void*)data, size, "rb");
    if (!icc_file) {
	assert_file_closed(&icc_file);
	return 0;
	}
	
    
    // Write input data to temporary ICC file
    const char* icc_filename = "temp_input.icc";
    FILE* icc_out = fopen(icc_filename, "wb");
    if (icc_out) {
        uint8_t* buffer = (uint8_t*)malloc(size);
        if (buffer) {
            fread(buffer, 1, size, icc_file);
            fwrite(buffer, 1, size, icc_out);
            free(buffer);
        }
        assert_file_closed(&icc_out);;
    }
    assert_file_closed(&icc_file);;
    
    // 2. Create temporary cube file (using first part of data)
    const char* cube_filename = "temp_input.cube";
    FILE* cube_file = fopen(cube_filename, "wb");
    if (cube_file) {
        size_t cube_size = size > 1024 ? 1024 : size;
        fwrite(data, 1, cube_size, cube_file);
        assert_file_closed(&cube_file);;
    }
    
    // 3. Open profile from file
    hInputProfile = cmsOpenProfileFromFile(icc_filename, "r");
    if (!hInputProfile) {
        remove(icc_filename);
        remove(cube_filename);
        assert_file_closed(&cube_file);
	assert_file_closed(&icc_out);
	assert_file_closed(&icc_file);
	return 0;
    }
    
    // 4. Get context from profile
    context = cmsGetProfileContextID(hInputProfile);
    
    // 5. Create device link from cube file
    hCubeProfile = cmsCreateDeviceLinkFromCubeFileTHR(context, cube_filename);
    
    // 6. Create output profile (XYZ profile)
    hOutputProfile = cmsCreateXYZProfile();
    if (!hOutputProfile) {
        cmsCloseProfile(hInputProfile);
        remove(icc_filename);
        remove(cube_filename);
        assert_file_closed(&cube_file);
	assert_file_closed(&icc_out);
	assert_file_closed(&icc_file);
	return 0;
    }
    
    // 7. Create transform
    hTransform = cmsCreateTransform(hInputProfile, TYPE_RGB_8, 
                                    hOutputProfile, TYPE_RGB_8, 
                                    INTENT_PERCEPTUAL, 0);
    if (!hTransform) {
        cmsCloseProfile(hOutputProfile);
        cmsCloseProfile(hInputProfile);
        if (hCubeProfile) cmsCloseProfile(hCubeProfile);
        remove(icc_filename);
        remove(cube_filename);
        assert_file_closed(&cube_file);
	assert_file_closed(&icc_out);
	assert_file_closed(&icc_file);
	return 0;
    }
    
    // 8. Get transform output format
    cmsUInt32Number outputFormat = cmsGetTransformOutputFormat(hTransform);
    
    // 9. Read tag from input profile
    tagData = cmsReadTag(hInputProfile, cmsSigRedMatrixColumnTag);
    
    // 10. Create and estimate gamma for a tone curve
    cmsCurveSegment segment = {0};
    segment.Type = 1; // Linear
    segment.x0 = 0.0f;
    segment.x1 = 1.0f;
    segment.Params[0] = 1.0;
    segment.nGridPoints = 0;
    
    toneCurve = cmsBuildSegmentedToneCurve(context, 1, &segment);
    if (toneCurve) {
        cmsFloat64Number gamma = cmsEstimateGamma(toneCurve, 0.001);
        (void)gamma; // Avoid unused variable warning
    }
    
    // 11. Allocate pipeline
    pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline && toneCurve) {
        // 12. Create stage from tone curve
        cmsToneCurve* curves[3] = {toneCurve, toneCurve, toneCurve};
        stage = cmsStageAllocToneCurves(context, 3, curves);
        
        // 13. Insert stage into pipeline
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    
    // 14. Use cmsSliceSpace16
    if (pipeline) {
        cmsUInt32Number clutPoints[3] = {5, 5, 5};
        cmsSliceSpace16(3, clutPoints,  id_000033_dummySampler, NULL);
    }
    
    // 15. Allocate IT8 structure
    hIT8 = cmsIT8Alloc(context);
    if (hIT8) {
        // 16. Set IT8 property
        cmsIT8SetPropertyDbl(hIT8, "SAMPLE_COUNT", 10.0);
    }
    
    // 17. Create MLU and get wide string
    mlu = cmsMLUalloc(context, 1);
    if (mlu) {
        wchar_t wideBuffer[256];
        cmsUInt32Number charsRead = cmsMLUgetWide(mlu, "en", "US", 
                                                  wideBuffer, 256);
        (void)charsRead; // Avoid unused variable warning
    }
    
    // 18. Set color space (on a new profile to avoid corrupting input)
    cmsHPROFILE tempProfile = cmsCreateXYZProfile();
    if (tempProfile) {
        cmsSetColorSpace(tempProfile, cmsSigRgbData);
        cmsCloseProfile(tempProfile);
    }
    
    // 19. Create dictionary and add entry
    hDict = cmsDictAlloc(context);
    if (hDict && mlu) {
        wchar_t name[] = L"TestName";
        wchar_t value[] = L"TestValue";
        cmsDictAddEntry(hDict, name, value, mlu, mlu);
    }
    
    // 20-21. Clean up resources
    if (hDict) cmsDictFree(hDict);
    if (mlu) cmsMLUfree(mlu);
    if (hIT8) cmsIT8Free(hIT8);
    if (pipeline) cmsPipelineFree(pipeline);
    if (toneCurve) cmsFreeToneCurve(toneCurve);
    if (hTransform) cmsDeleteTransform(hTransform);
    if (hCubeProfile) cmsCloseProfile(hCubeProfile);
    if (hOutputProfile) cmsCloseProfile(hOutputProfile);
    if (hInputProfile) cmsCloseProfile(hInputProfile);
    
    // Clean up temporary files
    remove(icc_filename);
    remove(cube_filename);
    
    assert_file_closed(&cube_file);
	assert_file_closed(&icc_out);
	assert_file_closed(&icc_file);
	return 0;
}