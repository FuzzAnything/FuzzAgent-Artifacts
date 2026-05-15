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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsIT8LoadFromMem","cmsIT8Alloc","cmsDeleteContext","cmsIT8SetIndexColumn","cmsCMCdeltaE","cmsFloat2LabEncodedV2","cmsCreate_sRGBProfile","cmsCreateXYZProfileTHR","cmsCreateTransform","cmsGetTransformOutputFormat","cmsIT8SetDataDbl","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8Alloc","cmsIT8SetIndexColumn","cmsCMCdeltaE","cmsFloat2LabEncodedV2","cmsCreate_sRGBProfile","cmsCreateXYZProfileTHR","cmsCreateTransform","cmsGetTransformOutputFormat","cmsIT8SetDataDbl","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Open a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create IT8 handle from input data
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    void *it8_data = malloc(file_size);
    if (!it8_data) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(it8_data, 1, file_size, in_file);
    assert_file_closed(&in_file);;
    
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(ctx, it8_data, file_size);
    free(it8_data);
    
    if (!hIT8) {
        // If loading fails, create a new IT8 handle
        hIT8 = cmsIT8Alloc(ctx);
        if (!hIT8) {
            cmsDeleteContext(ctx);
            assert_file_closed(&in_file);
	return 0;
        }
    }

    // Use cmsIT8SetIndexColumn
    cmsIT8SetIndexColumn(hIT8, "Sample1");

    // Create Lab colors for delta E calculation
    cmsCIELab lab1, lab2;
    lab1.L = 50.0;
    lab1.a = 20.0;
    lab1.b = 30.0;
    lab2.L = 55.0;
    lab2.a = 25.0;
    lab2.b = 35.0;

    // Use cmsCMCdeltaE
    cmsFloat64Number deltaE = cmsCMCdeltaE(&lab1, &lab2, 2.0, 1.0);

    // Use cmsFloat2LabEncodedV2
    cmsUInt16Number encodedLab[3];
    cmsFloat2LabEncodedV2(encodedLab, &lab1);

    // Create a transform to get output format
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    cmsHPROFILE hXYZ = cmsCreateXYZProfileTHR(ctx);
    cmsHTRANSFORM hTransform = NULL;
    
    if (hSRGB && hXYZ) {
        hTransform = cmsCreateTransform(hSRGB, TYPE_RGB_8, hXYZ, TYPE_XYZ_DBL, 
                                         INTENT_PERCEPTUAL, 0);
    }

    // Use cmsGetTransformOutputFormat
    cmsUInt32Number outputFormat = 0;
    if (hTransform) {
        outputFormat = cmsGetTransformOutputFormat(hTransform);
    }

    // Use cmsIT8SetDataDbl
    cmsIT8SetDataDbl(hIT8, "Patch1", "Sample1", deltaE);

    // Use cmsIT8SaveToMem
    cmsUInt32Number bytesNeeded = 0;
    cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded);
    
    if (bytesNeeded > 0) {
        void *outputBuffer = malloc(bytesNeeded);
        if (outputBuffer) {
            cmsIT8SaveToMem(hIT8, outputBuffer, &bytesNeeded);
            
            // Write output to a file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(outputBuffer, 1, bytesNeeded, out_file);
                assert_file_closed(&out_file);;
            }
            free(outputBuffer);
        }
    }

    // Clean up
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    if (hSRGB) {
        cmsCloseProfile(hSRGB);
    }
    if (hXYZ) {
        cmsCloseProfile(hXYZ);
    }
    
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}