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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 6
//<Quality> {"density":13,"unique_branches":{"cmsCMCdeltaE":[[553,7,553,20,0,0,4,0],[553,24,553,37,0,0,4,0],[553,24,553,37,0,0,4,1]],"cmsOpenProfileFromIOhandlerTHR":[[1251,9,1251,23,0,0,4,1],[1256,9,1256,32,0,0,4,0],[1256,9,1256,32,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsDeleteContext","cmsIT8SetIndexColumn","cmsCMCdeltaE","cmsFloat2LabEncodedV2","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsIT8SetDataDbl","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsOpenProfileFromIOhandlerTHR","cmsOpenIOhandlerFromMem","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetIndexColumn","cmsCMCdeltaE","cmsFloat2LabEncodedV2","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsIT8SetDataDbl","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsOpenProfileFromIOhandlerTHR","cmsOpenIOhandlerFromMem","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing

    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	

    // 1. Create IT8 handle for color data management
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (!hIT8) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // 2. Set index column using data from input
    const char *columnName = "Sample1";
    cmsIT8SetIndexColumn(hIT8, columnName);

    // 3. Create two Lab colors for delta E calculation
    cmsCIELab Lab1, Lab2;
    // Use first 24 bytes for Lab1 (3 doubles)
    if (size >= 24) {
        memcpy(&Lab1.L, data, 8);
        memcpy(&Lab1.a, data + 8, 8);
        memcpy(&Lab1.b, data + 16, 8);
    } else {
        Lab1.L = 50.0; Lab1.a = 0.0; Lab1.b = 0.0;
    }
    // Use next 24 bytes for Lab2
    if (size >= 48) {
        memcpy(&Lab2.L, data + 24, 8);
        memcpy(&Lab2.a, data + 32, 8);
        memcpy(&Lab2.b, data + 40, 8);
    } else {
        Lab2.L = 55.0; Lab2.a = 5.0; Lab2.b = 5.0;
    }

    // 4. Calculate CMC delta E
    cmsFloat64Number deltaE = cmsCMCdeltaE(&Lab1, &Lab2, 2.0, 1.0);

    // 5. Convert Lab to encoded format
    cmsUInt16Number encodedLab[3];
    cmsFloat2LabEncodedV2(encodedLab, &Lab1);

    // 6. Create a simple transform to get output format
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    if (hTransform) {
        // 7. Get transform output format
        cmsUInt32Number outputFormat = cmsGetTransformOutputFormat(hTransform);
        (void)outputFormat; // Use variable to avoid unused warning
    }

    // 8. Set double data in IT8 table using delta E value
    const char *patchName = "Patch1";
    cmsIT8SetDataDbl(hIT8, patchName, columnName, deltaE);

    // 9. Save IT8 data to memory
    cmsUInt32Number bytesNeeded = 0;
    // First call to get required buffer size
    cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded);
    
    if (bytesNeeded > 0 && bytesNeeded < 100000) { // Reasonable size limit
        void *buffer = malloc(bytesNeeded);
        if (buffer) {
            cmsIT8SaveToMem(hIT8, buffer, &bytesNeeded);
            free(buffer);
        }
    }

    // 10. Consume remaining data by creating profile from it
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Try to open profile from the data
        cmsHPROFILE hInputProfile = cmsOpenProfileFromIOhandlerTHR(ctx, 
            cmsOpenIOhandlerFromMem(ctx, (void *)data, size, "rb"));
        if (hInputProfile) {
            cmsCloseProfile(hInputProfile);
        }
        assert_file_closed(&in_file);;
    }

    // Cleanup
    if (hTransform) cmsDeleteTransform(hTransform);
    if (hProfile) cmsCloseProfile(hProfile);
    if (hIT8) cmsIT8Free(hIT8);
    cmsDeleteContext(ctx);

    assert_file_closed(&in_file);
	return 0;
}