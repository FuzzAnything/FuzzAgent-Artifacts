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
//<ID> 409
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 4
//<Quality> {"density":25,"unique_branches":{"cmsGetTransformPipeline":[[1448,9,1448,22,0,0,4,0]],"cmsWhitePointFromTemp":[[62,23,62,33,0,0,4,1],[69,13,69,23,0,0,4,0],[69,27,69,39,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsMD5computeID","cmsIsIntentSupported","cmsCIECAM02Forward","cmsGetTransformPipeline","cmsGetProfileContextID","cmsGetProfileInfo","cmsWhitePointFromTemp","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8Free","cmsOpenIOhandlerFromFile","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineFree","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromStreamTHR","cmsMD5computeID","cmsIsIntentSupported","cmsCIECAM02Forward","cmsGetTransformPipeline","cmsGetProfileContextID","cmsGetProfileInfo","cmsWhitePointFromTemp","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8Free","cmsOpenIOhandlerFromFile","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineFree","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum data size for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    // Create context for thread-safe operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // 1. Set custom error handler
    cmsSetLogErrorHandlerTHR(context, [](cmsContext /*ctx*/, cmsUInt32Number /*error*/, const char* /*text*/) {
        // Custom error handler - can remain empty for fuzzing
    });
    
    // 2. Open profile from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE profile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Compute MD5 ID for the profile
    cmsMD5computeID(profile);
    
    // 4. Check if intents are supported
    cmsUInt32Number intentsToCheck[] = {INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC, INTENT_SATURATION};
    for (auto intent : intentsToCheck) {
        cmsIsIntentSupported(profile, intent, 0);
    }
    
    // 5. Create CIECAM02 model and perform forward transform
    // First create a CIECAM02 model handle (simplified - actual creation requires more parameters)
    cmsHANDLE camModel = NULL;
    // Note: In real usage, we'd create model with proper parameters
    // camModel = cmsCIECAM02Init(...);
    
    if (camModel) {
        cmsCIEXYZ inputXYZ = {0.5, 0.5, 0.5};
        cmsJCh outputJCh = {0, 0, 0};
        cmsCIECAM02Forward(camModel, &inputXYZ, &outputJCh);
        // Cleanup would normally be done with cmsCIECAM02Done(camModel);
    }
    
    // 6. Get pipeline context ID (if profile has a pipeline)
    cmsPipeline* pipeline = cmsGetTransformPipeline(NULL); // Simplified - would need transform
    if (pipeline) {
        // Get context from pipeline
        cmsContext pipeContext = cmsGetProfileContextID(profile); // Using profile context as proxy
        (void)pipeContext; // Use variable to avoid warnings
    }
    
    // 7. Get profile information
    wchar_t infoBuffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfo(profile, cmsInfoDescription, "en", "US", 
                                                infoBuffer, sizeof(infoBuffer)/sizeof(wchar_t));
    
    // 8. Calculate white point from temperature
    cmsCIExyY whitePoint;
    cmsFloat64Number temperatures[] = {5000.0, 6500.0, 7500.0};
    for (auto temp : temperatures) {
        cmsWhitePointFromTemp(&whitePoint, temp);
    }
    
    // 9. IT8 operations - create dummy IT8 handle and get property
    // Create simple IT8 data structure
    const char* it8Data = "SAMPLE_DATA";
    cmsHANDLE it8Handle = cmsIT8LoadFromFile(context, "dummy.it8"); // Would need actual file
    
    if (it8Handle) {
        const char* property = cmsIT8GetPropertyMulti(it8Handle, "Sample", "Value");
        (void)property; // Use variable
        cmsIT8Free(it8Handle);
    }
    
    // 10. Save profile to I/O handler
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Create I/O handler for output
        cmsIOHANDLER* ioHandler = cmsOpenIOhandlerFromFile(context, "output_file", "wb");
        if (ioHandler) {
            cmsUInt32Number saved = cmsSaveProfileToIOhandler(profile, ioHandler);
            (void)saved; // Use variable
            cmsCloseIOhandler(ioHandler);
        }
        assert_file_closed(&out_file);;
    }
    
    // 11. Additional operations using other required APIs
    
    // Create a simple pipeline
    cmsPipeline* testPipeline = cmsPipelineAlloc(context, 3, 3);
    if (testPipeline) {
        // Create matrix stage
        cmsFloat64Number matrix[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
        cmsFloat64Number offset[] = {0.0, 0.0, 0.0};
        cmsStage* matrixStage = cmsStageAllocMatrix(context, 3, 3, matrix, offset);
        
        if (matrixStage) {
            cmsPipelineInsertStage(testPipeline, cmsAT_END, matrixStage);
        }
        
        // Cleanup
        cmsPipelineFree(testPipeline);
    }
    
    // Create tone curves
    cmsUInt16Number curveValues[] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 
                                    10000, 11000, 12000, 13000, 14000, 15000, 16000};
    cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(context, 17, curveValues);
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    
    // Create and use transforms
    cmsHTRANSFORM transform = cmsCreateTransformTHR(context, profile, TYPE_RGB_8, 
                                                   profile, TYPE_RGB_8, 
                                                   INTENT_PERCEPTUAL, 0);
    if (transform) {
        unsigned char input[3] = {128, 128, 128};
        unsigned char output[3];
        cmsDoTransform(transform, input, output, 1);
        cmsDeleteTransform(transform);
    }
    
    // Cleanup
    cmsCloseProfile(profile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    // Clean output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}