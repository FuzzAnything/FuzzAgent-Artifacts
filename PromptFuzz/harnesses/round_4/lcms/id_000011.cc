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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["cmsOpenProfileFromMem","cmsReadTag","cmsMLUtranslationsCount","cmsGetProfileContextID","cmsOpenIOhandlerFromNULL","cmsD50_xyY","cmsBuildParametricToneCurve","cmsCreateRGBProfile","cmsStageAllocIdentity","cmsGetStageContextID","cmsStageFree","cmsFreeToneCurve","cmsCloseIOhandler","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsReadTag","cmsMLUtranslationsCount","cmsGetProfileContextID","cmsOpenIOhandlerFromNULL","cmsD50_xyY","cmsBuildParametricToneCurve","cmsCreateRGBProfile","cmsStageAllocIdentity","cmsGetStageContextID","cmsStageFree","cmsFreeToneCurve","cmsCloseIOhandler","cmsCloseProfile","cmsCloseProfile"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs together to achieve an event involving reading a profile from memory, extracting information from it, creating a new RGB profile, and managing resources:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<47) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeDoublingArray(fuzz_double_array_1, fuzz_double_array_size_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_6_2 = fuzz_double_array_1;//fuzzer shim end}




    // Early return if insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 1. Read profile from memory using the input data
    cmsHPROFILE input_profile = cmsOpenProfileFromMem(data, size);
    if (!input_profile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Read a tag from the profile (using copyright tag as example)
    cmsTagSignature sig = cmsSigCopyrightTag;
    void *tag_data = cmsReadTag(input_profile, sig);
    
    // 3. Get MLU translations count if tag exists and is MLU type
    cmsUInt32Number translation_count = 0;
    if (tag_data) {
        cmsMLU *mlu = (cmsMLU*)tag_data;
        translation_count = cmsMLUtranslationsCount(mlu);
    }
    
    // 4. Create an I/O handler from memory for writing
    cmsContext context = cmsGetProfileContextID(input_profile);
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromNULL(context);
    
    // 5. Create a new RGB profile with default parameters
    // Using D50 white point and sRGB primaries
    const cmsCIExyY* d50_white = cmsD50_xyY();
    cmsCIExyYTRIPLE primaries;
    
    // sRGB primaries
    primaries.Red.x = 0.6400;
    primaries.Red.y = 0.3300;
    primaries.Red.Y = 1.0;
    
    primaries.Green.x = 0.3000;
    primaries.Green.y = 0.6000;
    primaries.Green.Y = 1.0;
    
    primaries.Blue.x = 0.1500;
    primaries.Blue.y = 0.0600;
    primaries.Blue.Y = 1.0;
    
    // Gamma 2.2 tone curves
    cmsToneCurve* transfer_function[3];
    cmsFloat64Number gamma_params[5] = {2.2, 2.2, 2.2, 2.2, 2.2};
    cmsToneCurve* gamma_curve = cmsBuildParametricToneCurve(context, 4, fuzzer_var_cmsBuildParametricToneCurve_6_2);
    
    transfer_function[0] = gamma_curve;
    transfer_function[1] = gamma_curve;
    transfer_function[2] = gamma_curve;
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfile(d50_white, &primaries, transfer_function);
    
    // 6. Get stage context ID (need a stage first)
    // Create a simple identity stage
    cmsStage* identity_stage = cmsStageAllocIdentity(context, fuzz_uint32_t_2);
    if (identity_stage) {
        cmsContext stage_context = cmsGetStageContextID(identity_stage);
        (void)stage_context; // Use to avoid unused variable warning
        cmsStageFree(identity_stage);
    }
    
    // 7. Clean up resources
    if (gamma_curve) cmsFreeToneCurve(gamma_curve);
    if (io_handler) cmsCloseIOhandler(io_handler);
    if (rgb_profile) cmsCloseProfile(rgb_profile);
    if (input_profile) cmsCloseProfile(input_profile);
    if (in_file) assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}