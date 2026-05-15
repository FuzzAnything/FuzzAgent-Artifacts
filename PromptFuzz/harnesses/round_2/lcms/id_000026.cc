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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["cmsCreateContext","cmsfilelength","cmsIT8LoadFromMem","cmsIT8TableCount","cmsIT8SetDataRowCol","cmsStageAllocCLutFloat","cmsDeltaE","cmsStageFree","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsfilelength","cmsIT8LoadFromMem","cmsIT8TableCount","cmsIT8SetDataRowCol","cmsStageAllocCLutFloat","cmsDeltaE","cmsStageFree","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful processing
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
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
    
    // 1. Create an IT8 handle from input data
    cmsHANDLE hIT8 = nullptr;
    {
        fseek(in_file, 0, SEEK_SET);
        long file_len = cmsfilelength(in_file);
        if (file_len > 0) {
            void* buffer = malloc(file_len);
            if (buffer) {
                fread(buffer, 1, file_len, in_file);
                hIT8 = cmsIT8LoadFromMem(context, buffer, file_len);
                free(buffer);
            }
        }
    }
    
    // 2. Process IT8 data if available
    if (hIT8) {
        // Get table count
        cmsUInt32Number tableCount = cmsIT8TableCount(hIT8);
        
        // Set some data in the IT8 table
        // Use a small value from input data as string for setting
        char val_str[32] = {0};
        size_t copy_len = size > 31 ? 31 : size;
        memcpy(val_str, data, copy_len);
        val_str[copy_len] = '\0';
        
        // Set data in first row/col if table exists
        if (tableCount > 0) {
            cmsIT8SetDataRowCol(hIT8, 0, 0, val_str);
        }
    }
    
    // 3. Create a CLUT stage using input data
    cmsStage* clut_stage = nullptr;
    {
        // Create a simple 3x3x3 CLUT with 3 input and 3 output channels
        const cmsUInt32Number gridPoints = 3;
        const cmsUInt32Number inputChan = 3;
        const cmsUInt32Number outputChan = 3;
        const size_t table_size = gridPoints * gridPoints * gridPoints * outputChan;
        
        cmsFloat32Number* table = (cmsFloat32Number*)malloc(table_size * sizeof(cmsFloat32Number));
        if (table) {
            // Fill table with normalized data from input
            for (size_t i = 0; i < table_size && i < size / sizeof(float); i++) {
                // Convert byte data to float in range [0, 1]
                table[i] = (cmsFloat32Number)data[i % size] / 255.0f;
            }
            // Pad if needed
            for (size_t i = size / sizeof(float); i < table_size; i++) {
                table[i] = 0.0f;
            }
            
            clut_stage = cmsStageAllocCLutFloat(context, gridPoints, inputChan, outputChan, table);
            free(table);
        }
    }
    
    // 4. Create Lab values for deltaE calculation
    cmsCIELab lab1 = {0.0, 0.0, 0.0};
    cmsCIELab lab2 = {0.0, 0.0, 0.0};
    
    // Use input data to set Lab values if enough bytes available
    if (size >= 6 * sizeof(double)) {
        // Use first 6 doubles from input for Lab values
        const double* dbl_data = reinterpret_cast<const double*>(data);
        lab1.L = dbl_data[0];
        lab1.a = dbl_data[1];
        lab1.b = dbl_data[2];
        lab2.L = dbl_data[3];
        lab2.a = dbl_data[4];
        lab2.b = dbl_data[5];
    } else {
        // Default values
        lab1.L = 50.0; lab1.a = 0.0; lab1.b = 0.0;
        lab2.L = 50.0; lab2.a = 10.0; lab2.b = 10.0;
    }
    
    // Calculate deltaE
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
    
    // 5. Create and use Gamut Boundary Description if possible
    // Note: GDB functions require a handle, which we don't have from input
    // We'll simulate the call but won't actually check against real GDB
    // For fuzzing purposes, we can pass nullptr
    
    // Cleanup
    if (clut_stage) {
        cmsStageFree(clut_stage);
    }
    
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}