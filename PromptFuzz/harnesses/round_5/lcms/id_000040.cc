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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 7
//<Quality> {"density":8,"unique_branches":{"CurvesAlloc":[[1332,13,1332,30,0,0,4,0],[1334,23,1334,36,0,0,4,0],[1334,23,1334,36,0,0,4,1]],"OptimizeByJoiningCurves":[[1467,13,1467,46,0,0,4,0],[1470,18,1470,29,0,0,4,1]],"FastEvaluateCurves8":[[1359,15,1359,33,0,0,4,0],[1359,15,1359,33,0,0,4,1]]},"library_calls":["cmsOpenProfileFromMem","cmsGetEncodedCMMversion","cmsCreateTransformTHR","cmsGetNamedColorList","cmsAllocProfileSequenceDescription","cmsBuildTabulatedToneCurve16","cmsGetToneCurveEstimatedTable","cmsSmoothToneCurve","cmsIT8LoadFromFile","cmsIT8TableCount","cmsIT8Free","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsDeleteTransform","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetEncodedCMMversion","cmsCreateTransformTHR","cmsGetNamedColorList","cmsAllocProfileSequenceDescription","cmsBuildTabulatedToneCurve16","cmsGetToneCurveEstimatedTable","cmsSmoothToneCurve","cmsIT8LoadFromFile","cmsIT8TableCount","cmsIT8Free","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsDeleteTransform","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
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
	  // Need minimum data for meaningful operations

    // 1. Create a profile from input data for initial operations
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	

    // 2. Get the encoded CMM version
    int cmmVersion = cmsGetEncodedCMMversion();
    (void)cmmVersion;  // Use to avoid unused variable warning

    // 3. Create a transform from the profile to get named color list
    cmsUInt32Number inputFormat = TYPE_RGB_8;
    cmsUInt32Number outputFormat = TYPE_Lab_8;
    
    cmsHTRANSFORM hTransform = cmsCreateTransformTHR(
        NULL, hProfile, inputFormat, NULL, outputFormat, 
        INTENT_PERCEPTUAL, 0
    );
    
    if (hTransform) {
        // 4. Get named color list from transform
        cmsNAMEDCOLORLIST* namedColorList = cmsGetNamedColorList(hTransform);
        if (namedColorList) {
            // 5. Create a profile sequence description
            cmsSEQ* seq = cmsAllocProfileSequenceDescription(NULL, 1);
            if (seq) {
                // 6. Create a tone curve from input data
                // Use part of input data for tone curve values
                size_t curveDataSize = size < 256 ? size : 256;
                cmsUInt16Number* curveValues = (cmsUInt16Number*)malloc(curveDataSize);
                if (curveValues) {
                    // Copy and convert data to 16-bit values
                    for (size_t i = 0; i < curveDataSize / 2 && i < 128; i++) {
                        curveValues[i] = (data[i * 2] << 8) | data[i * 2 + 1];
                    }
                    
                    cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(
                        NULL, 
                        (curveDataSize / 2) < 128 ? (curveDataSize / 2) : 128,
                        curveValues
                    );
                    
                    free(curveValues);
                    
                    if (toneCurve) {
                        // 7. Get estimated table from tone curve
                        const cmsUInt16Number* estimatedTable = 
                            cmsGetToneCurveEstimatedTable(toneCurve);
                        (void)estimatedTable;  // Use to avoid warning
                        
                        // 8. Smooth the tone curve
                        cmsBool smoothResult = cmsSmoothToneCurve(toneCurve, 1.0);
                        (void)smoothResult;  // Use to avoid warning
                        
                        // 9. Create an IT8 handle from a file
                        FILE* in_file = fmemopen((void*)data, size, "rb");
                        if (in_file) {
                            cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, "input_file");
                            if (hIT8) {
                                // 10. Get IT8 table count
                                cmsUInt32Number tableCount = cmsIT8TableCount(hIT8);
                                (void)tableCount;  // Use to avoid warning
                                
                                // Clean up IT8 handle
                                cmsIT8Free(hIT8);
                            }
                            assert_file_closed(&in_file);;
                        }
                        
                        // Clean up tone curve
                        cmsFreeToneCurve(toneCurve);
                    }
                }
                
                // Clean up sequence
                cmsFreeProfileSequenceDescription(seq);
            }
        }
        
        // Clean up transform
        cmsDeleteTransform(hTransform);
    }

    // 11. Additional processing with the profile
    // Create a simple transform and process some data
    if (size >= 10) {
        cmsHTRANSFORM simpleTransform = cmsCreateTransform(
            hProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8, 
            INTENT_RELATIVE_COLORIMETRIC, 0
        );
        
        if (simpleTransform) {
            uint8_t input[3] = {data[0], data[1], data[2]};
            uint8_t output[3];
            cmsDoTransform(simpleTransform, input, output, 1);
            cmsDeleteTransform(simpleTransform);
        }
    }

    // 12. Clean up profile
    cmsCloseProfile(hProfile);

    return 0;
}