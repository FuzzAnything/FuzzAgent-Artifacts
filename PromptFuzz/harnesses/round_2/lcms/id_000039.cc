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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsIT8LoadFromFile","cmsIT8Alloc","cmsCreateNULLProfileTHR","cmsIT8Free","cmsDeleteContext","cmsBuildTabulatedToneCurve16","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsGetToneCurveSegment","cmsBuildSegmentedToneCurve","cmsFreeToneCurve","cmsSliceSpace16","cmsGetPostScriptCRD","cmsCIE94DeltaE","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsSaveProfileToStream","cmsFreeToneCurve","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromFile","cmsIT8Alloc","cmsCreateNULLProfileTHR","cmsBuildTabulatedToneCurve16","cmsGetToneCurveSegment","cmsBuildSegmentedToneCurve","cmsFreeToneCurve","cmsSliceSpace16","cmsGetPostScriptCRD","cmsCIE94DeltaE","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsSaveProfileToStream","cmsFreeToneCurve","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Minimal size check

    // 1. Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // 2. Write input data to file for IT8 loading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Try to load IT8 data from file
    cmsHANDLE it8 = cmsIT8LoadFromFile(ctx, "input_file");
    if (!it8) {
        // If loading fails, allocate new IT8 handle
        it8 = cmsIT8Alloc(ctx);
    }

    // 4. Create NULL profile
    cmsHPROFILE null_profile = cmsCreateNULLProfileTHR(ctx);
    if (!null_profile) {
        cmsIT8Free(it8);
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 5. Build tabulated tone curve
    const size_t curve_entries = 256;
    cmsUInt16Number curve_values[curve_entries];
    size_t data_offset = 0;
    
    // Use input data for curve values (with bounds checking)
    for (size_t i = 0; i < curve_entries; i++) {
        if (data_offset + 1 < size) {
            curve_values[i] = (data[data_offset] << 8) | data[data_offset + 1];
            data_offset += 2;
        } else {
            curve_values[i] = i * 257;  // Default linear curve
        }
    }
    
    cmsToneCurve *tab_curve = cmsBuildTabulatedToneCurve16(ctx, curve_entries, curve_values);
    if (!tab_curve) {
        cmsCloseProfile(null_profile);
        cmsIT8Free(it8);
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 6. Get segment from tabulated curve
    const cmsCurveSegment *segment = cmsGetToneCurveSegment(0, tab_curve);
    if (segment) {
        // 7. Build segmented tone curve using the segment
        cmsCurveSegment segments[1];
        memcpy(&segments[0], segment, sizeof(cmsCurveSegment));
        cmsToneCurve *seg_curve = cmsBuildSegmentedToneCurve(ctx, 1, segments);
        if (seg_curve) {
            cmsFreeToneCurve(seg_curve);
        }
    }

    // 8. Prepare for cmsSliceSpace16
    cmsUInt32Number clutPoints[3] = {5, 5, 5};  // 5x5x5 CLUT
    size_t total_points = clutPoints[0] * clutPoints[1] * clutPoints[2];
    
    // Simple sampler that does nothing
    cmsSAMPLER16 sampler = [](const cmsUInt16Number* in, cmsUInt16Number* out, void* cargo) {
        return 1;  // Success
    };
    
    cmsSliceSpace16(3, clutPoints, sampler, nullptr);

    // 9. Get PostScript CRD from null profile
    cmsUInt32Number buffer_size = 4096;
    char *ps_buffer = (char*)malloc(buffer_size);
    if (ps_buffer) {
        cmsUInt32Number written = cmsGetPostScriptCRD(ctx, null_profile, 
                                                      INTENT_PERCEPTUAL, 0, 
                                                      ps_buffer, buffer_size);
        free(ps_buffer);
    }

    // 10. Calculate CIE94 DeltaE between two Lab colors
    cmsCIELab lab1, lab2;
    
    // Extract Lab values from input data (with bounds checking)
    if (data_offset + 47 < size) {
        // Use data for Lab values (simplified - just copying bytes)
        memcpy(&lab1, data + data_offset, sizeof(cmsCIELab));
        memcpy(&lab2, data + data_offset + sizeof(cmsCIELab), sizeof(cmsCIELab));
        data_offset += 2 * sizeof(cmsCIELab);
    } else {
        // Default values if insufficient data
        lab1.L = 50.0; lab1.a = 0.0; lab1.b = 0.0;
        lab2.L = 60.0; lab2.a = 10.0; lab2.b = 10.0;
    }
    
    cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);

    // 11. Create transform and perform transformation
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, 
                                                    null_profile, 
                                                    TYPE_Lab_16, 
                                                    null_profile, 
                                                    TYPE_Lab_16, 
                                                    INTENT_PERCEPTUAL, 0);
    
    if (transform) {
        // Prepare input/output buffers
        const size_t pixel_count = 16;
        cmsUInt16Number input[pixel_count * 3];
        cmsUInt16Number output[pixel_count * 3];
        
        // Fill input with data (with bounds checking)
        for (size_t i = 0; i < pixel_count * 3; i++) {
            if (data_offset + 1 < size) {
                input[i] = (data[data_offset] << 8) | data[data_offset + 1];
                data_offset += 2;
            } else {
                input[i] = 0x8000;  // Midpoint for Lab
            }
        }
        
        // Perform transformation
        cmsDoTransform(transform, input, output, pixel_count);
        
        // Delete transform
        cmsDeleteTransform(transform);
    }

    // 12. Save profile to stream
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        cmsSaveProfileToStream(null_profile, out_file);
        assert_file_closed(&out_file);;
    }

    // 13. Cleanup
    cmsFreeToneCurve(tab_curve);
    cmsCloseProfile(null_profile);
    cmsIT8Free(it8);
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}