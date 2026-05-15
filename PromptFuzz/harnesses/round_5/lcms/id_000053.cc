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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 1
//<Quality> {"density":23,"unique_branches":{"cmsIT8GetDataRowColDbl":[[2839,9,2839,23,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsDeleteContext","cmsGetColorSpace","cmsBuildParametricToneCurve","cmsCreateGrayProfile","cmsStageAllocIdentity","cmsStageInputChannels","cmsGetStageContextID","cmsMLUalloc","cmsMLUsetASCII","cmsMLUtranslationsCodes","cmsIT8LoadFromFile","cmsIT8GetDataRowColDbl","cmsIT8GetDataRowCol","cmsStageAllocCLut16bitGranular","cmsStageFree","cmsIT8Free","cmsMLUfree","cmsStageFree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsGetColorSpace","cmsBuildParametricToneCurve","cmsCreateGrayProfile","cmsStageAllocIdentity","cmsStageInputChannels","cmsGetStageContextID","cmsMLUalloc","cmsMLUsetASCII","cmsMLUtranslationsCodes","cmsIT8LoadFromFile","cmsIT8GetDataRowColDbl","cmsIT8GetDataRowCol","cmsStageAllocCLut16bitGranular","cmsStageFree","cmsIT8Free","cmsMLUfree","cmsStageFree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*To achieve the described event using the provided lcms APIs, we'll create a fuzz driver that:
1. Creates a context
2. Creates a NULL profile
3. Gets the color space of the profile
4. Builds a parametric tone curve
5. Creates a gray profile
6. Allocates identity and CLUT stages
7. Allocates and manipulates MLU objects
8. Loads and processes IT8 data from the input byte stream

Here's the complete implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data for meaningful processing

    // Create temporary file for IT8 data
    FILE *it8_file = fmemopen((void *)data, size, "rb");
    if (!it8_file) {
	assert_file_closed(&it8_file);
	return 0;
	}
	

    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        assert_file_closed(&it8_file);;
        assert_file_closed(&it8_file);
	return 0;
    }

    // 2. Create NULL profile
    cmsHPROFILE null_profile = cmsCreateNULLProfileTHR(context);
    if (!null_profile) {
        cmsDeleteContext(context);
        assert_file_closed(&it8_file);;
        assert_file_closed(&it8_file);
	return 0;
    }

    // 3. Get color space of NULL profile
    cmsColorSpaceSignature color_space = cmsGetColorSpace(null_profile);

    // 4. Build parametric tone curve (simple gamma 2.2 curve)
    cmsFloat64Number gamma_params[1] = {2.2};
    cmsToneCurve *tone_curve = cmsBuildParametricToneCurve(context, 1, gamma_params);
    
    if (tone_curve) {
        // 5. Create gray profile with D50 white point
        cmsCIExyY white_point_d50 = {0.3457, 0.3585, 1.0};
        cmsHPROFILE gray_profile = cmsCreateGrayProfile(&white_point_d50, tone_curve);
        
        if (gray_profile) {
            // 6. Allocate identity stage
            cmsStage *identity_stage = cmsStageAllocIdentity(context, 3);
            if (identity_stage) {
                // 7. Get input channels of identity stage
                cmsUInt32Number input_channels = cmsStageInputChannels(identity_stage);
                
                // 8. Get stage context ID
                cmsContext stage_context = cmsGetStageContextID(identity_stage);
                
                // 9. Allocate MLU
                cmsMLU *mlu = cmsMLUalloc(context, 10);
                if (mlu) {
                    // Set ASCII string in MLU
                    char lang_code[3] = {'e', 'n', '\0'};
                    char country_code[3] = {'U', 'S', '\0'};
                    cmsMLUsetASCII(mlu, lang_code, country_code, "Test String");
                    
                    // Get translation codes
                    char out_lang[3], out_country[3];
                    cmsMLUtranslationsCodes(mlu, 0, out_lang, out_country);
                }
                
                // 10. Load IT8 data from file
                rewind(it8_file);
                cmsHANDLE it8_handle = cmsIT8LoadFromFile(context, "input_file");
                
                if (it8_handle) {
                    // Create temporary output file for IT8 data
                    FILE *temp_out = fopen("output_file", "wb");
                    if (temp_out) {
                        fwrite(data, 1, size, temp_out);
                        assert_file_closed(&temp_out);;
                    }
                    
                    // 11. Get data from IT8 (try multiple rows/cols)
                    for (int row = 0; row < 3; row++) {
                        for (int col = 0; col < 3; col++) {
                            // Try to get double value
                            cmsFloat64Number dbl_val = cmsIT8GetDataRowColDbl(it8_handle, row, col);
                            (void)dbl_val; // Prevent unused variable warning
                            
                            // Try to get string value
                            const char *str_val = cmsIT8GetDataRowCol(it8_handle, row, col);
                            (void)str_val; // Prevent unused variable warning
                        }
                    }
                    
                    // 12. Allocate CLUT stage
                    cmsUInt32Number clut_points[] = {5, 5, 5}; // 5x5x5 CLUT
                    cmsUInt16Number table_data[5 * 5 * 5 * 3]; // 3 output channels
                    
                    // Fill with some data
                    for (size_t i = 0; i < sizeof(table_data)/sizeof(table_data[0]); i++) {
                        table_data[i] = static_cast<cmsUInt16Number>(i * 1000);
                    }
                    
                    cmsStage *clut_stage = cmsStageAllocCLut16bitGranular(
                        context, 
                        clut_points, 
                        3, // input channels
                        3, // output channels
                        table_data
                    );
                    
                    // Clean up CLUT stage if created
                    if (clut_stage) {
                        cmsStageFree(clut_stage);
                    }
                    
                    // 13. Free IT8 handle
                    cmsIT8Free(it8_handle);
                }
                
                // 14. Free MLU if allocated
                if (mlu) {
                    cmsMLUfree(mlu);
                }
                
                // 15. Free identity stage
                cmsStageFree(identity_stage);
            }
            
            // 16. Close gray profile
            cmsCloseProfile(gray_profile);
        }
        
        // 17. Free tone curve
        cmsFreeToneCurve(tone_curve);
    }
    
    // 18. Close NULL profile
    cmsCloseProfile(null_profile);
    
    // 19. Delete context
    cmsDeleteContext(context);
    
    // 20. Close file handle
    assert_file_closed(&it8_file);;
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&it8_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}