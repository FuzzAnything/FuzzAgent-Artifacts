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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsDeleteContext","cmsIT8SetIndexColumn","cmsCreate_sRGBProfile","cmsCreate_OkLabProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsCreateTransform","cmsGetTransformOutputFormat","cmsCMCdeltaE","cmsIT8SetDataDbl","cmsFloat2LabEncodedV2","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetIndexColumn","cmsCreate_sRGBProfile","cmsCreate_OkLabProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsCMCdeltaE","cmsIT8SetDataDbl","cmsFloat2LabEncodedV2","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 32) {
	return 0;
	}
	 // Need minimum data for operations

    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // 1. Create IT8 handle and set up IT8 table
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (!hIT8) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Set index column using first few bytes as column name
    char columnName[16] = {0};
    size_t colNameLen = size > 16 ? 16 : size;
    memcpy(columnName, data, colNameLen);
    columnName[colNameLen-1] = '\0';
    cmsIT8SetIndexColumn(hIT8, columnName);

    // 2. Create a transform to get output format
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    cmsHPROFILE hLab = cmsCreate_OkLabProfile(ctx);
    
    if (!hSRGB || !hLab) {
        if (hSRGB) cmsCloseProfile(hSRGB);
        if (hLab) cmsCloseProfile(hLab);
        cmsIT8Free(hIT8);
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsHTRANSFORM hTransform = cmsCreateTransform(hSRGB, TYPE_RGB_8, hLab, TYPE_Lab_8, 
                                                   INTENT_PERCEPTUAL, 0);
    
    if (hTransform) {
        // Get transform output format
        cmsUInt32Number outputFormat = cmsGetTransformOutputFormat(hTransform);
        
        // 3. Create Lab colors for delta E calculation
        cmsCIELab lab1, lab2;
        
        // Use data to initialize Lab values
        size_t offset = 0;
        if (size >= offset + 24) {
            // Use first 24 bytes for Lab values (8 bytes each for L,a,b)
            lab1.L = *reinterpret_cast<const double*>(data + offset); offset += 8;
            lab1.a = *reinterpret_cast<const double*>(data + offset); offset += 8;
            lab1.b = *reinterpret_cast<const double*>(data + offset); offset += 8;
            
            if (size >= offset + 24) {
                lab2.L = *reinterpret_cast<const double*>(data + offset); offset += 8;
                lab2.a = *reinterpret_cast<const double*>(data + offset); offset += 8;
                lab2.b = *reinterpret_cast<const double*>(data + offset); offset += 8;
            } else {
                // Use same values for lab2 if not enough data
                lab2 = lab1;
            }
        } else {
            // Use default values if not enough data
            lab1.L = lab2.L = 50.0;
            lab1.a = lab2.a = 0.0;
            lab1.b = lab2.b = 0.0;
        }
        
        // Calculate CMC delta E
        cmsFloat64Number lParam = 1.0;
        cmsFloat64Number cParam = 1.0;
        if (size > offset + 16) {
            lParam = *reinterpret_cast<const double*>(data + offset); offset += 8;
            cParam = *reinterpret_cast<const double*>(data + offset); offset += 8;
        }
        
        cmsFloat64Number deltaE = cmsCMCdeltaE(&lab1, &lab2, lParam, cParam);
        
        // 4. Set IT8 data with delta E value
        cmsIT8SetDataDbl(hIT8, "Patch1", columnName, deltaE);
        
        // 5. Convert Lab to encoded format
        cmsUInt16Number encodedLab[3];
        cmsFloat2LabEncodedV2(encodedLab, &lab1);
        
        // 6. Save IT8 table to memory
        cmsUInt32Number bytesNeeded = 0;
        cmsIT8SaveToMem(hIT8, nullptr, &bytesNeeded);
        
        if (bytesNeeded > 0 && bytesNeeded < 1024*1024) { // Reasonable size limit
            void* buffer = malloc(bytesNeeded);
            if (buffer) {
                cmsIT8SaveToMem(hIT8, buffer, &bytesNeeded);
                free(buffer);
            }
        }
        
        cmsDeleteTransform(hTransform);
    }
    
    // Cleanup
    cmsCloseProfile(hSRGB);
    cmsCloseProfile(hLab);
    cmsIT8Free(hIT8);
    cmsDeleteContext(ctx);
    
    return 0;
}