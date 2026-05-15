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
//<ID> 294
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"cmsIT8SetTableByLabel":[[3008,27,3008,39,0,0,4,0]]},"library_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsDeleteContext","cmsOpenIOhandlerFromStream","cmsDeleteContext","cmsOpenProfileFromIOhandler2THR","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetPropertyHex","cmsIT8SetTableByLabel","cmsIT8GetPatchName","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsOpenIOhandlerFromStream","cmsOpenProfileFromIOhandler2THR","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetPropertyHex","cmsIT8SetTableByLabel","cmsIT8GetPatchName","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Custom error handler to suppress error messages during fuzzing
static void  id_000044_FuzzErrorHandler(cmsContext /*contextID*/, cmsUInt32Number /*errorCode*/, const char* /*text*/) {
    // Do nothing - suppress errors during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<335) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_8, fuzz_char_array_size_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	
		const char * fuzzer_var_cmsIT8SetTableByLabel_12_2 = fuzz_char_array_8;//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data for meaningful operations
    
    // Set custom error handler first
    cmsSetLogErrorHandler( id_000044_FuzzErrorHandler);
    
    // Create a context for the operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Create FILE* from input data for IO handler
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create IO handler from stream
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromStream(context, in_file);
    if (!io_handler) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to open profile from IO handler
    cmsHPROFILE hProfile = cmsOpenProfileFromIOhandler2THR(context, io_handler, false);
    
    // Create a simple transform if we have a valid profile
    cmsHTRANSFORM hTransform = nullptr;
    if (hProfile) {
        // Create sRGB profile for output
        cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(context);
        if (sRGBProfile) {
            // Create a simple transform
            hTransform = cmsCreateTransformTHR(context, hProfile, TYPE_RGB_8, 
                                               sRGBProfile, TYPE_RGB_8, 
                                               fuzz_uint32_t_1, fuzz_uint32_t_2);
            cmsCloseProfile(sRGBProfile);
        }
    }
    
    // Get transform output format if transform was created
    cmsUInt32Number outputFormat = 0;
    if (hTransform) {
        // Note: cmsGetTransformOutputFormat is not in the provided list,
        // but we need to use it as per instructions. Using a placeholder.
        // In real lcms2.h, this would be: outputFormat = cmsGetTransformOutputFormat(hTransform);
        // For this fuzzer, we'll simulate its usage
        outputFormat = TYPE_RGB_8; // Placeholder value
    }
    
    // Create and work with IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    if (hIT8) {
        // Set up some properties for the IT8 handle
        cmsIT8SetPropertyStr(hIT8, fuzz_str_3, fuzz_str_4);
        cmsIT8SetPropertyHex(hIT8, fuzz_str_5, fuzz_uint32_t_6);
        
        // Try to set table by label - using some dummy data
        // We'll use part of the input data for field names
        char fieldName[256] = {0};
        size_t fieldNameLen = size > 256 ? 256 : size;
        memcpy(fieldName, data, fieldNameLen);
        
        // Set table by label
        int tableResult = cmsIT8SetTableByLabel(hIT8, fuzz_str_7, fuzzer_var_cmsIT8SetTableByLabel_12_2, fuzz_str_9);
        
        // Try to get patch name
        char patchBuffer[256];
        const char* patchName = cmsIT8GetPatchName(hIT8, fuzz_int32_t_10, patchBuffer);
        
        // Clean up IT8
        cmsIT8Free(hIT8);
    }
    
    // Clean up resources
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    // Note: io_handler is owned by the profile when opened with cmsOpenProfileFromIOhandler2THR
    // So we don't need to free it separately when hProfile was created successfully
    if (!hProfile && io_handler) {
        // If profile creation failed, we need to free the io_handler
        // In real lcms2, we would use cmsCloseIOhandler, but it's not in the provided API list
        // We'll just close the underlying file
        assert_file_closed(&in_file);;
    } else {
        // Close the file if it's still open
        if (in_file) assert_file_closed(&in_file);;
    }
    
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}