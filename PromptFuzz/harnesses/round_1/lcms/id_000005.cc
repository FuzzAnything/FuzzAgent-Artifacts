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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDictAlloc","cmsDeleteContext","cmsGBDAlloc","cmsDictFree","cmsDeleteContext","cmsPlugin","cmsGDBAddPoint","cmsBFDdeltaE","cmsDesaturateLab","cmsDictGetEntryList","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsGBDFree","cmsDictFree","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDictAlloc","cmsGBDAlloc","cmsPlugin","cmsGDBAddPoint","cmsBFDdeltaE","cmsDesaturateLab","cmsDictGetEntryList","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsGBDFree","cmsDictFree","cmsDeleteContext"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create context for lcms operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Create a dictionary to work with
    cmsHANDLE hDict = cmsDictAlloc(ctx);
    if (!hDict) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create a gamut boundary descriptor handle
    cmsHANDLE hGBD = cmsGBDAlloc(ctx);
    if (!hGBD) {
        cmsDictFree(hDict);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Try to load a plugin from input data if possible
    // Since we don't have valid plugin data, we'll use a minimal approach
    cmsBool pluginResult = cmsPlugin(NULL); // Note: NULL plugin for fuzzing
    
    // Parse Lab values from input data
    // We need at least 48 bytes: 2x cmsCIELab (3 doubles each = 24 bytes each)
    const size_t minLabSize = sizeof(cmsCIELab) * 2;
    if (size >= minLabSize) {
        const cmsCIELab* lab1 = reinterpret_cast<const cmsCIELab*>(data);
        const cmsCIELab* lab2 = reinterpret_cast<const cmsCIELab*>(data + sizeof(cmsCIELab));
        
        // Add Lab point to GBD
        cmsGDBAddPoint(hGBD, lab1);
        
        // Calculate BFD delta E between two Lab values
        cmsFloat64Number deltaE = cmsBFDdeltaE(lab1, lab2);
        (void)deltaE; // Prevent unused variable warning
        
        // Desaturate the first Lab value
        cmsCIELab labDesat = *lab1;
        cmsDesaturateLab(&labDesat, 100.0, 0.0, 100.0, -100.0);
    }
    
    // Get dictionary entry list
    const cmsDICTentry* entryList = cmsDictGetEntryList(hDict);
    (void)entryList; // Prevent unused variable warning
    
    // Create a CLUT with granular control
    // We need at least 12 bytes for clutPoints (3x uint32) plus some table data
    if (size >= 12 + 4) {
        // Read clutPoints from input (3 values for 3D CLUT)
        cmsUInt32Number clutPoints[3] = {0};
        size_t offset = 0;
        for (int i = 0; i < 3 && offset + 4 <= size; i++) {
            clutPoints[i] = *reinterpret_cast<const cmsUInt32Number*>(data + offset);
            offset += 4;
        }
        
        // Ensure reasonable grid sizes for fuzzing
        for (int i = 0; i < 3; i++) {
            if (clutPoints[i] < 2) clutPoints[i] = 2;
            if (clutPoints[i] > 10) clutPoints[i] = 10; // Limit for performance
        }
        
        // Calculate table size (3 input channels, 3 output channels)
        cmsUInt32Number tableSize = clutPoints[0] * clutPoints[1] * clutPoints[2] * 3;
        
        // Allocate and fill table with data from input
        cmsFloat32Number* table = reinterpret_cast<cmsFloat32Number*>(
            malloc(tableSize * sizeof(cmsFloat32Number)));
        
        if (table) {
            // Fill table with values from input data
            size_t dataRemaining = size - offset;
            size_t floatsToRead = tableSize;
            if (dataRemaining / sizeof(cmsFloat32Number) < floatsToRead) {
                floatsToRead = dataRemaining / sizeof(cmsFloat32Number);
            }
            
            for (size_t i = 0; i < floatsToRead; i++) {
                if (offset + 4 <= size) {
                    table[i] = *reinterpret_cast<const cmsFloat32Number*>(data + offset);
                    offset += 4;
                } else {
                    table[i] = 0.0f;
                }
            }
            
            // Fill remaining with zeros
            for (size_t i = floatsToRead; i < tableSize; i++) {
                table[i] = 0.0f;
            }
            
            // Create CLUT stage
            cmsStage* clutStage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, 3, 3, table);
            if (clutStage) {
                cmsStageFree(clutStage);
            }
            
            free(table);
        }
    }
    
    // Clean up resources
    if (hGBD) {
        cmsGBDFree(hGBD);
    }
    
    if (hDict) {
        cmsDictFree(hDict);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}