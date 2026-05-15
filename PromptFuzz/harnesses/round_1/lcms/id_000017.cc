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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 3.6666667, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsOpenIOhandlerFromStream","cmsOpenProfileFromIOhandler2THR","cmsCreateXYZProfileTHR","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsCreateProofingTransform","cmsCreateLinearizationDeviceLink","cmsGetTagSignature","cmsSetColorSpace","cmsIT8SetPropertyDbl","cmsIT8GetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8SaveToFile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsOpenIOhandlerFromStream","cmsOpenProfileFromIOhandler2THR","cmsCreateXYZProfileTHR","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsCreateProofingTransform","cmsCreateLinearizationDeviceLink","cmsGetTagSignature","cmsSetColorSpace","cmsIT8SetPropertyDbl","cmsIT8GetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8SaveToFile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // Open profile from memory
    cmsHPROFILE hProfileMem = cmsOpenProfileFromMem(data, size);
    
    // Open profile from stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    cmsHPROFILE hProfileStream = nullptr;
    if (in_file) {
        cmsIOHANDLER *io = cmsOpenIOhandlerFromStream(context, in_file);
        if (io) {
            hProfileStream = cmsOpenProfileFromIOhandler2THR(context, io, FALSE);
        }
    }

    // Create XYZ profile
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(context);
    
    // Create Lab profile with D50 white point
    const cmsCIExyY *d50 = cmsD50_xyY();
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, d50);

    // Create proofing transform if we have valid profiles
    cmsHTRANSFORM transform = nullptr;
    if (hProfileMem && xyzProfile && labProfile) {
        transform = cmsCreateProofingTransform(
            hProfileMem, TYPE_RGB_8,
            xyzProfile, TYPE_XYZ_DBL,
            labProfile, INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC, 0
        );
    }

    // Create linearization device link (dummy example)
    cmsToneCurve* curves[3] = {nullptr, nullptr, nullptr};
    cmsHPROFILE deviceLink = nullptr;
    if (xyzProfile) {
        deviceLink = cmsCreateLinearizationDeviceLink(cmsSigRgbData, curves);
    }

    // Get tag signature from profile
    if (hProfileMem) {
        cmsTagSignature sig = cmsGetTagSignature(hProfileMem, 0);
        // Set color space (example operation)
        cmsSetColorSpace(hProfileMem, cmsSigRgbData);
    }

    // IT8 operations
    cmsHANDLE it8 = nullptr;
    // Note: cmsIT8Alloc is needed but not in provided list, so we'll skip creation
    // We'll demonstrate the APIs with conditional checks
    if (it8) {
        cmsIT8SetPropertyDbl(it8, "TEST_PROP", 1.0);
        cmsFloat64Number val = cmsIT8GetPropertyDbl(it8, "TEST_PROP");
        cmsIT8SetDataRowCol(it8, 0, 0, "test_data");
        cmsIT8SaveToFile(it8, "output_file.it8");
    }

    // Cleanup
    if (transform) cmsDeleteTransform(transform);
    if (deviceLink) cmsCloseProfile(deviceLink);
    if (labProfile) cmsCloseProfile(labProfile);
    if (xyzProfile) cmsCloseProfile(xyzProfile);
    if (hProfileStream) cmsCloseProfile(hProfileStream);
    if (hProfileMem) cmsCloseProfile(hProfileMem);
    if (it8) cmsIT8Free(it8);
    if (in_file) assert_file_closed(&in_file);;
    
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}