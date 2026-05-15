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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"SetData":[[1788,9,1788,30,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsDeleteContext","cmsIT8TableCount","cmsIT8SetDataRowCol","cmsDeltaE","cmsGDBCheckPoint","cmsStageAllocCLutFloat","cmsStageFree","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8TableCount","cmsIT8SetDataRowCol","cmsDeltaE","cmsGDBCheckPoint","cmsStageAllocCLutFloat","cmsStageFree","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for meaningful operations
    
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Create IT8 handle from input data
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(ctx, data, size);
    if (!hIT8) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Get number of tables
    cmsUInt32Number tableCount = cmsIT8TableCount(hIT8);
    
    // Set some data in the IT8 table if we have at least one table
    if (tableCount > 0) {
        // Use first 4 bytes as row/col values, modulo to keep within reasonable bounds
        int row = data[0] % 10;
        int col = data[1] % 10;
        
        // Create a string value from input data (safe copy)
        char val[32];
        size_t val_len = (size - 2) > 31 ? 31 : size - 2;
        if (val_len > 0) {
            memcpy(val, data + 2, val_len);
            val[val_len] = '\0';
            cmsIT8SetDataRowCol(hIT8, row, col, val);
        }
    }
    
    // Create GBD (Gamut Boundary Descriptor) for color checking
    // Note: We need to create a GBD handle - using simple approach with NULL
    // since actual GBD creation APIs aren't in the provided list
    cmsHANDLE hGBD = NULL;
    
    // Create Lab colors for DeltaE calculation and GBD check
    cmsCIELab lab1, lab2;
    
    // Initialize Lab values from input data (using modulo to keep in valid ranges)
    lab1.L = (data[0] % 100) + 0.0;  // L: 0-100
    lab1.a = ((int8_t)data[1] % 128) + 0.0;  // a: -128 to 127
    lab1.b = ((int8_t)data[2] % 128) + 0.0;  // b: -128 to 127
    
    lab2.L = (data[3] % 100) + 0.0;
    lab2.a = ((int8_t)data[4] % 128) + 0.0;
    lab2.b = ((int8_t)data[5] % 128) + 0.0;
    
    // Calculate DeltaE between two Lab colors
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
    
    // Check point against GBD (will return false since hGBD is NULL)
    if (hGBD) {
        cmsGDBCheckPoint(hGBD, &lab1);
    }
    
    // Create CLUT (Color Lookup Table) stage
    // Simple 2x2x2 grid for RGB to RGB transformation
    const cmsUInt32Number nGridPoints = 2;
    const cmsUInt32Number inputChan = 3;  // RGB
    const cmsUInt32Number outputChan = 3; // RGB
    const cmsUInt32Number tableSize = nGridPoints * nGridPoints * nGridPoints * outputChan;
    
    cmsFloat32Number* table = (cmsFloat32Number*)malloc(tableSize * sizeof(cmsFloat32Number));
    if (table) {
        // Fill table with data from input (normalized to 0-1)
        size_t data_offset = 6;
        for (cmsUInt32Number i = 0; i < tableSize && data_offset < size; i++, data_offset++) {
            table[i] = data[data_offset] / 255.0f;
        }
        
        // Create CLUT stage
        cmsStage* clutStage = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, table);
        
        if (clutStage) {
            // We could use the stage here, but just creating it is enough for fuzzing
            cmsStageFree(clutStage);
        }
        
        free(table);
    }
    
    // Cleanup
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}