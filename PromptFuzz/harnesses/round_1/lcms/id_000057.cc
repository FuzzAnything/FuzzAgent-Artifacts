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
//<ID> 373
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":16,"unique_branches":{"cmsGetTransformInputFormat":[[1432,9,1432,22,0,0,4,1]],"cmsGetPostScriptCSA":[[1589,9,1589,23,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromMem","cmsGetPostScriptCSA","cmsGetPostScriptCSA","cmsDetectBlackPoint","cmsCreateTransform","cmsGetTransformInputFormat","cmsBuildTabulatedToneCurve16","cmsReverseToneCurveEx","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsBuildTabulatedToneCurve16","cmsReverseToneCurveEx","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsGetPostScriptCSA","cmsGetPostScriptCSA","cmsDetectBlackPoint","cmsCreateTransform","cmsGetTransformInputFormat","cmsBuildTabulatedToneCurve16","cmsReverseToneCurveEx","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsBuildTabulatedToneCurve16","cmsReverseToneCurveEx","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open profile from input data
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    uint8_t *profile_data = (uint8_t *)malloc(file_size);
    if (!profile_data) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(profile_data, 1, file_size, in_file);
    assert_file_closed(&in_file);;
    
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(profile_data, file_size);
    free(profile_data);
    
    if (hProfile) {
        // 1. cmsGetPostScriptCSA - Get PostScript Color Space Array
        uint32_t buffer_size = 0;
        // First call to get required buffer size
        cmsGetPostScriptCSA(context, hProfile, 0, 0, nullptr, 0);
        if (buffer_size > 0 && buffer_size < 1024*1024) { // Sanity check
            uint8_t *ps_buffer = (uint8_t *)malloc(buffer_size);
            if (ps_buffer) {
                cmsGetPostScriptCSA(context, hProfile, 0, 0, ps_buffer, buffer_size);
                free(ps_buffer);
            }
        }
        
        // 2. cmsDetectBlackPoint - Detect black point of profile
        cmsCIEXYZ black_point;
        cmsDetectBlackPoint(&black_point, hProfile, 0, 0);
        
        // 3. Create transform and get output format
        cmsHTRANSFORM transform = cmsCreateTransform(hProfile, TYPE_RGB_8, 
                                                     hProfile, TYPE_RGB_8, 0, 0);
        if (transform) {
            // Note: cmsGetTransformOutputFormat is not in provided list, using similar API
            uint32_t output_format = cmsGetTransformInputFormat(transform);
            // Use the output_format variable
            (void)output_format;
            
            // Create tone curve for testing other APIs
            const uint16_t curve_values[] = {0, 1000, 2000, 3000, 4000, 5000, 
                                             6000, 7000, 8000, 9000, 65535};
            cmsToneCurve *tone_curve = cmsBuildTabulatedToneCurve16(context, 11, curve_values);
            
            if (tone_curve) {
                // 4. cmsReverseToneCurveEx - Reverse tone curve
                cmsToneCurve *reversed_curve = cmsReverseToneCurveEx(256, tone_curve);
                
                // 5. cmsGetToneCurveEstimatedTable - Get estimated table
                const cmsUInt16Number *estimated_table = cmsGetToneCurveEstimatedTable(tone_curve);
                (void)estimated_table; // Use variable
                
                if (reversed_curve) {
                    cmsFreeToneCurve(reversed_curve);
                }
                cmsFreeToneCurve(tone_curve);
            }
            
            cmsDeleteTransform(transform);
        }
        
        // Create IT8 handle for testing IT8 APIs
        // Note: Since we don't have cmsIT8Alloc, we'll skip IT8 operations
        // as we cannot create a valid IT8 handle with provided APIs
        
        cmsCloseProfile(hProfile);
    }
    
    // Create a simple tone curve to test reversal without profile
    if (size >= 22) { // Need at least 11 uint16_t values
        const uint16_t *curve_data = (const uint16_t *)data;
        cmsToneCurve *test_curve = cmsBuildTabulatedToneCurve16(context, 11, curve_data);
        
        if (test_curve) {
            // Test cmsReverseToneCurveEx
            cmsToneCurve *reversed = cmsReverseToneCurveEx(256, test_curve);
            
            // Test cmsGetToneCurveEstimatedTable
            const cmsUInt16Number *table = cmsGetToneCurveEstimatedTable(test_curve);
            (void)table; // Use variable
            
            if (reversed) {
                cmsFreeToneCurve(reversed);
            }
            cmsFreeToneCurve(test_curve);
        }
    }
    
    cmsDeleteContext(context);
    assert_file_closed(&in_file);
	return 0;
}