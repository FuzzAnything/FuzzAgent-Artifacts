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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 103.5, nr_unique_branch: 18
//<Quality> {"density":23,"unique_branches":{"_cmsReadMediaWhitePoint":[[73,9,73,20,0,0,4,1],[79,9,79,54,0,0,4,0],[79,9,79,54,0,0,4,1],[81,13,81,62,0,0,4,1]],"cmsCreateExtendedTransform":[[1226,9,1226,30,0,0,4,0],[1226,34,1226,65,0,0,4,1]],"ComputeConversion":[[368,9,368,47,0,0,4,0],[373,13,373,70,0,0,4,1],[374,13,374,74,0,0,4,1],[376,13,376,67,0,0,4,1],[377,13,377,71,0,0,4,1],[379,13,381,84,0,0,4,1]],"_cmsLinkProfiles":[[1126,13,1126,58,0,0,4,0]],"OptimizeByResampling":[[802,9,802,47,0,0,4,0],[805,9,805,51,0,0,4,1]],"_cmsReadCHAD":[[102,9,102,20,0,0,4,0]],"ComputeAbsoluteIntent":[[263,9,263,31,0,0,4,0]],"cmsCreateProofingTransformTHR":[[1388,9,1388,65,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsAllocProfileSequenceDescription","cmsOpenProfileFromMemTHR","cmsCreateXYZProfile","cmsSetHeaderModel","cmsSetHeaderRenderingIntent","cmsGetDeviceClass","cmsIT8LoadFromMem","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataRowColDbl","cmsCreate_sRGBProfileTHR","cmsCreateLab2Profile","cmsCreateProofingTransformTHR","cmsStageAllocCLut16bit","cmsStageFree","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsFreeProfileSequenceDescription","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsAllocProfileSequenceDescription","cmsOpenProfileFromMemTHR","cmsCreateXYZProfile","cmsSetHeaderModel","cmsSetHeaderRenderingIntent","cmsGetDeviceClass","cmsIT8LoadFromMem","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataRowColDbl","cmsCreate_sRGBProfileTHR","cmsCreateLab2Profile","cmsCreateProofingTransformTHR","cmsStageAllocCLut16bit","cmsStageFree","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsFreeProfileSequenceDescription","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<55) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing
    
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Create profile sequence description
    cmsSEQ *seq = cmsAllocProfileSequenceDescription(ctx, fuzz_uint32_t_1);
    
    // 2. Create profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, data, size);
    if (!hProfile) {
        // If profile creation fails, create a simple XYZ profile
        hProfile = cmsCreateXYZProfile();
    }
    
    // 3. Set header model and rendering intent
    cmsSetHeaderModel(hProfile, fuzz_uint32_t_2); // Arbitrary model value
    cmsSetHeaderRenderingIntent(hProfile, fuzz_uint32_t_3);
    
    // 4. Get device class
    cmsProfileClassSignature deviceClass = cmsGetDeviceClass(hProfile);
    (void)deviceClass; // Use variable to avoid unused warning
    
    // 5. Create IT8 handle from input data
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(ctx, data, size);
    if (!hIT8) {
        hIT8 = cmsIT8Alloc(ctx);
    }
    
    // 6. Set IT8 properties
    cmsIT8SetPropertyStr(hIT8, fuzz_str_4, fuzz_str_5);
    
    // 7. Set IT8 data row/col
    cmsIT8SetDataRowColDbl(hIT8, fuzz_int32_t_6, fuzz_int32_t_7, 1.0);
    
    // 8. Create proofing transform
    cmsHPROFILE hProof = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE hOutput = cmsCreateLab2Profile(nullptr);
    
    cmsHTRANSFORM hTransform = cmsCreateProofingTransformTHR(
        ctx,
        hProfile,                    // Input profile
        TYPE_RGB_8,                  // Input format
        hOutput,                     // Output profile
        TYPE_Lab_8,                  // Output format
        hProof,                      // Proofing profile
        fuzz_uint32_t_8,           // Intent
        fuzz_uint32_t_9,// Proofing intent
        fuzz_uint32_t_10
    );
    
    // 9. Create CLUT16 stage
    cmsUInt16Number table[8] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000};
    cmsStage *clutStage = cmsStageAllocCLut16bit(
        ctx,
        2,          // nGridPoints
        1,          // inputChan
        1,          // outputChan
        table       // Table
    );
    
    // Cleanup
    if (clutStage) cmsStageFree(clutStage);
    if (hTransform) cmsDeleteTransform(hTransform);
    if (hIT8) cmsIT8Free(hIT8);
    if (hProfile) cmsCloseProfile(hProfile);
    if (hProof) cmsCloseProfile(hProof);
    if (hOutput) cmsCloseProfile(hOutput);
    if (seq) cmsFreeProfileSequenceDescription(seq);
    cmsDeleteContext(ctx);
    
    return 0;
}