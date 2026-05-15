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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 40
//<Quality> {"density":5,"unique_branches":{"encodeUTF8":[[277,34,277,37,0,0,4,1],[279,13,279,26,0,0,4,0],[279,30,279,43,0,0,4,1],[283,17,283,30,0,0,4,0],[283,34,283,47,0,0,4,1],[288,17,288,34,0,0,4,1],[290,28,290,50,0,0,4,1],[294,22,294,40,0,0,4,0],[294,22,294,40,0,0,4,1],[296,21,296,24,0,0,4,0],[296,21,296,24,0,0,4,1],[296,28,296,43,0,0,4,0],[296,47,296,69,0,0,4,0],[303,22,303,41,0,0,4,0],[303,22,303,41,0,0,4,1],[305,21,305,24,0,0,4,0],[305,21,305,24,0,0,4,1],[305,28,305,43,0,0,4,0],[305,47,305,69,0,0,4,0],[315,21,315,24,0,0,4,0],[315,21,315,24,0,0,4,1],[315,28,315,43,0,0,4,0],[315,47,315,69,0,0,4,0],[315,47,315,69,0,0,4,1]],"cmsMLUgetUTF8":[[606,9,606,21,0,0,4,0],[617,9,617,33,0,0,4,0]],"cmsReadRawTag":[[2020,9,2020,21,0,0,4,0],[2020,9,2020,21,0,0,4,1],[2020,25,2020,40,0,0,4,1],[2022,9,2022,55,0,0,4,1],[2027,9,2027,14,0,0,4,0],[2027,9,2027,14,0,0,4,1],[2030,9,2030,34,0,0,4,0],[2038,13,2038,25,0,0,4,0],[2038,13,2038,25,0,0,4,1],[2040,17,2040,37,0,0,4,1],[2043,17,2043,65,0,0,4,1],[2044,17,2044,75,0,0,4,1]],"cmsGetProfileInfoUTF8":[[1042,9,1042,20,0,0,4,0],[1042,9,1042,20,0,0,4,1]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileInfoUTF8","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineFree","cmsPluginTHR","cmsFloat2XYZEncoded","cmsReadRawTag","cmsReadRawTag","cmsAllocNamedColorList","cmsDupNamedColorList","cmsFreeNamedColorList","cmsFreeNamedColorList","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileInfoUTF8","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineFree","cmsPluginTHR","cmsFloat2XYZEncoded","cmsReadRawTag","cmsReadRawTag","cmsAllocNamedColorList","cmsDupNamedColorList","cmsFreeNamedColorList","cmsFreeNamedColorList","cmsCloseProfile"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful processing
    
    // Use the input data as an ICC profile in memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	
    
    // Get profile info in UTF8
    char buffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfoUTF8(hProfile, cmsInfoDescription, "en", "US", 
                                                     buffer, sizeof(buffer));
    
    // Create a simple pipeline for evaluation
    cmsPipeline* pipeline = cmsPipelineAlloc(0, 3, 3);
    if (pipeline) {
        // Create identity matrix
        cmsFloat64Number identity[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
        cmsStage* matrix = cmsStageAllocMatrix(0, 3, 3, identity, NULL);
        if (matrix) {
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, matrix);
            
            // Evaluate through pipeline
            cmsUInt16Number in[3] = {0x8000, 0x8000, 0x8000};  // Mid values
            cmsUInt16Number out[3];
            cmsPipelineEval16(in, out, pipeline);
        }
        cmsPipelineFree(pipeline);
    }
    
    // Try to register a plugin (using NULL as minimal attempt)
    cmsPluginTHR(0, NULL);
    
    // Convert float XYZ to encoded
    cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
    cmsUInt16Number encoded[3];
    cmsFloat2XYZEncoded(encoded, &xyz);
    
    // Attempt to duplicate named color list if profile has one
    // First check if profile has named color tag
    cmsUInt32Number tagSize = cmsReadRawTag(hProfile, cmsSigNamedColor2Tag, NULL, 0);
    if (tagSize > 0) {
        void* tagData = malloc(tagSize);
        if (tagData) {
            cmsReadRawTag(hProfile, cmsSigNamedColor2Tag, tagData, tagSize);
            
            // Create a named color list from the tag data
            // Note: This is simplified - actual parsing would be more complex
            cmsNAMEDCOLORLIST* list = cmsAllocNamedColorList(0, 1, 3, "P", "S");
            if (list) {
                cmsNAMEDCOLORLIST* dupList = cmsDupNamedColorList(list);
                if (dupList) {
                    cmsFreeNamedColorList(dupList);
                }
                cmsFreeNamedColorList(list);
            }
            free(tagData);
        }
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}