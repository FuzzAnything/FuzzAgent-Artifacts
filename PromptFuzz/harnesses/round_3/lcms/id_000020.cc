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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 4
//<Quality> {"density":12,"unique_branches":{"WriteStr":[[1829,13,1829,21,0,0,4,1]],"cmsIT8SaveToMem":[[2068,9,2068,16,0,0,4,1],[2068,20,2068,38,0,0,4,0],[2085,9,2085,16,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsSetHeaderModel","cmsCreate_OkLabProfile","cmsIsTag","cmsReadTag","cmsIsToneCurveMultisegment","cmsFloat2XYZEncoded","cmsIT8LoadFromMem","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsSetHeaderModel","cmsCreate_OkLabProfile","cmsIsTag","cmsReadTag","cmsIsToneCurveMultisegment","cmsFloat2XYZEncoded","cmsIT8LoadFromMem","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context for all operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Create NULL profile and set its header model
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
    if (nullProfile) {
        // Set an arbitrary model value (using first 4 bytes of input data if available)
        cmsUInt32Number model = 0;
        if (size >= 4) {
            memcpy(&model, data, 4);
        }
        cmsSetHeaderModel(nullProfile, model);
    }
    
    // 2. Create OkLab profile
    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(ctx);
    
    // 3. Extract tone curve from OkLab profile and check if multisegment
    cmsToneCurve* toneCurve = NULL;
    if (okLabProfile) {
        // Try to get the tone curve from the AToB0 tag (common in profiles)
        if (cmsIsTag(okLabProfile, cmsSigAToB0Tag)) {
            toneCurve = (cmsToneCurve*)cmsReadTag(okLabProfile, cmsSigAToB0Tag);
        }
    }
    
    // Check if the tone curve is multisegment (if we found one)
    if (toneCurve) {
        cmsIsToneCurveMultisegment(toneCurve);
    }
    
    // 4. Convert floating point XYZ to encoded 16-bit
    cmsCIEXYZ fxyz;
    cmsUInt16Number encodedXYZ[3];
    if (size >= 24) {
        // Use input data to populate XYZ values
        memcpy(&fxyz, data, sizeof(fxyz));
        // Ensure reasonable values to avoid extreme conversions
        fxyz.X = (fxyz.X < 0) ? 0 : (fxyz.X > 2.0) ? 2.0 : fxyz.X;
        fxyz.Y = (fxyz.Y < 0) ? 0 : (fxyz.Y > 2.0) ? 2.0 : fxyz.Y;
        fxyz.Z = (fxyz.Z < 0) ? 0 : (fxyz.Z > 2.0) ? 2.0 : fxyz.Z;
        
        cmsFloat2XYZEncoded(encodedXYZ, &fxyz);
    }
    
    // 5. Create IT8 handle from input data and save to memory
    if (size > 0) {
        cmsHANDLE it8 = cmsIT8LoadFromMem(ctx, data, size);
        if (it8) {
            // First call to get required buffer size
            cmsUInt32Number bytesNeeded = 0;
            cmsIT8SaveToMem(it8, NULL, &bytesNeeded);
            
            if (bytesNeeded > 0) {
                // Allocate buffer and save IT8 data
                void* buffer = malloc(bytesNeeded);
                if (buffer) {
                    cmsIT8SaveToMem(it8, buffer, &bytesNeeded);
                    free(buffer);
                }
            }
            
            // Clean up IT8 handle
            cmsIT8Free(it8);
        }
    }
    
    // 6. Clean up all resources
    if (nullProfile) cmsCloseProfile(nullProfile);
    if (okLabProfile) cmsCloseProfile(okLabProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}