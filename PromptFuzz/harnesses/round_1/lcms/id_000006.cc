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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 3
//<Quality> {"density":13,"unique_branches":{"cmsGDBAddPoint":[[370,9,370,31,0,0,4,1],[379,13,379,30,0,0,4,0],[379,13,379,30,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsPlugin","cmsGBDAlloc","cmsGDBAddPoint","cmsGDBAddPoint","cmsBFDdeltaE","cmsDesaturateLab","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsDictAddEntry","cmsDictGetEntryList","cmsMLUfree","cmsMLUfree","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsDictFree","cmsGBDFree","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsPlugin","cmsGBDAlloc","cmsGDBAddPoint","cmsGDBAddPoint","cmsBFDdeltaE","cmsDesaturateLab","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsDictAddEntry","cmsDictGetEntryList","cmsMLUfree","cmsMLUfree","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsDictFree","cmsGBDFree","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations
    
    cmsContext context = nullptr;
    cmsHANDLE gbd = nullptr;
    cmsHANDLE dict = nullptr;
    cmsStage* stage = nullptr;
    
    // 1. Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Try to call cmsPlugin (with null plugin)
    cmsPlugin(nullptr);
    
    // 3. Create GDB handle and add Lab points from input data
    // Convert first 24 bytes to cmsCIELab structure
    if (size >= sizeof(cmsCIELab)) {
        cmsCIELab lab1, lab2;
        
        // Parse Lab data from input
        memcpy(&lab1, data, sizeof(cmsCIELab));
        if (size >= 2 * sizeof(cmsCIELab)) {
            memcpy(&lab2, data + sizeof(cmsCIELab), sizeof(cmsCIELab));
            
            // Create a GDB and add points
            gbd = cmsGBDAlloc(context);
            if (gbd) {
                cmsGDBAddPoint(gbd, &lab1);
                cmsGDBAddPoint(gbd, &lab2);
            }
            
            // 4. Compute BFD deltaE between two Lab colors
            cmsBFDdeltaE(&lab1, &lab2);
            
            // 5. Desaturate Lab color
            cmsDesaturateLab(&lab1, 100.0, -100.0, 100.0, -100.0);
        }
    }
    
    // 6. Create dictionary and get entry list
    dict = cmsDictAlloc(context);
    if (dict) {
        // Add an entry to dictionary
        const wchar_t name[] = L"TestName";
        const wchar_t value[] = L"TestValue";
        cmsMLU* displayName = cmsMLUalloc(context, 1);
        cmsMLU* displayValue = cmsMLUalloc(context, 1);
        
        if (displayName && displayValue) {
            cmsMLUsetWide(displayName, "en", "US", name);
            cmsMLUsetWide(displayValue, "en", "US", value);
            
            cmsDictAddEntry(dict, name, value, displayName, displayValue);
            
            // Get entry list
            const cmsDICTentry* entry = cmsDictGetEntryList(dict);
            while (entry) {
                entry = entry->Next;
            }
        }
        
        if (displayName) cmsMLUfree(displayName);
        if (displayValue) cmsMLUfree(displayValue);
    }
    
    // 7. Create CLUT stage using input data for table
    // Create a simple 2x2x2 CLUT with 3 input channels and 3 output channels
    const cmsUInt32Number clutPoints[] = {2, 2, 2};
    const cmsUInt32Number inputChan = 3;
    const cmsUInt32Number outputChan = 3;
    const cmsUInt32Number tableSize = 2 * 2 * 2 * 3 * 3; // 72 float values
    
    if (size >= tableSize * sizeof(cmsFloat32Number)) {
        const cmsFloat32Number* table = reinterpret_cast<const cmsFloat32Number*>(data);
        
        stage = cmsStageAllocCLutFloatGranular(context, clutPoints, inputChan, 
                                              outputChan, table);
    }
    
    // Cleanup
    if (stage) cmsStageFree(stage);
    if (dict) cmsDictFree(dict);
    if (gbd) cmsGBDFree(gbd);
    if (context) cmsDeleteContext(context);
    
    return 0;
}