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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["cmsCreateContext","cmsBuildTabulatedToneCurve16","cmsBuildGamma","cmsCreateLinearizationDeviceLinkTHR","cmsFreeToneCurve","cmsDeleteContext","cmsLCh2Lab","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsDoTransformStride","cmsDeleteTransform","cmsCloseProfile","cmsGetAlarmCodes","cmsGetHeaderAttributes","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsBuildTabulatedToneCurve16","cmsBuildGamma","cmsCreateLinearizationDeviceLinkTHR","cmsLCh2Lab","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsDoTransformStride","cmsDeleteTransform","cmsCloseProfile","cmsGetAlarmCodes","cmsGetHeaderAttributes","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 256) {
	return 0;
	}
	  // Not enough data for meaningful processing
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Create tone curves from input data (using first 256 bytes for each curve)
    cmsToneCurve* transferFunctions[3] = {NULL, NULL, NULL};
    cmsUInt16Number curveData[256];
    
    for (int i = 0; i < 3 && (i * 512 + 512) <= size; i++) {
        if ((i * 512 + 512) <= size) {
            // Use 512 bytes per curve (256 * 2 bytes)
            memcpy(curveData, data + (i * 512), 512);
            transferFunctions[i] = cmsBuildTabulatedToneCurve16(context, 256, curveData);
        }
    }
    
    // Create at least one identity curve if we couldn't create from data
    for (int i = 0; i < 3; i++) {
        if (!transferFunctions[i]) {
            transferFunctions[i] = cmsBuildGamma(context, 1.0);
        }
    }
    
    // 1. Create linearization device link profile
    cmsHPROFILE linearProfile = cmsCreateLinearizationDeviceLinkTHR(
        context, cmsSigRgbData, transferFunctions);
    
    if (!linearProfile) {
        // Cleanup and return
        for (int i = 0; i < 3; i++) {
            if (transferFunctions[i]) cmsFreeToneCurve(transferFunctions[i]);
        }
        cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Convert LCh to Lab
    cmsCIELCh lch;
    cmsCIELab lab;
    
    // Use bytes from input for LCh values
    if (size >= 24) {
        memcpy(&lch, data, 24);
        cmsLCh2Lab(&lab, &lch);
        
        // 3. Add point to gamut boundary descriptor (if we had one)
        // Create a simple gamut boundary descriptor
        cmsHANDLE gbd = cmsGBDAlloc(context);
        if (gbd) {
            // 4. Add the Lab point to GDB
            cmsGDBAddPoint(gbd, &lab);
            cmsGBDFree(gbd);
        }
    }
    
    // 5. Create transform and perform color transformation
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(context);
    if (sRGBProfile) {
        cmsHTRANSFORM transform = cmsCreateTransformTHR(
            context, linearProfile, TYPE_RGB_8, sRGBProfile, TYPE_RGB_8,
            INTENT_PERCEPTUAL, 0);
        
        if (transform) {
            // Prepare input/output buffers from data
            const size_t pixelCount = 16;
            size_t bytesNeeded = pixelCount * 3;
            
            if (bytesNeeded * 2 <= size) {
                uint8_t inputBuffer[48];  // 16 pixels * 3 channels
                uint8_t outputBuffer[48];
                
                // Use data for input
                size_t offset = 24;  // Skip LCh data
                if (offset + bytesNeeded <= size) {
                    memcpy(inputBuffer, data + offset, bytesNeeded);
                    
                    // 3. Perform transform with stride
                    cmsDoTransformStride(transform, inputBuffer, outputBuffer, 
                                        pixelCount, 0);  // Default stride
                }
            }
            
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(sRGBProfile);
    }
    
    // 6. Get alarm codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodes(alarmCodes);
    
    // 7. Get header attributes
    cmsUInt64Number flags;
    cmsGetHeaderAttributes(linearProfile, &flags);
    
    // Cleanup
    cmsCloseProfile(linearProfile);
    
    for (int i = 0; i < 3; i++) {
        if (transferFunctions[i]) cmsFreeToneCurve(transferFunctions[i]);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}