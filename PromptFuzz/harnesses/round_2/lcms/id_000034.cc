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
//<ID> 233
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 19
//<Quality> {"density":10,"unique_branches":{"ParseCube":[[3101,9,3101,20,0,0,4,0],[3103,17,3103,63,0,0,4,1],[3129,9,3129,26,0,0,4,0],[3131,17,3131,57,0,0,4,1],[3188,17,3188,29,0,0,4,0],[3195,21,3195,34,0,0,4,1],[3202,21,3202,38,0,0,4,1],[3204,29,3204,38,0,0,4,0],[3204,29,3204,38,0,0,4,1],[3208,25,3208,52,0,0,4,1],[3219,17,3219,66,0,0,4,1]],"cmsCreateDeviceLinkFromCubeFileTHR":[[3276,9,3276,21,0,0,4,0],[3277,13,3277,63,0,0,4,1]],"ReadNumbers":[[3068,17,3068,22,0,0,4,0],[3068,17,3068,22,0,0,4,1],[3070,13,3070,30,0,0,4,1],[3073,17,3073,34,0,0,4,0]],"CLUTElemDup":[[500,13,500,34,0,0,4,0],[502,17,502,45,0,0,4,1]]},"library_calls":["cmsOpenProfileFromFile","cmsOpenProfileFromMem","cmsGetProfileContextID","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateXYZProfileTHR","cmsCreateTransform","cmsGetTransformOutputFormat","cmsReadTag","cmsBuildSegmentedToneCurve","cmsEstimateGamma","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsSliceSpace16","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsMLUalloc","cmsMLUsetWide","cmsMLUgetWide","cmsSetColorSpace","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsDictAddEntry","cmsMLUfree","cmsMLUfree","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsPipelineFree","cmsFreeToneCurve","cmsMLUfree","cmsDictFree"],"critical_calls":["cmsOpenProfileFromFile","cmsOpenProfileFromMem","cmsGetProfileContextID","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateXYZProfileTHR","cmsCreateTransform","cmsGetTransformOutputFormat","cmsReadTag","cmsBuildSegmentedToneCurve","cmsEstimateGamma","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsSliceSpace16","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsMLUalloc","cmsMLUsetWide","cmsMLUgetWide","cmsSetColorSpace","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsDictAddEntry","cmsMLUfree","cmsMLUfree","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsPipelineFree","cmsFreeToneCurve","cmsMLUfree","cmsDictFree"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;
    }

    // Create input file from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for cube data
    FILE* cube_file = fopen("input_file.cube", "wb");
    if (!cube_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&cube_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Write some cube data for device link creation
    const char* cube_data = 
        "TITLE \"Test Cube\"\n"
        "LUT_3D_SIZE 2\n"
        "0.0 0.0 0.0\n"
        "1.0 0.0 0.0\n"
        "0.0 1.0 0.0\n"
        "1.0 1.0 0.0\n"
        "0.0 0.0 1.0\n"
        "1.0 0.0 1.0\n"
        "0.0 1.0 1.0\n"
        "1.0 1.0 1.0\n";
    fwrite(cube_data, 1, strlen(cube_data), cube_file);
    assert_file_closed(&cube_file);;

    // Variables to hold resources
    cmsHPROFILE input_profile = nullptr;
    cmsHPROFILE device_link_profile = nullptr;
    cmsHPROFILE output_profile = nullptr;
    cmsHTRANSFORM transform = nullptr;
    cmsContext context = nullptr;
    cmsPipeline* pipeline = nullptr;
    cmsToneCurve* tone_curve = nullptr;
    cmsHANDLE it8 = nullptr;
    cmsHANDLE dict = nullptr;
    cmsMLU* mlu = nullptr;
    cmsStage* stage = nullptr;
    
    // 1. Open profile from input file
    input_profile = cmsOpenProfileFromFile("input_file", "rb");
    
    // If input profile couldn't be opened from file, try from memory
    if (!input_profile) {
        // Reset file pointer to beginning
        fseek(in_file, 0, SEEK_SET);
        
        // Read ICC profile from memory
        uint8_t* profile_data = (uint8_t*)malloc(size);
        if (!profile_data) {
            assert_file_closed(&in_file);;
            assert_file_closed(&cube_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
        }
        size_t read_size = fread(profile_data, 1, size, in_file);
        
        // Try to open from memory using cmsOpenProfileFromMem
        input_profile = cmsOpenProfileFromMem(profile_data, read_size);
        free(profile_data);
        
        if (!input_profile) {
            assert_file_closed(&in_file);;
            assert_file_closed(&cube_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
        }
    }
    
    // 2. Get context from profile
    context = cmsGetProfileContextID(input_profile);
    
    // 3. Create device link from cube file
    device_link_profile = cmsCreateDeviceLinkFromCubeFileTHR(context, "input_file.cube");
    
    // 4. Create XYZ profile for output
    output_profile = cmsCreateXYZProfileTHR(context);
    
    if (input_profile && output_profile) {
        // 5. Create transform
        transform = cmsCreateTransform(input_profile, TYPE_RGB_8, 
                                       output_profile, TYPE_XYZ_16,
                                       INTENT_PERCEPTUAL, 
                                       cmsFLAGS_NOOPTIMIZE | cmsFLAGS_HIGHRESPRECALC);
        
        // 6. Get transform output format
        if (transform) {
            cmsUInt32Number output_format = cmsGetTransformOutputFormat(transform);
        }
    }
    
    // 7. Read tag from profile
    if (input_profile) {
        cmsTagSignature tag_sig = cmsSigMediaWhitePointTag;
        void* tag_data = cmsReadTag(input_profile, tag_sig);
    }
    
    // 8. Create and estimate gamma from tone curve
    if (context) {
        cmsCurveSegment segments[1];
        segments[0].x0 = 0.0;
        segments[0].x1 = 1.0;
        segments[0].Type = 1; // Linear segment
        segments[0].Params[0] = 1.0; // Gamma = 1.0
        
        tone_curve = cmsBuildSegmentedToneCurve(context, 1, segments);
        
        if (tone_curve) {
            // 9. Estimate gamma
            cmsFloat64Number estimated_gamma = cmsEstimateGamma(tone_curve, 0.001);
        }
    }
    
    // 10. Create pipeline
    if (context) {
        pipeline = cmsPipelineAlloc(context, 3, 3);
    }
    
    // 11. Create stage and insert into pipeline
    if (pipeline && tone_curve) {
        // Create LUT from tone curve
        cmsToneCurve* curves[3] = { tone_curve, tone_curve, tone_curve };
        stage = cmsStageAllocToneCurves(context, 3, curves);
        
        if (stage) {
            // 12. Insert stage into pipeline
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    
    // 13. Create sampler for cmsSliceSpace16
    if (context) {
        cmsUInt32Number clutPoints[3] = { 3, 3, 3 };
        
        auto sampler = [](const cmsUInt16Number* in, cmsUInt16Number* out, void* cargo) -> cmsInt32Number {
            // Simple identity transform
            out[0] = in[0];
            out[1] = in[1];
            out[2] = in[2];
            return 1;
        };
        
        // 14. Slice space
        cmsSliceSpace16(3, clutPoints, sampler, nullptr);
    }
    
    // 15. Create IT8 handler
    if (context) {
        it8 = cmsIT8Alloc(context);
    }
    
    // 16. Set property in IT8
    if (it8) {
        cmsIT8SetPropertyDbl(it8, "SAMPLE_ID", 1.0);
    }
    
    // 17. Create MLU and get wide string
    if (context) {
        mlu = cmsMLUalloc(context, 1);
        if (mlu) {
            // Set some data in MLU
            cmsMLUsetWide(mlu, "en", "US", L"Test Description");
            
            // Get wide string
            wchar_t wide_buffer[256];
            cmsMLUgetWide(mlu, "en", "US", wide_buffer, 256);
        }
    }
    
    // 18. Set color space of device link
    if (device_link_profile) {
        cmsSetColorSpace(device_link_profile, cmsSigRgbData);
    }
    
    // 19. Create dictionary and add entry
    if (context) {
        dict = cmsDictAlloc(context);
        if (dict && mlu) {
            cmsMLU* display_name = cmsMLUalloc(context, 1);
            cmsMLU* display_value = cmsMLUalloc(context, 1);
            
            if (display_name && display_value) {
                cmsMLUsetWide(display_name, "en", "US", L"Display Name");
                cmsMLUsetWide(display_value, "en", "US", L"Display Value");
                
                cmsDictAddEntry(dict, L"TestEntry", L"TestValue", display_name, display_value);
            }
            
            if (display_name) cmsMLUfree(display_name);
            if (display_value) cmsMLUfree(display_value);
        }
    }
    
    // Clean up resources
    if (transform) {
        // 20. Delete transform
        cmsDeleteTransform(transform);
    }
    
    if (it8) {
        // 21. Free IT8 handler
        cmsIT8Free(it8);
    }
    
    if (input_profile) {
        // 22. Close profiles
        cmsCloseProfile(input_profile);
    }
    
    if (output_profile) {
        cmsCloseProfile(output_profile);
    }
    
    if (device_link_profile) {
        cmsCloseProfile(device_link_profile);
    }
    
    if (pipeline) {
        // 23. Free pipeline
        cmsPipelineFree(pipeline);
    }
    
    if (tone_curve) {
        // Free tone curve (duplicate will be freed with pipeline)
        if (!stage) {  // Only free if not inserted into pipeline
            cmsFreeToneCurve(tone_curve);
        }
    }
    
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    if (dict) {
        cmsDictFree(dict);
    }
    
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("input_file.cube");
    
    assert_file_closed(&cube_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file.cube");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file.cube");
	assert_file_name_closed("input_file");
	return 0;
}