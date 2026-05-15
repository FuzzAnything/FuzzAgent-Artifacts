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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.5, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsIT8LoadFromFile","cmsIT8GetDataRowColDbl","cmsIT8GetPatchByName","cmsIT8GetPropertyMulti","cmsIT8Free","cmsBuildTabulatedToneCurve16","cmsIsToneCurveDescending","cmsReverseToneCurve","cmsCreateGrayProfile","cmsReadTag","cmsPipelineOutputChannels","cmsCloseProfile","cmsAllocProfileSequenceDescription","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsOpenProfileFromMem","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromFile","cmsIT8GetDataRowColDbl","cmsIT8GetPatchByName","cmsIT8GetPropertyMulti","cmsIT8Free","cmsBuildTabulatedToneCurve16","cmsIsToneCurveDescending","cmsReverseToneCurve","cmsCreateGrayProfile","cmsReadTag","cmsPipelineOutputChannels","cmsCloseProfile","cmsAllocProfileSequenceDescription","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsOpenProfileFromMem","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Split input data for multiple uses
    size_t split_point = size / 2;
    const uint8_t* first_half = data;
    const uint8_t* second_half = data + split_point;
    size_t first_size = split_point;
    size_t second_size = size - split_point;
    
    // 1. Create IT8 handle from input data
    FILE* in_file = fmemopen((void *)first_half, first_size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Save IT8 data to a file since cmsIT8LoadFromFile requires a file
    FILE* it8_file = fopen("it8_input_file", "wb");
    if (it8_file) {
        fwrite(first_half, 1, first_size, it8_file);
        assert_file_closed(&it8_file);;
    }
    
    cmsHANDLE it8 = cmsIT8LoadFromFile(context, "it8_input_file");
    
    // 2. Use IT8 APIs
    if (it8) {
        // Get data from row and column
        double row_col_value = cmsIT8GetDataRowColDbl(it8, 0, 0);
        
        // Get patch by name - try with some sample names
        int patch_num = cmsIT8GetPatchByName(it8, "patch1");
        
        // Get property with multi-key
        const char* prop_value = cmsIT8GetPropertyMulti(it8, "key", "subkey");
        
        // Clean up IT8
        cmsIT8Free(it8);
    }
    
    assert_file_closed(&in_file);;
    
    // 3. Create a tone curve from input data
    if (second_size >= sizeof(cmsUInt16Number) * 256) {
        cmsUInt16Number curve_values[256];
        memcpy(curve_values, second_half, sizeof(cmsUInt16Number) * 256);
        
        cmsToneCurve* tone_curve = cmsBuildTabulatedToneCurve16(
            context, 
            256, 
            curve_values
        );
        
        if (tone_curve) {
            // 4. Check if tone curve is descending
            cmsBool is_descending = cmsIsToneCurveDescending(tone_curve);
            
            // 5. Reverse the tone curve
            cmsToneCurve* reversed_curve = cmsReverseToneCurve(tone_curve);
            
            // 6. Create a simple pipeline using the tone curve
            // Create a gray profile with the tone curve
            cmsCIExyY white_point;
            white_point.x = 0.3127;
            white_point.y = 0.3290;
            white_point.Y = 1.0;
            
            cmsHPROFILE gray_profile = cmsCreateGrayProfile(
                &white_point, 
                tone_curve
            );
            
            if (gray_profile) {
                // Read pipeline from profile (assuming it exists)
                void* pipeline_data = cmsReadTag(gray_profile, cmsSigGrayTRCTag);
                
                if (pipeline_data) {
                    // 7. Get output channels from pipeline
                    cmsUInt32Number output_channels = cmsPipelineOutputChannels(
                        (cmsPipeline*)pipeline_data
                    );
                }
                
                cmsCloseProfile(gray_profile);
            }
            
            // 8. Allocate profile sequence description
            cmsSEQ* seq_desc = cmsAllocProfileSequenceDescription(context, 1);
            
            // Clean up
            if (reversed_curve) cmsFreeToneCurve(reversed_curve);
            cmsFreeToneCurve(tone_curve);
            if (seq_desc) cmsFreeProfileSequenceDescription(seq_desc);
        }
    }
    
    // 9. Additional testing with different input consumption
    // Test opening profile from memory
    if (size > 128) {
        cmsHPROFILE profile = cmsOpenProfileFromMem(data, (cmsUInt32Number)size);
        if (profile) {
            cmsCloseProfile(profile);
        }
    }
    
    // Clean up
    remove("it8_input_file");
    remove("output_file");
    
    cmsDeleteContext(context);
    
    assert_file_closed(&it8_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}