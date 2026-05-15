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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"cmsGetTransformOutputFormat":[[1440,9,1440,22,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsCreateTransformTHR","cmsGetTransformOutputFormat","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8LoadFromMem","cmsIT8GetSheetType","cmsIT8GetData","cmsIT8Free","cmsIT8Free","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsMLUtranslationsCount","cmsMLUfree","cmsBuildGamma","cmsGetToneCurveEstimatedTableEntries","cmsFreeToneCurve","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineEvalFloat","cmsPipelineFree","cmsAdaptToIlluminant","cmsOpenProfileFromStream","cmsGetColorSpace","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsCreateTransformTHR","cmsGetTransformOutputFormat","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8LoadFromMem","cmsIT8GetSheetType","cmsIT8GetData","cmsIT8Free","cmsIT8Free","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsMLUtranslationsCount","cmsMLUfree","cmsBuildGamma","cmsGetToneCurveEstimatedTableEntries","cmsFreeToneCurve","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineEvalFloat","cmsPipelineFree","cmsAdaptToIlluminant","cmsOpenProfileFromStream","cmsGetColorSpace","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


// Include lcms headers

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Create a NULL profile (identity transform)
    cmsHPROFILE nullProfile = cmsCreateNULLProfile();
    
    // Create ink limiting device link profile
    cmsHPROFILE inkLimitProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, cmsSigCmykData, 400.0);
    
    // Create transform using the NULL profile
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, 
        nullProfile, TYPE_CMYK_16,
        inkLimitProfile, TYPE_CMYK_16,
        INTENT_RELATIVE_COLORIMETRIC, 0);
    
    // Get transform output format
    cmsUInt32Number outputFormat = cmsGetTransformOutputFormat(transform);
    
    // Create IT8 handler and use with input data
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (it8) {
        // Set IT8 property
        cmsIT8SetPropertyStr(it8, "DESCRIPTOR", "FuzzTest");
        
        // Try to parse input data as IT8
        cmsHANDLE it8FromMem = cmsIT8LoadFromMem(ctx, data, (cmsUInt32Number)size);
        if (it8FromMem) {
            // Get data from IT8 if available
            const char* sheetType = cmsIT8GetSheetType(it8FromMem);
            
            // Try to get sample data (use safe indices)
            const char* sampleData = cmsIT8GetData(it8FromMem, "0", "R");
            
            cmsIT8Free(it8FromMem);
        }
        cmsIT8Free(it8);
    }
    
    // Create MLU for translations
    cmsMLU* mlu = cmsMLUalloc(ctx, 10);
    if (mlu) {
        // Set some translations
        cmsMLUsetWide(mlu, "en", "US", L"Test");
        cmsMLUsetWide(mlu, "fr", "FR", L"Test");
        
        // Get translations count
        cmsUInt32Number translationCount = cmsMLUtranslationsCount(mlu);
        
        cmsMLUfree(mlu);
    }
    
    // Create tone curve and get estimated table entries
    cmsFloat64Number gammaVal = 2.2;
    cmsToneCurve* toneCurve = cmsBuildGamma(ctx, gammaVal);
    if (toneCurve) {
        cmsUInt32Number estimatedEntries = cmsGetToneCurveEstimatedTableEntries(toneCurve);
        cmsFreeToneCurve(toneCurve);
    }
    
    // Create pipeline for evaluation
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Add identity matrix stage
        cmsStage* matrixStage = cmsStageAllocMatrix(ctx, 3, 3, 
            (const cmsFloat64Number[]){1,0,0,0,1,0,0,0,1}, NULL);
        cmsPipelineInsertStage(pipeline, cmsAT_END, matrixStage);
        
        // Evaluate pipeline with float data
        cmsFloat32Number input[3] = {0.5f, 0.5f, 0.5f};
        cmsFloat32Number output[3];
        cmsPipelineEvalFloat(input, output, pipeline);
        
        cmsPipelineFree(pipeline);
    }
    
    // Use cmsAdaptToIlluminant with sample XYZ values
    cmsCIEXYZ sourceWhite = {0.9505, 1.0000, 1.0890}; // D65
    cmsCIEXYZ illuminant = {0.9642, 1.0000, 0.8251};  // D50
    cmsCIEXYZ value = {0.5, 0.5, 0.5};
    cmsCIEXYZ result;
    
    cmsAdaptToIlluminant(&result, &sourceWhite, &illuminant, &value);
    
    // Use input data through FILE* interface
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Try to open profile from stream
        cmsHPROFILE streamProfile = cmsOpenProfileFromStream(in_file, "r");
        if (streamProfile) {
            // Process the profile
            cmsColorSpaceSignature cs = cmsGetColorSpace(streamProfile);
            cmsCloseProfile(streamProfile);
        }
        assert_file_closed(&in_file);;
    }
    
    // Create output file for writing (if needed)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data if needed
        fwrite(data, 1, size < 100 ? size : 100, out_file);
        assert_file_closed(&out_file);;
        remove("output_file"); // Clean up
    }
    
    // Clean up all resources
    if (transform) cmsDeleteTransform(transform);
    if (inkLimitProfile) cmsCloseProfile(inkLimitProfile);
    if (nullProfile) cmsCloseProfile(nullProfile);
    cmsDeleteContext(ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}