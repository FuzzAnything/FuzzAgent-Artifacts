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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 98, nr_unique_branch: 27
//<Quality> {"density":14,"unique_branches":{"WriteDataFormat":[[1948,9,1948,25,0,0,4,0]],"WriteStr":[[1811,9,1811,20,0,0,4,1],[1819,9,1819,19,0,0,4,1],[1829,13,1829,21,0,0,4,0],[1831,17,1831,35,0,0,4,0],[1831,17,1831,35,0,0,4,1]],"cmsIT8SaveToMem":[[2068,9,2068,16,0,0,4,0],[2068,20,2068,38,0,0,4,1],[2075,15,2075,36,0,0,4,0],[2075,15,2075,36,0,0,4,1],[2085,9,2085,16,0,0,4,0]],"WriteData":[[1974,12,1974,20,0,0,4,0]],"WriteHeader":[[1872,29,1872,40,0,0,4,0],[1872,29,1872,40,0,0,4,1],[1874,13,1874,32,0,0,4,1],[1894,13,1894,76,0,0,4,1],[1906,13,1906,21,0,0,4,0],[1910,13,1910,32,0,0,4,0],[1910,13,1910,32,0,0,4,1],[1914,13,1914,33,0,0,4,0],[1914,13,1914,33,0,0,4,1],[1918,13,1918,35,0,0,4,1],[1922,13,1922,30,0,0,4,1],[1926,13,1926,28,0,0,4,1],[1930,13,1930,20,0,0,4,1]],"IsAvailableOnList":[[1293,9,1293,16,0,0,4,1],[1297,13,1297,20,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8GetPatchName","cmsIT8SaveToMem","cmsBuildGamma","cmsSmoothToneCurve","cmsStageAllocCLut16bitGranular","cmsGetStageContextID","cmsStageOutputChannels","cmsStageFree","cmsD50_XYZ","cmsCreateNULLProfileTHR","cmsGetTagSignature","cmsGetHeaderCreationDateTime","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8GetPatchName","cmsIT8SaveToMem","cmsBuildGamma","cmsSmoothToneCurve","cmsStageAllocCLut16bitGranular","cmsGetStageContextID","cmsStageOutputChannels","cmsStageFree","cmsD50_XYZ","cmsCreateNULLProfileTHR","cmsGetTagSignature","cmsGetHeaderCreationDateTime","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Include necessary lcms headers

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    cmsContext ctx = nullptr;
    cmsHANDLE it8 = nullptr;
    cmsHPROFILE profile = nullptr;
    cmsToneCurve* curve = nullptr;
    cmsStage* stage = nullptr;
    cmsPipeline* pipeline = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    
    // Create context
    ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Load IT8 from memory input
    it8 = cmsIT8LoadFromMem(ctx, data, size);
    if (it8) {
        // 2. Get patch name from IT8
        char buffer[256];
        const char* patch_name = cmsIT8GetPatchName(it8, 0, buffer);
        
        // 3. Save IT8 to memory (simulate using output)
        cmsUInt32Number bytes_needed = 0;
        uint8_t* output_buffer = new uint8_t[size];
        cmsIT8SaveToMem(it8, output_buffer, &bytes_needed);
        delete[] output_buffer;
    }
    
    // 4. Create a simple tone curve and smooth it
    curve = cmsBuildGamma(ctx, 2.2);
    if (curve) {
        cmsSmoothToneCurve(curve, 1.0);
    }
    
    // 5. Create a dummy stage/pipeline to get context ID
    cmsUInt32Number clutPoints[] = {2, 2, 2};
    stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, 3, 3, nullptr);
    if (stage) {
        // 6. Get stage context ID
        cmsContext stage_ctx = cmsGetStageContextID(stage);
        
        // 7. Get stage output channels
        cmsUInt32Number output_channels = cmsStageOutputChannels(stage);
        
        // Clean up stage
        cmsStageFree(stage);
    }
    
    // 8. Get D50 white point
    const cmsCIEXYZ* d50 = cmsD50_XYZ();
    
    // 9. Create a NULL profile and use related APIs
    profile = cmsCreateNULLProfileTHR(ctx);
    if (profile) {
        // 10. Get tag signature
        cmsTagSignature sig = cmsGetTagSignature(profile, 0);
        
        // 11. Get header creation datetime
        struct tm creation_date;
        cmsGetHeaderCreationDateTime(profile, &creation_date);
        
        // 12. Use input data for file operations
        in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Read some data (example usage)
            uint8_t header[4];
            fread(header, 1, 4, in_file);
            assert_file_closed(&in_file);;
        }
        
        // 13. Write output to file
        out_file = fopen("output_file", "wb");
        if (out_file) {
            // Write something (example usage)
            fwrite("test", 1, 4, out_file);
            assert_file_closed(&out_file);;
        }
    }
    
    // Cleanup
    if (curve) cmsFreeToneCurve(curve);
    if (it8) cmsIT8Free(it8);
    if (profile) cmsCloseProfile(profile);
    if (ctx) cmsDeleteContext(ctx);
    
    return 0;
}