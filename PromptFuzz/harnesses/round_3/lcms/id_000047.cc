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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsGetHeaderManufacturer","cmsGetColorSpace","cmsCreateInkLimitingDeviceLink","cmsCreateInkLimitingDeviceLinkTHR","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsfilelength","cmsSaveProfileToStream","cmsLab2XYZ","cmsCIE94DeltaE","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsGetHeaderManufacturer","cmsGetColorSpace","cmsCreateInkLimitingDeviceLink","cmsCreateInkLimitingDeviceLinkTHR","cmsfilelength","cmsSaveProfileToStream","cmsLab2XYZ","cmsCIE94DeltaE","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // 1. Open profile from memory (consumes input data)
    cmsHPROFILE hInProfile = cmsOpenProfileFromMem(data, size);
    if (!hInProfile) {
	return 0;
	}
	

    // 2. Get profile context and manufacturer
    cmsContext context = cmsGetProfileContextID(hInProfile);
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hInProfile);

    // 3. Create ink limiting device links
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hInProfile);
    cmsFloat64Number limit = 0.85;
    cmsHPROFILE hInkLimit1 = cmsCreateInkLimitingDeviceLink(colorSpace, limit);
    cmsHPROFILE hInkLimit2 = cmsCreateInkLimitingDeviceLinkTHR(context, colorSpace, limit);

    // 4. Create FILE* from input data for cmsfilelength
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsCloseProfile(hInkLimit2);
        cmsCloseProfile(hInkLimit1);
        cmsCloseProfile(hInProfile);
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Use cmsfilelength
    long file_len = cmsfilelength(in_file);
    assert_file_closed(&in_file);;

    // 6. Create output file for saving profile
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        cmsSaveProfileToStream(hInProfile, out_file);
        assert_file_closed(&out_file);;
    }

    // 7. Create and use named color list (if available)
    // Note: We need to obtain a named color list from somewhere.
    // Since we don't have one, we'll create a minimal dummy scenario.
    // In real usage, this would come from a profile's named color tag.
    cmsNAMEDCOLORLIST* namedColorList = NULL;
    // We can't create a named color list with given APIs, so we'll skip
    // cmsNamedColorCount and cmsFreeNamedColorList since we have no list

    // 8. Create dummy IT8 handle for cmsIT8GetDataRowColDbl
    // Note: We don't have APIs to create IT8 handle in given list
    // We'll skip this API as we cannot create valid hIT8

    // 9. Use color conversion functions with dummy data
    cmsCIEXYZ whitePoint = {0.95047, 1.00000, 1.08883}; // D65
    cmsCIELab lab1 = {50.0, 20.0, 30.0};
    cmsCIELab lab2 = {55.0, 25.0, 35.0};
    cmsCIEXYZ xyz_result;
    
    cmsLab2XYZ(&whitePoint, &xyz_result, &lab1);
    cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);

    // 10. Cleanup
    if (hInkLimit2) cmsCloseProfile(hInkLimit2);
    if (hInkLimit1) cmsCloseProfile(hInkLimit1);
    if (hInProfile) cmsCloseProfile(hInProfile);
    
    // Note: namedColorList is NULL, so cmsFreeNamedColorList not called

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}