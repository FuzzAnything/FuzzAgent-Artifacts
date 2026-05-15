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
//<ID> 230
//<Prompt> []
/*<Combination>: [
*/
//<score> 8.333333, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsOpenProfileFromStreamTHR","cmsCreateNULLProfileTHR","cmsDetectDestinationBlackPoint","cmsPipelineAlloc","cmsStageAllocMatrix","cmsStageFree","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsGetTagSignature","cmsMLUdup","cmsDictNextEntry","cmsUnregisterPlugins","cmsMLUgetWide","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsGetTransformContextID","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsMLUfree","cmsMLUfree","cmsCloseProfile","cmsPipelineFree","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsOpenProfileFromStreamTHR","cmsCreateNULLProfileTHR","cmsDetectDestinationBlackPoint","cmsPipelineAlloc","cmsStageAllocMatrix","cmsStageFree","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsGetTagSignature","cmsMLUdup","cmsDictNextEntry","cmsUnregisterPlugins","cmsMLUgetWide","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsGetTransformContextID","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsMLUfree","cmsMLUfree","cmsCloseProfile","cmsPipelineFree","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 128) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Write input data to a temporary file for profile loading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open profile from the file
    cmsHPROFILE profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!profile) {
        // Try to create a profile from the file stream
        rewind(in_file);
        profile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    }
    
    // If still no profile, create a default one
    if (!profile) {
        profile = cmsCreateNULLProfileTHR(context);
    }
    
    // Detect destination black point
    cmsCIEXYZ blackPoint;
    cmsDetectDestinationBlackPoint(&blackPoint, profile, 0, 0);
    
    // Allocate a pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // Create and add a matrix stage to pipeline
    cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0,
                                  0.0, 1.0, 0.0,
                                  0.0, 0.0, 1.0};
    cmsFloat64Number offset[3] = {0.0, 0.0, 0.0};
    
    cmsStage *matrixStage = cmsStageAllocMatrix(context, 3, 3, matrix, offset);
    if (matrixStage && pipeline) {
        // Add stage to pipeline (simulate - actual API would be cmsPipelineInsertStage)
        // Free it separately since we can't add without proper API
        cmsStageFree(matrixStage);
    }
    
    // Set header properties
    cmsSetHeaderRenderingIntent(profile, 0);
    cmsSetEncodedICCversion(profile, 0x02100000);
    
    // Allocate MLU structure
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    
    // Get first tag signature
    cmsTagSignature firstTag = cmsGetTagSignature(profile, 0);
    
    // Duplicate MLU
    cmsMLU *mluDup = cmsMLUdup(mlu);
    
    // Try to iterate dictionary entries if profile has dictionary
    const cmsDICTentry *dictEntry = cmsDictNextEntry(nullptr);
    // Note: This will return nullptr since we start with nullptr
    
    // Unregister plugins
    cmsUnregisterPlugins();
    
    // Get MLU wide string
    wchar_t wideBuffer[256];
    cmsUInt32Number wideSize = cmsMLUgetWide(mlu, "en", "US", wideBuffer, 256);
    
    // Create a simple transform to get transform context ID
    cmsHTRANSFORM transform = nullptr;
    cmsHPROFILE inputProfile = cmsCreateLab4Profile(nullptr);
    cmsHPROFILE outputProfile = cmsCreateXYZProfileTHR(context);
    
    if (inputProfile && outputProfile) {
        transform = cmsCreateTransformTHR(context, inputProfile, 
                                          TYPE_Lab_DBL, 
                                          outputProfile, 
                                          TYPE_XYZ_DBL, 
                                          0, 0);
    }
    
    // Get transform context ID if transform exists
    if (transform) {
        cmsContext transformContext = cmsGetTransformContextID(transform);
        // Clean up transform
        cmsDeleteTransform(transform);
    }
    
    // Clean up profiles
    if (inputProfile) cmsCloseProfile(inputProfile);
    if (outputProfile) cmsCloseProfile(outputProfile);
    
    // Free MLU structures
    if (mlu) cmsMLUfree(mlu);
    if (mluDup) cmsMLUfree(mluDup);
    
    // Close main profile
    if (profile) cmsCloseProfile(profile);
    
    // Free pipeline if allocated
    if (pipeline) cmsPipelineFree(pipeline);
    
    // Close file
    assert_file_closed(&in_file);;
    
    // Delete context
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}