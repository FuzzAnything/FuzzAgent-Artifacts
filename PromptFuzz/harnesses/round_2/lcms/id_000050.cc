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
//<ID> 389
//<Prompt> []
/*<Combination>: [
*/
//<score> 140, nr_unique_branch: 27
//<Quality> {"density":20,"unique_branches":{"cmsMLUtranslationsCount":[[691,9,691,20,0,0,4,1]],"Type_LUT16_Read":[[2385,9,2385,87,0,0,4,0]],"DefaultEvalParametricFn":[[365,13,365,18,0,0,4,0],[367,17,367,61,0,0,4,1]],"PrelinEval8":[[967,25,967,33,0,0,4,1],[974,17,974,25,0,0,4,1],[974,29,974,37,0,0,4,0],[981,21,981,29,0,0,4,0],[981,33,981,41,0,0,4,0],[981,33,981,41,0,0,4,1],[988,37,988,45,0,0,4,1],[995,29,995,37,0,0,4,0],[995,29,995,37,0,0,4,1],[995,41,995,49,0,0,4,0],[1002,33,1002,41,0,0,4,0],[1002,45,1002,53,0,0,4,0]],"cmsCMCdeltaE":[[553,7,553,20,0,0,4,1],[564,7,564,32,0,0,4,0],[569,7,569,21,0,0,4,0],[569,25,569,39,0,0,4,0],[577,8,577,18,0,0,4,1]],"MatShaperEval16":[[1567,10,1567,18,0,0,4,0],[1567,26,1567,38,0,0,4,0],[1568,10,1568,18,0,0,4,0],[1568,26,1568,38,0,0,4,0],[1569,10,1569,18,0,0,4,0],[1569,26,1569,38,0,0,4,0]]},"library_calls":["cmsSetLogErrorHandlerTHR","cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsOpenProfileFromMemTHR","cmsDetectBlackPoint","cmsBuildGamma","cmsCreateRGBProfile","cmsCreate_sRGBProfile","cmsCreateProofingTransform","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8SetPropertyHex","cmsIT8SetData","cmsIT8SetData","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8SetDataRowColDbl","cmsIT8GetDataRowColDbl","cmsIT8GetPropertyMulti","cmsStageAllocCLut16bitGranular","cmsStageData","cmsCMCdeltaE","cmsReadTag","cmsMLUtranslationsCount","cmsDoTransform","cmsDoTransform","cmsStageFree","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsSetLogErrorHandlerTHR","cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromFileTHR","cmsOpenProfileFromMemTHR","cmsDetectBlackPoint","cmsBuildGamma","cmsCreateRGBProfile","cmsCreate_sRGBProfile","cmsCreateProofingTransform","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8SetPropertyHex","cmsIT8SetData","cmsIT8SetData","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8SetDataRowColDbl","cmsIT8GetDataRowColDbl","cmsIT8GetPropertyMulti","cmsStageAllocCLut16bitGranular","cmsStageData","cmsCMCdeltaE","cmsReadTag","cmsMLUtranslationsCount","cmsDoTransform","cmsDoTransform","cmsStageFree","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*To create a fuzz driver using the specified lcms APIs, I'll design a scenario that:

1. Loads a profile from input data
2. Performs color management operations including black point detection and proofing transformations
3. Uses IT8 color data and MLU translations
4. Creates CLUT stages for color transformations
5. Computes color differences

