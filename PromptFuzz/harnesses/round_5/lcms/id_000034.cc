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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"cmsDetectRGBProfileGamma":[[618,9,618,52,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsDeleteContext","cmsD50_XYZ","cmsXYZ2xyY","cmsCreateLab2ProfileTHR","cmsIT8Free","cmsDeleteContext","cmsMD5computeID","cmsCreateNULLProfile","cmsDetectRGBProfileGamma","cmsCloseProfile","cmsBuildGamma","cmsEstimateGamma","cmsFreeToneCurve","cmsIT8SetDataRowColDbl","cmsIT8GetPatchName","cmsCIECAM02Init","cmsCIECAM02Reverse","cmsCIECAM02Done","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsD50_XYZ","cmsXYZ2xyY","cmsCreateLab2ProfileTHR","cmsMD5computeID","cmsCreateNULLProfile","cmsDetectRGBProfileGamma","cmsCloseProfile","cmsBuildGamma","cmsEstimateGamma","cmsFreeToneCurve","cmsIT8SetDataRowColDbl","cmsIT8GetPatchName","cmsCIECAM02Init","cmsCIECAM02Reverse","cmsCIECAM02Done","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create IT8 handle from input data
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, size);
    if (!it8) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Create Lab profile with D50 white point
    const cmsCIEXYZ* d50_xyz = cmsD50_XYZ();
    cmsCIExyY d50_xyY;
    cmsXYZ2xyY(&d50_xyY, d50_xyz);
    
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(context, &d50_xyY);
    if (!labProfile) {
        cmsIT8Free(it8);
        cmsDeleteContext(context);
        return 0;
    }
    
    // 3. Compute MD5 for the Lab profile
    cmsMD5computeID(labProfile);
    
    // 4. Detect RGB profile gamma (using NULL profile as RGB)
    cmsHPROFILE nullProfile = cmsCreateNULLProfile();
    if (nullProfile) {
        cmsDetectRGBProfileGamma(nullProfile, 0.001);
        cmsCloseProfile(nullProfile);
    }
    
    // 5. Create a tone curve and estimate gamma
    cmsFloat64Number gammaParams[1] = {2.2};
    cmsToneCurve* toneCurve = cmsBuildGamma(context, 2.2);
    if (toneCurve) {
        cmsEstimateGamma(toneCurve, 0.001);
        cmsFreeToneCurve(toneCurve);
    }
    
    // 6. Set IT8 data (using sample data from input)
    if (size > 20) {
        char patchName[20];
        char sampleName[20];
        char value[20];
        
        // Extract strings from input data
        size_t offset = 0;
        size_t len = (size - offset > 19) ? 19 : size - offset;
        memcpy(patchName, data + offset, len);
        patchName[len] = '\0';
        offset += len;
        
        if (offset < size) {
            len = (size - offset > 19) ? 19 : size - offset;
            memcpy(sampleName, data + offset, len);
            sampleName[len] = '\0';
            offset += len;
        }
        
        if (offset < size) {
            len = (size - offset > 19) ? 19 : size - offset;
            memcpy(value, data + offset, len);
            value[len] = '\0';
        }
        
        // Use cmsIT8SetData if available, otherwise use cmsIT8SetDataRowCol
        // Note: cmsIT8SetData is not in the provided API list, so we'll use
        // cmsIT8SetDataRowColDbl as an alternative
        cmsIT8SetDataRowColDbl(it8, 0, 0, 1.0);
    }
    
    // 7. Get patch name
    char buffer[256];
    const char* patchName = cmsIT8GetPatchName(it8, 0, buffer);
    
    // 8. CIECAM02 reverse transform
    cmsViewingConditions vc;
    memset(&vc, 0, sizeof(vc));
    vc.whitePoint.X = d50_xyz->X;
    vc.whitePoint.Y = d50_xyz->Y;
    vc.whitePoint.Z = d50_xyz->Z;
    vc.La = 100.0; // Adapting luminance
    vc.Yb = 20.0; // Background luminance
    vc.surround = 1.0; // Average surround
    
    cmsHANDLE cam02 = cmsCIECAM02Init(context, &vc);
    if (cam02) {
        cmsJCh jch;
        jch.J = 50.0; // Lightness
        jch.C = 30.0; // Chroma
        jch.h = 180.0; // Hue
        
        cmsCIEXYZ xyzOut;
        cmsCIECAM02Reverse(cam02, &jch, &xyzOut);
        cmsCIECAM02Done(cam02);
    }
    
    // Cleanup
    cmsIT8Free(it8);
    cmsCloseProfile(labProfile);
    cmsDeleteContext(context);
    
    return 0;
}