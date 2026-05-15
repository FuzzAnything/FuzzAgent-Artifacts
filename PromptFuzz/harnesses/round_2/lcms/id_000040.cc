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
//<ID> 307
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"cmsIT8LoadFromFile":[[2629,10,2629,37,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsDeleteContext","cmsCloseProfile","cmsDeleteContext","cmsBuildTabulatedToneCurve16","cmsCloseProfile","cmsDeleteContext","cmsGetToneCurveSegment","cmsBuildSegmentedToneCurve","cmsIT8LoadFromFile","cmsIT8Alloc","cmsIT8Free","cmsIT8Free","cmsEvalToneCurve16","cmsEvalToneCurve16","cmsEvalToneCurve16","cmsEvalToneCurve16","cmsEvalToneCurve16","cmsEvalToneCurve16","cmsSliceSpace16","cmsGetPostScriptCRD","cmsCIE94DeltaE","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsSaveProfileToStream","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsBuildTabulatedToneCurve16","cmsGetToneCurveSegment","cmsBuildSegmentedToneCurve","cmsIT8LoadFromFile","cmsIT8Alloc","cmsIT8Free","cmsIT8Free","cmsEvalToneCurve16","cmsEvalToneCurve16","cmsEvalToneCurve16","cmsEvalToneCurve16","cmsEvalToneCurve16","cmsEvalToneCurve16","cmsSliceSpace16","cmsGetPostScriptCRD","cmsCIE94DeltaE","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsSaveProfileToStream","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteContext"],"visited":0}
/**/


// Include necessary LCMS headers

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





    if (size < 4) {
	return 0;
	}
	
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Create NULL profile
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(context);
    if (!nullProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Build tabulated tone curve from input data
    // Use first 256*2 = 512 bytes for tone curve values (if available)
    size_t curveSize = size / 2;
    if (curveSize > 256) curveSize = 256;
    if (curveSize < 2) curveSize = 2;
    
    cmsUInt16Number* curveValues = (cmsUInt16Number*)malloc(curveSize * sizeof(cmsUInt16Number));
    if (!curveValues) {
        cmsCloseProfile(nullProfile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Copy and convert data to 16-bit values
    for (size_t i = 0; i < curveSize && i * 2 + 1 < size; i++) {
        curveValues[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    
    cmsToneCurve* tabulatedCurve = cmsBuildTabulatedToneCurve16(context, 
                                                                (cmsUInt32Number)curveSize, 
                                                                curveValues);
    free(curveValues);
    
    if (!tabulatedCurve) {
        cmsCloseProfile(nullProfile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Get tone curve segment
    const cmsCurveSegment* segment = cmsGetToneCurveSegment(0, tabulatedCurve);
    
    // Build segmented tone curve using the segment
    cmsToneCurve* segmentedCurve = NULL;
    if (segment) {
        segmentedCurve = cmsBuildSegmentedToneCurve(context, 1, segment);
    }
    
    // Use remaining data for IT8 operations
    size_t it8DataSize = size - (curveSize * 2);
    if (it8DataSize > 0 && it8DataSize < size) {
        // Create temporary file for IT8 data
        FILE* it8File = fopen("input_file", "wb");
        if (it8File) {
            fwrite(data + (curveSize * 2), 1, it8DataSize, it8File);
            assert_file_closed(&it8File);;
            
            // Load IT8 from file
            cmsHANDLE it8Handle = cmsIT8LoadFromFile(context, "input_file");
            if (it8Handle) {
                // Allocate another IT8 handle
                cmsHANDLE it8Alloc = cmsIT8Alloc(context);
                if (it8Alloc) {
                    cmsIT8Free(it8Alloc);
                }
                cmsIT8Free(it8Handle);
            }
            
            // Clean up temp file
            remove("input_file");
        }
    }
    
    // Use cmsSliceSpace16 with a sampler function
    if (segmentedCurve) {
        // Define a simple sampler function
        auto sampler = [](const cmsUInt16Number* in, cmsUInt16Number* out, void* cargo) -> cmsInt32Number {
            cmsToneCurve* curve = (cmsToneCurve*)cargo;
            if (curve && in && out) {
                out[0] = cmsEvalToneCurve16(curve, in[0]);
                out[1] = cmsEvalToneCurve16(curve, in[1]);
                out[2] = cmsEvalToneCurve16(curve, in[2]);
            }
            return 1;
        };
        
        cmsUInt32Number clutPoints[] = {10, 10, 10};
        cmsSliceSpace16(3, clutPoints, sampler, (void*)segmentedCurve);
    }
    
    // Get PostScript CRD
    char psBuffer[1024];
    cmsUInt32Number psSize = cmsGetPostScriptCRD(context, nullProfile, 
                                                 INTENT_PERCEPTUAL, 
                                                 0, 
                                                 psBuffer, 
                                                 sizeof(psBuffer));
    
    // Compute CIE94 delta E
    cmsCIELab lab1 = {50.0, 0.0, 0.0};
    cmsCIELab lab2 = {55.0, 10.0, 5.0};
    cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);
    
    // Create transform using the null profile
    cmsUInt32Number inputFormat = TYPE_Lab_DBL;
    cmsUInt32Number outputFormat = TYPE_Lab_DBL;
    
    cmsHTRANSFORM transform = cmsCreateTransformTHR(context, 
                                                   nullProfile, 
                                                   inputFormat,
                                                   nullProfile, 
                                                   outputFormat,
                                                   INTENT_PERCEPTUAL,
                                                   0);
    
    if (transform) {
        // Prepare input/output buffers
        double inputLab[3] = {50.0, 0.0, 0.0};
        double outputLab[3] = {0.0, 0.0, 0.0};
        
        // Do transform
        cmsDoTransform(transform, inputLab, outputLab, 1);
        
        // Delete transform
        cmsDeleteTransform(transform);
    }
    
    // Save profile to stream
    FILE* outStream = fopen("output_file", "wb");
    if (outStream) {
        cmsSaveProfileToStream(nullProfile, outStream);
        assert_file_closed(&outStream);;
        remove("output_file");
    }
    
    // Clean up
    cmsCloseProfile(nullProfile);
    
    if (segmentedCurve) {
        cmsFreeToneCurve(segmentedCurve);
    }
    
    if (tabulatedCurve) {
        cmsFreeToneCurve(tabulatedCurve);
    }
    
    cmsDeleteContext(context);
    
    assert_file_closed(&outStream);
	return 0;
}