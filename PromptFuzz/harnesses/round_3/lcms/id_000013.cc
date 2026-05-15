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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 12
//<Quality> {"density":23,"unique_branches":{"cmsMD5computeID":[[279,9,279,59,0,0,4,1],[283,9,283,20,0,0,4,1],[286,9,286,58,0,0,4,1],[290,9,290,20,0,0,4,1]],"cmsMD5add":[[178,9,178,44,0,0,4,1],[185,9,185,10,0,0,4,1],[203,12,203,21,0,0,4,0],[203,12,203,21,0,0,4,1]],"cmsMD5alloc":[[157,9,157,20,0,0,4,1]],"cmsGetProfileIOhandler":[[580,9,580,20,0,0,4,1]],"cmsMD5finish":[[228,9,228,18,0,0,4,0],[228,9,228,18,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsMD5computeID","cmsGetProfileIOhandler","cmsD50_xyY","cmsCreateLab4Profile","cmsCloseProfile","cmsDeleteContext","cmsCreateTransform","cmsGetTransformPipeline","cmsDoTransformLineStride","cmsDeltaE","cmsCIE2000DeltaE","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetDataRowColDbl","cmsReadTag","cmsMLUgetASCII","cmsMLUfree","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsMD5computeID","cmsGetProfileIOhandler","cmsD50_xyY","cmsCreateLab4Profile","cmsCreateTransform","cmsGetTransformPipeline","cmsDoTransformLineStride","cmsDeltaE","cmsCIE2000DeltaE","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetDataRowColDbl","cmsReadTag","cmsMLUgetASCII","cmsMLUfree","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create profiles from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create input profile from stream
    cmsHPROFILE input_profile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!input_profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Compute MD5 for the profile
    cmsBool md5_result = cmsMD5computeID(input_profile);
    (void)md5_result; // Use result to avoid unused variable warning
    
    // 4. Get I/O handler from profile
    cmsIOHANDLER *io_handler = cmsGetProfileIOhandler(input_profile);
    (void)io_handler; // Use handler to avoid unused variable warning
    
    // 5. Create Lab profile for comparison
    const cmsCIExyY *d50 = cmsD50_xyY();
    cmsHPROFILE lab_profile = cmsCreateLab4Profile(d50);
    if (!lab_profile) {
        cmsCloseProfile(input_profile);
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Create transform for color conversion
    cmsHTRANSFORM transform = cmsCreateTransform(
        input_profile, TYPE_Lab_DBL,
        lab_profile, TYPE_Lab_DBL,
        INTENT_PERCEPTUAL, 0
    );
    
    if (transform) {
        // 7. Get transform pipeline
        cmsPipeline *pipeline = cmsGetTransformPipeline(transform);
        (void)pipeline; // Use pipeline to avoid unused variable warning
        
        // 8. Prepare Lab color data for transformation
        cmsCIELab lab1, lab2;
        size_t lab_size = sizeof(cmsCIELab);
        
        if (size >= 2 * lab_size) {
            // Use first part of input data for lab1
            memcpy(&lab1, data, lab_size);
            // Use second part for lab2
            memcpy(&lab2, data + lab_size, lab_size);
            
            // 9. Perform transformation using line stride
            // Prepare input and output buffers
            cmsCIELab input_labs[2] = {lab1, lab2};
            cmsCIELab output_labs[2];
            
            cmsDoTransformLineStride(
                transform,
                input_labs, output_labs,
                2, // PixelsPerLine
                1, // LineCount
                sizeof(cmsCIELab) * 2, // BytesPerLineIn
                sizeof(cmsCIELab) * 2, // BytesPerLineOut
                sizeof(cmsCIELab), // BytesPerPlaneIn
                sizeof(cmsCIELab)  // BytesPerPlaneOut
            );
            
            // 10. Compute Delta E between the two transformed colors
            cmsFloat64Number deltaE = cmsDeltaE(&output_labs[0], &output_labs[1]);
            
            // 11. Compute CIE2000 Delta E with default parameters
            cmsFloat64Number deltaE2000 = cmsCIE2000DeltaE(
                &output_labs[0], &output_labs[1],
                1.0, 1.0, 1.0 // Kl, Kc, Kh = 1.0
            );
            
            (void)deltaE; // Use results to avoid unused variable warnings
            (void)deltaE2000;
        }
        
        // 12. Create IT8 handle for color data management
        cmsHANDLE it8 = cmsIT8Alloc(context);
        if (it8) {
            // Set sheet type
            cmsIT8SetSheetType(it8, "COLOR");
            
            // Set data in IT8 table using row/col
            if (size > 200) {
                cmsIT8SetDataRowColDbl(it8, 0, 0, 
                    static_cast<cmsFloat64Number>(data[0]) / 255.0);
                
                // Try to get ASCII from MLU (if profile has MLU data)
                cmsMLU *mlu = static_cast<cmsMLU*>(
                    cmsReadTag(input_profile, cmsSigProfileDescriptionTag));
                
                if (mlu) {
                    char buffer[256];
                    cmsUInt32Number buffer_size = sizeof(buffer);
                    cmsMLUgetASCII(mlu, "en", "US", buffer, buffer_size);
                    cmsMLUfree(mlu);
                }
            }
            
            cmsIT8Free(it8);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // 13. Cleanup
    cmsCloseProfile(lab_profile);
    cmsCloseProfile(input_profile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}