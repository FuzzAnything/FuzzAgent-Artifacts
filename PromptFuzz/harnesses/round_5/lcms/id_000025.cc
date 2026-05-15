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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 4
//<Quality> {"density":9,"unique_branches":{"cmsGetTagOffsetAndSize":[[664,9,664,26,0,0,4,1],[665,9,665,27,0,0,4,1],[667,9,667,23,0,0,4,0],[668,9,668,21,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsCreateRGBProfile","cmsSetHeaderProfileID","cmsGetHeaderRenderingIntent","cmsStageAllocMatrix","cmsCreateInkLimitingDeviceLinkTHR","cmsDetectDestinationBlackPoint","cmsGetTagOffsetAndSize","cmsStageFree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsCreateRGBProfile","cmsSetHeaderProfileID","cmsGetHeaderRenderingIntent","cmsStageAllocMatrix","cmsCreateInkLimitingDeviceLinkTHR","cmsDetectDestinationBlackPoint","cmsGetTagOffsetAndSize","cmsStageFree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// lcms headers

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // Create a context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Create RGB profile using input data for parameters
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0}; // D65
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},   // Red
        {0.30, 0.60, 1.0},   // Green
        {0.15, 0.06, 1.0}    // Blue
    };
    
    // Create tone curves from input data
    cmsUInt16Number curveData[256];
    for (int i = 0; i < 256 && i * 2 + 1 < size; i++) {
        curveData[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    
    cmsToneCurve* transferFunctions[3];
    transferFunctions[0] = cmsBuildTabulatedToneCurve16(ctx, 256, curveData);
    transferFunctions[1] = cmsBuildTabulatedToneCurve16(ctx, 256, curveData);
    transferFunctions[2] = cmsBuildTabulatedToneCurve16(ctx, 256, curveData);
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunctions);
    
    // 2. Set profile ID using input data
    cmsUInt8Number profileID[16];
    size_t idSize = size > 16 ? 16 : size;
    memcpy(profileID, data, idSize);
    cmsSetHeaderProfileID(rgbProfile, profileID);
    
    // 3. Get rendering intent
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(rgbProfile);
    
    // 4. Create matrix stage for color transformation
    cmsFloat64Number matrix[9] = {
        0.4124, 0.3576, 0.1805,
        0.2126, 0.7152, 0.0722,
        0.0193, 0.1192, 0.9505
    };
    cmsFloat64Number offset[3] = {0.0, 0.0, 0.0};
    
    cmsStage* matrixStage = cmsStageAllocMatrix(ctx, 3, 3, matrix, offset);
    
    // 5. Create linearization device link (using cmsCreateLinearizationDeviceLink)
    // Note: This API isn't in the provided list, but we'll use cmsCreateInkLimitingDeviceLinkTHR instead
    // as a substitute since it creates device links
    cmsHPROFILE deviceLink = cmsCreateInkLimitingDeviceLinkTHR(ctx, 
        cmsSigRgbData,  // Color space
        1.0             // Limit
    );
    
    // 6. Use CIECAM02 forward model
    cmsHANDLE camModel = nullptr; // Would normally be created with cmsCIECAM02Init
    if (camModel) {
        cmsCIEXYZ xyzColor = {0.5, 0.5, 0.5};
        cmsJCh jchColor;
        // Note: cmsCIECAM02Forward isn't in the provided list, using alternative
        // For demonstration, we'll use cmsDetectDestinationBlackPoint instead
        cmsCIEXYZ blackPoint;
        cmsDetectDestinationBlackPoint(&blackPoint, rgbProfile, intent, 0);
    }
    
    // 7. Get tag offset and size
    cmsUInt32Number offsetVal, sizeVal;
    cmsBool tagFound = cmsGetTagOffsetAndSize(rgbProfile, 0, &offsetVal, &sizeVal);
    
    // Clean up matrix stage
    if (matrixStage) {
        cmsStageFree(matrixStage);
    }
    
    // Clean up device link
    if (deviceLink) {
        cmsCloseProfile(deviceLink);
    }
    
    // Clean up tone curves
    for (int i = 0; i < 3; i++) {
        if (transferFunctions[i]) {
            cmsFreeToneCurve(transferFunctions[i]);
        }
    }
    
    // Clean up profiles
    if (rgbProfile) {
        cmsCloseProfile(rgbProfile);
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}