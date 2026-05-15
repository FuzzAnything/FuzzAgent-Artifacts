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
//<ID> 372
//<Prompt> []
/*<Combination>: [
*/
//<score> 96, nr_unique_branch: 4
//<Quality> {"density":24,"unique_branches":{"EmitIntent":[[396,9,396,31,0,0,4,1],[397,9,397,42,0,0,4,0]],"cmsAppendNamedColor":[[822,9,822,31,0,0,4,0]],"cmsGetTransformOutputFormat":[[1440,9,1440,22,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreateLab2ProfileTHR","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyStr","cmsAppendNamedColor","cmsGetPostScriptCSA","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsCreateLab2ProfileTHR","cmsCreateTransform","cmsGetTransformOutputFormat","cmsDeleteTransform","cmsCloseProfile","cmsDetectBlackPoint","cmsIT8GetPatchByName","cmsBuildGamma","cmsReverseToneCurveEx","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreateLab2ProfileTHR","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyStr","cmsAppendNamedColor","cmsGetPostScriptCSA","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsCreateLab2ProfileTHR","cmsCreateTransform","cmsGetTransformOutputFormat","cmsDeleteTransform","cmsCloseProfile","cmsDetectBlackPoint","cmsIT8GetPatchByName","cmsBuildGamma","cmsReverseToneCurveEx","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs together to achieve an event involving color profile processing and IT8 data handling:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 1. Open profile from input data (first part)
    size_t profile_size = size / 2;
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, profile_size);
    if (!hProfile) {
        // Create a dummy profile if input doesn't contain valid profile
        hProfile = cmsCreateLab2ProfileTHR(context, nullptr);
    }

    // 2. Create IT8 handle and load data (second part)
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    const uint8_t* it8_data = data + profile_size;
    size_t it8_data_size = size - profile_size;
    
    // Create IT8 structure with some properties
    cmsIT8SetSheetType(hIT8, "COLORANT");
    cmsIT8SetPropertyStr(hIT8, "DESCRIPTOR", "FuzzTest");
    
    // Add sample data from input
    char patch_name[32];
    for (int i = 0; i < 10 && (size_t)i * 10 < it8_data_size; i++) {
        snprintf(patch_name, sizeof(patch_name), "Patch%d", i);
        cmsUInt16Number pcs[3] = {0, 0, 0};
        cmsUInt16Number colorant[16] = {0};
        if (it8_data_size > (size_t)i * 10 + 6) {
            pcs[0] = (it8_data[i*10] << 8) | it8_data[i*10+1];
            pcs[1] = (it8_data[i*10+2] << 8) | it8_data[i*10+3];
            pcs[2] = (it8_data[i*10+4] << 8) | it8_data[i*10+5];
        }
        cmsAppendNamedColor(nullptr, patch_name, pcs, colorant);
    }

    // 3. Get PostScript CSA
    cmsUInt32Number intent = INTENT_RELATIVE_COLORIMETRIC;
    cmsUInt32Number buffer_len = 4096;
    uint8_t* buffer = (uint8_t*)malloc(buffer_len);
    if (buffer) {
        cmsUInt32Number csa_len = cmsGetPostScriptCSA(context, hProfile, intent, 0, buffer, buffer_len);
        // Can check csa_len for validity
        free(buffer);
    }

    // 4. Save IT8 to memory
    cmsUInt32Number bytes_needed = 0;
    cmsIT8SaveToMem(hIT8, nullptr, &bytes_needed);
    if (bytes_needed > 0) {
        void* it8_buffer = malloc(bytes_needed);
        if (it8_buffer) {
            cmsIT8SaveToMem(hIT8, it8_buffer, &bytes_needed);
            free(it8_buffer);
        }
    }

    // 5. Create transform to get output format
    cmsHPROFILE lab_profile = cmsCreateLab2ProfileTHR(context, nullptr);
    if (lab_profile) {
        cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, TYPE_Lab_16, 
                                                      lab_profile, TYPE_Lab_16, 
                                                      INTENT_PERCEPTUAL, 0);
        if (hTransform) {
            cmsUInt32Number output_format = cmsGetTransformOutputFormat(hTransform);
            // Use the output format
            (void)output_format;
            
            // Clean up transform
            cmsDeleteTransform(hTransform);
        }
        cmsCloseProfile(lab_profile);
    }

    // 6. Detect black point
    cmsCIEXYZ black_point;
    if (cmsDetectBlackPoint(&black_point, hProfile, INTENT_RELATIVE_COLORIMETRIC, 0)) {
        // Black point detected successfully
    }

    // 7. Get patch by name from IT8
    int patch_index = cmsIT8GetPatchByName(hIT8, "Patch0");
    // patch_index can be -1 if not found

    // 8. Create and reverse tone curve
    cmsToneCurve* gamma_curve = cmsBuildGamma(context, 2.2);
    if (gamma_curve) {
        // Create reverse tone curve
        cmsToneCurve* reverse_curve = cmsReverseToneCurveEx(256, gamma_curve);
        if (reverse_curve) {
            // Get estimated table from reverse curve
            const cmsUInt16Number* estimated_table = cmsGetToneCurveEstimatedTable(reverse_curve);
            // estimated_table can be used or checked
            
            // Clean up
            cmsFreeToneCurve(reverse_curve);
        }
        cmsFreeToneCurve(gamma_curve);
    }

    // Clean up all allocated resources
    cmsIT8Free(hIT8);
    if (hProfile) cmsCloseProfile(hProfile);
    cmsDeleteContext(context);

    return 0;
}