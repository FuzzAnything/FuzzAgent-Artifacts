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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 143, nr_unique_branch: 28
//<Quality> {"density":26,"unique_branches":{"Type_Curve_Write":[[1389,9,1389,31,0,0,4,0],[1389,35,1389,64,0,0,4,1],[1400,9,1400,53,0,0,4,1]],"cmsMLUgetASCII":[[563,9,563,23,0,0,4,0]],"DecideCurveType":[[1440,9,1440,25,0,0,4,0]],"cmsSaveProfileToIOhandler":[[1564,13,1564,45,0,0,4,0]],"MemoryWrite":[[219,9,219,29,0,0,4,0]],"Type_Text_Write":[[974,9,974,18,0,0,4,1],[978,9,978,21,0,0,4,1]],"_cmsWriteHeader":[[1030,9,1030,80,0,0,4,0]],"_cmsWriteUInt8Number":[[262,9,262,57,0,0,4,1]],"_cmsWriteUInt16Array":[[288,15,288,20,0,0,4,0],[288,15,288,20,0,0,4,1],[289,13,289,49,0,0,4,1]],"Type_Text_Description_Write":[[1229,9,1229,17,0,0,4,1],[1237,13,1237,25,0,0,4,1],[1240,13,1240,25,0,0,4,1],[1262,9,1262,45,0,0,4,1],[1263,9,1263,40,0,0,4,1],[1265,9,1265,38,0,0,4,1],[1267,9,1267,45,0,0,4,1],[1269,9,1269,49,0,0,4,1],[1272,9,1272,38,0,0,4,1],[1273,9,1273,37,0,0,4,1],[1275,9,1275,36,0,0,4,1],[1278,9,1278,42,0,0,4,1],[1284,9,1284,13,0,0,4,0],[1285,9,1285,13,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsDeleteContext","cmsOpenIOhandlerFromFile","cmsCreate_sRGBProfileTHR","cmsCloseIOhandler","cmsDeleteContext","cmsSetEncodedICCversion","cmsBuildGamma","cmsGetToneCurveParametricType","cmsEvalToneCurve16","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8Free","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetTranslation","cmsMLUfree","cmsSaveProfileToMem","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseIOhandler","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenIOhandlerFromFile","cmsCreate_sRGBProfileTHR","cmsSetEncodedICCversion","cmsBuildGamma","cmsGetToneCurveParametricType","cmsEvalToneCurve16","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8Free","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetTranslation","cmsMLUfree","cmsSaveProfileToMem","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseIOhandler","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


// Simple error handler to suppress console output during fuzzing
static void  id_000020_FuzzErrorHandler(cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, const char* /*Text*/) {
    // Do nothing to avoid console spam during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<58) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_10, fuzz_char_array_size_10, fdp);
	
		const void * fuzzer_var_cmsDoTransform_22_1 = fuzz_char_array_10;//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early exit if we don't have enough data for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    // Create a context for the operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Set the error handler for this context
    cmsSetLogErrorHandlerTHR(context,  id_000020_FuzzErrorHandler);
    
    // Create a temporary file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create an output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create an IO handler from the input file
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromFile(context, "input_file", fuzz_str_1);
    if (!io_handler) {
        // Fall back to using the memory file
        rewind(in_file);
        // We'll use the in_file directly for other operations
    }
    
    // Create a simple sRGB profile
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(context);
    if (!profile) {
        if (io_handler) cmsCloseIOhandler(io_handler);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 1. Set encoded ICC version using data from input
    // Use first 4 bytes as version number if available
    if (size >= 4) {
        cmsUInt32Number version = *reinterpret_cast<const cmsUInt32Number*>(data);
        cmsSetEncodedICCversion(profile, version);
    }
    
    // 2. Create and test tone curve
    // Create a simple gamma tone curve
    cmsToneCurve* tone_curve = cmsBuildGamma(context, 2.2);
    if (tone_curve) {
        // Get parametric type of tone curve
        cmsInt32Number param_type = cmsGetToneCurveParametricType(tone_curve);
        
        // Evaluate tone curve at various points using input data
        for (size_t i = 0; i < size && i < 100; i += 2) {
            if (i + 1 < size) {
                cmsUInt16Number input_value = (data[i] << 8) | data[i + 1];
                cmsUInt16Number result = cmsEvalToneCurve16(tone_curve, input_value);
                (void)result; // Suppress unused variable warning
            }
        }
        
        // Clean up tone curve
        cmsFreeToneCurve(tone_curve);
    }
    
    // 3. Create and use IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        // Try to set a property using input data
        if (size >= 8) {
            // Use bytes 4-7 as a double value
            cmsFloat64Number dbl_value;
            memcpy(&dbl_value, data + 4, sizeof(cmsFloat64Number));
            
            // Set property on IT8 handle
            cmsIT8SetPropertyDbl(it8, fuzz_str_2, dbl_value);
        }
        
        // Clean up IT8 handle
        cmsIT8Free(it8);
    }
    
    // 4. Create and use MLU (Multi Localized Unicode) object
    cmsMLU* mlu = cmsMLUalloc(context, fuzz_uint32_t_3);
    if (mlu) {
        // Try to add some data to MLU
        if (size >= 20) {
            // Use a portion of input data as ASCII string
            char buffer[100];
            size_t copy_size = size < 100 ? size : 100;
            memcpy(buffer, data, copy_size);
            buffer[copy_size - 1] = '\0';
            
            // Add to MLU
            cmsMLUsetASCII(mlu, fuzz_str_4, fuzz_str_5, buffer);
            
            // Try to get translation
            char obtained_lang[3] = {0};
            char obtained_country[3] = {0};
            cmsMLUgetTranslation(mlu, fuzz_str_6, fuzz_str_7, obtained_lang, obtained_country);
        }
        
        // Clean up MLU
        cmsMLUfree(mlu);
    }
    
    // 5. Try to save profile to memory using input data as buffer
    if (size >= 1024) {
        void* buffer = malloc(size);
        if (buffer) {
            cmsUInt32Number bytes_needed = 0;
            cmsSaveProfileToMem(profile, buffer, &bytes_needed);
            free(buffer);
        }
    }
    
    // 6. Try to write profile to output file
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    // Create a simple transform
    cmsHTRANSFORM transform = cmsCreateTransformTHR(
        context,
        profile,
        TYPE_RGB_8,
        profile,
        TYPE_RGB_8,
        fuzz_uint32_t_8,
        fuzz_uint32_t_9
    );
    
    if (transform) {
        // Process some data through the transform
        unsigned char input_pixel[3] = {0x80, 0x80, 0x80};
        unsigned char output_pixel[3];
        
        for (size_t i = 0; i < size && i < 1000; i += 3) {
            if (i + 2 < size) {
                input_pixel[0] = data[i];
                input_pixel[1] = data[i + 1];
                input_pixel[2] = data[i + 2];
                cmsDoTransform(transform, fuzzer_var_cmsDoTransform_22_1, output_pixel, 1);
            }
        }
        
        cmsDeleteTransform(transform);
    }
    
    // Clean up resources
    if (io_handler) {
        cmsCloseIOhandler(io_handler);
    }
    
    cmsCloseProfile(profile);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    // Remove the output file we created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}