Here's the complete implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data
    
    cmsContext context = nullptr;
    cmsHPROFILE inputProfile = nullptr;
    cmsHPROFILE outputProfile = nullptr;
    cmsHPROFILE proofingProfile = nullptr;
    cmsHTRANSFORM transform = nullptr;
    cmsHANDLE it8 = nullptr;
    FILE* in_file = nullptr;
    cmsStage* clutStage = nullptr;
    const cmsMLU* mlu = nullptr;
    
    // Set up error handler
    auto errorHandler = [](cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, const char* /*Text*/) {
        // Silent error handler for fuzzing
    };
    cmsSetLogErrorHandlerTHR(nullptr, errorHandler);
    
    // Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Set error handler for this context
    cmsSetLogErrorHandlerTHR(context, errorHandler);
    
    // Use input data as a profile
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to open profile from file
    inputProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!inputProfile) {
        // If can't open from file, try from memory
        rewind(in_file);
        inputProfile = cmsOpenProfileFromMemTHR(context, data, static_cast<cmsUInt32Number>(size));
    }
    
    if (inputProfile) {
        // 1. Detect black point
        cmsCIEXYZ blackPoint;
        if (cmsDetectBlackPoint(&blackPoint, inputProfile, INTENT_PERCEPTUAL, 0)) {
            // Black point detected successfully
        }
        
        // 2. Create proofing transform
        // Create output and proofing profiles (simple RGB profiles)
        cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
        cmsCIExyYTRIPLE primaries = {
            {0.64, 0.33, 1.0},
            {0.30, 0.60, 1.0},
            {0.15, 0.06, 1.0}
        };
        
        // Create gamma curve
        cmsToneCurve* gamma = cmsBuildGamma(context, 2.2);
        cmsToneCurve* transferFunction[3] = {gamma, gamma, gamma};
        
        outputProfile = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunction);
        proofingProfile = cmsCreate_sRGBProfile();
        
        if (outputProfile && proofingProfile) {
            cmsUInt32Number inputFormat = TYPE_RGB_8;
            cmsUInt32Number outputFormat = TYPE_RGB_8;
            
            transform = cmsCreateProofingTransform(inputProfile, inputFormat,
                                                   outputProfile, outputFormat,
                                                   proofingProfile,
                                                   INTENT_PERCEPTUAL,
                                                   INTENT_ABSOLUTE_COLORIMETRIC,
                                                   0);
        }
        
        // Free gamma curve
        cmsFreeToneCurve(gamma);
    }
    
    // 3. Create IT8 handle and use IT8 functions
    it8 = cmsIT8Alloc(context);
    if (it8) {
        // Set some properties
        cmsIT8SetPropertyDbl(it8, "SAMPLE_ID", 1.0);
        cmsIT8SetPropertyHex(it8, "COLORANT_COUNT", 3);
        
        // Add some data
        cmsIT8SetData(it8, "A1", "R", "0.5");
        cmsIT8SetData(it8, "A1", "G", "0.5");
        cmsIT8SetData(it8, "A1", "B", "0.5");
        
        // Get data back
        cmsFloat64Number val = cmsIT8GetDataDbl(it8, "A1", "R");
        
        // Set and get row/col data
        cmsIT8SetDataRowColDbl(it8, 0, 0, 0.75);
        val = cmsIT8GetDataRowColDbl(it8, 0, 0);
        
        // Get property
        const char* prop = cmsIT8GetPropertyMulti(it8, "ORIGINATOR", "");
    }
    
    // 4. Create CLUT stage
    if (context) {
        cmsUInt32Number clutPoints[] = {5, 5, 5};
        cmsUInt32Number inputChan = 3;
        cmsUInt32Number outputChan = 3;
        
        // Create table data (simplified)
        size_t tableSize = 5 * 5 * 5 * 3;
        cmsUInt16Number* table = static_cast<cmsUInt16Number*>(calloc(tableSize, sizeof(cmsUInt16Number)));
        
        if (table) {
            // Fill with some values based on input data
            for (size_t i = 0; i < tableSize && i < size; i++) {
                table[i] = static_cast<cmsUInt16Number>(data[i] << 8);
            }
            
            clutStage = cmsStageAllocCLut16bitGranular(context, clutPoints, 
                                                      inputChan, outputChan, table);
            
            if (clutStage) {
                // Get stage data
                void* stageData = cmsStageData(clutStage);
                // stageData could be used here
            }
            
            free(table);
        }
    }
    
    // 5. Compute CMC deltaE
    cmsCIELab lab1 = {50.0, 2.0, -1.0};
    cmsCIELab lab2 = {52.0, 1.5, -0.5};
    cmsFloat64Number deltaE = cmsCMCdeltaE(&lab1, &lab2, 2.0, 1.0);
    
    // 6. Get MLU translations count if we have a profile
    if (inputProfile) {
        // Try to read a tag that contains MLU
        mlu = static_cast<const cmsMLU*>(cmsReadTag(inputProfile, cmsSigProfileDescriptionTag));
        if (mlu) {
            cmsUInt32Number translationCount = cmsMLUtranslationsCount(mlu);
        }
    }
    
    // 7. If we have a transform, use it
    if (transform) {
        // Prepare test colors
        unsigned char inputRGB[3] = {128, 64, 192};
        unsigned char outputRGB[3] = {0, 0, 0};
        
        // Transform single pixel
        cmsDoTransform(transform, inputRGB, outputRGB, 1);
        
        // Test with more data if available
        if (size >= 300) {
            size_t pixelCount = size / 3;
            if (pixelCount > 100) pixelCount = 100;
            
            unsigned char* bulkInput = static_cast<unsigned char*>(malloc(pixelCount * 3));
            unsigned char* bulkOutput = static_cast<unsigned char*>(malloc(pixelCount * 3));
            
            if (bulkInput && bulkOutput) {
                // Fill with input data
                for (size_t i = 0; i < pixelCount * 3 && i < size; i++) {
                    bulkInput[i] = data[i];
                }
                
                cmsDoTransform(transform, bulkInput, bulkOutput, static_cast<cmsUInt32Number>(pixelCount));
                
                free(bulkInput);
                free(bulkOutput);
            }
        }
    }
    
    // Clean up
    if (clutStage) cmsStageFree(clutStage);
    if (transform) cmsDeleteTransform(transform);
    if (it8) cmsIT8Free(it8);
    if (outputProfile) cmsCloseProfile(outputProfile);
    if (proofingProfile) cmsCloseProfile(proofingProfile);
    if (inputProfile) cmsCloseProfile(inputProfile);
    if (in_file) assert_file_closed(&in_file);;
    if (context) cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}