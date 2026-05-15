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
//<ID> 223
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"Clamp_L_doubleV4":[[272,9,272,18,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsD50_XYZ","cmsXYZ2xyY","cmsCreateLab2ProfileTHR","cmsMD5computeID","cmsCreateNULLProfile","cmsSaveProfileToStream","cmsCloseProfile","cmsCloseProfile","cmsBuildGamma","cmsFreeToneCurve","cmsIT8GetPatchName","cmsIT8SetTableByLabel","cmsCIECAM02Init","cmsCIECAM02Reverse","cmsCreateBCHSWabstractProfileTHR","cmsCloseProfile","cmsCIECAM02Done","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsD50_XYZ","cmsXYZ2xyY","cmsCreateLab2ProfileTHR","cmsMD5computeID","cmsCreateNULLProfile","cmsSaveProfileToStream","cmsCloseProfile","cmsCloseProfile","cmsBuildGamma","cmsFreeToneCurve","cmsIT8GetPatchName","cmsIT8SetTableByLabel","cmsCIECAM02Init","cmsCIECAM02Reverse","cmsCreateBCHSWabstractProfileTHR","cmsCloseProfile","cmsCIECAM02Done","cmsIT8Free","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Need minimum data for operations

    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 2. Load IT8 table from memory (first half of data)
    size_t it8_size = size / 2;
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, it8_size);
    
    // 3. Get D50 white point and convert to xyY
    const cmsCIEXYZ* d50_xyz = cmsD50_XYZ();
    cmsCIExyY white_point;
    cmsXYZ2xyY(&white_point, d50_xyz);
    
    // 4. Create Lab profile
    cmsHPROFILE lab_profile = cmsCreateLab2ProfileTHR(context, &white_point);
    
    // 5. Compute MD5 for Lab profile
    cmsMD5computeID(lab_profile);
    
    // 6. Create NULL profile
    cmsHPROFILE null_profile = cmsCreateNULLProfile();
    
    // 7. Save NULL profile to stream using input data as file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Use a temporary output file
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            cmsSaveProfileToStream(null_profile, out_file);
            assert_file_closed(&out_file);;
        }
        assert_file_closed(&in_file);;
    }
    
    // 8. Close profiles
    cmsCloseProfile(lab_profile);
    cmsCloseProfile(null_profile);
    
    // 9. Create and free gamma curve
    cmsToneCurve* gamma_curve = cmsBuildGamma(context, 2.2);
    if (gamma_curve) {
        cmsFreeToneCurve(gamma_curve);
    }
    
    // 10. Get patch name from IT8 if available
    if (it8) {
        char buffer[256];
        const char* patch_name = cmsIT8GetPatchName(it8, 0, buffer);
        
        // 11. Set IT8 table by label (using second half of data as string)
        if (size > it8_size + 1) {
            const char* field_name = (const char*)(data + it8_size);
            size_t field_len = strnlen(field_name, size - it8_size);
            if (field_len > 0) {
                cmsIT8SetTableByLabel(it8, "Data", field_name, "string");
            }
        }
    }
    
    // 12. Initialize CIECAM02 model
    cmsViewingConditions vc;
    memset(&vc, 0, sizeof(vc));
    vc.whitePoint = *d50_xyz;
    vc.Yb = 20.0;
    vc.La = 64.0;
    vc.surround = 1;
    vc.D_value = 1.0;
    
    cmsHANDLE ciecam = cmsCIECAM02Init(context, &vc);
    
    // 13. Perform CIECAM02 reverse transform
    if (ciecam) {
        cmsJCh jch;
        cmsCIEXYZ xyz_out;
        jch.J = 50.0;
        jch.C = 30.0;
        jch.h = 180.0;
        cmsCIECAM02Reverse(ciecam, &jch, &xyz_out);
    }
    
    // 14. Create BCHSW abstract profile
    cmsHPROFILE bchsw_profile = cmsCreateBCHSWabstractProfileTHR(
        context, 
        256,      // nLUTPoints
        1.0,      // Bright
        1.0,      // Contrast
        0.0,      // Hue
        1.0,      // Saturation
        5000,     // TempSrc
        6500      // TempDest
    );
    
    // 15. Cleanup
    if (bchsw_profile) {
        cmsCloseProfile(bchsw_profile);
    }
    
    if (ciecam) {
        cmsCIECAM02Done(ciecam);
    }
    
    if (it8) {
        cmsIT8Free(it8);
    }
    
    cmsDeleteContext(context);
    
    // Clean up temporary output file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}