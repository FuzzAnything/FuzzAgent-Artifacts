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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 30
//<Quality> {"density":4,"unique_branches":{"cmsNamedColorInfo":[[863,9,863,31,0,0,4,1],[865,9,865,53,0,0,4,1],[868,9,868,13,0,0,4,0],[869,9,869,15,0,0,4,0],[870,9,870,15,0,0,4,0],[871,9,871,12,0,0,4,0],[874,9,874,17,0,0,4,0]],"NonlinearCompression":[[154,17,154,22,0,0,4,0],[154,17,154,22,0,0,4,1],[155,13,155,28,0,0,4,1]],"cmsCIECAM02Done":[[436,9,436,14,0,0,4,0]],"InverseNonlinearity":[[306,17,306,22,0,0,4,0],[306,17,306,22,0,0,4,1],[307,13,307,37,0,0,4,0],[307,13,307,37,0,0,4,1]],"InverseChromaticAdaptation":[[344,17,344,22,0,0,4,0],[344,17,344,22,0,0,4,1]],"InverseCorrelates":[[261,10,261,18,0,0,4,0],[261,10,261,18,0,0,4,1],[269,13,269,43,0,0,4,0],[269,13,269,43,0,0,4,1]],"cmsIT8DefineDblFormat":[[3054,9,3054,26,0,0,4,1]],"ChromaticAdaptation":[[116,17,116,22,0,0,4,0],[116,17,116,22,0,0,4,1]],"cmsCIECAM02Init":[[369,8,369,87,0,0,4,1],[387,5,387,27,0,0,4,1],[393,5,393,23,0,0,4,1],[399,5,399,22,0,0,4,1],[405,5,405,12,0,0,4,0],[417,9,417,34,0,0,4,1]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsMLUalloc","cmsMLUsetUTF8","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsBuildGamma","cmsReverseToneCurveEx","cmsFreeToneCurve","cmsCIECAM02Init","cmsCIECAM02Reverse","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorCount","cmsNamedColorInfo","cmsFreeToneCurve","cmsCIECAM02Done","cmsIT8Free","cmsMLUfree","cmsFreeNamedColorList","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsMLUalloc","cmsMLUsetUTF8","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsBuildGamma","cmsReverseToneCurveEx","cmsFreeToneCurve","cmsCIECAM02Init","cmsCIECAM02Reverse","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorCount","cmsNamedColorInfo","cmsFreeToneCurve","cmsCIECAM02Done","cmsIT8Free","cmsMLUfree","cmsFreeNamedColorList","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<66) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Minimal size for meaningful processing

    // 1. Create profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	

    // 2. Get profile info using cmsGetProfileInfo
    wchar_t profileInfoBuffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfo(hProfile, cmsInfoDescription, fuzz_str_1, fuzz_str_2, 
                                                 profileInfoBuffer, sizeof(profileInfoBuffer)/sizeof(wchar_t));

    // 3. Create and use MLU with UTF8 string
    cmsMLU* mlu = cmsMLUalloc(NULL, fuzz_uint32_t_3);
    if (mlu) {
        // Use a portion of input data as UTF8 string (simplified - real usage needs valid UTF8)
        size_t str_len = size > 256 ? 256 : size;
        char* temp_str = new char[str_len + 1];
        memcpy(temp_str, data, str_len);
        temp_str[str_len] = 0;
        cmsMLUsetUTF8(mlu, fuzz_str_4, fuzz_str_5, temp_str);
        delete[] temp_str;
    }

    // 4. Create IT8 handle and define double format
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (hIT8) {
        cmsIT8DefineDblFormat(hIT8, "%.6f");
    }

    // 5. Create tone curve and reverse it
    cmsToneCurve* toneCurve = cmsBuildGamma(NULL, 2.2);
    if (toneCurve) {
        cmsToneCurve* reversedCurve = cmsReverseToneCurveEx(fuzz_uint32_t_6, toneCurve);
        if (reversedCurve) {
            cmsFreeToneCurve(reversedCurve);
        }
    }

    // 6. Create CIECAM02 model and use reverse transform
    cmsViewingConditions vc;
    vc.whitePoint.X = 0.95047;
    vc.whitePoint.Y = 1.00000;
    vc.whitePoint.Z = 1.08883;
    vc.Yb = 20.0;
    vc.La = 100.0;
    vc.surround = 1;
    vc.D_value = 1.0;
    
    cmsHANDLE hModel = cmsCIECAM02Init(NULL, &vc);
    if (hModel) {
        cmsJCh jch;
        cmsCIEXYZ xyz;
        // Use some input data for JCh values
        if (size >= 24) {
            jch.J = (double)data[0] / 255.0 * 100.0;
            jch.C = (double)data[1] / 255.0 * 150.0;
            jch.h = (double)data[2] / 255.0 * 360.0;
            cmsCIECAM02Reverse(hModel, &jch, &xyz);
        }
    }

    // 7. Create named color list and get info
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(NULL, fuzz_uint32_t_9, 3, fuzz_str_7, fuzz_str_8);
    if (namedColorList) {
        char name[256];
        char prefix[256];
        char suffix[256];
        cmsUInt16Number pcs[3];
        cmsUInt16Number colorant[16];
        
        // Add a color using input data
        if (size >= 6) {
            cmsUInt16Number pcs_temp[3] = {
                (cmsUInt16Number)(data[0] << 8 | data[1]),
                (cmsUInt16Number)(data[2] << 8 | data[3]),
                (cmsUInt16Number)(data[4] << 8 | data[5])
            };
            cmsUInt16Number colorant_temp[3] = {0, 0, 0};
            cmsAppendNamedColor(namedColorList, fuzz_str_10, pcs_temp, colorant_temp);
        }
        
        // Get color info
        if (cmsNamedColorCount(namedColorList) > 0) {
            cmsNamedColorInfo(namedColorList, fuzz_uint32_t_11, name, prefix, suffix, pcs, colorant);
        }
    }

    // Cleanup
    if (toneCurve) cmsFreeToneCurve(toneCurve);
    if (hModel) cmsCIECAM02Done(hModel);
    if (hIT8) cmsIT8Free(hIT8);
    if (mlu) cmsMLUfree(mlu);
    if (namedColorList) cmsFreeNamedColorList(namedColorList);
    if (hProfile) cmsCloseProfile(hProfile);

    return 0;
}