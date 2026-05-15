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
//<ID> 537
//<Prompt> []
/*<Combination>: [
*/
//<score> 104, nr_unique_branch: 3
//<Quality> {"density":26,"unique_branches":{"cmsPipelineCat":[[1619,34,1619,55,0,0,4,0]],"cmsGetTransformContextID":[[1423,9,1423,22,0,0,4,1]],"_MultiplyMatrix":[[213,29,213,61,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenIOhandlerFromStream","cmsDeleteContext","cmsMLUalloc","cmsMLUsetWide","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsCreateTransformTHR","cmsCloseProfile","cmsFreeToneCurve","cmsGetTransformContextID","cmsPipelineAlloc","cmsPipelineAlloc","cmsBuildGamma","cmsBuildGamma","cmsEstimateGamma","cmsJoinToneCurve","cmsCreateGrayProfileTHR","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsPipelineCat","cmsPipelineFree","cmsPipelineFree","cmsDeleteTransform","cmsIT8Free","cmsDictFree","cmsMLUfree","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromStream","cmsMLUalloc","cmsMLUsetWide","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsCreateTransformTHR","cmsCloseProfile","cmsFreeToneCurve","cmsGetTransformContextID","cmsPipelineAlloc","cmsPipelineAlloc","cmsBuildGamma","cmsBuildGamma","cmsEstimateGamma","cmsJoinToneCurve","cmsCreateGrayProfileTHR","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsPipelineCat","cmsPipelineFree","cmsPipelineFree","cmsDeleteTransform","cmsIT8Free","cmsDictFree","cmsMLUfree","cmsCloseIOhandler","cmsDeleteContext"],"visited":0}
/**/


// Include necessary lcms headers

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data for operations
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Create FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open I/O handler from stream
    cmsIOHANDLER* io_handler = cmsOpenIOhandlerFromStream(context, in_file);
    if (!io_handler) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate MLU
    cmsMLU* mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        // Convert some data to wide string for MLU
        // Use first 100 bytes or available size
        size_t str_size = (size > 100) ? 100 : size;
        wchar_t wide_str[51];
        for (size_t i = 0; i < 50 && i < str_size/2; ++i) {
            wide_str[i] = (wchar_t)(data[i] << 8 | data[i+1]);
        }
        wide_str[50] = 0;
        
        // Set wide string in MLU
        cmsMLUsetWide(mlu, "en", "US", wide_str);
    }
    
    // Create a simple transform to get its context ID
    cmsHTRANSFORM transform = nullptr;
    {
        // Create gray profile
        cmsToneCurve* gamma_curve = cmsBuildGamma(context, 2.2);
        if (gamma_curve) {
            cmsCIExyY white_point = {0.3127, 0.3290, 1.0}; // D65
            cmsHPROFILE gray_profile = cmsCreateGrayProfileTHR(context, &white_point, gamma_curve);
            
            if (gray_profile) {
                // Create transform
                transform = cmsCreateTransformTHR(context, 
                    gray_profile, TYPE_GRAY_8,
                    gray_profile, TYPE_GRAY_8,
                    INTENT_PERCEPTUAL, 0);
                
                cmsCloseProfile(gray_profile);
            }
            cmsFreeToneCurve(gamma_curve);
        }
    }
    
    // Get transform context ID if transform was created
    if (transform) {
        cmsContext transform_context = cmsGetTransformContextID(transform);
        (void)transform_context; // Use variable to avoid unused warning
    }
    
    // Create and manipulate pipelines
    cmsPipeline* pipeline1 = cmsPipelineAlloc(context, 3, 3);
    cmsPipeline* pipeline2 = cmsPipelineAlloc(context, 3, 3);
    
    if (pipeline1 && pipeline2) {
        // Create tone curves for demonstration
        cmsToneCurve* curve1 = cmsBuildGamma(context, 1.8);
        cmsToneCurve* curve2 = cmsBuildGamma(context, 2.4);
        
        if (curve1 && curve2) {
            // Estimate gamma
            cmsFloat64Number estimated_gamma = cmsEstimateGamma(curve1, 0.001);
            (void)estimated_gamma; // Use variable
            
            // Join tone curves
            cmsToneCurve* joined_curve = cmsJoinToneCurve(context, curve1, curve2, 256);
            if (joined_curve) {
                // Create another gray profile with joined curve
                cmsCIExyY white_point2 = {0.3457, 0.3585, 1.0}; // Different white point
                cmsHPROFILE gray_profile2 = cmsCreateGrayProfileTHR(context, &white_point2, joined_curve);
                if (gray_profile2) {
                    cmsCloseProfile(gray_profile2);
                }
                cmsFreeToneCurve(joined_curve);
            }
            
            cmsFreeToneCurve(curve1);
            cmsFreeToneCurve(curve2);
        }
        
        // Concatenate pipelines
        cmsPipelineCat(pipeline1, pipeline2);
    }
    
    // IT8 operations (simulated since we don't have actual IT8 data)
    // Create a dummy IT8 handle and get data
    cmsHANDLE it8_handle = nullptr;
    // Note: cmsIT8GetData would normally require a valid IT8 handle
    // For fuzzing, we'll skip actual IT8 operations since we don't have valid data
    
    // Dictionary operations (simulated)
    cmsHANDLE dict_handle = nullptr;
    // Note: cmsDictFree would normally require a valid dictionary handle
    
    // Cleanup
    if (pipeline1) cmsPipelineFree(pipeline1);
    if (pipeline2) cmsPipelineFree(pipeline2);
    
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (it8_handle) {
        // cmsIT8GetData(it8_handle, "patch", "sample"); // Would need valid handle
        cmsIT8Free(it8_handle);
    }
    
    if (dict_handle) {
        cmsDictFree(dict_handle);
    }
    
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    if (io_handler) {
        cmsCloseIOhandler(io_handler);
    }
    
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}