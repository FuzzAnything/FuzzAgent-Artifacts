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
//<ID> 256
//<Prompt> []
/*<Combination>: [
*/
//<score> 160, nr_unique_branch: 31
//<Quality> {"density":20,"unique_branches":{"PackXYZDoubleFromFloat":[[731,31,731,44,1,0,4,1]],"cmsSliceSpaceFloat":[[918,9,918,34,0,0,4,1],[921,9,921,26,0,0,4,1],[923,17,923,39,0,0,4,0],[923,17,923,39,0,0,4,1],[926,35,926,40,0,0,4,0],[926,35,926,40,0,0,4,1],[935,13,935,38,0,0,4,1]],"cmsDetectRGBProfileGamma":[[618,9,618,52,0,0,4,0],[622,35,622,59,0,0,4,1],[628,9,628,21,0,0,4,1],[633,9,633,22,0,0,4,0],[633,9,633,22,0,0,4,1],[639,17,639,24,0,0,4,0],[639,17,639,24,0,0,4,1],[648,17,648,24,0,0,4,0],[648,17,648,24,0,0,4,1],[653,9,653,24,0,0,4,1]],"cmsEstimateGamma":[[1494,9,1494,15,0,0,4,0],[1499,9,1499,24,0,0,4,1]],"cmsCreateGrayProfileTHR":[[235,9,235,14,0,0,4,1],[256,9,256,45,0,0,4,1],[259,9,259,19,0,0,4,0],[262,13,262,71,0,0,4,1],[265,9,265,25,0,0,4,0],[267,13,267,75,0,0,4,1]],"ColorSpaceIsCompatible":[[502,33,502,53,0,0,4,1]],"cmsDictDup":[[1171,9,1171,21,0,0,4,1],[1175,12,1175,25,0,0,4,0],[1175,12,1175,25,0,0,4,1],[1177,13,1177,107,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsBuildGamma","cmsDeleteContext","cmsCreateRGBProfileTHR","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetASCII","cmsMLUsetASCII","cmsDictAddEntry","cmsDictDup","cmsMLUfree","cmsMLUfree","cmsDictFree","cmsDictFree","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsDetectRGBProfileGamma","cmsGetHeaderCreationDateTime","cmsSliceSpaceFloat","cmsGetToneCurveEstimatedTableEntries","cmsOpenProfileFromStream","cmsDetectRGBProfileGamma","cmsGetHeaderCreationDateTime","cmsCloseProfile","cmsSaveProfileToStream","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsBuildGamma","cmsCreateRGBProfileTHR","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetASCII","cmsMLUsetASCII","cmsDictAddEntry","cmsDictDup","cmsMLUfree","cmsMLUfree","cmsDictFree","cmsDictFree","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsDetectRGBProfileGamma","cmsGetHeaderCreationDateTime","cmsSliceSpaceFloat","cmsGetToneCurveEstimatedTableEntries","cmsOpenProfileFromStream","cmsDetectRGBProfileGamma","cmsGetHeaderCreationDateTime","cmsCloseProfile","cmsSaveProfileToStream","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<70) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegralArray(uint32_t, fuzz_uint32_t_array_9, fuzz_uint32_t_array_size_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
	
		const cmsUInt32Number * fuzzer_var_cmsSliceSpaceFloat_19_1 = fuzz_uint32_t_array_9;
		cmsUInt32Number fuzzer_size_cmsSliceSpaceFloat_19_1 = static_cast<cmsUInt32Number>(fuzz_uint32_t_array_size_9);//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create RGB profile with dummy data
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},
        {0.30, 0.60, 1.0},
        {0.15, 0.06, 1.0}
    };
    
    // Create tone curves for RGB profile
    cmsToneCurve* transferFunctions[3];
    cmsFloat64Number gamma = 2.2;
    for (int i = 0; i < 3; i++) {
        transferFunctions[i] = cmsBuildGamma(context, gamma);
        if (!transferFunctions[i]) {
            cmsDeleteContext(context);
            return 0;
        }
    }
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(context, &whitePoint, &primaries, 
                                                     (cmsToneCurve* const*)transferFunctions);
    
    // 2. Create dictionary and duplicate it
    cmsHANDLE dict = cmsDictAlloc(context);
    if (dict) {
        cmsMLU* displayName = cmsMLUalloc(context, fuzz_uint32_t_1);
        cmsMLU* displayValue = cmsMLUalloc(context, fuzz_uint32_t_2);
        if (displayName && displayValue) {
            cmsMLUsetASCII(displayName, fuzz_str_3, fuzz_str_4, fuzz_str_5);
            cmsMLUsetASCII(displayValue, fuzz_str_6, fuzz_str_7, fuzz_str_8);
            cmsDictAddEntry(dict, L"TestKey", L"TestValue", displayName, displayValue);
        }
        cmsHANDLE dupDict = cmsDictDup(dict);
        if (displayName) cmsMLUfree(displayName);
        if (displayValue) cmsMLUfree(displayValue);
        cmsDictFree(dict);
        if (dupDict) cmsDictFree(dupDict);
    }
    
    // 3. Create gray profile
    cmsToneCurve* grayTransfer = cmsBuildGamma(context, 1.8);
    cmsHPROFILE grayProfile = cmsCreateGrayProfileTHR(context, &whitePoint, grayTransfer);
    
    // 4. Detect RGB profile gamma
    if (rgbProfile) {
        cmsFloat64Number detectedGamma = cmsDetectRGBProfileGamma(rgbProfile, 0.01);
        (void)detectedGamma; // Use to avoid unused warning
    }
    
    // 5. Get header creation datetime
    if (rgbProfile) {
        struct tm creationDate;
        cmsGetHeaderCreationDateTime(rgbProfile, &creationDate);
    }
    
    // 6. Use cmsSliceSpaceFloat with input data
    if (size >= 12) {
        cmsUInt32Number clutPoints[3] = {2, 2, 2}; // Simple 2x2x2 CLUT
        
        // Simple sampler that does nothing
        auto sampler = [](const cmsFloat32Number* in, cmsFloat32Number* out, void* cargo) -> cmsBool {
            (void)in; (void)out; (void)cargo;
            return TRUE;
        };
        
        // Use part of input data as cargo
        void* cargo = (void*)(data + (size % (size - 8)));
        cmsSliceSpaceFloat(fuzzer_size_cmsSliceSpaceFloat_19_1, fuzzer_var_cmsSliceSpaceFloat_19_1, sampler, cargo);
    }
    
    // 7. Get tone curve estimated table entries
    if (transferFunctions[0]) {
        cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(transferFunctions[0]);
        (void)entries; // Use to avoid unused warning
    }
    
    // Try to consume input data by creating profiles from it
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHPROFILE profileFromData = cmsOpenProfileFromStream(in_file, fuzz_str_10);
        if (profileFromData) {
            // Try to detect gamma from this profile too
            cmsFloat64Number gamma2 = cmsDetectRGBProfileGamma(profileFromData, 0.01);
            (void)gamma2;
            
            // Get creation date
            struct tm date2;
            cmsGetHeaderCreationDateTime(profileFromData, &date2);
            
            cmsCloseProfile(profileFromData);
        }
        assert_file_closed(&in_file);;
    }
    
    // Write a test profile to file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file && rgbProfile) {
        cmsSaveProfileToStream(rgbProfile, out_file);
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (transferFunctions[i]) cmsFreeToneCurve(transferFunctions[i]);
    }
    if (grayTransfer) cmsFreeToneCurve(grayTransfer);
    if (rgbProfile) cmsCloseProfile(rgbProfile);
    if (grayProfile) cmsCloseProfile(grayProfile);
    cmsDeleteContext(context);
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}