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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["cmsOpenProfileFromStream","cmsGetHeaderProfileID","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8GetPropertyDbl","cmsCreate_sRGBProfile","cmsCreateTransform","cmsChangeBuffersFormat","cmsDeleteTransform","cmsCloseProfile","cmsLabEncoded2Float","cmsBuildGamma","cmsIsToneCurveMonotonic","cmsFreeToneCurve","cmsPlugin","cmsIT8Free","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStream","cmsGetHeaderProfileID","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8GetPropertyDbl","cmsCreate_sRGBProfile","cmsCreateTransform","cmsChangeBuffersFormat","cmsDeleteTransform","cmsCloseProfile","cmsLabEncoded2Float","cmsBuildGamma","cmsIsToneCurveMonotonic","cmsFreeToneCurve","cmsPlugin","cmsIT8Free","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Open profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get profile ID using cmsGetHeaderProfileID
    cmsUInt8Number profileID[16];
    cmsGetHeaderProfileID(hProfile, profileID);

    // 3. Create a simple IT8 handle for cmsIT8GetPropertyDbl
    // We'll create a minimal IT8 structure
    cmsHANDLE hIT8 = cmsIT8Alloc(nullptr);
    if (hIT8) {
        // Set a property so we can get it back
        cmsIT8SetPropertyDbl(hIT8, "TestProp", 1.23);
        // Use cmsIT8GetPropertyDbl
        double propValue = cmsIT8GetPropertyDbl(hIT8, "TestProp");
        (void)propValue; // Use value to avoid unused variable warning
    }

    // 4. Create a transform and use cmsChangeBuffersFormat
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    if (hSRGB) {
        cmsHTRANSFORM hTransform = cmsCreateTransform(
            hProfile, TYPE_RGB_8, hSRGB, TYPE_RGB_8, 
            INTENT_PERCEPTUAL, 0);
        
        if (hTransform) {
            // Change buffer format
            cmsChangeBuffersFormat(hTransform, 
                TYPE_RGB_8, TYPE_BGR_8);
            
            // Clean up transform
            cmsDeleteTransform(hTransform);
        }
        cmsCloseProfile(hSRGB);
    }

    // 5. Use cmsLabEncoded2Float
    cmsUInt16Number encodedLab[3] = {0x8000, 0x8000, 0x8000}; // Midpoint values
    cmsCIELab labFloat;
    cmsLabEncoded2Float(&labFloat, encodedLab);

    // 6. Check monotonicity of tone curves
    // Get first tone curve from profile if available
    cmsToneCurve* toneCurve = nullptr;
    
    // Try to extract tone curve from profile
    // Create a simple gamma curve for testing
    cmsToneCurve* testCurve = cmsBuildGamma(nullptr, 2.2);
    if (testCurve) {
        // Use cmsIsToneCurveMonotonic
        cmsBool isMonotonic = cmsIsToneCurveMonotonic(testCurve);
        (void)isMonotonic; // Use value to avoid unused variable warning
        
        cmsFreeToneCurve(testCurve);
    }

    // 7. Use cmsPlugin with null plugin (just to call the API)
    // Note: cmsPlugin is typically used to register plugins
    cmsPlugin(nullptr);

    // 8. Clean up resources
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}