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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"ParseFloatNumber":[[728,12,728,19,0,0,4,1]]},"library_calls":["cmsOpenProfileFromStream","cmsGetHeaderProfileID","cmsCreate_sRGBProfile","cmsCreateTransform","cmsChangeBuffersFormat","cmsBuildTabulatedToneCurve16","cmsIsToneCurveMonotonic","cmsFreeToneCurve","cmsLabEncoded2Float","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8GetPropertyDbl","cmsIT8Free","cmsPlugin","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStream","cmsGetHeaderProfileID","cmsCreate_sRGBProfile","cmsCreateTransform","cmsChangeBuffersFormat","cmsBuildTabulatedToneCurve16","cmsIsToneCurveMonotonic","cmsFreeToneCurve","cmsLabEncoded2Float","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8GetPropertyDbl","cmsIT8Free","cmsPlugin","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // 1. Create a FILE* from input data for profile reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open ICC profile from the input data
    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get profile ID using cmsGetHeaderProfileID
    cmsUInt8Number profileID[16];
    cmsGetHeaderProfileID(hProfile, profileID);
    
    // 4. Check if tone curves are monotonic
    // First create a simple transform to access tone curves
    cmsHPROFILE sRGB = cmsCreate_sRGBProfile();
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, TYPE_RGB_8, sRGB, TYPE_RGB_8,
                                                  INTENT_PERCEPTUAL, 0);
    
    if (hTransform) {
        // Change buffer format (example: from packed to planar)
        cmsChangeBuffersFormat(hTransform, TYPE_RGB_8, TYPE_RGB_8_PLANAR);
        
        // Create a dummy tone curve for monotonicity check
        cmsUInt16Number curvePoints[3] = {0, 128, 65535};
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(0, 3, curvePoints);
        if (toneCurve) {
            cmsBool isMonotonic = cmsIsToneCurveMonotonic(toneCurve);
            cmsFreeToneCurve(toneCurve);
        }
    }
    
    // 5. Convert encoded Lab to float
    cmsUInt16Number encodedLab[3] = {32768, 32768, 32768};  // Midpoint values
    cmsCIELab labFloat;
    cmsLabEncoded2Float(&labFloat, encodedLab);
    
    // 6. Create an IT8 handle and get property
    // First create a simple IT8 structure
    cmsHANDLE hIT8 = cmsIT8Alloc(0);
    if (hIT8) {
        // Set some properties
        cmsIT8SetPropertyDbl(hIT8, "SAMPLE_ID", 1.0);
        
        // Get property value (though we just set it)
        cmsFloat64Number propValue = cmsIT8GetPropertyDbl(hIT8, "SAMPLE_ID");
        
        cmsIT8Free(hIT8);
    }
    
    // 7. Call cmsPlugin (with NULL since we don't have actual plugin)
    cmsPlugin(NULL);
    
    // 8. Cleanup
    if (hTransform) cmsDeleteTransform(hTransform);
    if (sRGB) cmsCloseProfile(sRGB);
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}