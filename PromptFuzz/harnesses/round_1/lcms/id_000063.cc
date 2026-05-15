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
//<ID> 425
//<Prompt> []
/*<Combination>: [
*/
//<score> 141.33333, nr_unique_branch: 9
//<Quality> {"density":53,"unique_branches":{"DefaultEvalParametricFn":[[352,13,352,18,0,0,4,0],[354,17,354,61,0,0,4,1]],"cmsStageSampleCLut16bit":[[796,13,796,37,0,0,4,0]],"_cmsReadOutputLUT":[[623,17,623,53,0,0,4,1]],"_cmsICCcolorSpace":[[790,8,790,20,0,0,4,0]],"AddConversion":[[460,17,460,38,0,0,4,0],[461,17,461,128,0,0,4,1]],"EmitHeader":[[347,9,347,28,0,0,4,1]],"OutputValueSampler":[[580,21,580,86,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCreateXYZProfileTHR","cmsDeleteContext","cmsCreate_OkLabProfile","cmsCloseProfile","cmsDeleteContext","cmsBuildParametricToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDupToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsPipelineAlloc","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsStageAllocIdentity","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsStageType","cmsPipelineInsertStage","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateTransform","cmsCloseProfile","cmsGetPostScriptCRD","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsDeleteTransform","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateXYZProfileTHR","cmsCreate_OkLabProfile","cmsBuildParametricToneCurve","cmsDupToneCurve","cmsPipelineAlloc","cmsStageAllocIdentity","cmsStageType","cmsPipelineInsertStage","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateTransform","cmsCloseProfile","cmsGetPostScriptCRD","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsDeleteTransform","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Create a context for lcms operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	

    // 1. Create XYZ profile
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(context);
    if (!xyzProfile) {
        cmsDeleteContext(context);
        return 0;
    }

    // 2. Create OkLab profile
    cmsHPROFILE oklabProfile = cmsCreate_OkLabProfile(context);
    if (!oklabProfile) {
        cmsCloseProfile(xyzProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // 3. Build a gamma tone curve (using 2.2 as default gamma)
    cmsFloat64Number gamma = 2.2;
    cmsToneCurve* gammaCurve = cmsBuildParametricToneCurve(context, 1, &gamma); // Type 1 = gamma function
    if (!gammaCurve) {
        cmsCloseProfile(oklabProfile);
        cmsCloseProfile(xyzProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // 4. Duplicate the gamma tone curve
    cmsToneCurve* dupGammaCurve = cmsDupToneCurve(gammaCurve);
    if (!dupGammaCurve) {
        cmsFreeToneCurve(gammaCurve);
        cmsCloseProfile(oklabProfile);
        cmsCloseProfile(xyzProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // 5. Create a pipeline (LUT) with 3 input and 3 output channels
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (!pipeline) {
        cmsFreeToneCurve(dupGammaCurve);
        cmsFreeToneCurve(gammaCurve);
        cmsCloseProfile(oklabProfile);
        cmsCloseProfile(xyzProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // 6. Create an identity stage
    cmsStage* identityStage = cmsStageAllocIdentity(context, 3);
    if (!identityStage) {
        cmsPipelineFree(pipeline);
        cmsFreeToneCurve(dupGammaCurve);
        cmsFreeToneCurve(gammaCurve);
        cmsCloseProfile(oklabProfile);
        cmsCloseProfile(xyzProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // 7. Get stage type
    cmsStageSignature stageType = cmsStageType(identityStage);

    // 8. Insert the identity stage into the pipeline
    cmsBool insertResult = cmsPipelineInsertStage(pipeline, cmsAT_END, identityStage);
    if (!insertResult) {
        // Note: identityStage is now owned by pipeline, don't free separately
        cmsPipelineFree(pipeline);
        cmsFreeToneCurve(dupGammaCurve);
        cmsFreeToneCurve(gammaCurve);
        cmsCloseProfile(oklabProfile);
        cmsCloseProfile(xyzProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // 9. Create a device link from cube file using input data
    // First, write input data to a temporary file
    FILE* cubeFile = fmemopen((void*)data, size, "rb");
    if (!cubeFile) {
        cmsPipelineFree(pipeline);
        cmsFreeToneCurve(dupGammaCurve);
        cmsFreeToneCurve(gammaCurve);
        cmsCloseProfile(oklabProfile);
        cmsCloseProfile(xyzProfile);
        cmsDeleteContext(context);
        assert_file_closed(&cubeFile);
	return 0;
    }
    
    // Write to a temporary file for cmsCreateDeviceLinkFromCubeFileTHR
    FILE* tempCubeFile = fopen("temp_cube.cube", "wb");
    if (tempCubeFile) {
        uint8_t buffer[4096];
        size_t bytesRead;
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), cubeFile)) > 0) {
            fwrite(buffer, 1, bytesRead, tempCubeFile);
        }
        assert_file_closed(&tempCubeFile);;
    }
    assert_file_closed(&cubeFile);;
    
    // Create device link from cube file
    cmsHPROFILE deviceLink = cmsCreateDeviceLinkFromCubeFileTHR(context, "temp_cube.cube");
    
    // Clean up temporary cube file
    remove("temp_cube.cube");

    // 10. Create a color transform
    cmsHTRANSFORM transform = NULL;
    if (deviceLink) {
        transform = cmsCreateTransform(
            deviceLink,                      // Input profile
            TYPE_RGB_8,                      // Input format
            oklabProfile,                    // Output profile  
            TYPE_Lab_8,                      // Output format
            INTENT_PERCEPTUAL,               // Rendering intent
            cmsFLAGS_NOCACHE                 // Flags
        );
        cmsCloseProfile(deviceLink);
    }

    // 11. Get PostScript CRD
    if (oklabProfile) {
        char psBuffer[4096];
        cmsUInt32Number psSize = cmsGetPostScriptCRD(
            context,
            oklabProfile,
            INTENT_PERCEPTUAL,
            0,
            psBuffer,
            sizeof(psBuffer)
        );
        // psSize contains the actual size needed/used
    }

    // 12. Load IT8 file from input data (using second half of data)
    // Create another temporary file for IT8 data
    size_t halfSize = size / 2;
    if (halfSize > 0) {
        FILE* it8File = fopen("temp_it8.it8", "wb");
        if (it8File) {
            fwrite(data + halfSize, 1, size - halfSize, it8File);
            assert_file_closed(&it8File);;
            
            // Load IT8 file
            cmsHANDLE it8 = cmsIT8LoadFromFile(context, "temp_it8.it8");
            if (it8) {
                // Set index column
                cmsIT8SetIndexColumn(it8, "Sample");
                
                // Get table count
                cmsUInt32Number tableCount = cmsIT8TableCount(it8);
                
                // Free IT8 handle
                cmsIT8Free(it8);
            }
            
            // Clean up temporary IT8 file
            remove("temp_it8.it8");
        }
    }

    // 13. Clean up resources
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    cmsPipelineFree(pipeline);          // This also frees the identityStage
    cmsFreeToneCurve(dupGammaCurve);
    cmsFreeToneCurve(gammaCurve);
    cmsCloseProfile(oklabProfile);
    cmsCloseProfile(xyzProfile);
    cmsDeleteContext(context);

    assert_file_closed(&tempCubeFile);
	assert_file_closed(&cubeFile);
	return 0;
}