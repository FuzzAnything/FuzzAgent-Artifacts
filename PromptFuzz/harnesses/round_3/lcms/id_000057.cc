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
//<ID> 388
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 5
//<Quality> {"density":17,"unique_branches":{"cmsIT8EnumPropertyMulti":[[2720,8,2720,58,0,0,4,1],[2728,20,2728,31,0,0,4,0],[2728,20,2728,31,0,0,4,1],[2729,12,2729,31,0,0,4,1],[2735,9,2735,22,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsDeleteContext","cmsSetHeaderModel","cmsIsTag","cmsReadTag","cmsBuildGamma","cmsSmoothToneCurve","cmsIsToneCurveMultisegment","cmsFloat2XYZEncoded","cmsFreeToneCurve","cmsIT8LoadFromMem","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCreateExtendedTransform","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsSetHeaderModel","cmsIsTag","cmsReadTag","cmsBuildGamma","cmsSmoothToneCurve","cmsIsToneCurveMultisegment","cmsFloat2XYZEncoded","cmsFreeToneCurve","cmsIT8LoadFromMem","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCreateExtendedTransform","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a fuzz driver that utilizes the specified LCMS APIs to achieve an event of processing a color profile and IT8 data:

*/


// Include LCMS headers

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create NULL profile
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(context);
    if (!nullProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Set header model using data
    cmsUInt32Number model = *(reinterpret_cast<const cmsUInt32Number*>(data));
    cmsSetHeaderModel(nullProfile, model);
    
    // 3. Check for tags in NULL profile
    cmsBool hasTag = cmsIsTag(nullProfile, cmsSigMediaWhitePointTag);
    
    // 4. Read tag if it exists
    if (hasTag) {
        void* tagData = cmsReadTag(nullProfile, cmsSigMediaWhitePointTag);
        // tagData may be null for NULL profile
    }
    
    // 5. Create and process tone curve
    cmsToneCurve* curve = cmsBuildGamma(context, 2.2);
    if (curve) {
        // Smooth the tone curve
        cmsSmoothToneCurve(curve, 1.0);
        
        // Check if multisegment
        cmsBool isMultiSeg = cmsIsToneCurveMultisegment(curve);
        
        // Convert XYZ for testing
        cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
        cmsUInt16Number encodedXYZ[3];
        cmsFloat2XYZEncoded(encodedXYZ, &xyz);
        
        cmsFreeToneCurve(curve);
    }
    
    // 6. Load IT8 data from input
    // Use a portion of input for IT8
    size_t it8Size = size > 1000 ? 1000 : size;
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, static_cast<cmsUInt32Number>(it8Size));
    
    if (it8) {
        // 7. Enumerate IT8 properties
        const char** subProps = nullptr;
        cmsUInt32Number propCount = cmsIT8EnumPropertyMulti(it8, "DATA", &subProps);
        
        // 8. Free IT8
        cmsIT8Free(it8);
    }
    
    // 9. Create extended transform
    cmsHPROFILE profiles[2] = {nullProfile, nullProfile};
    cmsBool bpc[2] = {FALSE, FALSE};
    cmsUInt32Number intents[2] = {INTENT_PERCEPTUAL, INTENT_PERCEPTUAL};
    cmsFloat64Number adaptStates[2] = {1.0, 1.0};
    
    // Create transform with NULL profiles
    cmsHTRANSFORM transform = cmsCreateExtendedTransform(
        context,
        2,           // nProfiles
        profiles,    // hProfiles
        bpc,         // BPC
        intents,     // Intents
        adaptStates, // AdaptationStates
        nullptr,     // hGamutProfile
        0,           // nGamutPCSposition
        TYPE_RGB_8,  // InputFormat
        TYPE_RGB_8,  // OutputFormat
        cmsFLAGS_NOCACHE
    );
    
    // Cleanup
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    // 10. Close profile
    cmsCloseProfile(nullProfile);
    
    // 11. Delete context
    cmsDeleteContext(context);
    
    return 0;
}