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
//<ID> 472
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 5
//<Quality> {"density":21,"unique_branches":{"ToSpherical":[[130,8,130,19,0,0,4,0]],"cmsIT8EnumProperties":[[2687,32,2687,41,0,0,4,1],[2693,9,2693,22,0,0,4,1]],"cmsGDBCheckPoint":[[398,9,398,20,0,0,4,1],[401,9,401,31,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsGetTagSignature","cmsBuildTabulatedToneCurveFloat","cmsReverseToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsGBDAlloc","cmsGDBCheckPoint","cmsGBDFree","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsStageInputChannels","cmsPipelineFree","cmsPipelineFree","cmsIT8Alloc","cmsIT8EnumProperties","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsGetTagSignature","cmsBuildTabulatedToneCurveFloat","cmsReverseToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsGBDAlloc","cmsGDBCheckPoint","cmsGBDFree","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsStageInputChannels","cmsPipelineFree","cmsIT8Alloc","cmsIT8EnumProperties","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Minimum size for meaningful processing

    // Use input data as ICC profile file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Open profile from file stream
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    
    // If profile opened successfully, use the APIs
    if (hProfile) {
        // 1. cmsGetTagSignature - get first tag signature
        cmsTagSignature firstTag = cmsGetTagSignature(hProfile, 0);
        
        // 2. cmsReverseToneCurve - create and reverse a tone curve
        cmsFloat32Number curveValues[5] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurveFloat(context, 5, curveValues);
        if (toneCurve) {
            cmsToneCurve* reversedCurve = cmsReverseToneCurve(toneCurve);
            if (reversedCurve) {
                cmsFreeToneCurve(reversedCurve);
            }
            cmsFreeToneCurve(toneCurve);
        }
        
        // 3. cmsGDBCheckPoint - create GBD and check point
        cmsHANDLE hGBD = cmsGBDAlloc(context);
        if (hGBD) {
            cmsCIELab labPoint = {50.0, 0.0, 0.0};  // Neutral gray point
            cmsBool inGamut = cmsGDBCheckPoint(hGBD, &labPoint);
            cmsGBDFree(hGBD);
        }
        
        // 4. cmsStageInputChannels - create stage and get input channels
        cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
        if (pipeline) {
            // Create a matrix stage
            cmsFloat64Number identityMatrix[9] = {1,0,0,0,1,0,0,0,1};
            cmsFloat64Number offset[3] = {0,0,0};
            cmsStage* matrixStage = cmsStageAllocMatrix(context, 3, 3, identityMatrix, offset);
            if (matrixStage) {
                // Add stage to pipeline
                cmsPipelineInsertStage(pipeline, cmsAT_END, matrixStage);
                
                // Get input channels
                cmsUInt32Number inputChannels = cmsStageInputChannels(matrixStage);
                
                cmsPipelineFree(pipeline);
            } else {
                cmsPipelineFree(pipeline);
            }
        }
        
        // 5. cmsIT8EnumProperties - create IT8 handle and enumerate properties
        cmsHANDLE hIT8 = cmsIT8Alloc(context);
        if (hIT8) {
            char** propertyNames = NULL;
            cmsUInt32Number numProperties = cmsIT8EnumProperties(hIT8, &propertyNames);
            
            // Note: propertyNames array needs to be freed by caller according to lcms docs
            if (propertyNames) {
                // Free each string and then the array
                for (cmsUInt32Number i = 0; i < numProperties; i++) {
                    free(propertyNames[i]);
                }
                free(propertyNames);
            }
            
            cmsIT8Free(hIT8);
        }
        
        cmsCloseProfile(hProfile);
    }
    
    // Clean up resources
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}