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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"smooth2":[[1161,18,1161,47,0,0,4,0]],"cmsSmoothToneCurve":[[1246,25,1246,35,0,0,4,0],[1252,25,1252,91,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsDeleteContext","cmsSetHeaderModel","cmsIsTag","cmsReadTag","cmsBuildGamma","cmsSmoothToneCurve","cmsIsToneCurveMultisegment","cmsFloat2XYZEncoded","cmsFreeToneCurve","cmsIT8LoadFromMem","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCreateExtendedTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsSetHeaderModel","cmsIsTag","cmsReadTag","cmsBuildGamma","cmsSmoothToneCurve","cmsIsToneCurveMultisegment","cmsFloat2XYZEncoded","cmsFreeToneCurve","cmsIT8LoadFromMem","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCreateExtendedTransform","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs together to handle potential ICC profile and IT8 data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create NULL profile
    cmsHPROFILE null_profile = cmsCreateNULLProfileTHR(context);
    if (!null_profile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Set header model using some data from input
    if (size >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number model;
        memcpy(&model, data, sizeof(model));
        cmsSetHeaderModel(null_profile, model);
    }
    
    // 3. Check if tag exists and read it
    cmsBool is_tag = cmsIsTag(null_profile, cmsSigMediaWhitePointTag);
    if (is_tag) {
        void* tag_data = cmsReadTag(null_profile, cmsSigMediaWhitePointTag);
        // tag_data might be NULL, that's OK
    }
    
    // 4. Create and smooth a tone curve using input data
    cmsToneCurve* curve = cmsBuildGamma(context, 2.2);
    if (curve) {
        // Use lambda from input if available
        cmsFloat64Number lambda = 1.0;
        if (size >= sizeof(lambda) + 4) {
            memcpy(&lambda, data + 4, sizeof(lambda));
        }
        cmsSmoothToneCurve(curve, lambda);
        
        // Check if tone curve is multisegment
        cmsBool is_multi = cmsIsToneCurveMultisegment(curve);
        
        // Convert XYZ to encoded format using some dummy data
        cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
        cmsUInt16Number encoded_xyz[3];
        cmsFloat2XYZEncoded(encoded_xyz, &xyz);
        
        cmsFreeToneCurve(curve);
    }
    
    // 5. Load IT8 data from memory (using the input data)
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, (cmsUInt32Number)size);
    if (it8) {
        // Enumerate IT8 properties
        const char** subpropertyNames = NULL;
        cmsUInt32Number prop_count = cmsIT8EnumPropertyMulti(it8, "SAMPLE_NAME", &subpropertyNames);
        
        // Free IT8 data
        cmsIT8Free(it8);
    }
    
    // 6. Create extended transform using the NULL profile
    cmsHPROFILE profiles[2] = {null_profile, null_profile};
    cmsBool bpc[2] = {FALSE, FALSE};
    cmsUInt32Number intents[2] = {INTENT_PERCEPTUAL, INTENT_PERCEPTUAL};
    cmsFloat64Number adaptation[2] = {1.0, 1.0};
    
    cmsHTRANSFORM transform = cmsCreateExtendedTransform(
        context,
        2,                    // nProfiles
        profiles,             // hProfiles
        bpc,                  // BPC
        intents,              // Intents
        adaptation,           // AdaptationStates
        NULL,                 // hGamutProfile
        0,                    // nGamutPCSposition
        TYPE_RGB_8,           // InputFormat
        TYPE_RGB_8,           // OutputFormat
        0                     // dwFlags
    );
    
    // 7. Clean up resources
    if (transform) {
        // Transform would need to be deleted by cmsDeleteTransform,
        // but we don't have that API in our list, so we'll just close the profile
    }
    
    cmsCloseProfile(null_profile);
    cmsDeleteContext(context);
    
    return 0;
}