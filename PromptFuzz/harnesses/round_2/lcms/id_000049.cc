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
//<ID> 388
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsDeleteContext","cmsDetectBlackPoint","cmsIT8Alloc","cmsIT8LoadFromMem","cmsIT8GetDataDbl","cmsIT8GetDataRowColDbl","cmsIT8Free","cmsIT8Free","cmsCreateProofingTransform","cmsStageAllocCLut16bitGranular","cmsStageData","cmsStageFree","cmsReadTag","cmsMLUtranslationsCount","cmsCMCdeltaE","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromFileTHR","cmsDetectBlackPoint","cmsIT8Alloc","cmsIT8LoadFromMem","cmsIT8GetDataDbl","cmsIT8GetDataRowColDbl","cmsIT8Free","cmsIT8Free","cmsCreateProofingTransform","cmsStageAllocCLut16bitGranular","cmsStageData","cmsStageFree","cmsReadTag","cmsMLUtranslationsCount","cmsCMCdeltaE","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
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
	  // Need minimum data for meaningful processing
    
    // Initialize LCMS context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Set log error handler
    cmsSetLogErrorHandlerTHR(context, [](cmsContext, cmsUInt32Number, const char *) {});
    
    // Parse input data as ICC profile
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    assert_file_closed(&in_file);;
    
    if (!hProfile) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Detect black point using the profile
    cmsCIEXYZ blackPoint;
    cmsDetectBlackPoint(&blackPoint, hProfile, INTENT_PERCEPTUAL, 0);
    
    // Create IT8 handle for color data parsing
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    if (hIT8) {
        // Load IT8 data from remaining buffer portion
        size_t it8_offset = size > 512 ? 512 : size/2;
        cmsHANDLE hIT8Loaded = cmsIT8LoadFromMem(context, data + it8_offset, size - it8_offset);
        if (hIT8Loaded) {
            // Get IT8 data using different query methods
            cmsFloat64Number it8_val1 = cmsIT8GetDataDbl(hIT8Loaded, "patch1", "sample1");
            cmsFloat64Number it8_val2 = cmsIT8GetDataRowColDbl(hIT8Loaded, 0, 0);
            
            cmsIT8Free(hIT8Loaded);
        }
        cmsIT8Free(hIT8);
    }
    
    // Create proofing transform for color conversion
    cmsUInt32Number inputFormat = TYPE_RGB_8;
    cmsUInt32Number outputFormat = TYPE_CMYK_8;
    
    // Use same profile for input, output and proofing for simplicity
    cmsHTRANSFORM hTransform = cmsCreateProofingTransform(hProfile, inputFormat,
                                                         hProfile, outputFormat,
                                                         hProfile, INTENT_PERCEPTUAL,
                                                         INTENT_ABSOLUTE_COLORIMETRIC, 0);
    
    // Process CLUT data if transform was created
    if (hTransform) {
        // Allocate CLUT stage for color lookup
        cmsUInt32Number clutPoints[] = {5, 5, 5};
        cmsUInt16Number tableData[5*5*5*3] = {0};  // 3 output channels
        
        cmsStage *clutStage = cmsStageAllocCLut16bitGranular(context, clutPoints, 3, 3, tableData);
        if (clutStage) {
            // Access stage data
            void *stageData = cmsStageData(clutStage);
            (void)stageData;  // Use data to avoid unused warning
            
            cmsStageFree(clutStage);
        }
        
        // Get MLU translations count from profile
        const cmsMLU *mlu = (const cmsMLU *)cmsReadTag(hProfile, cmsSigProfileDescriptionTag);
        if (mlu) {
            cmsUInt32Number transCount = cmsMLUtranslationsCount(mlu);
            (void)transCount;
        }
        
        // Calculate color difference using CMC formula
        cmsCIELab lab1 = {50.0, 0.0, 0.0};
        cmsCIELab lab2 = {55.0, 5.0, 5.0};
        cmsFloat64Number deltaE = cmsCMCdeltaE(&lab1, &lab2, 1.0, 1.0);
        (void)deltaE;
        
        cmsDeleteTransform(hTransform);
    }
    
    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}