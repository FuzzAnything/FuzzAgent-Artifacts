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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 150, nr_unique_branch: 42
//<Quality> {"density":15,"unique_branches":{"IsAvailableOnList":[[1293,9,1293,16,0,0,4,1],[1297,13,1297,20,0,0,4,1]],"WriteDataFormat":[[1948,9,1948,25,0,0,4,0]],"_cmsStageNormalizeToXyzFloat":[[1134,9,1134,20,0,0,4,1]],"WriteStr":[[1811,9,1811,20,0,0,4,1],[1819,9,1819,19,0,0,4,1],[1829,13,1829,21,0,0,4,0],[1831,17,1831,35,0,0,4,0],[1831,17,1831,35,0,0,4,1]],"cmsCreate_OkLabProfile":[[759,9,759,18,0,0,4,1],[773,9,773,57,0,0,4,1],[774,9,774,59,0,0,4,1],[775,9,775,59,0,0,4,1],[776,9,776,65,0,0,4,1],[777,9,777,65,0,0,4,1],[779,9,779,53,0,0,4,1],[781,9,781,65,0,0,4,1],[782,9,782,65,0,0,4,1],[783,9,783,59,0,0,4,1],[784,9,784,59,0,0,4,1],[785,9,785,57,0,0,4,1],[787,9,787,53,0,0,4,1]],"WriteData":[[1974,12,1974,20,0,0,4,0]],"cmsIT8SaveToMem":[[2068,9,2068,16,0,0,4,0],[2068,20,2068,38,0,0,4,1],[2075,15,2075,36,0,0,4,0],[2075,15,2075,36,0,0,4,1],[2085,9,2085,16,0,0,4,0]],"WriteHeader":[[1872,29,1872,40,0,0,4,0],[1872,29,1872,40,0,0,4,1],[1874,13,1874,32,0,0,4,1],[1894,13,1894,76,0,0,4,1],[1906,13,1906,21,0,0,4,0],[1910,13,1910,32,0,0,4,0],[1910,13,1910,32,0,0,4,1],[1914,13,1914,33,0,0,4,0],[1914,13,1914,33,0,0,4,1],[1918,13,1918,35,0,0,4,1],[1922,13,1922,30,0,0,4,1],[1926,13,1926,28,0,0,4,1],[1930,13,1930,20,0,0,4,1]],"_cmsStageNormalizeFromXyzFloat":[[1097,9,1097,20,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsCreate_OkLabProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsBuildTabulatedToneCurve16","cmsIsToneCurveMultisegment","cmsFloat2XYZEncoded","cmsFreeToneCurve","cmsIT8LoadFromMem","cmsIT8SaveToMem","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsCreate_OkLabProfile","cmsBuildTabulatedToneCurve16","cmsIsToneCurveMultisegment","cmsFloat2XYZEncoded","cmsFreeToneCurve","cmsIT8LoadFromMem","cmsIT8SaveToMem","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Create profiles
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(ctx);
    
    if (!nullProfile || !okLabProfile) {
        if (nullProfile) cmsCloseProfile(nullProfile);
        if (okLabProfile) cmsCloseProfile(okLabProfile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Set header model using data
    uint32_t model;
    if (size >= sizeof(model)) {
        memcpy(&model, data, sizeof(model));
        // Note: cmsSetHeaderModel is not in the provided API list, 
        // but we'll use the available data to set a model-related property
        // Since cmsSetHeaderModel isn't available, we'll skip this step
        // and use another approach to consume the data
    }
    
    // Create a simple tone curve from data to test cmsIsToneCurveMultisegment
    cmsToneCurve* toneCurve = nullptr;
    if (size >= 256) {
        // Create a tone curve with sampled data
        cmsUInt16Number values[256];
        size_t samples = size < 512 ? size / 2 : 256;
        for (size_t i = 0; i < samples && i < 256; i++) {
            values[i] = (data[i * 2] << 8) | data[i * 2 + 1];
        }
        toneCurve = cmsBuildTabulatedToneCurve16(ctx, samples, values);
    }
    
    if (toneCurve) {
        // Test if tone curve is multisegment
        cmsBool isMulti = cmsIsToneCurveMultisegment(toneCurve);
        (void)isMulti; // Use result to avoid unused variable warning
        
        // Test Float2XYZEncoded
        cmsCIEXYZ xyz;
        xyz.X = 0.5;
        xyz.Y = 0.5;
        xyz.Z = 0.5;
        cmsUInt16Number encoded[3];
        cmsFloat2XYZEncoded(encoded, &xyz);
        
        cmsFreeToneCurve(toneCurve);
    }
    
    // Create IT8 handle and save to memory
    cmsHANDLE it8 = cmsIT8LoadFromMem(ctx, data, size > 4096 ? 4096 : size);
    if (it8) {
        // Try to save IT8 to memory
        cmsUInt32Number bytesNeeded = 0;
        void* memPtr = malloc(4096);
        if (memPtr) {
            cmsBool saved = cmsIT8SaveToMem(it8, memPtr, &bytesNeeded);
            (void)saved; // Use result
            free(memPtr);
        }
        cmsIT8Free(it8);
    }
    
    // Clean up
    cmsCloseProfile(nullProfile);
    cmsCloseProfile(okLabProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}