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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"SetData":[[1788,9,1788,30,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsAllocProfileSequenceDescription","cmsOpenProfileFromMemTHR","cmsFreeProfileSequenceDescription","cmsDeleteContext","cmsGetDeviceClass","cmsSetHeaderModel","cmsSetHeaderRenderingIntent","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataRowColDbl","cmsStageAllocCLut16bit","cmsStageFree","cmsCreate_sRGBProfileTHR","cmsCreateProofingTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsIT8Free","cmsFreeProfileSequenceDescription","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsAllocProfileSequenceDescription","cmsOpenProfileFromMemTHR","cmsGetDeviceClass","cmsSetHeaderModel","cmsSetHeaderRenderingIntent","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataRowColDbl","cmsStageAllocCLut16bit","cmsStageFree","cmsCreate_sRGBProfileTHR","cmsCreateProofingTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsIT8Free","cmsFreeProfileSequenceDescription","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for operations
    
    // Create context for operations
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    
    // 1. Allocate profile sequence description
    cmsSEQ *seq = cmsAllocProfileSequenceDescription(ctx, fuzz_uint32_t_1);
    
    // 2. Open profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, data, (cmsUInt32Number)size);
    if (!hProfile) {
        cmsFreeProfileSequenceDescription(seq);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 3. Get device class of profile
    cmsProfileClassSignature deviceClass = cmsGetDeviceClass(hProfile);
    
    // 4. Set header model and rendering intent
    cmsSetHeaderModel(hProfile, fuzz_uint32_t_2);  // Using fuzzed data
    cmsSetHeaderRenderingIntent(hProfile, data[0] % 4);  // Valid intents: 0-3
    
    // 5. Create IT8 handle and set properties
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        // Set string property
        char propName[32];
        char propValue[32];
        snprintf(propName, sizeof(propName), "Property_%d", data[1]);
        snprintf(propValue, sizeof(propValue), "Value_%d", data[2]);
        cmsIT8SetPropertyStr(hIT8, propName, propValue);
        
        // Set double value at row/col
        int row = data[3] % 10;
        int col = data[4] % 10;
        double val = (double)data[5] / 255.0;
        cmsIT8SetDataRowColDbl(hIT8, row, col, val);
    }
    
    // 6. Create 16-bit CLUT table
    const int gridPoints = 3;
    const int inputChan = 3;
    const int outputChan = 3;
    const int tableSize = gridPoints * gridPoints * gridPoints * outputChan;
    cmsUInt16Number *clutTable = (cmsUInt16Number*)malloc(tableSize * sizeof(cmsUInt16Number));
    
    if (clutTable) {
        // Fill table with fuzzed data
        for (int i = 0; i < tableSize && i < (int)size - 10; i++) {
            clutTable[i] = (cmsUInt16Number)(data[i + 10] << 8 | data[i + 11]);
        }
        
        // Create CLUT stage
        cmsStage *clutStage = cmsStageAllocCLut16bit(ctx, gridPoints, inputChan, outputChan, clutTable);
        if (clutStage) {
            cmsStageFree(clutStage);
        }
        free(clutTable);
    }
    
    // 7. Create proofing transform
    // Create sRGB profile for output
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(ctx);
    if (sRGBProfile && hProfile) {
        // Use the input profile for proofing as well
        cmsHTRANSFORM transform = cmsCreateProofingTransformTHR(
            ctx,
            hProfile,                    // Input profile
            TYPE_RGB_8,                  // Input format
            sRGBProfile,                 // Output profile
            TYPE_RGB_8,                  // Output format
            hProfile,                    // Proofing profile
            fuzz_uint32_t_3,           // Intent
            fuzz_uint32_t_4,// Proofing intent
            fuzz_uint32_t_5                            // Flags
        );
        
        if (transform) {
            // Test transform with some data
            unsigned char input[12];
            unsigned char output[12];
            
            // Copy fuzzed data to input
            size_t copySize = size > sizeof(input) ? sizeof(input) : size;
            memcpy(input, data, copySize);
            
            // Apply transform
            cmsDoTransform(transform, input, output, 1);
            
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(sRGBProfile);
    }
    
    // 8. Clean up
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    if (seq) {
        cmsFreeProfileSequenceDescription(seq);
    }
    
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}