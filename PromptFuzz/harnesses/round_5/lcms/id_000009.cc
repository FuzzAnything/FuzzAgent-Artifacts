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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"WriteStr":[[1831,17,1831,35,0,0,4,0]],"cmsIT8SaveToMem":[[2068,20,2068,38,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsDeleteContext","cmsCreate_sRGBProfile","cmsCreateXYZProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsCMCdeltaE","cmsFloat2LabEncodedV2","cmsIT8SetIndexColumn","cmsIT8SetDataDbl","cmsIT8SaveToMem","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsCreate_sRGBProfile","cmsCreateXYZProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsCMCdeltaE","cmsFloat2LabEncodedV2","cmsIT8SetIndexColumn","cmsIT8SetDataDbl","cmsIT8SaveToMem","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Create IT8 handle from input data
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(ctx, data, size);
    if (!hIT8) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create transform to get output format
    cmsHPROFILE srcProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE dstProfile = cmsCreateXYZProfile();
    cmsHTRANSFORM transform = cmsCreateTransform(srcProfile, TYPE_RGB_8, dstProfile, 
                                                  TYPE_XYZ_DBL, INTENT_PERCEPTUAL, 0);
    
    if (transform) {
        // Get transform output format
        cmsUInt32Number outputFormat = cmsGetTransformOutputFormat(transform);
        (void)outputFormat; // Use value to avoid unused warning
        
        // Create two Lab color values for CMC delta E calculation
        cmsCIELab lab1, lab2;
        
        // Use first 24 bytes for Lab1 (3 doubles)
        if (size >= 24) {
            memcpy(&lab1.L, data, sizeof(double));
            memcpy(&lab1.a, data + 8, sizeof(double));
            memcpy(&lab1.b, data + 16, sizeof(double));
        } else {
            lab1.L = 50.0; lab1.a = 0.0; lab1.b = 0.0;
        }
        
        // Use next 24 bytes for Lab2 (3 doubles)
        if (size >= 48) {
            memcpy(&lab2.L, data + 24, sizeof(double));
            memcpy(&lab2.a, data + 32, sizeof(double));
            memcpy(&lab2.b, data + 40, sizeof(double));
        } else {
            lab2.L = 60.0; lab2.a = 10.0; lab2.b = 20.0;
        }
        
        // Calculate CMC delta E
        cmsFloat64Number deltaE = cmsCMCdeltaE(&lab1, &lab2, 1.0, 1.0);
        
        // Encode Lab to 16-bit
        cmsUInt16Number encodedLab[3];
        cmsFloat2LabEncodedV2(encodedLab, &lab1);
        
        // Set index column in IT8
        cmsIT8SetIndexColumn(hIT8, "Sample1");
        
        // Set data as double in IT8 - use deltaE value
        cmsIT8SetDataDbl(hIT8, "Patch1", "Sample1", deltaE);
        
        // Save IT8 to memory
        void *memBuffer = malloc(size);
        if (memBuffer) {
            cmsUInt32Number bytesNeeded = 0;
            cmsIT8SaveToMem(hIT8, memBuffer, &bytesNeeded);
            free(memBuffer);
        }
        
        // Clean up transform
        cmsDeleteTransform(transform);
    }
    
    // Clean up profiles
    cmsCloseProfile(srcProfile);
    cmsCloseProfile(dstProfile);
    
    // Clean up IT8
    cmsIT8Free(hIT8);
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}