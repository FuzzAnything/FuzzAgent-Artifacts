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
//<ID> 501
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 4
//<Quality> {"density":4,"unique_branches":{"_cmsRegisterMemHandlerPlugin":[[287,13,287,30,0,0,4,1]],"_cmsLockMutex":[[693,9,693,35,0,0,4,0]],"cmsSetLogErrorHandlerTHR":[[538,13,538,23,0,0,4,0]],"_cmsUnlockMutex":[[702,9,702,37,0,0,4,1]]},"library_calls":["cmsSetLogErrorHandlerTHR","cmsUnregisterPlugins","cmsPluginTHR","cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStream","cmsDeleteContext","cmsIsCLUT","cmsGetTagSignature","cmsBuildParametricToneCurve","cmsStageAllocToneCurves","cmsStageDup","cmsSaveProfileToFile","cmsStageFree","cmsStageFree","cmsFreeToneCurveTriple","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsSetLogErrorHandlerTHR","cmsUnregisterPlugins","cmsPluginTHR","cmsCreateContext","cmsOpenProfileFromStream","cmsIsCLUT","cmsGetTagSignature","cmsBuildParametricToneCurve","cmsStageAllocToneCurves","cmsStageDup","cmsSaveProfileToFile","cmsStageFree","cmsStageFree","cmsFreeToneCurveTriple","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to process ICC profile data:

*/


// lcms headers

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    cmsContext context = nullptr;
    cmsHPROFILE profile = nullptr;
    cmsStage *stage = nullptr;
    cmsStage *dupStage = nullptr;
    cmsToneCurve *curves[3] = {nullptr, nullptr, nullptr};
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // Set up error handler
    cmsSetLogErrorHandlerTHR(nullptr, nullptr);
    
    // Unregister plugins
    cmsUnregisterPlugins();
    
    // Try to register a dummy plugin (will likely fail but we call the API)
    cmsPluginTHR(nullptr, nullptr);
    
    // Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // Create input file from fuzzer data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Open profile from input stream
    profile = cmsOpenProfileFromStream(in_file, "r");
    if (!profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if profile has CLUT for perceptual intent and forward direction
    cmsBool hasCLUT = cmsIsCLUT(profile, INTENT_PERCEPTUAL, LCMS_USED_AS_INPUT);
    
    // Get first tag signature
    cmsTagSignature firstTag = cmsGetTagSignature(profile, 0);
    
    // Create some dummy tone curves for demonstration
    cmsFloat64Number params[5] = {1.0, 0.0, 0.0, 0.0, 0.0};
    for (int i = 0; i < 3; i++) {
        curves[i] = cmsBuildParametricToneCurve(context, 1, params);
    }
    
    // Create a dummy stage (identity) to duplicate
    if (curves[0] && curves[1] && curves[2]) {
        stage = cmsStageAllocToneCurves(context, 3, curves);
        if (stage) {
            // Duplicate the stage
            dupStage = cmsStageDup(stage);
        }
    }
    
    // Create output file
    out_file = fopen("output_file", "wb");
    if (out_file && profile) {
        // Save profile to file
        cmsSaveProfileToFile(profile, "output_file");
    }
    
    // Clean up
    if (dupStage) {
        cmsStageFree(dupStage);
    }
    if (stage) {
        cmsStageFree(stage);
    }
    
    // Free tone curve triple
    if (curves[0] || curves[1] || curves[2]) {
        cmsFreeToneCurveTriple(curves);
    }
    
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    // Clean up output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}