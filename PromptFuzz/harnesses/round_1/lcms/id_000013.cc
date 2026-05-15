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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"cmsIT8SaveToFile":[[2035,13,2035,28,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsOpenIOhandlerFromStream","cmsOpenProfileFromIOhandler2THR","cmsCreateXYZProfileTHR","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsCreateProofingTransform","cmsCreateLinearizationDeviceLink","cmsGetTagSignature","cmsSetColorSpace","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyUncooked","cmsIT8SaveToFile","cmsIT8Free","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsOpenIOhandlerFromStream","cmsOpenProfileFromIOhandler2THR","cmsCreateXYZProfileTHR","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsCreateProofingTransform","cmsCreateLinearizationDeviceLink","cmsGetTagSignature","cmsSetColorSpace","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyUncooked","cmsIT8SaveToFile","cmsIT8Free","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open input file from memory stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Open profile from memory stream
    cmsHPROFILE hInProfile = cmsOpenProfileFromMem(data, size);
    if (!hInProfile) {
        // Try opening from file handler
        cmsIOHANDLER *io = cmsOpenIOhandlerFromStream(ctx, in_file);
        if (io) {
            hInProfile = cmsOpenProfileFromIOhandler2THR(ctx, io, false);
        }
    }

    // Open output profile (use XYZ profile as destination)
    cmsHPROFILE hOutProfile = cmsCreateXYZProfileTHR(ctx);
    const cmsCIExyY *d50 = cmsD50_xyY();
    cmsHPROFILE hProofProfile = cmsCreateLab4ProfileTHR(ctx, d50);

    // Create proofing transform
    cmsHTRANSFORM hTransform = nullptr;
    if (hInProfile && hOutProfile && hProofProfile) {
        hTransform = cmsCreateProofingTransform(
            hInProfile,
            TYPE_RGB_8,
            hOutProfile,
            TYPE_XYZ_DBL,
            hProofProfile,
            INTENT_PERCEPTUAL,
            INTENT_ABSOLUTE_COLORIMETRIC,
            0
        );
    }

    // Main event implementation
    if (hTransform) {
        // 1. Create device link profile from transform (simulated)
        // Note: cmsTransform2DeviceLink is not in provided APIs, using alternative
        // Create a device link profile manually
        cmsHPROFILE hDeviceLink = cmsCreateLinearizationDeviceLink(
            cmsSigRgbData,
            nullptr
        );
        
        if (hDeviceLink) {
            // 2. Get tag signature
            cmsTagSignature sig = cmsGetTagSignature(hDeviceLink, 0);
            
            // 3. Set color space
            cmsSetColorSpace(hDeviceLink, cmsSigRgbData);
            
            // 4. Create IT8 handle and save to file
            cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
            if (hIT8) {
                cmsIT8SetSheetType(hIT8, "CGATS.17");
                cmsIT8SetPropertyUncooked(hIT8, "ORIGINATOR", "Fuzzer");
                cmsIT8SaveToFile(hIT8, "output.it8");
                cmsIT8Free(hIT8);
            }
            
            cmsCloseProfile(hDeviceLink);
        }
    }

    // Clean up
    if (hTransform) cmsDeleteTransform(hTransform);
    if (hProofProfile) cmsCloseProfile(hProofProfile);
    if (hOutProfile) cmsCloseProfile(hOutProfile);
    if (hInProfile) cmsCloseProfile(hInProfile);
    if (in_file) assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);

    assert_file_closed(&in_file);
	return 0;
}