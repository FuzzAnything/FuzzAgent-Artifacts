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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreateLab2ProfileTHR","cmsDeleteContext","cmsCreateTransform","cmsGetPostScriptCSA","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8SetDataDbl","cmsIT8SetDataDbl","cmsIT8SetDataDbl","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsIT8GetPatchByName","cmsGetTransformInputFormat","cmsDetectBlackPoint","cmsBuildTabulatedToneCurve16","cmsReverseToneCurveEx","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreateLab2ProfileTHR","cmsCreateTransform","cmsGetPostScriptCSA","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8SetDataDbl","cmsIT8SetDataDbl","cmsIT8SetDataDbl","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsIT8GetPatchByName","cmsGetTransformInputFormat","cmsDetectBlackPoint","cmsBuildTabulatedToneCurve16","cmsReverseToneCurveEx","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	  // Need minimal data for operations
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        // If we can't create profile from input, create a dummy one
        cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
        hProfile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);
    }
    
    if (!hProfile) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create transform to use with cmsGetTransformOutputFormat
    cmsUInt32Number inputFormat = TYPE_Lab_DBL;
    cmsUInt32Number outputFormat = TYPE_RGB_DBL;
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, inputFormat, 
                                                  hProfile, outputFormat, 
                                                  INTENT_PERCEPTUAL, 0);
    
    // 1. cmsGetPostScriptCSA
    void *psBuffer = malloc(4096);
    if (psBuffer) {
        cmsUInt32Number psSize = cmsGetPostScriptCSA(ctx, hProfile, 
                                                    INTENT_PERCEPTUAL, 0,
                                                    psBuffer, 4096);
        (void)psSize; // Use result to avoid unused warning
        free(psBuffer);
    }
    
    // 2. cmsIT8SaveToMem - Create and save IT8 data
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        cmsUInt32Number bytesNeeded = 0;
        
        // Add some dummy data to IT8 structure
        cmsIT8SetPropertyStr(hIT8, "DESCRIPTOR", "Test IT8 Data");
        cmsIT8SetDataFormat(hIT8, 0, "SAMPLE_ID");
        cmsIT8SetDataFormat(hIT8, 1, "RGB_R");
        cmsIT8SetDataFormat(hIT8, 2, "RGB_G");
        cmsIT8SetDataFormat(hIT8, 3, "RGB_B");
        
        // Add a patch
        cmsIT8SetData(hIT8, "Patch1", "SAMPLE_ID", "TestPatch");
        cmsIT8SetDataDbl(hIT8, "Patch1", "RGB_R", 0.5);
        cmsIT8SetDataDbl(hIT8, "Patch1", "RGB_G", 0.5);
        cmsIT8SetDataDbl(hIT8, "Patch1", "RGB_B", 0.5);
        
        // Try to save IT8 to memory
        cmsBool saveResult = cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded);
        if (saveResult && bytesNeeded > 0) {
            void *it8Buffer = malloc(bytesNeeded);
            if (it8Buffer) {
                cmsIT8SaveToMem(hIT8, it8Buffer, &bytesNeeded);
                free(it8Buffer);
            }
        }
        
        // 5. cmsIT8GetPatchByName
        int patchIndex = cmsIT8GetPatchByName(hIT8, "Patch1");
        (void)patchIndex; // Use result
    }
    
    // 3. cmsGetTransformOutputFormat - Note: This function doesn't exist in provided APIs
    // Using cmsGetTransformInputFormat instead (available in provided APIs)
    if (hTransform) {
        cmsUInt32Number inputFmt = cmsGetTransformInputFormat(hTransform);
        (void)inputFmt; // Use result
    }
    
    // 4. cmsDetectBlackPoint
    cmsCIEXYZ blackPoint;
    cmsBool blackResult = cmsDetectBlackPoint(&blackPoint, hProfile, 
                                            INTENT_PERCEPTUAL, 0);
    (void)blackResult; // Use result
    
    // 6. cmsReverseToneCurveEx and 7. cmsGetToneCurveEstimatedTable
    // Create a tone curve from input data
    if (size >= 256) {
        // Create tone curve from first part of input data
        cmsToneCurve *toneCurve = cmsBuildTabulatedToneCurve16(ctx, 
            size < 65536 ? size/2 : 128, 
            (const cmsUInt16Number *)data);
        
        if (toneCurve) {
            // Reverse the tone curve
            cmsToneCurve *reversedCurve = cmsReverseToneCurveEx(256, toneCurve);
            
            if (reversedCurve) {
                // Get estimated table
                const cmsUInt16Number *estimatedTable = 
                    cmsGetToneCurveEstimatedTable(reversedCurve);
                (void)estimatedTable; // Use result
                
                cmsFreeToneCurve(reversedCurve);
            }
            
            cmsFreeToneCurve(toneCurve);
        }
    }
    
    // Cleanup
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
    
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}