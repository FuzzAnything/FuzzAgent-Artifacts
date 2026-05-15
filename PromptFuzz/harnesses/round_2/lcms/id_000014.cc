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
//<ID> 84
//<Prompt> []
/*<Combination>: [
*/
//<score> 147, nr_unique_branch: 47
//<Quality> {"density":14,"unique_branches":{"_cmsReasonableGridpointsByColorspace":[[700,9,700,23,0,0,4,0]],"OptimizeByResampling":[[663,47,663,82,0,0,4,0]],"WriteInputLUT":[[873,9,873,22,0,0,4,1],[883,5,883,11,0,0,4,1],[890,5,890,11,0,0,4,1],[891,5,891,11,0,0,4,0],[898,17,898,35,0,0,4,1],[908,17,908,20,0,0,4,1],[915,5,915,12,0,0,4,1]],"_cmsEndPointsBySpace":[[742,33,742,38,0,0,4,0],[743,32,743,37,0,0,4,1]],"EmitCIEBasedDEF":[[764,5,764,11,0,0,4,1],[771,5,771,11,0,0,4,0],[779,5,779,12,0,0,4,1],[786,9,786,52,0,0,4,1],[795,9,795,48,0,0,4,0]],"OptimizeMatrixShaper":[[1691,12,1691,41,0,0,4,0]],"_cmsICCcolorSpace":[[775,8,775,20,0,0,4,0]],"OptimizeByComputingLinearization":[[1062,47,1062,82,0,0,4,0]],"cmsChannelsOfColorSpace":[[882,5,882,26,0,0,4,0]],"OutputValueSampler":[[569,9,569,23,0,0,4,1]],"cmsCreateExtendedTransform":[[1253,13,1253,69,0,0,4,0]],"PatchLUT":[[485,9,485,25,0,0,4,0],[497,13,497,29,0,0,4,1],[498,13,498,29,0,0,4,1],[499,13,499,29,0,0,4,1],[500,13,500,29,0,0,4,1]],"IsProperColorSpace":[[1076,9,1076,25,0,0,4,0]],"DefaultICCintents":[[576,31,576,66,0,0,4,0],[576,70,576,86,0,0,4,1]],"GenerateCSA":[[1046,9,1046,61,0,0,4,0],[1048,13,1048,55,0,0,4,0],[1057,13,1057,40,0,0,4,0],[1058,13,1058,40,0,0,4,0],[1058,13,1058,40,0,0,4,1],[1067,13,1067,24,0,0,4,0],[1071,13,1071,118,0,0,4,1],[1078,16,1078,62,0,0,4,1],[1093,9,1093,20,0,0,4,1]],"OptimizeByJoiningCurves":[[1410,47,1410,82,0,0,4,0]],"_cmsReadInputLUT":[[317,9,317,61,0,0,4,0],[322,13,322,23,0,0,4,0]],"WriteNamedColorCSA":[[996,9,996,22,0,0,4,0]],"cmsDeleteTransform":[[162,9,162,28,0,0,4,0]],"BuildRGBOutputMatrixShaper":[[479,9,479,19,0,0,4,0],[479,23,479,33,0,0,4,0],[479,37,479,47,0,0,4,0]]},"library_calls":["cmsOpenProfileFromFile","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsReadTag","cmsEstimateGamma","cmsGetPostScriptCSA","cmsGetProfileContextID","cmsGetProfileContextID","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsBuildSegmentedToneCurve","cmsFreeToneCurve","cmsSliceSpace16","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsReadTag","cmsMLUgetWide","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsPipelineFree","cmsStageFree"],"critical_calls":["cmsOpenProfileFromFile","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsReadTag","cmsEstimateGamma","cmsGetPostScriptCSA","cmsGetProfileContextID","cmsGetProfileContextID","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsBuildSegmentedToneCurve","cmsFreeToneCurve","cmsSliceSpace16","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsReadTag","cmsMLUgetWide","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsPipelineFree"],"visited":1}
/**/


