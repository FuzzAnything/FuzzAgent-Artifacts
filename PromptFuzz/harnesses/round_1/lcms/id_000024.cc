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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsOpenProfileFromMemTHR","cmsGetProfileInfoASCII","cmsBuildTabulatedToneCurve16","cmsCreateGrayProfile","cmsSetHeaderModel","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageAllocCLutFloat","cmsStageSampleCLutFloat","cmsStageFree","cmsPipelineEval16","cmsPipelineFree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsOpenProfileFromMemTHR","cmsGetProfileInfoASCII","cmsBuildTabulatedToneCurve16","cmsCreateGrayProfile","cmsSetHeaderModel","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageAllocCLutFloat","cmsStageSampleCLutFloat","cmsStageFree","cmsPipelineEval16","cmsPipelineFree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


// Custom error handler to be set by cmsSetLogErrorHandler
static void  id_000024_FuzzErrorHandler(cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, const char* /*Text*/) {
    // Empty handler to suppress error output during fuzzing
}

// Sampler function for cmsStageSampleCLutFloat
static int  id_000024_FloatSampler(const cmsFloat32Number* in, cmsFloat32Number* out, void* cargo) {
    // Simple passthrough sampler
    for (int i = 0; i < 3; i++) {
        out[i] = in[i];
    }
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Set error handler first
    cmsSetLogErrorHandler( id_000024_FuzzErrorHandler);
    
    // 2. Create a FILE* from input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    
    // 3. Try to open profile from memory (simulating lcms output data)
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(context, data, (cmsUInt32Number)size);
    
    if (profile) {
        // 4. Get profile info in ASCII
        char buffer[256];
        cmsGetProfileInfoASCII(profile, cmsInfoDescription, "en", "US", buffer, sizeof(buffer));
        
        // 5. Create a simple gray profile
        cmsCIExyY whitepoint = {0.3127, 0.3290, 1.0}; // D65
        
        // Create a simple gamma curve from input data
        cmsUInt16Number gammaTable[256];
        for (int i = 0; i < 256 && i < (int)size; i++) {
            gammaTable[i] = (cmsUInt16Number)((data[i] / 255.0) * 65535.0);
        }
        cmsToneCurve* transferFunc = cmsBuildTabulatedToneCurve16(context, 256, gammaTable);
        
        cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitepoint, transferFunc);
        
        if (grayProfile) {
            // 6. Set header model (using some value from input)
            cmsUInt32Number model = (size > 4) ? *((cmsUInt32Number*)data) : 1234;
            cmsSetHeaderModel(grayProfile, model);
            
            // 7. Create a simple pipeline for evaluation
            cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
            if (pipeline) {
                // Add an identity stage
                cmsStage* stage = cmsStageAllocIdentity(context, 3);
                cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
                
                // 8. Use cmsStageSampleCLutFloat
                // First create a CLUT stage
                cmsStage* clutStage = cmsStageAllocCLutFloat(context, 5, 3, 3, nullptr);
                if (clutStage) {
                    float cargo = 1.0f;
                    cmsStageSampleCLutFloat(clutStage,  id_000024_FloatSampler, &cargo, 0);
                    cmsStageFree(clutStage);
                }
                
                // 9. Evaluate using cmsPipelineEval16
                cmsUInt16Number input[3] = {0, 32768, 65535};
                cmsUInt16Number output[3];
                cmsPipelineEval16(input, output, pipeline);
                
                cmsPipelineFree(pipeline);
            }
            
            cmsCloseProfile(grayProfile);
        }
        
        if (transferFunc) {
            cmsFreeToneCurve(transferFunc);
        }
        
        cmsCloseProfile(profile);
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}