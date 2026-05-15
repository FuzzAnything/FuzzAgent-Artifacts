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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 135, nr_unique_branch: 49
//<Quality> {"density":15,"unique_branches":{"cmsIsCLUT":[[835,9,835,55,0,0,4,0]],"_cmsDeleteTagByPos":[[725,9,725,34,0,0,4,1]],"cmsGetProfileInfoUTF8":[[1042,9,1042,20,0,0,4,0],[1042,9,1042,20,0,0,4,1]],"cmsWriteRawTag":[[2139,9,2139,55,0,0,4,1],[2141,9,2141,34,0,0,4,1],[2157,9,2157,32,0,0,4,1]],"cmsMLUgetUTF8":[[602,9,602,20,0,0,4,1],[606,9,606,21,0,0,4,0],[606,9,606,21,0,0,4,1],[611,9,611,23,0,0,4,1],[614,9,614,24,0,0,4,1],[617,9,617,33,0,0,4,0],[617,9,617,33,0,0,4,1]],"AllocateDataSet":[[1736,54,1736,69,0,0,4,0]],"freeOneTag":[[1653,13,1653,32,0,0,4,1]],"encodeUTF8":[[277,12,277,30,0,0,4,0],[277,12,277,30,0,0,4,1],[277,34,277,37,0,0,4,0],[277,34,277,37,0,0,4,1],[279,13,279,26,0,0,4,0],[279,13,279,26,0,0,4,1],[279,30,279,43,0,0,4,1],[283,17,283,30,0,0,4,0],[283,17,283,30,0,0,4,1],[283,34,283,47,0,0,4,1],[288,17,288,34,0,0,4,0],[288,17,288,34,0,0,4,1],[290,21,290,24,0,0,4,0],[290,21,290,24,0,0,4,1],[290,28,290,50,0,0,4,0],[290,28,290,50,0,0,4,1],[294,22,294,40,0,0,4,0],[294,22,294,40,0,0,4,1],[296,21,296,24,0,0,4,0],[296,21,296,24,0,0,4,1],[296,28,296,43,0,0,4,0],[296,47,296,69,0,0,4,0],[303,22,303,41,0,0,4,0],[303,22,303,41,0,0,4,1],[305,21,305,24,0,0,4,0],[305,21,305,24,0,0,4,1],[305,28,305,43,0,0,4,0],[305,47,305,69,0,0,4,0],[315,21,315,24,0,0,4,0],[315,21,315,24,0,0,4,1],[315,28,315,43,0,0,4,0],[315,47,315,69,0,0,4,0],[315,47,315,69,0,0,4,1]]},"library_calls":["cmsOpenProfileFromMem","cmsCreateProfilePlaceholder","cmsIsIntentSupported","cmsSetPCS","cmsCreateTransform","cmsGetNamedColorList","cmsFreeNamedColorList","cmsDeleteTransform","cmsGetProfileInfoUTF8","cmsSetHeaderAttributes","cmsxyY2XYZ","cmsWriteRawTag","cmsIT8LoadFromMem","cmsIT8SetPropertyHex","cmsIT8Free","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsCreateProfilePlaceholder","cmsIsIntentSupported","cmsSetPCS","cmsCreateTransform","cmsGetNamedColorList","cmsFreeNamedColorList","cmsDeleteTransform","cmsGetProfileInfoUTF8","cmsSetHeaderAttributes","cmsxyY2XYZ","cmsWriteRawTag","cmsIT8LoadFromMem","cmsIT8SetPropertyHex","cmsIT8Free","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<37) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeChar(fuzz_char_2, fdp);
		FDPConsumeChar(fuzz_char_3, fdp);
		FDPConsumeChar(fuzz_char_4, fdp);
		FDPConsumeChar(fuzz_char_5, fdp);
		FDPConsumeChar(fuzz_char_6, fdp);
		FDPConsumeChar(fuzz_char_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
	
		const char fuzzer_var_cmsGetProfileInfoUTF8_8_2[3] = {fuzz_char_2,fuzz_char_3,fuzz_char_4};
		const char fuzzer_var_cmsGetProfileInfoUTF8_8_3[3] = {fuzz_char_5,fuzz_char_6,fuzz_char_7};//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for operations

    // 1. Create a profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        // If profile creation fails, create a placeholder
        hProfile = cmsCreateProfilePlaceholder(NULL);
        if (!hProfile) return 0;
    }

    // 2. Check if intent is supported
    cmsUInt32Number intent = data[0] % 4;  // Use first byte for intent
    cmsUInt32Number direction = data[1] % 2;  // Use second byte for direction
    cmsBool isSupported = cmsIsIntentSupported(hProfile, intent, direction);

    // 3. Set PCS (Profile Connection Space)
    cmsColorSpaceSignature pcs = (data[2] % 2) ? cmsSigLabData : cmsSigXYZData;
    cmsSetPCS(hProfile, pcs);

    // 4. Create a transform to get named color list
    cmsHTRANSFORM xform = cmsCreateTransform(hProfile, TYPE_Lab_DBL, NULL, 
                                           TYPE_Lab_DBL, intent, fuzz_uint32_t_1);
    if (xform) {
        cmsNAMEDCOLORLIST* namedColorList = cmsGetNamedColorList(xform);
        if (namedColorList) {
            // Clean up named color list
            cmsFreeNamedColorList(namedColorList);
        }
        cmsDeleteTransform(xform);
    }

    // 5. Get profile info in UTF8
    char buffer[256];
    const char lang[3] = {'e', 'n', '\0'};
    const char country[3] = {'U', 'S', '\0'};
    cmsUInt32Number infoSize = cmsGetProfileInfoUTF8(hProfile, cmsInfoDescription, 
                                                    fuzzer_var_cmsGetProfileInfoUTF8_8_2, fuzzer_var_cmsGetProfileInfoUTF8_8_3, buffer, sizeof(buffer));

    // 6. Set header attributes
    cmsUInt64Number flags = 0;
    if (size >= 16) {
        memcpy(&flags, data + 8, sizeof(cmsUInt64Number));
    }
    cmsSetHeaderAttributes(hProfile, fuzz_uint64_t_8);

    // 7. Convert xyY to XYZ
    cmsCIExyY xyY;
    cmsCIEXYZ xyz;
    if (size >= 48) {
        memcpy(&xyY, data + 16, sizeof(cmsCIExyY));
        cmsxyY2XYZ(&xyz, &xyY);
    }

    // 8. Write raw tag data
    if (size >= 100) {
        cmsTagSignature tagSig = cmsSigCopyrightTag;
        const void* tagData = data + 48;
        cmsUInt32Number tagSize = size - 48;
        if (tagSize > 100) tagSize = 100;
        cmsWriteRawTag(hProfile, tagSig, tagData, tagSize);
    }

    // 9. Create IT8 handle and set property
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(NULL, data, size > 1000 ? 1000 : size);
    if (hIT8) {
        cmsUInt32Number hexVal = 0;
        if (size >= 60) {
            memcpy(&hexVal, data + 52, sizeof(cmsUInt32Number));
        }
        cmsIT8SetPropertyHex(hIT8, fuzz_str_9, fuzz_uint32_t_10);
        // Clean up IT8 handle
        cmsIT8Free(hIT8);
    }

    // Clean up profile
    cmsCloseProfile(hProfile);

    return 0;
}