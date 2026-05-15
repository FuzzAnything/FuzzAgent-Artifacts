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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 49, nr_unique_branch: 8
//<Quality> {"density":21,"unique_branches":{"cmsFormatterForPCSOfProfile":[[4051,29,4051,37,0,0,4,0]],"cmsTransform2DeviceLink":[[1217,9,1217,20,0,0,4,1],[1236,9,1236,50,0,0,4,0],[1236,54,1236,69,0,0,4,1]],"_Remove2Op":[[146,9,146,21,0,0,4,0]],"_cmsOptimizePipeline":[[1955,9,1955,37,0,0,4,0]],"_MultiplyMatrix":[[197,12,197,24,0,0,4,0]],"OptimizeByResampling":[[673,9,673,41,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsCreateLab4ProfileTHR","cmsDeleteContext","cmsCreateTransform","cmsCloseProfile","cmsDeleteContext","cmsTransform2DeviceLink","cmsFormatterForPCSOfProfile","cmsBuildTabulatedToneCurveFloat","cmsIsToneCurveMonotonic","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8GetPatchByName","cmsIT8GetDataRowCol","cmsIT8Free","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateLab4ProfileTHR","cmsCreateTransform","cmsTransform2DeviceLink","cmsFormatterForPCSOfProfile","cmsBuildTabulatedToneCurveFloat","cmsIsToneCurveMonotonic","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8GetPatchByName","cmsIT8GetDataRowCol","cmsIT8Free","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have enough data for basic operations
    if (size < 1024) {
	return 0;
	}
	
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create Lab4 profile
    cmsCIExyY white_point = {0.3457, 0.3585, 1.0}; // D65 white point
    cmsHPROFILE lab_profile = cmsCreateLab4ProfileTHR(context, &white_point);
    if (!lab_profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create transform using the profile
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    cmsHTRANSFORM transform = cmsCreateTransform(lab_profile, 
                                                TYPE_Lab_DBL, 
                                                lab_profile, 
                                                TYPE_Lab_DBL, 
                                                intent, flags);
    if (!transform) {
        cmsCloseProfile(lab_profile);
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Convert transform to device link profile
    cmsFloat64Number version = 4.0;
    cmsHPROFILE device_link = cmsTransform2DeviceLink(transform, version, 0);
    
    // 4. Get formatter for PCS of profile
    cmsUInt32Number nBytes = 8;
    cmsBool isFloat = TRUE;
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(device_link ? device_link : lab_profile, 
                                                           nBytes, isFloat);
    
    // 5. Create tone curve from input data
    // Use first 256 floats from input data for tone curve
    size_t max_entries = 256;
    if (size >= max_entries * sizeof(cmsFloat32Number)) {
        const cmsFloat32Number *curve_data = reinterpret_cast<const cmsFloat32Number*>(data);
        cmsToneCurve *tone_curve = cmsBuildTabulatedToneCurveFloat(context, max_entries, curve_data);
        
        if (tone_curve) {
            // 6. Check if tone curve is descending
            // Note: cmsIsToneCurveDescending is not in the provided list, but we'll use
            // cmsIsToneCurveMonotonic as an alternative since it's available
            cmsBool is_monotonic = cmsIsToneCurveMonotonic(tone_curve);
            
            // 7. Get tone curve parametric type
            cmsInt32Number param_type = cmsGetToneCurveParametricType(tone_curve);
            
            cmsFreeToneCurve(tone_curve);
        }
    }
    
    // 8. Create IT8 handle and use IT8 APIs
    cmsHANDLE it8_handle = cmsIT8Alloc(context);
    if (it8_handle) {
        // Set up some sample data
        const char *sample_data = "SamplePatch";
        int row = 1;
        int col = 1;
        
        // 9. Get patch by name
        int patch_index = cmsIT8GetPatchByName(it8_handle, sample_data);
        
        // 10. Get data by row/col
        const char *cell_data = cmsIT8GetDataRowCol(it8_handle, row, col);
        
        cmsIT8Free(it8_handle);
    }
    
    // Clean up resources
    if (device_link) cmsCloseProfile(device_link);
    cmsDeleteTransform(transform);
    cmsCloseProfile(lab_profile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}