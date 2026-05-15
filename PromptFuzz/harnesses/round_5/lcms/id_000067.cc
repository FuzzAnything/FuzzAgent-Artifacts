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
//<ID> 432
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 3
//<Quality> {"density":7,"unique_branches":{"cmsOpenIOhandlerFromMem":[[297,13,297,27,0,0,4,0],[328,9,328,11,0,0,4,0],[329,9,329,18,0,0,4,0]]},"library_calls":["cmsOpenIOhandlerFromMem","cmsD50_xyY","cmsCreateLab2Profile","cmsDetectBlackPoint","cmsSetAdaptationState","cmsGBDAlloc","cmsGDBAddPoint","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8EnumDataFormat","cmsBuildGamma","cmsGetToneCurveSegment","cmsFreeToneCurve","cmsSliceSpaceFloat","cmsGetPostScriptColorResource","cmsIT8Free","cmsGBDFree","cmsCloseProfile","cmsCloseIOhandler"],"critical_calls":["cmsOpenIOhandlerFromMem","cmsD50_xyY","cmsCreateLab2Profile","cmsDetectBlackPoint","cmsSetAdaptationState","cmsGBDAlloc","cmsGDBAddPoint","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8EnumDataFormat","cmsBuildGamma","cmsGetToneCurveSegment","cmsFreeToneCurve","cmsSliceSpaceFloat","cmsGetPostScriptColorResource","cmsIT8Free","cmsGBDFree","cmsCloseProfile","cmsCloseIOhandler"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check minimum size needed for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    // Open input data as a FILE stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for PostScript resource
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create I/O handler for PostScript output
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromMem(NULL, NULL, 0, "w");
    if (!io_handler) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create a Lab profile with D50 white point
    const cmsCIExyY *white_point = cmsD50_xyY();
    cmsHPROFILE lab_profile = cmsCreateLab2Profile(white_point);
    
    // 2. Detect black point of the Lab profile
    cmsCIEXYZ black_point;
    cmsDetectBlackPoint(&black_point, lab_profile, 0, 0);
    
    // 3. Set adaptation state (using first 8 bytes as double)
    double adaptation_value = 0.0;
    if (size >= sizeof(double)) {
        memcpy(&adaptation_value, data, sizeof(double));
    }
    cmsSetAdaptationState(adaptation_value);
    
    // 4. Create GDB and add a point (using next 24 bytes for Lab coordinates)
    cmsHANDLE gdb_handle = cmsGBDAlloc(NULL);
    if (gdb_handle && size >= sizeof(double) + sizeof(cmsCIELab)) {
        cmsCIELab lab_point;
        memcpy(&lab_point, data + sizeof(double), sizeof(cmsCIELab));
        cmsGDBAddPoint(gdb_handle, &lab_point);
    }
    
    // 5. Create IT8 handle and set properties
    cmsHANDLE it8_handle = cmsIT8Alloc(NULL);
    if (it8_handle) {
        // Set double property (using next 8 bytes after Lab)
        double property_value = 1.0;
        if (size >= sizeof(double) * 2 + sizeof(cmsCIELab)) {
            memcpy(&property_value, data + sizeof(double) + sizeof(cmsCIELab), sizeof(double));
        }
        cmsIT8SetPropertyDbl(it8_handle, "TestProperty", property_value);
        
        // Enumerate data formats
        char **sample_names = NULL;
        int format_count = cmsIT8EnumDataFormat(it8_handle, &sample_names);
    }
    
    // 6. Get tone curve segment from Lab profile
    cmsToneCurve *curve = cmsBuildGamma(NULL, 2.2);
    if (curve) {
        const cmsCurveSegment *segment = cmsGetToneCurveSegment(0, curve);
        cmsFreeToneCurve(curve);
    }
    
    // 7. Slice space float operation
    if (size >= 200) {
        cmsUInt32Number clutPoints[] = {3, 3, 3};
        
        // Define a simple sampler function
        auto sampler = [](const cmsFloat32Number *in, cmsFloat32Number *out, void *cargo) -> int {
            for (int i = 0; i < 3; i++) {
                out[i] = in[i] * 0.5f;
            }
            return 1;
        };
        
        // Use remaining data as cargo
        void *cargo = (void*)(data + 100);
        cmsSliceSpaceFloat(3, clutPoints, sampler, cargo);
    }
    
    // 8. Get PostScript color resource
    cmsGetPostScriptColorResource(NULL, cmsPS_RESOURCE_CSA, lab_profile, 0, 0, io_handler);
    
    // 9. Clean up all resources
    if (it8_handle) cmsIT8Free(it8_handle);
    if (gdb_handle) cmsGBDFree(gdb_handle);
    if (lab_profile) cmsCloseProfile(lab_profile);
    if (io_handler) cmsCloseIOhandler(io_handler);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}