// Include lcms2 headers

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
	 // Need minimum data for meaningful fuzzing
    
    // Create input file from memory
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
    
    cmsContext context = NULL;
    cmsHPROFILE input_profile = NULL;
    cmsHPROFILE srgb_profile = NULL;
    cmsHPROFILE output_profile = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsPipeline *pipeline = NULL;
    cmsStage *stage = NULL;
    cmsToneCurve *tone_curve = NULL;
    cmsHANDLE it8_handle = NULL;
    cmsMLU *mlu = NULL;
    cmsCurveSegment segment;
    
    // 1. Open input profile from file
    input_profile = cmsOpenProfileFromFile("input_file", "r");
    
    // 2. Create sRGB profile
    srgb_profile = cmsCreate_sRGBProfile();
    
    // 3. Create transform
    if (input_profile && srgb_profile) {
        transform = cmsCreateTransform(
            srgb_profile, 
            TYPE_RGB_8, 
            input_profile, 
            TYPE_RGB_8, 
            INTENT_PERCEPTUAL, 
            0
        );
    }
    
    // 4. Get transform output format
    if (transform) {
        cmsUInt32Number output_format = cmsGetTransformOutputFormat(transform);
        (void)output_format; // Use to avoid unused variable warning
    }
    
    // 5. Read a tag from profile
    if (input_profile) {
        void *tag_data = cmsReadTag(input_profile, cmsSigRedTRCTag);
        if (tag_data) {
            tone_curve = (cmsToneCurve*)tag_data;
            
            // 6. Estimate gamma from tone curve
            if (tone_curve) {
                cmsFloat64Number gamma = cmsEstimateGamma(tone_curve, 0.001);
                (void)gamma; // Use to avoid unused variable warning
            }
        }
    }
    
    // 7. Get PostScript CSA
    if (input_profile) {
        unsigned char buffer[1024];
        cmsUInt32Number buffer_len = cmsGetPostScriptCSA(
            cmsGetProfileContextID(input_profile),
            input_profile,
            INTENT_PERCEPTUAL,
            0,
            buffer,
            sizeof(buffer)
        );
        (void)buffer_len; // Use to avoid unused variable warning
    }
    
    // 8. Create pipeline
    context = cmsGetProfileContextID(input_profile ? input_profile : srgb_profile);
    if (context) {
        pipeline = cmsPipelineAlloc(context, 3, 3);
    }
    
    // 9. Allocate identity stage
    if (pipeline && context) {
        stage = cmsStageAllocIdentity(context, 3);
    }
    
    // 10. Insert stage into pipeline
    if (pipeline && stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
    }
    
    // 11. Build segmented tone curve
    if (context) {
        // Initialize curve segment
        memset(&segment, 0, sizeof(segment));
        segment.x0 = 0.0f;
        segment.x1 = 1.0f;
        segment.Type = 1; // Gamma
        segment.Params[0] = 2.2; // Gamma value
        
        cmsToneCurve *seg_curve = cmsBuildSegmentedToneCurve(context, 1, &segment);
        if (seg_curve) {
            // 12. Free tone curve
            cmsFreeToneCurve(seg_curve);
        }
    }
    
    // 13. Use cmsSliceSpace16
    if (pipeline) {
        cmsUInt32Number clutPoints[] = {17, 17, 17};
        
        // Define a simple sampler function
        auto sampler = [](const cmsUInt16Number* in, cmsUInt16Number* out, void* cargo) -> cmsInt32Number {
            (void)cargo;
            // Simple identity transform
            out[0] = in[0];
            out[1] = in[1];
            out[2] = in[2];
            return 1;
        };
        
        cmsBool result = cmsSliceSpace16(3, clutPoints, sampler, NULL);
        (void)result; // Use to avoid unused variable warning
    }
    
    // 14. Allocate IT8 handle
    if (context) {
        it8_handle = cmsIT8Alloc(context);
    }
    
    // 15. Set IT8 property (double)
    if (it8_handle) {
        cmsBool set_result = cmsIT8SetPropertyDbl(it8_handle, "SAMPLE_FACTOR", 1.0);
        (void)set_result; // Use to avoid unused variable warning
    }
    
    // 16. Get MLU wide string
    if (input_profile) {
        // Try to get copyright tag
        mlu = (cmsMLU*)cmsReadTag(input_profile, cmsSigCopyrightTag);
        if (mlu) {
            wchar_t wide_buffer[256];
            cmsUInt32Number wide_len = cmsMLUgetWide(
                mlu,
                "en",  // Language code
                "US",  // Country code
                wide_buffer,
                sizeof(wide_buffer) / sizeof(wchar_t)
            );
            (void)wide_len; // Use to avoid unused variable warning
        }
    }
    
    // 17. Delete transform
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    // 18. Free IT8 handle
    if (it8_handle) {
        cmsIT8Free(it8_handle);
    }
    
    // 19. Close profiles
    if (input_profile) {
        cmsCloseProfile(input_profile);
    }
    if (srgb_profile) {
        cmsCloseProfile(srgb_profile);
    }
    if (output_profile) {
        cmsCloseProfile(output_profile);
    }
    
    // 20. Free pipeline
    if (pipeline) {
        cmsPipelineFree(pipeline);
    } else if (stage) {
        // If pipeline wasn't created but stage was, free stage separately
        cmsStageFree(stage);
    }
    
    // Clean up file handles
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