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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 14.5, nr_unique_branch: 0
//<Quality> {"density":29,"unique_branches":{},"library_calls":["cmsCreateContext","cmsSetAdaptationState","cmsOpenProfileFromFile","cmsOpenIOhandlerFromNULL","cmsOpenProfileFromIOhandlerTHR","cmsReadRawTag","cmsBuildGamma","cmsReverseToneCurveEx","cmsFreeToneCurve","cmsFreeToneCurve","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsDoTransformLineStride","cmsXYZ2Lab","cmsDeleteTransform","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyDbl","cmsIT8SetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8EnumDataFormat","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetAdaptationState","cmsOpenProfileFromFile","cmsOpenIOhandlerFromNULL","cmsOpenProfileFromIOhandlerTHR","cmsReadRawTag","cmsBuildGamma","cmsReverseToneCurveEx","cmsFreeToneCurve","cmsFreeToneCurve","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsDoTransformLineStride","cmsXYZ2Lab","cmsDeleteTransform","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyDbl","cmsIT8SetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8SetDataRowCol","cmsIT8EnumDataFormat","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/*Here's a C++ fuzz driver using the specified lcms APIs to process color management data:

*/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data
    
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
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Set adaptation state
    cmsSetAdaptationState(0.5); // Using fixed value since we need double
    
    // 2. Open profile from file (using the input file)
    rewind(in_file);
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("input_file", "r");
    if (!hProfile) {
        // Try with IO handler approach instead
        rewind(in_file);
        cmsIOHANDLER* io = cmsOpenIOhandlerFromNULL(context);
        if (io) {
            hProfile = cmsOpenProfileFromIOhandlerTHR(context, io);
            // Note: Profile owns the IO handler, don't free separately
        }
    }
    
    // 3. Read raw tag if profile exists
    if (hProfile) {
        // Try to read a known tag - use profile description if available
        void* buffer = malloc(1024);
        if (buffer) {
            cmsUInt32Number bytesRead = cmsReadRawTag(hProfile, cmsSigProfileDescriptionTag, 
                                                     buffer, 1024);
            free(buffer);
        }
        
        // 4. Create reverse tone curve for processing
        cmsToneCurve* gamma = cmsBuildGamma(context, 2.2);
        if (gamma) {
            cmsToneCurve* reverseGamma = cmsReverseToneCurveEx(1024, gamma);
            if (reverseGamma) {
                cmsFreeToneCurve(reverseGamma);
            }
            cmsFreeToneCurve(gamma);
        }
        
        // 5. Create transform and perform conversion
        cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(context);
        if (sRGBProfile) {
            cmsHTRANSFORM transform = cmsCreateTransform(hProfile, 
                                                         TYPE_RGB_8, 
                                                         sRGBProfile, 
                                                         TYPE_RGB_8, 
                                                         INTENT_PERCEPTUAL, 
                                                         0);
            
            if (transform) {
                // 6. Prepare buffers for transform with stride
                const int width = 2;
                const int height = 2;
                const int channels = 3;
                const int stride_in = width * channels;
                const int stride_out = width * channels;
                
                unsigned char input_buffer[width * height * channels];
                unsigned char output_buffer[width * height * channels];
                
                // Fill input buffer with data from fuzzed input
                size_t data_to_copy = std::min(sizeof(input_buffer), size);
                memcpy(input_buffer, data, data_to_copy);
                
                // 7. Perform transform with line stride
                cmsDoTransformLineStride(transform, 
                                        input_buffer, 
                                        output_buffer, 
                                        width, 
                                        height, 
                                        stride_in, 
                                        stride_out, 
                                        0,  // Bytes per plane in
                                        0); // Bytes per plane out
                
                // 8. Use XYZ to Lab conversion
                cmsCIEXYZ whitePoint = {0.95047, 1.00000, 1.08883}; // D65
                cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
                cmsCIELab lab;
                cmsXYZ2Lab(&whitePoint, &lab, &xyz);
                
                cmsDeleteTransform(transform);
            }
            cmsCloseProfile(sRGBProfile);
        }
        
        // 9. Work with IT8 data format enumeration
        cmsHANDLE hIT8 = cmsIT8Alloc(context);
        if (hIT8) {
            // Set some properties to create data format
            cmsIT8SetSheetType(hIT8, "CGATS.17");
            cmsIT8SetPropertyDbl(hIT8, "NUMBER_OF_FIELDS", 4.0);
            cmsIT8SetPropertyDbl(hIT8, "NUMBER_OF_SETS", 2.0);
            
            // Create sample data
            cmsIT8SetDataRowCol(hIT8, 0, 0, "R");
            cmsIT8SetDataRowCol(hIT8, 0, 1, "G");
            cmsIT8SetDataRowCol(hIT8, 0, 2, "B");
            cmsIT8SetDataRowCol(hIT8, 0, 3, "Name");
            cmsIT8SetDataRowCol(hIT8, 1, 0, "255");
            cmsIT8SetDataRowCol(hIT8, 1, 1, "128");
            cmsIT8SetDataRowCol(hIT8, 1, 2, "64");
            cmsIT8SetDataRowCol(hIT8, 1, 3, "Sample");
            
            // 10. Enumerate data format
            char** sampleNames = nullptr;
            int formatCount = cmsIT8EnumDataFormat(hIT8, &sampleNames);
            
            // Clean up sample names if allocated
            if (sampleNames) {
                for (int i = 0; i < formatCount && sampleNames[i]; i++) {
                    // Names are owned by IT8 handle, just clear pointer array
                }
            }
            
            cmsIT8Free(hIT8);
        }
        
        cmsCloseProfile(hProfile);
    }
    
    // Clean up
    cmsDeleteContext(context);
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