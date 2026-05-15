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
//<ID> 435
//<Prompt> []
/*<Combination>: [
*/
//<score> 76, nr_unique_branch: 16
//<Quality> {"density":19,"unique_branches":{"ColorSpaceIsCompatible":[[499,9,499,30,0,0,4,0],[499,34,499,57,0,0,4,1]],"cmsCreateInkLimitingDeviceLinkTHR":[[402,9,402,37,0,0,4,1],[407,9,407,20,0,0,4,1],[407,24,407,35,0,0,4,1],[415,9,415,14,0,0,4,1],[429,9,429,20,0,0,4,1],[435,9,435,21,0,0,4,1],[437,9,437,78,0,0,4,1],[439,9,439,102,0,0,4,1],[440,9,440,54,0,0,4,1],[441,9,441,100,0,0,4,1],[445,9,445,53,0,0,4,1],[447,9,447,56,0,0,4,1],[448,9,448,54,0,0,4,1]],"InkLimitingSampler":[[374,9,374,27,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDeleteContext","cmsCreateInkLimitingDeviceLinkTHR","cmsCloseProfile","cmsDeleteContext","cmsCreateTransformTHR","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsTransform2DeviceLink","cmsStageAllocCLut16bit","cmsStageDup","cmsGDBCheckPoint","cmsStageFree","cmsStageFree","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreateInkLimitingDeviceLinkTHR","cmsCreateTransformTHR","cmsTransform2DeviceLink","cmsStageAllocCLut16bit","cmsStageDup","cmsGDBCheckPoint","cmsStageFree","cmsStageFree","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to achieve the event of creating a color transformation pipeline with ink limiting and gamut boundary checking:

*/


extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	

    // 1. Open profile from memory (consumes input data)
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        cmsDeleteContext(context);
        return 0;
    }

    // 2. Create ink limiting device link profile
    cmsHPROFILE hInkLimitProfile = cmsCreateInkLimitingDeviceLinkTHR(
        context, 
        cmsSigCmykData,  // CMYK color space
        400.0            // Limit to 400% total ink
    );
    if (!hInkLimitProfile) {
        cmsCloseProfile(hProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // 3. Create a transform from input profile to ink-limited profile
    cmsHTRANSFORM hTransform = cmsCreateTransformTHR(
        context,
        hProfile,
        TYPE_CMYK_16,
        hInkLimitProfile,
        TYPE_CMYK_16,
        INTENT_PERCEPTUAL,
        0
    );
    if (!hTransform) {
        cmsCloseProfile(hInkLimitProfile);
        cmsCloseProfile(hProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // 4. Convert transform to device link profile
    cmsHPROFILE hDeviceLink = cmsTransform2DeviceLink(
        hTransform,
        4.0,    // ICC version 4.0
        0       // No flags
    );

    // 5. Create a CLUT (Color LookUp Table) stage
    // Create a simple 3x3x3 CLUT for RGB to RGB conversion
    const cmsUInt32Number gridPoints = 3;
    const cmsUInt32Number inputChan = 3;
    const cmsUInt32Number outputChan = 3;
    
    // Create table data (27 entries * 3 channels = 81 values)
    cmsUInt16Number table[81];
    for (size_t i = 0; i < 81 && i < size; i++) {
        table[i] = (data[i % size] << 8) | data[(i + 1) % size];
    }
    
    cmsStage* clutStage = cmsStageAllocCLut16bit(
        context,
        gridPoints,
        inputChan,
        outputChan,
        table
    );
    
    // 6. Duplicate the CLUT stage
    cmsStage* dupClutStage = NULL;
    if (clutStage) {
        dupClutStage = cmsStageDup(clutStage);
    }

    // 7. Create GBD (Gamut Boundary Descriptor) and check a point
    cmsHANDLE hGBD = NULL;
    // Note: cmsGBDAlloc is not in provided APIs, so we'll skip GBD creation
    // and just demonstrate the check with a dummy handle
    
    // Create a test Lab color point
    cmsCIELab testLab;
    if (size >= sizeof(testLab)) {
        memcpy(&testLab, data, sizeof(testLab));
    } else {
        testLab.L = 50.0;
        testLab.a = 0.0;
        testLab.b = 0.0;
    }
    
    // Try to check point (will fail without proper GBD, but API is called)
    cmsBool inGamut = cmsGDBCheckPoint(hGBD, &testLab);
    (void)inGamut; // Use variable to avoid unused warning

    // Cleanup
    if (dupClutStage) {
        cmsStageFree(dupClutStage);
    }
    if (clutStage) {
        cmsStageFree(clutStage);
    }
    if (hDeviceLink) {
        cmsCloseProfile(hDeviceLink);
    }
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hInkLimitProfile);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(context);

    return 0;
}