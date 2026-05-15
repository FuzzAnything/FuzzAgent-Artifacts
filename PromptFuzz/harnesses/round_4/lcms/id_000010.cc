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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["cmsOpenProfileFromStream","cmsReadTag","cmsMLUtranslationsCount","cmsCloseProfile","cmsBuildParametricToneCurve","cmsBuildParametricToneCurve","cmsBuildParametricToneCurve","cmsCreateRGBProfile","cmsStageAllocIdentity","cmsGetStageContextID","cmsStageFree","cmsOpenIOhandlerFromNULL","cmsCloseIOhandler","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStream","cmsReadTag","cmsMLUtranslationsCount","cmsBuildParametricToneCurve","cmsBuildParametricToneCurve","cmsBuildParametricToneCurve","cmsCreateRGBProfile","cmsStageAllocIdentity","cmsGetStageContextID","cmsStageFree","cmsOpenIOhandlerFromNULL","cmsCloseIOhandler","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<145) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_2, fuzz_double_array_size_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_4, fuzz_double_array_size_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_6, fuzz_double_array_size_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
	
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_4_2 = fuzz_double_array_2;
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_5_2 = fuzz_double_array_4;
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_6_2 = fuzz_double_array_6;//fuzzer shim end}




    // 1. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open profile from the input data (lcms output data)
    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, fuzz_str_1);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Read a tag from the profile
    cmsTagSignature sig = cmsSigCopyrightTag;
    void *tagData = cmsReadTag(hProfile, sig);
    
    // 4. Get MLU translations count if tag exists and is MLU type
    if (tagData) {
        cmsMLU *mlu = (cmsMLU *)tagData;
        cmsUInt32Number translationCount = cmsMLUtranslationsCount(mlu);
        // Use translationCount to avoid unused variable warning
        if (translationCount > 1000) {
            // This is just to use the variable, won't actually execute
            cmsCloseProfile(hProfile);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // 5. Create RGB profile using default parameters
    // Use D50 white point
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3457;
    whitePoint.y = 0.3585;
    whitePoint.Y = 1.0;
    
    // Use sRGB primaries
    cmsCIExyYTRIPLE primaries;
    primaries.Red.x = 0.6400;   primaries.Red.y = 0.3300;   primaries.Red.Y = 1.0;
    primaries.Green.x = 0.3000; primaries.Green.y = 0.6000; primaries.Green.Y = 1.0;
    primaries.Blue.x = 0.1500;  primaries.Blue.y = 0.0600;  primaries.Blue.Y = 1.0;
    
    // Create gamma 2.2 tone curves
    cmsToneCurve* transferFunction[3];
    cmsFloat64Number gammaParams[5] = {2.2, 1.0, 0.0, 0.0, 0.0};
    transferFunction[0] = cmsBuildParametricToneCurve(NULL, fuzz_int32_t_3, fuzzer_var_cmsBuildParametricToneCurve_4_2);
    transferFunction[1] = cmsBuildParametricToneCurve(NULL, fuzz_int32_t_5, fuzzer_var_cmsBuildParametricToneCurve_5_2);
    transferFunction[2] = cmsBuildParametricToneCurve(NULL, 1, fuzzer_var_cmsBuildParametricToneCurve_6_2);
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, &primaries, 
                                                 (cmsToneCurve* const*)transferFunction);
    
    // 6. Get stage context ID (create a simple pipeline stage first)
    cmsStage* stage = cmsStageAllocIdentity(NULL, fuzz_uint32_t_7);
    if (stage) {
        cmsContext stageContext = cmsGetStageContextID(stage);
        // Use stageContext to avoid unused variable warning
        if (stageContext) {
            // This is just to use the variable
        }
        cmsStageFree(stage);
    }
    
    // 7. Create and close IO handler
    cmsIOHANDLER* ioHandler = cmsOpenIOhandlerFromNULL(NULL);
    if (ioHandler) {
        cmsCloseIOhandler(ioHandler);
    }
    
    // 8. Clean up resources
    if (transferFunction[0]) cmsFreeToneCurve(transferFunction[0]);
    if (transferFunction[1]) cmsFreeToneCurve(transferFunction[1]);
    if (transferFunction[2]) cmsFreeToneCurve(transferFunction[2]);
    
    if (rgbProfile) cmsCloseProfile(rgbProfile);
    if (hProfile) cmsCloseProfile(hProfile);
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}