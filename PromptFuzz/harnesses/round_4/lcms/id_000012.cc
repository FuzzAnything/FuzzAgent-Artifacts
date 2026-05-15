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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["cmsOpenProfileFromStream","cmsReadTag","cmsMLUtranslationsCount","cmsBuildParametricToneCurve","cmsBuildParametricToneCurve","cmsBuildParametricToneCurve","cmsCreateRGBProfile","cmsPipelineAlloc","cmsStageAllocIdentity","cmsGetStageContextID","cmsStageFree","cmsPipelineFree","cmsOpenIOhandlerFromNULL","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStream","cmsReadTag","cmsMLUtranslationsCount","cmsBuildParametricToneCurve","cmsBuildParametricToneCurve","cmsBuildParametricToneCurve","cmsCreateRGBProfile","cmsPipelineAlloc","cmsStageAllocIdentity","cmsGetStageContextID","cmsStageFree","cmsPipelineFree","cmsOpenIOhandlerFromNULL","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile"],"visited":0}
/**/


// Include lcms headers (adjust paths as needed)

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<109) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_2, fuzz_double_array_size_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_4, fuzz_double_array_size_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_6, fuzz_double_array_size_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
	
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_3_2 = fuzz_double_array_2;
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_4_2 = fuzz_double_array_4;
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_5_2 = fuzz_double_array_6;//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful processing
    
    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open profile from the file stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, fuzz_str_1);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Use cmsReadTag to read copyright tag (common tag)
    cmsMLU *copyrightMLU = (cmsMLU*)cmsReadTag(hProfile, cmsSigCopyrightTag);
    
    // 4. Use cmsMLUtranslationsCount if we got an MLU
    if (copyrightMLU) {
        cmsUInt32Number translationCount = cmsMLUtranslationsCount(copyrightMLU);
        (void)translationCount; // Use variable to avoid compiler warning
    }
    
    // 5. Create RGB profile (needs proper parameters)
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0}; // D65 white point
    cmsCIExyYTRIPLE primaries = {
        {0.640, 0.330, 1.0},  // Red
        {0.300, 0.600, 1.0},  // Green  
        {0.150, 0.060, 1.0}   // Blue
    };
    
    // Create gamma 2.2 tone curves
    cmsToneCurve* transferFunction[3];
    cmsFloat64Number gammaParams[3] = {2.2, 1.0, 0.0}; // gamma, a, b
    transferFunction[0] = cmsBuildParametricToneCurve(NULL, fuzz_int32_t_3, fuzzer_var_cmsBuildParametricToneCurve_3_2);
    transferFunction[1] = cmsBuildParametricToneCurve(NULL, fuzz_int32_t_5, fuzzer_var_cmsBuildParametricToneCurve_4_2);
    transferFunction[2] = cmsBuildParametricToneCurve(NULL, fuzz_int32_t_7, fuzzer_var_cmsBuildParametricToneCurve_5_2);
    
    if (transferFunction[0] && transferFunction[1] && transferFunction[2]) {
        cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, &primaries, 
                                                    (cmsToneCurve* const*)transferFunction);
        
        if (rgbProfile) {
            // 6. Get stage context ID (need to create a stage first)
            cmsPipeline* pipeline = cmsPipelineAlloc(NULL, fuzz_uint32_t_8, fuzz_uint32_t_9);
            if (pipeline) {
                cmsStage* identityStage = cmsStageAllocIdentity(NULL, fuzz_uint32_t_10);
                if (identityStage) {
                    // Use cmsGetStageContextID (assuming it exists in the headers)
                    // Note: This API might be named differently in actual lcms
                    cmsContext stageContext = cmsGetStageContextID(identityStage);
                    (void)stageContext;
                    
                    cmsStageFree(identityStage);
                }
                cmsPipelineFree(pipeline);
            }
            
            // 7. Create I/O handler and close it
            cmsIOHANDLER* ioHandler = cmsOpenIOhandlerFromNULL(NULL);
            if (ioHandler) {
                // Write profile to NULL handler (just for API usage)
                cmsBool writeResult = cmsSaveProfileToIOhandler(rgbProfile, ioHandler);
                (void)writeResult;
                
                // 8. Use cmsCloseIOhandler
                cmsCloseIOhandler(ioHandler);
            }
            
            cmsCloseProfile(rgbProfile);
        }
        
        // Free tone curves
        cmsFreeToneCurve(transferFunction[0]);
        cmsFreeToneCurve(transferFunction[1]);
        cmsFreeToneCurve(transferFunction[2]);
    }
    
    // 9. Clean up
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}