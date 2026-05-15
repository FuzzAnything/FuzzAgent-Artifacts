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
//<ID> 243
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 2
//<Quality> {"density":24,"unique_branches":{"cmsSmoothToneCurve":[[1227,17,1227,44,0,0,4,1]],"AllocateToneCurveStruct":[[221,9,221,25,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromFile","cmsIT8TableCount","cmsIT8GetDataRowCol","cmsIT8Free","cmsOpenProfileFromMemTHR","cmsGetEncodedCMMversion","cmsCreateNULLProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetNamedColorList","cmsAllocProfileSequenceDescription","cmsBuildTabulatedToneCurve16","cmsGetToneCurveEstimatedTable","cmsSmoothToneCurve","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsBuildTabulatedToneCurve16","cmsGetToneCurveEstimatedTable","cmsSmoothToneCurve","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromFile","cmsIT8TableCount","cmsIT8GetDataRowCol","cmsIT8Free","cmsOpenProfileFromMemTHR","cmsGetEncodedCMMversion","cmsCreateNULLProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetNamedColorList","cmsAllocProfileSequenceDescription","cmsBuildTabulatedToneCurve16","cmsGetToneCurveEstimatedTable","cmsSmoothToneCurve","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsBuildTabulatedToneCurve16","cmsGetToneCurveEstimatedTable","cmsSmoothToneCurve","cmsFreeToneCurve","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 16) {
	return 0;
	}
	 // Too small for meaningful processing

    // Initialize context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // 1. Try to parse as IT8 file
    FILE *it8_file = fmemopen((void *)data, size, "rb");
    if (it8_file) {
        cmsHANDLE hIT8 = cmsIT8LoadFromFile(ctx, "input_file");
        if (hIT8) {
            // Get table count
            cmsUInt32Number tableCount = cmsIT8TableCount(hIT8);
            (void)tableCount; // Use result to prevent optimization
            
            // Try to get some data from IT8
            if (tableCount > 0) {
                const char* sample = cmsIT8GetDataRowCol(hIT8, 0, 0);
                (void)sample;
            }
            
            // Clean up
            cmsIT8Free(hIT8);
        }
        assert_file_closed(&it8_file);;
    }

    // 2. Try to parse as ICC profile
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, data, (cmsUInt32Number)size);
    if (hProfile) {
        // Get CMM version
        int cmmVersion = cmsGetEncodedCMMversion();
        (void)cmmVersion;
        
        // Create a transform to get named color list
        cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
        if (nullProfile) {
            cmsUInt32Number formats[] = {TYPE_RGB_8, TYPE_RGB_8};
            cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
                ctx, &hProfile, 1, TYPE_RGB_8, TYPE_RGB_8, 
                INTENT_PERCEPTUAL, 0);
            
            if (transform) {
                // Get named color list
                cmsNAMEDCOLORLIST* namedColorList = cmsGetNamedColorList(transform);
                if (namedColorList) {
                    // Allocate profile sequence description
                    cmsSEQ* seq = cmsAllocProfileSequenceDescription(ctx, 1);
                    if (seq) {
                        // Create a simple tone curve
                        const cmsUInt16Number curvePoints[] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000, 16383};
                        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(ctx, 17, curvePoints);
                        
                        if (toneCurve) {
                            // Get estimated table
                            const cmsUInt16Number* estimatedTable = cmsGetToneCurveEstimatedTable(toneCurve);
                            (void)estimatedTable;
                            
                            // Smooth the tone curve
                            cmsSmoothToneCurve(toneCurve, 50.0);
                            
                            // Get estimated table again after smoothing
                            const cmsUInt16Number* smoothedTable = cmsGetToneCurveEstimatedTable(toneCurve);
                            (void)smoothedTable;
                            
                            cmsFreeToneCurve(toneCurve);
                        }
                        
                        cmsFreeProfileSequenceDescription(seq);
                    }
                }
                cmsDeleteTransform(transform);
            }
            cmsCloseProfile(nullProfile);
        }
        
        // Clean up profile
        cmsCloseProfile(hProfile);
    }

    // 3. Try to process as raw tone curve data
    if (size >= sizeof(cmsUInt16Number) * 256) { // Minimum for a tone curve
        // Create tone curve from input data
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(ctx, 
            (cmsUInt32Number)(size / sizeof(cmsUInt16Number)), 
            (const cmsUInt16Number*)data);
        
        if (toneCurve) {
            // Get estimated table
            const cmsUInt16Number* estimatedTable = cmsGetToneCurveEstimatedTable(toneCurve);
            (void)estimatedTable;
            
            // Smooth the tone curve
            cmsSmoothToneCurve(toneCurve, 100.0);
            
            cmsFreeToneCurve(toneCurve);
        }
    }

    // Clean up context
    cmsDeleteContext(ctx);
    
    assert_file_closed(&it8_file);
	return 0;
}