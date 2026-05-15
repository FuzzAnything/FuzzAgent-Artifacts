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
//<ID> 341
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 5
//<Quality> {"density":11,"unique_branches":{"IsAvailableOnList":[[1309,9,1309,20,0,0,4,1],[1312,12,1312,21,0,0,4,0],[1312,12,1312,21,0,0,4,1],[1314,13,1314,31,0,0,4,0]],"cmsIT8GetPropertyMulti":[[1612,9,1612,72,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreateLab4Profile","cmsDeleteContext","cmsBuildParametricToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext","cmsCreateRGBProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext","cmsIT8LoadFromMem","cmsIT8SetPropertyHex","cmsIT8GetPropertyMulti","cmsStageAllocCLut16bitGranular","cmsStageSampleCLut16bit","cmsFloat2LabEncoded","cmsGetColorSpace","cmsStageFree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateLab4Profile","cmsBuildParametricToneCurve","cmsCreateRGBProfile","cmsIT8LoadFromMem","cmsIT8SetPropertyHex","cmsIT8GetPropertyMulti","cmsStageAllocCLut16bitGranular","cmsStageSampleCLut16bit","cmsFloat2LabEncoded","cmsGetColorSpace","cmsStageFree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


// Sampler callback for cmsStageSampleCLut16bit
static int  id_000054_test_sampler(const cmsUInt16Number* in, cmsUInt16Number* out, void* cargo) {
    // Simple passthrough sampler
    for (int i = 0; i < 3; i++) {
        out[i] = in[i];
    }
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Create a Lab profile
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    cmsHPROFILE labProfile = cmsCreateLab4Profile(&whitePoint);
    if (!labProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create tone curves for linearization device link
    cmsToneCurve* toneCurves[3];
    for (int i = 0; i < 3; i++) {
        double params[5] = {2.4, 1.0/1.055, 0.055/1.055, 1/12.92, 0.04045};
        toneCurves[i] = cmsBuildParametricToneCurve(context, 4, params);
        if (!toneCurves[i]) {
            for (int j = 0; j < i; j++) cmsFreeToneCurve(toneCurves[j]);
            cmsCloseProfile(labProfile);
            cmsDeleteContext(context);
            return 0;
        }
    }
    
    // Try to create linearization device link (note: this API might not exist in all versions)
    // We'll use cmsCreateRGBProfile instead as fallback
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, NULL, toneCurves);
    if (!rgbProfile) {
        for (int i = 0; i < 3; i++) cmsFreeToneCurve(toneCurves[i]);
        cmsCloseProfile(labProfile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Load IT8 data from input
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, size);
    if (it8) {
        // Set property using hex value from input
        cmsUInt32Number hexValue = 0;
        if (size >= sizeof(hexValue)) {
            memcpy(&hexValue, data, sizeof(hexValue));
            cmsIT8SetPropertyHex(it8, "TestProperty", hexValue);
        }
        
        // Get property multi
        const char* multiProp = cmsIT8GetPropertyMulti(it8, "Test", "SubKey");
        (void)multiProp; // Use variable to avoid unused warning
    }
    
    // Create a CLUT stage
    cmsUInt32Number clutPoints[] = {5, 5, 5};
    cmsStage* stage = cmsStageAllocCLut16bitGranular(context, clutPoints, 3, 3, NULL);
    if (stage) {
        // Sample the CLUT
        cmsStageSampleCLut16bit(stage,  id_000054_test_sampler, NULL, 0);
    }
    
    // Convert Lab values
    cmsCIELab lab;
    lab.L = 50.0;
    lab.a = 0.0;
    lab.b = 0.0;
    cmsUInt16Number encodedLab[3];
    cmsFloat2LabEncoded(encodedLab, &lab);
    
    // Check color space (internal function - we'll use cmsGetColorSpace instead)
    cmsColorSpaceSignature colorspace = cmsGetColorSpace(rgbProfile);
    (void)colorspace; // Use variable
    
    // Cleanup
    if (stage) cmsStageFree(stage);
    for (int i = 0; i < 3; i++) {
        if (toneCurves[i]) cmsFreeToneCurve(toneCurves[i]);
    }
    if (rgbProfile) cmsCloseProfile(rgbProfile);
    if (labProfile) cmsCloseProfile(labProfile);
    if (it8) cmsIT8Free(it8);
    cmsDeleteContext(context);
    
    return 0;
}