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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["cmsDupContext","cmsCreateBCHSWabstractProfileTHR","cmsDeleteContext","cmsCreate_OkLabProfile","cmsCloseProfile","cmsDeleteContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8Free","cmsGetHeaderFlags","cmsCreateMultiprofileTransform","cmsDoTransform","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsDupContext","cmsCreateBCHSWabstractProfileTHR","cmsCreate_OkLabProfile","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8Free","cmsGetHeaderFlags","cmsCreateMultiprofileTransform","cmsDoTransform","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimal data
    
    // Create a context by duplicating default context
    cmsContext ctx = cmsDupContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Parse parameters from input data
    // Use first 7 doubles (56 bytes) for BCHSW parameters
    size_t offset = 0;
    double params[7] = {1.0, 1.0, 0.0, 1.0, 5000, 6500, 1.0};
    
    if (size >= sizeof(double) * 7) {
        memcpy(params, data, sizeof(double) * 7);
        offset += sizeof(double) * 7;
    }
    
    // Create BCHSW abstract profile
    cmsHPROFILE bchswProfile = cmsCreateBCHSWabstractProfileTHR(
        ctx,
        256,  // nLUTPoints
        params[0],  // Bright
        params[1],  // Contrast
        params[2],  // Hue
        params[3],  // Saturation
        static_cast<cmsUInt32Number>(params[4]),  // TempSrc
        static_cast<cmsUInt32Number>(params[5])   // TempDest
    );
    
    if (!bchswProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create OkLab profile
    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(ctx);
    if (!okLabProfile) {
        cmsCloseProfile(bchswProfile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create array of profiles for multiprofile transform
    cmsHPROFILE profiles[2];
    profiles[0] = bchswProfile;
    profiles[1] = okLabProfile;
    
    // Try to load IT8 data from remaining input
    cmsHANDLE it8 = NULL;
    if (size - offset > 0) {
        it8 = cmsIT8LoadFromMem(ctx, data + offset, size - offset);
    }
    
    // If we have IT8 handle, enumerate its data format
    if (it8) {
        char** sampleNames = NULL;
        int formatCount = cmsIT8EnumDataFormat(it8, &sampleNames);
        (void)formatCount;  // Mark as used
        cmsIT8Free(it8);
    }
    
    // Get header flags for BCHSW profile
    cmsUInt32Number flags = cmsGetHeaderFlags(bchswProfile);
    (void)flags;  // Mark as used
    
    // Create multiprofile transform
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(
        profiles, 2,
        TYPE_RGB_8,    // Input format
        TYPE_RGB_8,    // Output format
        INTENT_PERCEPTUAL,
        cmsFLAGS_NOCACHE
    );
    
    if (transform) {
        // Test transform with sample data
        uint8_t input[3] = {100, 150, 200};
        uint8_t output[3];
        cmsDoTransform(transform, input, output, 1);
        
        // Save OkLab profile to memory
        cmsUInt32Number bytesNeeded = 0;
        cmsSaveProfileToMem(okLabProfile, NULL, &bytesNeeded);
        
        if (bytesNeeded > 0) {
            std::vector<uint8_t> profileBuffer(bytesNeeded);
            cmsSaveProfileToMem(okLabProfile, profileBuffer.data(), &bytesNeeded);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // Cleanup
    cmsCloseProfile(okLabProfile);
    cmsCloseProfile(bchswProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}