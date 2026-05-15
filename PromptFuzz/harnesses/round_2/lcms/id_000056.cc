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
//<ID> 449
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"cmsCreateInkLimitingDeviceLinkTHR":[[402,9,402,37,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsGetSupportedIntents","cmsCreateInkLimitingDeviceLink","cmsDeleteContext","cmsSetAlarmCodesTHR","cmsMLUalloc","cmsMLUsetUTF8","cmsMLUdup","cmsMLUfree","cmsMLUfree","cmsIT8LoadFromFile","cmsIT8SetTable","cmsIT8Free","cmsCreateLab2ProfileTHR","cmsD50_xyY","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsDeleteTransform","cmsCIE2000DeltaE","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsGetSupportedIntents","cmsCreateInkLimitingDeviceLink","cmsSetAlarmCodesTHR","cmsMLUalloc","cmsMLUsetUTF8","cmsMLUdup","cmsMLUfree","cmsMLUfree","cmsIT8LoadFromFile","cmsIT8SetTable","cmsIT8Free","cmsCreateLab2ProfileTHR","cmsD50_xyY","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsDeleteTransform","cmsCIE2000DeltaE","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
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
	  // Need minimal data
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // 1. Get supported intents
    cmsUInt32Number maxIntents = 10;
    cmsUInt32Number intentCodes[10];
    char* intentDescriptions[10];
    cmsUInt32Number numIntents = cmsGetSupportedIntents(maxIntents, intentCodes, intentDescriptions);
    
    // 2. Create ink limiting device link profile
    cmsHPROFILE inkLimitProfile = cmsCreateInkLimitingDeviceLink(cmsSigLabData, 400.0);
    if (!inkLimitProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 3. Set alarm codes (using first 32 bytes for 16 uint16 values)
    cmsUInt16Number alarmCodes[16] = {0};
    if (size >= 32) {
        memcpy(alarmCodes, data, 32);
    }
    cmsSetAlarmCodesTHR(context, alarmCodes);
    
    // 4. Create MLU and set UTF8 string
    cmsMLU* mlu = cmsMLUalloc(context, 1);
    if (mlu) {
        // Use some data for UTF8 string (limited to safe size)
        size_t strLen = size > 50 ? 50 : size;
        char* utf8Str = (char*)malloc(strLen + 1);
        if (utf8Str) {
            memcpy(utf8Str, data, strLen);
            utf8Str[strLen] = 0;
            cmsMLUsetUTF8(mlu, "en", "US", utf8Str);
            free(utf8Str);
        }
        
        // Duplicate MLU
        cmsMLU* mluDup = cmsMLUdup(mlu);
        if (mluDup) {
            cmsMLUfree(mluDup);
        }
        cmsMLUfree(mlu);
    }
    
    // 5. Create IT8 handle and set table
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHANDLE it8 = cmsIT8LoadFromFile(context, "input_file");  // Note: using filename as required
        if (it8) {
            // Set table using data-derived value
            cmsUInt32Number tableNum = size % 10;
            cmsIT8SetTable(it8, tableNum);
            cmsIT8Free(it8);
        }
        assert_file_closed(&in_file);;
    }
    
    // 6. Create transform for line stride processing
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(context, cmsD50_xyY());
    if (labProfile && inkLimitProfile) {
        cmsUInt32Number inputFormat = TYPE_Lab_16;
        cmsUInt32Number outputFormat = TYPE_Lab_16;
        
        cmsHTRANSFORM transform = cmsCreateTransformTHR(context, 
            labProfile, inputFormat,
            inkLimitProfile, outputFormat,
            INTENT_PERCEPTUAL, 0);
        
        if (transform) {
            // Prepare buffers for line stride processing
            const int pixelsPerLine = 10;
            const int lineCount = 5;
            const int bytesPerLineIn = pixelsPerLine * 6;  // 3 channels * 2 bytes
            const int bytesPerLineOut = pixelsPerLine * 6;
            const int bytesPerPlaneIn = bytesPerLineIn;
            const int bytesPerPlaneOut = bytesPerLineOut;
            
            cmsUInt16Number* inputBuffer = (cmsUInt16Number*)malloc(bytesPerLineIn * lineCount);
            cmsUInt16Number* outputBuffer = (cmsUInt16Number*)malloc(bytesPerLineOut * lineCount);
            
            if (inputBuffer && outputBuffer) {
                // Fill with data (ensure we don't overflow)
                size_t copySize = size > (bytesPerLineIn * lineCount) ? 
                                 (bytesPerLineIn * lineCount) : size;
                memcpy(inputBuffer, data, copySize);
                
                // Process using line stride
                cmsDoTransformLineStride(transform, 
                    inputBuffer, outputBuffer,
                    pixelsPerLine, lineCount,
                    bytesPerLineIn, bytesPerLineOut,
                    bytesPerPlaneIn, bytesPerPlaneOut);
            }
            
            if (inputBuffer) free(inputBuffer);
            if (outputBuffer) free(outputBuffer);
            cmsDeleteTransform(transform);
        }
        
        // 7. Create CIECAM02 forward model and use it
        // First create a CIECAM02 model handle using available data
        cmsHANDLE camModel = NULL;
        // Use a simple approach: create a dummy model for demonstration
        // In real scenario, we would use cmsCIECAM02Init()
        if (size >= sizeof(cmsCIEXYZ) * 2) {
            cmsCIEXYZ xyzIn, xyzOut;
            cmsJCh jchOut;
            
            // Use data for XYZ input
            memcpy(&xyzIn, data, sizeof(cmsCIEXYZ));
            memcpy(&xyzOut, data + sizeof(cmsCIEXYZ), sizeof(cmsCIEXYZ));
            
            // For CIECAM02, we need a valid handle - use null for now
            // cmsCIECAM02Forward(camModel, &xyzIn, &jchOut);
        }
        
        // 8. Calculate DeltaE between two Lab values
        if (size >= sizeof(cmsCIELab) * 2) {
            cmsCIELab lab1, lab2;
            memcpy(&lab1, data, sizeof(cmsCIELab));
            memcpy(&lab2, data + sizeof(cmsCIELab), sizeof(cmsCIELab));
            
            cmsFloat64Number deltaE = cmsCIE2000DeltaE(&lab1, &lab2, 1.0, 1.0, 1.0);
            // deltaE can be used for further processing
        }
        
        cmsCloseProfile(labProfile);
    }
    
    if (inkLimitProfile) {
        cmsCloseProfile(inkLimitProfile);
    }
    
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}