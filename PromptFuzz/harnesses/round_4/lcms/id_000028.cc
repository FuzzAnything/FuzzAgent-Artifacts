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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 102, nr_unique_branch: 15
//<Quality> {"density":17,"unique_branches":{"cmsCreateBCHSWabstractProfileTHR":[[882,9,882,28,0,0,4,1],[896,9,896,14,0,0,4,1],[907,9,907,25,0,0,4,1],[912,15,912,39,0,0,4,0],[912,15,912,39,0,0,4,1],[914,9,914,21,0,0,4,1],[917,9,917,72,0,0,4,1],[920,9,920,59,0,0,4,1],[924,9,924,45,0,0,4,1],[927,9,927,74,0,0,4,1],[930,9,930,60,0,0,4,1]],"cmsWhitePointFromTemp":[[62,23,62,33,0,0,4,0]],"cmsLab2XYZ":[[165,9,165,27,0,0,4,1]],"cmsXYZ2Lab":[[147,9,147,27,0,0,4,1]],"bchswSampler":[[845,9,845,25,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataRowColDbl","cmsCreateBCHSWabstractProfile","cmsCreateBCHSWabstractProfile","cmsCreateBCHSWabstractProfile","cmsSetHeaderModel","cmsSetHeaderRenderingIntent","cmsGetDeviceClass","cmsCreateProofingTransformTHR","cmsStageAllocCLut16bit","cmsStageFree","cmsAllocProfileSequenceDescription","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataRowColDbl","cmsCreateBCHSWabstractProfile","cmsCreateBCHSWabstractProfile","cmsCreateBCHSWabstractProfile","cmsSetHeaderModel","cmsSetHeaderRenderingIntent","cmsGetDeviceClass","cmsCreateProofingTransformTHR","cmsStageAllocCLut16bit","cmsStageFree","cmsAllocProfileSequenceDescription","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" {
}

// Define TYPE_RGB_8 if not already defined
#ifndef TYPE_RGB_8
#define TYPE_RGB_8 0x02020200
#endif

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<67) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        // Set IT8 property string using part of input data
        char propName[100];
        size_t propLen = size > 100 ? 100 : size;
        memcpy(propName, data, propLen);
        propName[propLen - 1] = '\0';
        
        cmsIT8SetPropertyStr(hIT8, fuzz_str_1, propName);
        
        // Set IT8 data row/col with double value
        double val = (double)(data[0] % 256) / 255.0;
        cmsIT8SetDataRowColDbl(hIT8, fuzz_int32_t_2, fuzz_int32_t_3, val);
    }
    
    // Create profiles for proofing transform
    cmsHPROFILE hInput = cmsCreateBCHSWabstractProfile(fuzz_uint32_t_4, 0.5, 0.5, 0.5, 0.5, fuzz_uint32_t_5, fuzz_uint32_t_6);
    cmsHPROFILE hOutput = cmsCreateBCHSWabstractProfile(fuzz_uint32_t_7, 0.5, 0.5, 0.5, 0.5, fuzz_uint32_t_8, fuzz_uint32_t_9);
    cmsHPROFILE hProofing = cmsCreateBCHSWabstractProfile(fuzz_uint32_t_10, 0.5, 0.5, 0.5, 0.5, fuzz_uint32_t_11, fuzz_uint32_t_12);
    
    if (hInput && hOutput && hProofing) {
        // Set header model and rendering intent
        cmsUInt32Number model = *reinterpret_cast<const cmsUInt32Number*>(data);
        cmsUInt32Number intent = *reinterpret_cast<const cmsUInt32Number*>(data + 4);
        
        cmsSetHeaderModel(hInput, model);
        cmsSetHeaderRenderingIntent(hInput, intent);
        
        // Get device class (check if it's valid)
        cmsProfileClassSignature deviceClass = cmsGetDeviceClass(hInput);
        
        // Create proofing transform
        cmsHTRANSFORM hTransform = cmsCreateProofingTransformTHR(
            ctx, hInput, TYPE_RGB_8, hOutput, TYPE_RGB_8, 
            hProofing, intent, intent, 0);
        
        if (hTransform) {
            // Create CLUT table from input data
            const int gridPoints = 2;
            const int inputChan = 3;
            const int outputChan = 3;
            const int tableSize = gridPoints * gridPoints * gridPoints * outputChan;
            
            if (size >= tableSize * sizeof(cmsUInt16Number)) {
                cmsUInt16Number *table = new cmsUInt16Number[tableSize];
                for (size_t i = 0; i < tableSize && i < size / sizeof(cmsUInt16Number); i++) {
                    table[i] = reinterpret_cast<const cmsUInt16Number*>(data)[i % (size / sizeof(cmsUInt16Number))];
                }
                
                // Create CLUT stage
                cmsStage *clutStage = cmsStageAllocCLut16bit(
                    ctx, gridPoints, fuzz_uint32_t_13, outputChan, table);
                
                if (clutStage) {
                    cmsStageFree(clutStage);
                }
                
                delete[] table;
            }
            
            // Allocate profile sequence description
            cmsUInt32Number nSequences = 1;
            cmsSEQ *seq = cmsAllocProfileSequenceDescription(ctx, fuzz_uint32_t_14);
            
            // Clean up transform
            cmsDeleteTransform(hTransform);
        }
    }
    
    // Clean up profiles
    if (hInput) cmsCloseProfile(hInput);
    if (hOutput) cmsCloseProfile(hOutput);
    if (hProofing) cmsCloseProfile(hProofing);
    
    // Clean up IT8
    if (hIT8) cmsIT8Free(hIT8);
    
    // Close file and context
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}