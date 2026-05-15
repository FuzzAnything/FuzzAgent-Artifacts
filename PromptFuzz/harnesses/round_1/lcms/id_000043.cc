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
//<ID> 279
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 15
//<Quality> {"density":20,"unique_branches":{"WriteStr":[[1831,17,1831,35,0,0,4,0]],"WriteData":[[1974,12,1974,20,0,0,4,1],[1980,12,1980,35,0,0,4,0],[1982,24,1982,36,0,0,4,0],[1982,24,1982,36,0,0,4,1],[1986,28,1986,43,0,0,4,1]],"cmsTempFromWhitePoint":[[163,15,163,23,0,0,4,0],[163,15,163,23,0,0,4,1],[172,13,172,21,0,0,4,0],[172,13,172,21,0,0,4,1],[172,25,172,38,0,0,4,0],[172,25,172,38,0,0,4,1]],"WriteHeader":[[1910,13,1910,32,0,0,4,0],[1914,13,1914,33,0,0,4,1]],"cmsIT8SaveToMem":[[2068,20,2068,38,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsDeleteContext","cmsGetPCS","cmsGetHeaderProfileID","cmsTempFromWhitePoint","cmsBuildTabulatedToneCurve16","cmsStageAllocToneCurves","cmsStageDup","cmsStageFree","cmsStageFree","cmsFreeToneCurve","cmsIT8LoadFromMem","cmsIT8GetPatchName","cmsIT8SaveToMem","cmsIT8Free","cmsCreateTransform","cmsDoTransformStride","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsGetPCS","cmsGetHeaderProfileID","cmsTempFromWhitePoint","cmsBuildTabulatedToneCurve16","cmsStageAllocToneCurves","cmsStageDup","cmsStageFree","cmsStageFree","cmsFreeToneCurve","cmsIT8LoadFromMem","cmsIT8GetPatchName","cmsIT8SaveToMem","cmsIT8Free","cmsCreateTransform","cmsDoTransformStride","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	  // Minimum size for meaningful processing

    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	

    // 1. Create NULL profile using cmsCreateNULLProfileTHR
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(context);
    if (!nullProfile) {
        cmsDeleteContext(context);
        return 0;
    }

    // 2. Get PCS from the NULL profile
    cmsColorSpaceSignature pcs = cmsGetPCS(nullProfile);
    (void)pcs; // Use variable to avoid unused warning

    // 3. Get profile ID using cmsGetHeaderProfileID
    cmsUInt8Number profileID[16];
    cmsGetHeaderProfileID(nullProfile, profileID);

    // 4. Create white point from input data for cmsTempFromWhitePoint
    cmsCIExyY whitePoint;
    if (size >= sizeof(cmsCIExyY)) {
        memcpy(&whitePoint, data, sizeof(cmsCIExyY));
        // Ensure valid white point coordinates
        whitePoint.x = (whitePoint.x < 0.0) ? 0.0 : (whitePoint.x > 1.0) ? 1.0 : whitePoint.x;
        whitePoint.y = (whitePoint.y < 0.0) ? 0.0 : (whitePoint.y > 1.0) ? 1.0 : whitePoint.y;
        whitePoint.Y = (whitePoint.Y < 0.0) ? 0.0 : whitePoint.Y;
        
        cmsFloat64Number tempK;
        cmsBool tempResult = cmsTempFromWhitePoint(&tempK, &whitePoint);
        (void)tempResult; // Use variable to avoid unused warning
    }

    // 5. Create tone curve from input data using cmsBuildTabulatedToneCurve16
    const size_t toneCurveEntries = 256;
    if (size >= toneCurveEntries * sizeof(cmsUInt16Number)) {
        const cmsUInt16Number* curveData = reinterpret_cast<const cmsUInt16Number*>(data);
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(context, toneCurveEntries, curveData);
        
        if (toneCurve) {
            // 6. Create stage using tone curve and duplicate it with cmsStageDup
            cmsStage* stage = cmsStageAllocToneCurves(context, 1, &toneCurve);
            if (stage) {
                cmsStage* dupStage = cmsStageDup(stage);
                if (dupStage) {
                    cmsStageFree(dupStage);
                }
                cmsStageFree(stage);
            }
            cmsFreeToneCurve(toneCurve);
        }
    }

    // 7. Create IT8 handle from input data
    size_t it8Offset = toneCurveEntries * sizeof(cmsUInt16Number);
    if (it8Offset < size) {
        size_t it8Size = size - it8Offset;
        const uint8_t* it8Data = data + it8Offset;
        
        cmsHANDLE it8 = cmsIT8LoadFromMem(context, it8Data, it8Size);
        if (it8) {
            // 8. Get patch name using cmsIT8GetPatchName
            char patchName[256];
            const char* patchNamePtr = cmsIT8GetPatchName(it8, 0, patchName);
            (void)patchNamePtr; // Use variable to avoid unused warning

            // 9. Save IT8 to memory using cmsIT8SaveToMem
            cmsUInt32Number bytesNeeded = 0;
            void* savedData = malloc(size); // Allocate enough space
            if (savedData) {
                cmsBool saveResult = cmsIT8SaveToMem(it8, savedData, &bytesNeeded);
                (void)saveResult; // Use variable to avoid unused warning
                free(savedData);
            }
            
            cmsIT8Free(it8);
        }
    }

    // 10. Create transform for cmsDoTransformStride
    cmsHTRANSFORM transform = cmsCreateTransform(nullProfile, TYPE_RGB_16, 
                                                 nullProfile, TYPE_RGB_16, 
                                                 INTENT_PERCEPTUAL, 0);
    if (transform) {
        // 11. Perform transform using cmsDoTransformStride
        const size_t pixelCount = 16;
        const size_t stride = 3 * sizeof(cmsUInt16Number) * pixelCount; // 16 pixels
        
        if (size >= stride) {
            cmsUInt16Number inputBuffer[3 * pixelCount];
            cmsUInt16Number outputBuffer[3 * pixelCount];
            
            // Use part of input data for transform
            size_t dataToUse = std::min(sizeof(inputBuffer), size);
            memcpy(inputBuffer, data, dataToUse);
            
            cmsDoTransformStride(transform, inputBuffer, outputBuffer, 
                                pixelCount, stride);
        }
        
        cmsDeleteTransform(transform);
    }

    // Clean up
    cmsCloseProfile(nullProfile);
    cmsDeleteContext(context);

    return 0;
}