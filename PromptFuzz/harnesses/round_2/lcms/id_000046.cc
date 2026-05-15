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
//<ID> 356
//<Prompt> []
/*<Combination>: [
*/
//<score> 121, nr_unique_branch: 29
//<Quality> {"density":11,"unique_branches":{"SetLinks":[[1516,17,1516,23,0,0,4,1]],"Type_Chromaticity_Write":[[468,9,468,38,0,0,4,1],[469,9,469,38,0,0,4,1],[471,9,471,65,0,0,4,1],[472,9,472,67,0,0,4,1],[473,9,473,66,0,0,4,1]],"OptimizeByJoiningCurves":[[1458,9,1458,44,0,0,4,0],[1461,13,1461,71,0,0,4,1],[1467,13,1467,46,0,0,4,0],[1470,18,1470,29,0,0,4,1]],"SaveOneChromaticity":[[457,9,457,79,0,0,4,1],[458,9,458,79,0,0,4,1]],"Type_S15Fixed16_Write":[[781,15,781,25,0,0,4,0],[781,15,781,25,0,0,4,1],[783,13,783,52,0,0,4,1]],"CurvesFree":[[1273,16,1273,35,0,0,4,0],[1273,16,1273,35,0,0,4,1]],"AllCurvesAreLinear":[[462,13,462,45,0,0,4,0]],"CurvesAlloc":[[1307,9,1307,20,0,0,4,1],[1313,9,1313,28,0,0,4,1],[1318,15,1318,26,0,0,4,0],[1318,15,1318,26,0,0,4,1],[1322,13,1322,35,0,0,4,1],[1332,13,1332,30,0,0,4,0],[1334,23,1334,36,0,0,4,0],[1334,23,1334,36,0,0,4,1]],"cmsIT8SetTableByLabel":[[3008,27,3008,39,0,0,4,0]],"FastEvaluateCurves8":[[1359,15,1359,33,0,0,4,0],[1359,15,1359,33,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsBuildTabulatedToneCurveFloat","cmsBuildTabulatedToneCurveFloat","cmsBuildTabulatedToneCurveFloat","cmsCreateRGBProfileTHR","cmsSetHeaderAttributes","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsIT8GetPropertyDbl","cmsIT8Free","cmsCreateTransform","cmsGetTransformContextID","cmsGetAlarmCodesTHR","cmsDoTransform","cmsDeleteTransform","cmsLinkTag","cmsGetToneCurveEstimatedTableEntries","cmsOpenProfileFromStream","cmsGetHeaderFlags","cmsGetProfileVersion","cmsCloseProfile","cmsSaveProfileToFile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsBuildTabulatedToneCurveFloat","cmsBuildTabulatedToneCurveFloat","cmsBuildTabulatedToneCurveFloat","cmsCreateRGBProfileTHR","cmsSetHeaderAttributes","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsIT8GetPropertyDbl","cmsIT8Free","cmsCreateTransform","cmsGetTransformContextID","cmsGetAlarmCodesTHR","cmsDoTransform","cmsDeleteTransform","cmsLinkTag","cmsGetToneCurveEstimatedTableEntries","cmsOpenProfileFromStream","cmsGetHeaderFlags","cmsGetProfileVersion","cmsCloseProfile","cmsSaveProfileToFile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*To create a fuzz driver for the LCMS library using the specified APIs, we'll need to:

1. Create an RGB profile using the input data
2. Work with IT8 color data tables
3. Handle color transforms and alarm codes
4. Manage tone curves and profile attributes

