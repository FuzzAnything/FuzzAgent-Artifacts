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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 105, nr_unique_branch: 31
//<Quality> {"density":15,"unique_branches":{"cmsEstimateGamma":[[1484,13,1484,19,0,0,4,1],[1484,23,1484,29,0,0,4,1],[1494,9,1494,15,0,0,4,0],[1499,9,1499,24,0,0,4,0]],"cmsCreateExtendedTransform":[[1185,74,1185,106,0,0,4,0],[1189,13,1189,22,0,0,4,0],[1189,13,1189,22,0,0,4,1],[1189,26,1189,37,0,0,4,0],[1189,26,1189,37,0,0,4,1]],"FastEvaluateCurves16":[[1375,15,1375,33,0,0,4,0],[1375,15,1375,33,0,0,4,1]],"cmsDetectRGBProfileGamma":[[618,9,618,52,0,0,4,1],[622,9,622,31,0,0,4,0],[622,35,622,59,0,0,4,0],[622,35,622,59,0,0,4,1],[623,9,623,32,0,0,4,0],[623,36,623,63,0,0,4,0],[623,36,623,63,0,0,4,1],[628,9,628,21,0,0,4,1],[633,9,633,22,0,0,4,0],[633,9,633,22,0,0,4,1],[639,17,639,24,0,0,4,0],[639,17,639,24,0,0,4,1],[648,17,648,24,0,0,4,0],[648,17,648,24,0,0,4,1],[653,9,653,24,0,0,4,1]],"OptimizeByJoiningCurves":[[1467,13,1467,46,0,0,4,1],[1478,18,1478,29,0,0,4,1]],"CurvesAlloc":[[1332,13,1332,30,0,0,4,1],[1341,23,1341,36,0,0,4,0],[1341,23,1341,36,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsWhitePointFromTemp","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsIT8GetPatchName","cmsTagLinkedTo","cmsBuildGamma","cmsIsToneCurveDescending","cmsEvalToneCurve16","cmsReadTag","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsWhitePointFromTemp","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsIT8GetPatchName","cmsTagLinkedTo","cmsBuildGamma","cmsIsToneCurveDescending","cmsEvalToneCurve16","cmsReadTag","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    cmsContext context = nullptr;
    cmsHPROFILE profile = nullptr;
    cmsHANDLE it8_handle = nullptr;
    cmsHTRANSFORM transform = nullptr;
    cmsToneCurve* tone_curve = nullptr;
    cmsNAMEDCOLORLIST* named_color_list = nullptr;
    cmsCIExyY white_point;
    
    // 1. Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Open profile from memory
    profile = cmsOpenProfileFromMemTHR(context, data, size);
    
    if (profile) {
        // 3. Create white point from temperature (use first 4 bytes as temperature)
        float temp = *reinterpret_cast<const float*>(data);
        if (temp < 1000.0f) temp = 1000.0f;
        if (temp > 25000.0f) temp = 25000.0f;
        cmsWhitePointFromTemp(&white_point, static_cast<cmsFloat64Number>(temp));
        
        // 4. Load IT8 from memory (using same data)
        it8_handle = cmsIT8LoadFromMem(context, data, size);
        
        if (it8_handle) {
            // 5. Find data format
            const char* sample_name = "Sample";
            int format = cmsIT8FindDataFormat(it8_handle, sample_name);
            
            // 6. Get patch name if available
            char buffer[256];
            if (format >= 0) {
                const char* patch_name = cmsIT8GetPatchName(it8_handle, 0, buffer);
            }
        }
        
        // 7. Check tag linking
        cmsTagSignature linked_tag = cmsTagLinkedTo(profile, cmsSigRedTRCTag);
        
        // 8. Build gamma curve
        tone_curve = cmsBuildGamma(context, 2.2);
        
        if (tone_curve) {
            // 9. Check if tone curve is descending
            cmsBool is_descending = cmsIsToneCurveDescending(tone_curve);
            
            // 10. Evaluate tone curve
            cmsUInt16Number eval_result = cmsEvalToneCurve16(tone_curve, 128);
        }
        
        // 11. Read tag
        void* tag_data = cmsReadTag(profile, cmsSigCopyrightTag);
        
        // 12. Get named color list (if any)
        named_color_list = cmsGetNamedColorList(nullptr);
        if (named_color_list) {
            // 13. Get named color count
            cmsUInt32Number color_count = cmsNamedColorCount(named_color_list);
            
            if (color_count > 0) {
                char name[256], prefix[256], suffix[256];
                cmsUInt16Number pcs[3], colorant[16];
                
                // 14. Get named color info
                cmsBool info_result = cmsNamedColorInfo(
                    named_color_list, 0, 
                    name, prefix, suffix, 
                    pcs, colorant
                );
            }
        }
        
        // 15. Create transform if we have valid profile
        cmsUInt32Number input_format = TYPE_RGB_16;
        cmsUInt32Number output_format = TYPE_RGB_16;
        cmsUInt32Number intent = INTENT_PERCEPTUAL;
        cmsUInt32Number flags = 0;
        
        transform = cmsCreateTransform(
            profile, input_format,
            profile, output_format,
            intent, flags
        );
        
        if (transform) {
            // 16. Do transform
            cmsUInt16Number input_buffer[3] = {0x8000, 0x8000, 0x8000};
            cmsUInt16Number output_buffer[3];
            cmsDoTransform(transform, input_buffer, output_buffer, 1);
            
            // 17. Delete transform
            cmsDeleteTransform(transform);
        }
        
        // 18. Close profile
        cmsCloseProfile(profile);
    }
    
    // 19. Clean up resources
    if (it8_handle) {
        // Note: cmsIT8Free is not in the provided list, so we'll just set to nullptr
        // In real usage, we would need to free IT8 handle properly
    }
    
    if (tone_curve) {
        cmsFreeToneCurve(tone_curve);
    }
    
    // 20. Delete context
    cmsDeleteContext(context);
    
    return 0;
}