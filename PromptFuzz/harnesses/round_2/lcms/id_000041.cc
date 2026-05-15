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
//<ID> 313
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsGetTagCount","cmsAllocNamedColorList","cmsAppendNamedColor","cmsAppendNamedColor","cmsNamedColorCount","cmsFreeNamedColorList","cmsBuildGamma","cmsBuildGamma","cmsBuildGamma","cmsFreeToneCurveTriple","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsStageType","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsPipelineFree","_cmsICCcolorSpace","_cmsICCcolorSpace","_cmsICCcolorSpace","cmsPluginTHR","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsGetTagCount","cmsAllocNamedColorList","cmsAppendNamedColor","cmsAppendNamedColor","cmsNamedColorCount","cmsFreeNamedColorList","cmsBuildGamma","cmsBuildGamma","cmsBuildGamma","cmsFreeToneCurveTriple","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsStageType","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsPipelineFree","_cmsICCcolorSpace","_cmsICCcolorSpace","_cmsICCcolorSpace","cmsPluginTHR","cmsCloseProfile"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful operations
    
    // Open profile from memory input
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	
    
    // Create context from profile
    cmsContext contextID = cmsGetProfileContextID(hProfile);
    
    // Use cmsGetTagCount
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    
    // Create and manipulate named color list
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(contextID, 10, 3, "Prefix", "Suffix");
    if (namedColorList) {
        cmsUInt16Number PCS[3] = {32768, 32768, 32768}; // Mid-gray in Lab
        cmsUInt16Number Colorant[16] = {0};
        
        // Populate colorant array with some data from input
        for (int i = 0; i < 16 && i < (int)size; i++) {
            Colorant[i] = (cmsUInt16Number)(data[i] * 257); // Scale to 16-bit
        }
        
        // Use cmsAppendNamedColor
        cmsAppendNamedColor(namedColorList, "TestColor1", PCS, Colorant);
        cmsAppendNamedColor(namedColorList, "TestColor2", PCS, Colorant);
        
        // Use cmsNamedColorCount
        cmsUInt32Number colorCount = cmsNamedColorCount(namedColorList);
        
        cmsFreeNamedColorList(namedColorList);
    }
    
    // Create tone curves for testing cmsFreeToneCurveTriple
    cmsToneCurve* curves[3] = {nullptr, nullptr, nullptr};
    
    // Create some basic tone curves using data from input
    cmsFloat64Number gamma1 = 1.0 + (data[0] / 255.0) * 3.0; // Gamma between 1.0-4.0
    cmsFloat64Number gamma2 = 1.0 + (data[1] / 255.0) * 3.0;
    cmsFloat64Number gamma3 = 1.0 + (data[2] / 255.0) * 3.0;
    
    curves[0] = cmsBuildGamma(contextID, gamma1);
    curves[1] = cmsBuildGamma(contextID, gamma2);
    curves[2] = cmsBuildGamma(contextID, gamma3);
    
    // Use cmsFreeToneCurveTriple - note: this frees all three curves
    if (curves[0] && curves[1] && curves[2]) {
        cmsFreeToneCurveTriple(curves);
        // After this call, curves pointers become invalid
    }
    
    // Create a simple pipeline and get stage type
    cmsPipeline* pipeline = cmsPipelineAlloc(contextID, 3, 3);
    if (pipeline) {
        // Add a matrix stage
        cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0, 
                                       0.0, 1.0, 0.0, 
                                       0.0, 0.0, 1.0};
        cmsStage* matrixStage = cmsStageAllocMatrix(contextID, 3, 3, matrix, nullptr);
        
        if (matrixStage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, matrixStage);
            
            // Use cmsStageType
            cmsStageSignature stageSig = cmsStageType(matrixStage);
            
            // Get stage from pipeline
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (firstStage) {
                cmsStageSignature firstStageSig = cmsStageType(firstStage);
            }
        }
        
        cmsPipelineFree(pipeline);
    }
    
    // Use _cmsICCcolorSpace with different notations
    // This is an internal function but we'll use it as requested
    cmsColorSpaceSignature cs1 = _cmsICCcolorSpace(1); // RGB
    cmsColorSpaceSignature cs2 = _cmsICCcolorSpace(2); // Gray
    cmsColorSpaceSignature cs3 = _cmsICCcolorSpace(3); // CMYK
    
    // Try to use cmsPluginTHR with null plugin (will fail but we call it)
    cmsBool pluginResult = cmsPluginTHR(contextID, nullptr);
    
    // Clean up profile
    cmsCloseProfile(hProfile);
    
    return 0;
}