Here's a comprehensive fuzz driver that uses all the specified APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create RGB profile using input data
    cmsCIExyY white_point = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},   // Red
        {0.30, 0.60, 1.0},   // Green  
        {0.15, 0.06, 1.0}    // Blue
    };
    
    // Create tone curves from input data
    cmsFloat32Number curve_data[256];
    for (int i = 0; i < 256 && i * sizeof(float) < size; i++) {
        curve_data[i] = ((float)data[i % size]) / 255.0f;
    }
    
    cmsToneCurve* transfer_functions[3] = {
        cmsBuildTabulatedToneCurveFloat(context, 256, curve_data),
        cmsBuildTabulatedToneCurveFloat(context, 256, curve_data),
        cmsBuildTabulatedToneCurveFloat(context, 256, curve_data)
    };
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfileTHR(
        context, 
        &white_point, 
        &primaries, 
        transfer_functions
    );
    
    // 2. Set header attributes
    if (rgb_profile && size > 8) {
        cmsUInt64Number flags = *((cmsUInt64Number*)data);
        cmsSetHeaderAttributes(rgb_profile, flags);
    }
    
    // 3. Create IT8 handle and use IT8 functions
    cmsHANDLE it8_handle = cmsIT8Alloc(context);
    if (it8_handle) {
        // Set table by label using data for strings
        char set_name[50], field_name[50], expected_type[50];
        size_t offset = 0;
        
        if (size > offset + 50) {
            memcpy(set_name, data + offset, 49);
            set_name[49] = '\0';
            offset += 50;
        } else {
            strcpy(set_name, "DefaultSet");
        }
        
        if (size > offset + 50) {
            memcpy(field_name, data + offset, 49);
            field_name[49] = '\0';
            offset += 50;
        } else {
            strcpy(field_name, "DefaultField");
        }
        
        if (size > offset + 50) {
            memcpy(expected_type, data + offset, 49);
            expected_type[49] = '\0';
        } else {
            strcpy(expected_type, "double");
        }
        
        cmsIT8SetTableByLabel(it8_handle, set_name, field_name, expected_type);
        
        // Get property as double
        cmsFloat64Number prop_value = cmsIT8GetPropertyDbl(it8_handle, "SampleProperty");
        
        cmsIT8Free(it8_handle);
    }
    
    // 4. Create transform and get context ID
    cmsHTRANSFORM transform = nullptr;
    if (rgb_profile) {
        // Create a simple transform
        transform = cmsCreateTransform(
            rgb_profile, 
            TYPE_RGB_8, 
            rgb_profile, 
            TYPE_RGB_8, 
            INTENT_PERCEPTUAL, 
            0
        );
        
        if (transform) {
            // Get transform context ID
            cmsContext transform_context = cmsGetTransformContextID(transform);
            
            // Get alarm codes using the context
            cmsUInt16Number alarm_codes[16];
            cmsGetAlarmCodesTHR(transform_context, alarm_codes);
            
            // Use the transform with input data
            if (size >= 100) {
                uint8_t input_rgb[100];
                uint8_t output_rgb[100];
                memcpy(input_rgb, data, (size < 100) ? size : 100);
                cmsDoTransform(transform, input_rgb, output_rgb, 1);
            }
            
            cmsDeleteTransform(transform);
        }
    }
    
    // 5. Link tags if we have a profile
    if (rgb_profile) {
        // Try to link some tags - using data to determine which ones
        cmsTagSignature src_tag = (size > 0) ? 
            (cmsTagSignature)(data[0] % 20 + cmsSigMediaBlackPointTag) : 
            cmsSigMediaBlackPointTag;
        cmsTagSignature dest_tag = (size > 1) ? 
            (cmsTagSignature)(data[1] % 20 + cmsSigMediaWhitePointTag) : 
            cmsSigMediaWhitePointTag;
        
        cmsLinkTag(rgb_profile, src_tag, dest_tag);
    }
    
    // 6. Get tone curve estimated table entries
    if (transfer_functions[0]) {
        cmsUInt32Number estimated_entries = cmsGetToneCurveEstimatedTableEntries(transfer_functions[0]);
    }
    
    // 7. Try to open profile from the input file
    cmsHPROFILE input_profile = cmsOpenProfileFromStream(in_file, "r");
    if (input_profile) {
        // Read some tags if profile is valid
        cmsUInt32Number flags = cmsGetHeaderFlags(input_profile);
        cmsFloat64Number version = cmsGetProfileVersion(input_profile);
        cmsCloseProfile(input_profile);
    }
    
    // 8. Try to save profile to output file
    if (rgb_profile) {
        cmsSaveProfileToFile(rgb_profile, "output_file");
    }
    
    // Clean up
    for (int i = 0; i < 3; i++) {
        if (transfer_functions[i]) {
            cmsFreeToneCurve(transfer_functions[i]);
        }
    }
    
    if (rgb_profile) {
        cmsCloseProfile(rgb_profile);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    cmsDeleteContext(context);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}