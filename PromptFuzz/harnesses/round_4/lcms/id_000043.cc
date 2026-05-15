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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"cmsGetTransformOutputFormat":[[1440,9,1440,22,0,0,4,1]]},"library_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsDeleteContext","cmsOpenIOhandlerFromStream","cmsDeleteContext","cmsOpenProfileFromIOhandler2THR","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsGetTransformOutputFormat","cmsCloseProfile","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsIT8GetPatchName","cmsIT8Free","cmsDeleteTransform","cmsDeleteContext"],"critical_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsOpenIOhandlerFromStream","cmsOpenProfileFromIOhandler2THR","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsGetTransformOutputFormat","cmsCloseProfile","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsIT8GetPatchName","cmsIT8Free","cmsDeleteTransform","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // 1. Set error handler first
    cmsSetLogErrorHandler([](cmsContext, cmsUInt32Number, const char *) {
        // Error handler callback - no-op for fuzzing
    });
    
    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 2. Create FILE* from input data for cmsOpenIOhandlerFromStream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create I/O handler from stream
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromStream(ctx, in_file);
    if (!io_handler) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Try to open profile from I/O handler
    cmsHPROFILE hProfile = cmsOpenProfileFromIOhandler2THR(ctx, io_handler, false);
    
    // 5. Create a transform if we got a valid profile
    cmsHTRANSFORM hTransform = nullptr;
    if (hProfile) {
        // Create sRGB profile for output
        cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(ctx);
        if (sRGBProfile) {
            // Create transform
            hTransform = cmsCreateTransformTHR(ctx, hProfile, 
                                             TYPE_RGB_8, 
                                             sRGBProfile, 
                                             TYPE_RGB_8, 
                                             fuzz_uint32_t_1, 
                                             fuzz_uint32_t_2);
            
            // 6. Get transform output format
            if (hTransform) {
                cmsUInt32Number outputFormat = cmsGetTransformOutputFormat(hTransform);
                // Just use the value to prevent optimization
                volatile cmsUInt32Number dummy = outputFormat;
                (void)dummy;
            }
            
            cmsCloseProfile(sRGBProfile);
        }
        cmsCloseProfile(hProfile);
    }
    
    // 7. Create IT8 handle for IT8 operations
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        // 8. Set table by label
        // Use data-dependent values for table and field names
        const char *tableSet = "Table1";
        const char *field = "Field1";
        const char *type = "string";
        
        // Ensure we have enough data for the operation
        if (size > 50) {
            // Use first few bytes to determine which table/field to use
            int tableIndex = data[0] % 3;
            int fieldIndex = data[1] % 3;
            
            const char* tables[] = {"Table1", "Table2", "Table3"};
            const char* fields[] = {"Field1", "Field2", "Field3"};
            
            int result = cmsIT8SetTableByLabel(hIT8, 
                                             tables[tableIndex], 
                                             fields[fieldIndex], 
                                             fuzz_str_3);
            // Just use the result to prevent optimization
            volatile int dummy = result;
            (void)dummy;
        }
        
        // 9. Get patch name
        char buffer[256];
        if (size > 10) {
            // Use data to determine patch index
            int patchIndex = data[2] % 10;
            const char *patchName = cmsIT8GetPatchName(hIT8, patchIndex, buffer);
            // Just use the value to prevent optimization
            volatile const char *dummy = patchName;
            (void)dummy;
        }
        
        cmsIT8Free(hIT8);
    }
    
    // Clean up
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
    
    // Close the I/O handler (profile already closed it, but be safe)
    if (io_handler) {
        // Note: The I/O handler should be freed by cmsCloseProfile if profile was opened
        // If not, we need to free it manually, but lcms doesn't provide a free function
        // The I/O handler is owned by the profile, so we don't free it here
    }
    
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}