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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 20
//<Quality> {"density":7,"unique_branches":{"cmsDictFree":[[1113,17,1113,45,0,0,4,1],[1114,17,1114,45,0,0,4,1]],"cmsPluginTHR":[[551,10,551,24,0,0,4,0],[554,17,554,56,0,0,4,0],[554,17,554,56,0,0,4,1],[559,17,559,56,0,0,4,0],[559,17,559,56,0,0,4,1],[567,17,567,44,0,0,4,1],[571,17,571,47,0,0,4,1],[575,17,575,41,0,0,4,1],[579,17,579,37,0,0,4,1],[583,17,583,44,0,0,4,1],[587,17,587,49,0,0,4,1],[591,17,591,49,0,0,4,1],[595,17,595,53,0,0,4,1],[599,17,599,46,0,0,4,1],[603,17,603,43,0,0,4,1],[607,17,607,39,0,0,4,1],[611,17,611,49,0,0,4,1],[615,17,615,24,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDictAlloc","cmsDictAddEntry","cmsDictGetEntryList","cmsDesaturateLab","cmsBFDdeltaE","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsPlugin","cmsDictFree","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDictAlloc","cmsDictAddEntry","cmsDictGetEntryList","cmsDesaturateLab","cmsBFDdeltaE","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsPlugin","cmsDictFree","cmsDeleteContext"],"visited":1}
/**/


// Include lcms2 header (adjust if using different version)

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful processing
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Create dictionary and populate with entries from input data
    cmsHANDLE dict = cmsDictAlloc(ctx);
    if (dict) {
        // Use portions of input data as dictionary entries
        size_t offset = 0;
        int entryCount = 0;
        
        while (offset + 20 < size && entryCount < 5) {
            // Extract name and value from input data
            size_t name_len = std::min(size - offset, (size_t)10);
            wchar_t name[11];
            for (size_t i = 0; i < name_len; i++) {
                name[i] = static_cast<wchar_t>(data[offset + i]);
            }
            name[name_len] = 0;
            offset += name_len;
            
            if (offset + 10 > size) break;
            
            wchar_t value[11];
            size_t value_len = std::min(size - offset, (size_t)10);
            for (size_t i = 0; i < value_len; i++) {
                value[i] = static_cast<wchar_t>(data[offset + i]);
            }
            value[value_len] = 0;
            offset += value_len;
            
            // Add entry to dictionary (without display names for simplicity)
            cmsDictAddEntry(dict, name, value, NULL, NULL);
            entryCount++;
        }
        
        // Get dictionary entry list
        const cmsDICTentry *entry = cmsDictGetEntryList(dict);
        // Traverse entries (just to use the API)
        while (entry) {
            entry = entry->Next;
        }
    }
    
    // 2. Create Lab colors from input data for various operations
    std::vector<cmsCIELab> labPoints;
    size_t offset = size > 200 ? 50 : 0; // Start from different offset
    
    while (offset + 24 <= size && labPoints.size() < 10) {
        cmsCIELab lab;
        // Extract Lab values from input data (interpreting bytes as doubles)
        // This is just for fuzzing - not proper conversion
        if (offset + 24 <= size) {
            memcpy(&lab.L, data + offset, 8);
            memcpy(&lab.a, data + offset + 8, 8);
            memcpy(&lab.b, data + offset + 16, 8);
            offset += 24;
            
            // Bounds check values to avoid extreme values
            lab.L = std::min(std::max(lab.L, 0.0), 100.0);
            lab.a = std::min(std::max(lab.a, -128.0), 127.0);
            lab.b = std::min(std::max(lab.b, -128.0), 127.0);
            
            labPoints.push_back(lab);
        }
    }
    
    // 3. Create gamut boundary descriptor and add points
    // Note: cmsGDBAddPoint requires a GBD handle which we don't have API to create
    // So we'll skip this API as we can't properly create the GBD handle
    
    // 4. Perform desaturation on Lab colors
    if (labPoints.size() >= 1) {
        for (size_t i = 0; i < labPoints.size() && i < 3; i++) {
            cmsCIELab lab = labPoints[i];
            cmsDesaturateLab(&lab, 50.0, -50.0, 50.0, -50.0);
        }
    }
    
    // 5. Calculate BFD deltaE between Lab colors
    if (labPoints.size() >= 2) {
        for (size_t i = 0; i < labPoints.size() - 1; i++) {
            cmsFloat64Number delta = cmsBFDdeltaE(&labPoints[i], &labPoints[i + 1]);
            // delta value can be used or ignored for fuzzing
        }
    }
    
    // 6. Create CLUT stage with granular control
    if (size >= 100) {
        // Create simple CLUT parameters
        const int inputChan = 3;
        const int outputChan = 3;
        cmsUInt32Number clutPoints[] = {5, 5, 5}; // 5x5x5 grid
        
        // Calculate table size
        size_t tableSize = 5 * 5 * 5 * inputChan * outputChan;
        
        // Create table from input data (truncate if needed)
        std::vector<cmsFloat32Number> table(tableSize);
        size_t dataOffset = std::min(size - 1, (size_t)100);
        
        for (size_t i = 0; i < tableSize && dataOffset < size; i++, dataOffset++) {
            table[i] = static_cast<cmsFloat32Number>(data[dataOffset]) / 255.0f;
        }
        
        // Create CLUT stage
        cmsStage *clutStage = cmsStageAllocCLutFloatGranular(
            ctx, 
            clutPoints, 
            inputChan, 
            outputChan, 
            table.data()
        );
        
        if (clutStage) {
            cmsStageFree(clutStage);
        }
    }
    
    // 7. Register a plugin (simplified - just pass NULL)
    // Note: In real usage, this would be a proper plugin structure
    // For fuzzing, we'll pass a pointer to some input data
    if (size > 50) {
        // Create a minimal plugin-like structure from input data
        // This is just to exercise the API, not for actual plugin functionality
        char *pluginData = (char*)malloc(32);
        if (pluginData) {
            memcpy(pluginData, data, std::min(size, (size_t)32));
            cmsPlugin(pluginData);
            free(pluginData);
        }
    }
    
    // Clean up
    if (dict) {
        cmsDictFree(dict);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}