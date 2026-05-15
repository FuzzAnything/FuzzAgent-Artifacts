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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 58
//<Quality> {"density":6,"unique_branches":{"SaveDescription":[[3597,9,3597,38,0,0,4,1],[3603,13,3603,68,0,0,4,1]],"WriteSeqID":[[3728,9,3728,56,0,0,4,1],[3731,9,3731,61,0,0,4,1]],"WriteSetOfCurves":[[2880,21,2880,72,0,0,4,1]],"Type_ProfileSequenceId_Write":[[3748,9,3748,44,0,0,4,1],[3751,9,3751,79,0,0,4,1]],"Type_ProfileSequenceDesc_Write":[[3615,9,3615,43,0,0,4,1],[3617,15,3617,26,0,0,4,0],[3617,15,3617,26,0,0,4,1],[3621,13,3621,56,0,0,4,1],[3622,13,3622,58,0,0,4,1],[3623,13,3623,58,0,0,4,1],[3624,13,3624,57,0,0,4,1],[3626,13,3626,59,0,0,4,1],[3627,13,3627,52,0,0,4,1]],"_cmsWriteUInt64Number":[[331,9,331,60,0,0,4,1]],"WritePositionTable":[[287,9,287,31,0,0,4,1],[290,9,290,29,0,0,4,1],[296,15,296,24,0,0,4,0],[296,15,296,24,0,0,4,1],[298,13,298,42,0,0,4,1],[299,13,299,42,0,0,4,1],[303,15,303,24,0,0,4,0],[303,15,303,24,0,0,4,1],[309,13,309,54,0,0,4,1],[317,9,317,37,0,0,4,1],[319,15,319,25,0,0,4,0],[319,15,319,25,0,0,4,1],[320,13,320,58,0,0,4,1],[321,13,321,56,0,0,4,1],[324,9,324,35,0,0,4,1],[326,9,326,31,0,0,4,0],[327,9,327,29,0,0,4,0]],"Type_MLU_Write":[[1779,9,1779,20,0,0,4,0],[1782,15,1782,44,0,0,4,1],[1783,15,1783,45,0,0,4,1]],"WriteCLUT":[[2908,9,2908,30,0,0,4,1],[2914,15,2914,60,0,0,4,0],[2914,15,2914,60,0,0,4,1],[2917,9,2917,76,0,0,4,1],[2919,9,2919,62,0,0,4,1],[2920,9,2920,37,0,0,4,1],[2921,9,2921,37,0,0,4,1],[2922,9,2922,37,0,0,4,1],[2925,9,2925,23,0,0,4,1],[2933,13,2933,27,0,0,4,0],[2935,17,2935,72,0,0,4,1],[2942,9,2942,32,0,0,4,1]],"Type_LUTA2B_Write":[[2965,13,2965,83,0,0,4,0],[2966,17,2966,146,0,0,4,0],[2967,21,2967,146,0,0,4,1],[2994,9,2994,18,0,0,4,0],[2997,13,2997,70,0,0,4,1],[3000,9,3000,21,0,0,4,0],[3002,13,3002,70,0,0,4,1],[3002,35,3002,52,0,0,4,1],[3019,13,3019,70,0,0,4,1]]},"library_calls":["cmsfilelength","cmsOpenProfileFromMem","cmsGetProfileContextID","cmsGetHeaderManufacturer","cmsCreateInkLimitingDeviceLink","cmsCreateInkLimitingDeviceLinkTHR","cmsSaveProfileToStream","cmsLab2XYZ","cmsCIE94DeltaE","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsFreeNamedColorList"],"critical_calls":["cmsfilelength","cmsOpenProfileFromMem","cmsGetProfileContextID","cmsGetHeaderManufacturer","cmsCreateInkLimitingDeviceLink","cmsCreateInkLimitingDeviceLinkTHR","cmsSaveProfileToStream","cmsLab2XYZ","cmsCIE94DeltaE","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsFreeNamedColorList"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0 || data == nullptr) {
        return 0;
    }

    // 1. Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get file length using cmsfilelength
    long file_len = cmsfilelength(in_file);
    (void)file_len; // Use variable to avoid unused warning

    // 3. Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Get profile context ID
    cmsContext context = cmsGetProfileContextID(hProfile);
    (void)context; // Use variable

    // 5. Get header manufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    (void)manufacturer; // Use variable

    // 6. Create ink limiting device link (with and without context)
    cmsHPROFILE hLink = nullptr;
    cmsHPROFILE hLink2 = nullptr;
    
    // Try CMYK color space
    hLink = cmsCreateInkLimitingDeviceLink(cmsSigCmykData, 1.0);
    if (context) {
        hLink2 = cmsCreateInkLimitingDeviceLinkTHR(context, cmsSigCmykData, 1.0);
    }

    // 7. Create output file for saving profile
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // 8. Save profile to stream
        cmsSaveProfileToStream(hLink ? hLink : hProfile, out_file);
        assert_file_closed(&out_file);;
    }

    // 9. Create sample data for color conversion APIs
    cmsCIELab lab1 = {50.0, 0.0, 0.0};
    cmsCIELab lab2 = {50.0, 10.0, 10.0};
    cmsCIEXYZ whitePoint = {0.9505, 1.0000, 1.0890};
    cmsCIEXYZ xyzResult;
    
    // 10. Use color conversion APIs
    cmsLab2XYZ(&whitePoint, &xyzResult, &lab1);
    
    // 11. Calculate Delta E
    cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);
    (void)deltaE; // Use variable

    // 12. Cleanup: close all profiles
    if (hLink) {
        cmsCloseProfile(hLink);
    }
    if (hLink2) {
        cmsCloseProfile(hLink2);
    }
    cmsCloseProfile(hProfile);

    // 13. Create and cleanup named color list (empty)
    cmsNAMEDCOLORLIST* namedColorList = nullptr;
    // Note: We cannot create a named color list with given APIs, 
    // so we'll just demonstrate the cleanup function
    cmsFreeNamedColorList(namedColorList);

    // 14. Cleanup input file
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}