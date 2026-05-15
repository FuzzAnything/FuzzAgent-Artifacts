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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 49, nr_unique_branch: 6
//<Quality> {"density":14,"unique_branches":{"CubeSize":[[480,9,480,27,0,0,4,0]],"CLutElemTypeFree":[[538,9,538,22,0,0,4,1]],"cmsStageAllocCLut16bitGranular":[[581,9,581,15,0,0,4,0]],"cmsCreateBCHSWabstractProfileTHR":[[914,9,914,21,0,0,4,0]],"_cmsFreeInterpParams":[[179,9,179,18,0,0,4,1]],"InverseCorrelates":[[269,13,269,43,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsD50_XYZ","cmsXYZ2xyY","cmsCreateLab2ProfileTHR","cmsMD5computeID","cmsCreateNULLProfile","cmsSaveProfileToStream","cmsBuildGamma","cmsFreeToneCurve","cmsIT8GetPatchName","cmsCIECAM02Init","cmsCIECAM02Reverse","cmsCreateBCHSWabstractProfileTHR","cmsIT8SetTableByLabel","cmsCIECAM02Done","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsD50_XYZ","cmsXYZ2xyY","cmsCreateLab2ProfileTHR","cmsMD5computeID","cmsCreateNULLProfile","cmsSaveProfileToStream","cmsBuildGamma","cmsFreeToneCurve","cmsIT8GetPatchName","cmsCIECAM02Init","cmsCIECAM02Reverse","cmsCreateBCHSWabstractProfileTHR","cmsIT8SetTableByLabel","cmsCIECAM02Done","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // 1. Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 2. Load IT8 table from memory (using input data)
    cmsHANDLE it8 = cmsIT8LoadFromMem(ctx, data, size);
    
    // 3. Get D50 white point and convert to xyY
    const cmsCIEXYZ* d50 = cmsD50_XYZ();
    cmsCIExyY whitePoint;
    cmsXYZ2xyY(&whitePoint, d50);
    
    // 4. Create Lab profile using the white point
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);
    
    // 5. Compute MD5 ID for the Lab profile
    if (labProfile) {
        cmsMD5computeID(labProfile);
    }
    
    // 6. Create NULL profile
    cmsHPROFILE nullProfile = cmsCreateNULLProfile();
    
    // 7. Save NULL profile to stream (using output file)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file && nullProfile) {
        cmsSaveProfileToStream(nullProfile, out_file);
        assert_file_closed(&out_file);;
    } else {
        if (out_file) assert_file_closed(&out_file);;
    }
    
    // 8. Build gamma curve and free it
    cmsToneCurve* gammaCurve = cmsBuildGamma(ctx, 2.2);
    if (gammaCurve) {
        cmsFreeToneCurve(gammaCurve);
    }
    
    // 9. Get patch name from IT8 table if available
    if (it8) {
        char buffer[256];
        const char* patchName = cmsIT8GetPatchName(it8, 0, buffer);
        // Use patchName to avoid unused variable warning
        (void)patchName;
    }
    
    // 10. Initialize CIECAM02 model with viewing conditions
    cmsViewingConditions vc;
    vc.whitePoint = *d50;
    vc.Yb = 20.0;
    vc.La = 100.0;
    vc.surround = 1;
    vc.D_value = 1.0;
    
    cmsHANDLE cam02 = cmsCIECAM02Init(ctx, &vc);
    
    // 11. Use CIECAM02 to reverse transform a color
    if (cam02) {
        cmsJCh jch = {50.0, 30.0, 120.0};  // Sample color values
        cmsCIEXYZ xyz;
        cmsCIECAM02Reverse(cam02, &jch, &xyz);
    }
    
    // 12. Create BCHSW abstract profile
    cmsHPROFILE bchswProfile = cmsCreateBCHSWabstractProfileTHR(
        ctx, 1024, 1.0, 1.0, 0.0, 1.0, 5000, 6500);
    
    // 13. Set table by label for IT8 if available
    if (it8) {
        cmsIT8SetTableByLabel(it8, "0", "0", "text");
    }
    
    // 14. Clean up all resources
    if (cam02) {
        cmsCIECAM02Done(cam02);
    }
    
    if (it8) {
        cmsIT8Free(it8);
    }
    
    if (bchswProfile) {
        cmsCloseProfile(bchswProfile);
    }
    
    if (nullProfile) {
        cmsCloseProfile(nullProfile);
    }
    
    if (labProfile) {
        cmsCloseProfile(labProfile);
    }
    
    cmsDeleteContext(ctx);
    
    assert_file_closed(&out_file);
	return 0;
}