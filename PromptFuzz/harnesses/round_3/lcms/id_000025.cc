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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsGetProfileIOhandler","cmsD50_xyY","cmsCreateLab4Profile","cmsCloseProfile","cmsDeleteContext","cmsCreateTransform","cmsDoTransformLineStride","cmsDeltaE","cmsDetectDestinationBlackPoint","cmsReadTag","cmsMLUgetASCII","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetDataRowColDbl","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromStreamTHR","cmsGetProfileIOhandler","cmsD50_xyY","cmsCreateLab4Profile","cmsCreateTransform","cmsDoTransformLineStride","cmsDeltaE","cmsDetectDestinationBlackPoint","cmsReadTag","cmsMLUgetASCII","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetDataRowColDbl","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful processing
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Set up error handler
    cmsSetLogErrorHandlerTHR(context, [](cmsContext, cmsUInt32Number, const char*) {});
    
    // Create input FILE from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open profile from stream
    cmsHPROFILE input_profile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!input_profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get profile IO handler
    cmsIOHANDLER* io_handler = cmsGetProfileIOhandler(input_profile);
    
    // Create Lab4 profile with D50 white point
    const cmsCIExyY* d50 = cmsD50_xyY();
    cmsHPROFILE lab_profile = cmsCreateLab4Profile(d50);
    if (!lab_profile) {
        cmsCloseProfile(input_profile);
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create transform
    cmsHTRANSFORM transform = cmsCreateTransform(
        input_profile, TYPE_Lab_DBL,
        lab_profile, TYPE_Lab_DBL,
        INTENT_PERCEPTUAL, 0
    );
    
    if (transform) {
        // Prepare sample buffers
        const int pixels = 2;
        const int stride = 16;
        cmsCIELab input_lab[2];
        cmsCIELab output_lab[2];
        
        // Initialize with some values
        for (int i = 0; i < 2; i++) {
            input_lab[i].L = 50.0 + i * 10.0;
            input_lab[i].a = 0.0;
            input_lab[i].b = 0.0;
        }
        
        // Perform transform with stride
        cmsDoTransformLineStride(
            transform,
            input_lab, output_lab,
            pixels, 1,  // 1 line, 2 pixels per line
            stride, stride,
            0, 0
        );
        
        // Calculate DeltaE between the two transformed pixels
        cmsFloat64Number deltaE = cmsDeltaE(&output_lab[0], &output_lab[1]);
        
        // Detect black point
        cmsCIEXYZ black_point;
        cmsBool black_point_found = cmsDetectDestinationBlackPoint(
            &black_point, input_profile, INTENT_PERCEPTUAL, 0
        );
        
        // Read a tag from the profile
        void* tag_data = cmsReadTag(input_profile, cmsSigCopyrightTag);
        
        if (tag_data) {
            // Try to get ASCII string from MLU if it's an MLU tag
            cmsMLU* mlu = (cmsMLU*)tag_data;
            char buffer[256];
            cmsUInt32Number bytes_written = cmsMLUgetASCII(
                mlu, "en", "US", buffer, sizeof(buffer)
            );
        }
        
        // Create IT8 handle for color measurement data
        cmsHANDLE it8 = cmsIT8Alloc(context);
        if (it8) {
            cmsIT8SetSheetType(it8, "CGATS.17");
            cmsIT8SetDataRowColDbl(it8, 0, 0, 50.0);
            cmsIT8Free(it8);
        }
        
        // Clean up transform
        cmsDeleteTransform(transform);
    }
    
    // Clean up resources
    if (lab_profile) cmsCloseProfile(lab_profile);
    if (input_profile) cmsCloseProfile(input_profile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}