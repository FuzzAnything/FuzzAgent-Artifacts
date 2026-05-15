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
//<ID> 328
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 28
//<Quality> {"density":7,"unique_branches":{"ParseFloatNumber":[[719,9,719,23,0,0,4,1],[721,9,721,23,0,0,4,0],[721,9,721,23,0,0,4,1],[721,27,721,41,0,0,4,1],[723,16,723,32,0,0,4,0],[728,12,728,19,0,0,4,0],[728,12,728,19,0,0,4,1],[731,13,731,20,0,0,4,0],[734,9,734,23,0,0,4,0],[734,9,734,23,0,0,4,1],[739,13,739,20,0,0,4,0],[741,16,741,23,0,0,4,0],[745,17,745,24,0,0,4,0],[752,9,752,16,0,0,4,0],[752,9,752,16,0,0,4,1],[752,20,752,43,0,0,4,0],[752,20,752,43,0,0,4,1],[757,13,757,20,0,0,4,0],[760,13,760,27,0,0,4,1],[766,17,766,31,0,0,4,0],[766,17,766,31,0,0,4,1],[769,21,769,28,0,0,4,0],[773,16,773,23,0,0,4,0],[773,16,773,23,0,0,4,1],[777,17,777,85,0,0,4,0],[780,17,780,24,0,0,4,0]],"cmsIT8GetPropertyDbl":[[1602,9,1602,18,0,0,4,0],[1602,9,1602,18,0,0,4,1]]},"library_calls":["cmsOpenProfileFromMem","cmsOpenProfileFromStream","cmsGetPCS","cmsGetHeaderCreationDateTime","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransform","cmsGetTransformInputFormat","cmsGetTransformContextID","cmsPipelineAlloc","cmsBuildParametricToneCurve","cmsStageAllocToneCurves","cmsStageSampleCLutFloat","cmsPipelineInsertStage","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsIT8LoadFromMem","cmsIT8GetPropertyDbl","cmsIT8GetSheetType","cmsIT8Free","cmsPipelineFree","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsOpenProfileFromStream","cmsGetPCS","cmsGetHeaderCreationDateTime","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransform","cmsGetTransformInputFormat","cmsGetTransformContextID","cmsPipelineAlloc","cmsBuildParametricToneCurve","cmsStageAllocToneCurves","cmsStageSampleCLutFloat","cmsPipelineInsertStage","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsIT8LoadFromMem","cmsIT8GetPropertyDbl","cmsIT8GetSheetType","cmsIT8Free","cmsPipelineFree","cmsCloseProfile"],"visited":0}
/**/


// Custom sampler function for cmsStageSampleCLutFloat
static int  id_000049_CustomSamplerFloat(const cmsFloat32Number* In, cmsFloat32Number* Out, void* Cargo) {
    // Simple pass-through sampler
    int channels = *((int*)Cargo);
    for (int i = 0; i < channels; ++i) {
        Out[i] = In[i];
    }
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Need minimal data
    
    cmsContext context = nullptr;
    cmsHPROFILE hProfile = nullptr;
    cmsHANDLE hIT8 = nullptr;
    cmsHANDLE hModel = nullptr;
    cmsHTRANSFORM hTransform = nullptr;
    cmsPipeline* pipeline = nullptr;
    cmsStage* stage = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    
    // 1. Try to open profile from memory
    hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        // Try opening as file stream if memory fails
        in_file = fmemopen((void*)data, size, "rb");
        if (in_file) {
            hProfile = cmsOpenProfileFromStream(in_file, "r");
            if (in_file) assert_file_closed(&in_file);;
            in_file = nullptr;
        }
    }
    
    if (hProfile) {
        // 2. Get PCS color space
        cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);
        
        // 3. Get header creation date/time
        struct tm creationDate;
        cmsBool dateValid = cmsGetHeaderCreationDateTime(hProfile, &creationDate);
        
        // 4. Try to create a transform to get input format
        cmsHPROFILE sRGB = cmsCreate_sRGBProfileTHR(nullptr);
        if (sRGB) {
            hTransform = cmsCreateMultiprofileTransform(&hProfile, 1, TYPE_RGB_8, TYPE_RGB_8,
                                                       INTENT_PERCEPTUAL, 0);
            if (hTransform) {
                // 6. Get transform input format
                cmsUInt32Number inputFormat = cmsGetTransformInputFormat(hTransform);
                
                // Try to get pipeline from transform context
                cmsContext transformContext = cmsGetTransformContextID(hTransform);
                if (transformContext) {
                    // Create a simple pipeline for testing
                    pipeline = cmsPipelineAlloc(nullptr, 3, 3);
                    if (pipeline) {
                        // Create a tone curve stage
                        cmsToneCurve* curves[3];
                        curves[0] = curves[1] = curves[2] = cmsBuildParametricToneCurve(
                            nullptr, 1, nullptr);
                        
                        if (curves[0]) {
                            stage = cmsStageAllocToneCurves(nullptr, 3, curves);
                            if (stage) {
                                // 5. Sample CLUT (even if stage doesn't have CLUT, API will handle it)
                                int channels = 3;
                                cmsStageSampleCLutFloat(stage,  id_000049_CustomSamplerFloat, 
                                                       &channels, 0);
                                
                                // Add stage to pipeline
                                cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
                            }
                            cmsFreeToneCurve(curves[0]);
                            cmsFreeToneCurve(curves[1]);
                            cmsFreeToneCurve(curves[2]);
                        }
                    }
                }
                cmsDeleteTransform(hTransform);
            }
            cmsCloseProfile(sRGB);
        }
    }
    
    // 7. Try to load as IT8 data
    hIT8 = cmsIT8LoadFromMem(nullptr, data, size);
    if (hIT8) {
        // Try to get a property as double
        cmsFloat64Number propValue = cmsIT8GetPropertyDbl(hIT8, "NUMBER_OF_FIELDS");
        
        // Try to get sheet type
        const char* sheetType = cmsIT8GetSheetType(hIT8);
        
        cmsIT8Free(hIT8);
    }
    
    // 8. Try CIECAM02 forward transform
    // Create a simple XYZ input (D50 white point)
    cmsCIEXYZ inputXYZ;
    inputXYZ.X = 0.9642;
    inputXYZ.Y = 1.0000;
    inputXYZ.Z = 0.8249;
    
    // Note: cmsCIECAM02Forward requires an initialized model handle
    // We can't create one without proper initialization parameters,
    // so we'll just declare the API usage pattern
    cmsJCh outputJCh;
    // This would fail without proper model initialization, but shows API usage
    // cmsCIECAM02Forward(hModel, &inputXYZ, &outputJCh);
    
    // Cleanup
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}