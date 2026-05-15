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
//<ID> 389
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"cmsFloat2XYZEncoded":[[393,9,393,18,0,0,4,0],[405,9,405,18,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsDeleteContext","cmsSetHeaderModel","cmsIsTag","cmsReadTag","cmsBuildGamma","cmsSmoothToneCurve","cmsIsToneCurveMultisegment","cmsFreeToneCurve","cmsFloat2XYZEncoded","cmsIT8LoadFromMem","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCreateExtendedTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsSetHeaderModel","cmsReadTag","cmsIsTag","cmsBuildGamma","cmsSmoothToneCurve","cmsIsToneCurveMultisegment","cmsFreeToneCurve","cmsFloat2XYZEncoded","cmsIT8LoadFromMem","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCreateExtendedTransform","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Include LCMS headers

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context for LCMS operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // 1. Create a NULL profile
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(context);
    if (!nullProfile) {
        cmsDeleteContext(context);
        return 0;
    }

    // 2. Set header model using data (use first 4 bytes if available)
    if (size >= 4) {
        cmsUInt32Number model;
        memcpy(&model, data, 4);
        cmsSetHeaderModel(nullProfile, model);
    }

    // 3. Check if specific tags exist and read them
    cmsTagSignature tags[] = {
        cmsSigProfileDescriptionTag,
        cmsSigCopyrightTag,
        cmsSigMediaWhitePointTag
    };
    
    for (auto sig : tags) {
        if (cmsIsTag(nullProfile, sig)) {
            void* tagData = cmsReadTag(nullProfile, sig);
            // Tag data could be used here if needed
        }
    }

    // 4. Create and manipulate a tone curve
    if (size >= 8) {
        // Create a simple gamma tone curve
        cmsFloat64Number gamma;
        memcpy(&gamma, data, sizeof(gamma));
        if (gamma < 0.1) gamma = 0.1;
        if (gamma > 10.0) gamma = 10.0;
        
        cmsToneCurve* toneCurve = cmsBuildGamma(context, gamma);
        if (toneCurve) {
            // Smooth the tone curve
            cmsSmoothToneCurve(toneCurve, 1.0);
            
            // Check if it's multisegment
            cmsIsToneCurveMultisegment(toneCurve);
            
            // Free the tone curve
            cmsFreeToneCurve(toneCurve);
        }
    }

    // 5. Convert XYZ to encoded format
    if (size >= 24) {
        cmsCIEXYZ xyz;
        memcpy(&xyz.X, data, sizeof(xyz.X));
        memcpy(&xyz.Y, data + 8, sizeof(xyz.Y));
        memcpy(&xyz.Z, data + 16, sizeof(xyz.Z));
        
        cmsUInt16Number encodedXYZ[3];
        cmsFloat2XYZEncoded(encodedXYZ, &xyz);
    }

    // 6. Load IT8 table from memory
    cmsHANDLE it8 = nullptr;
    if (size > 0) {
        it8 = cmsIT8LoadFromMem(context, data, size);
        if (it8) {
            // Enumerate properties
            const char** subpropertyNames = nullptr;
            cmsUInt32Number propCount = cmsIT8EnumPropertyMulti(it8, "PROPERTY", &subpropertyNames);
            
            // Free IT8 table
            cmsIT8Free(it8);
        }
    }

    // 7. Create extended transform
    cmsHTRANSFORM transform = nullptr;
    if (nullProfile) {
        cmsHPROFILE profiles[] = {nullProfile};
        cmsBool bpc = FALSE;
        cmsUInt32Number intents[] = {INTENT_PERCEPTUAL};
        cmsFloat64Number adaptationStates[] = {1.0};
        
        transform = cmsCreateExtendedTransform(
            context,
            1,                    // nProfiles
            profiles,            // hProfiles
            &bpc,                // BPC
            intents,            // Intents
            adaptationStates,    // AdaptationStates
            nullptr,            // hGamutProfile
            0,                  // nGamutPCSposition
            TYPE_RGB_8,         // InputFormat
            TYPE_RGB_8,         // OutputFormat
            0                   // dwFlags
        );
    }

    // 8. Clean up
    if (transform) {
        // Transform would need to be deleted with cmsDeleteTransform,
        // but that's not in our API list, so we'll skip explicit deletion
        // as cmsDeleteContext should handle it
    }
    
    if (nullProfile) {
        cmsCloseProfile(nullProfile);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}