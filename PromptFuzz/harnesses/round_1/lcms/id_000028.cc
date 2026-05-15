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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 55, nr_unique_branch: 4
//<Quality> {"density":22,"unique_branches":{"cmsStageAllocCLut16bitGranular":[[581,9,581,15,0,0,4,0]],"_cmsFreeInterpParams":[[179,9,179,18,0,0,4,1]],"CLutElemTypeFree":[[538,9,538,22,0,0,4,1]],"cmsCreateProofingTransformTHR":[[1388,9,1388,65,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenIOhandlerFromStream","cmsCreateXYZProfileTHR","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsSetColorSpace","cmsGetTagSignature","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetWide","cmsMLUfree","cmsBuildGamma","cmsCreateLinearizationDeviceLink","cmsCreateProofingTransform","cmsIT8LoadFromFile","cmsIT8SetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8SaveToFile","cmsStageAllocCLut16bit","cmsStageSampleCLut16bit","cmsStageFree","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromStream","cmsCreateXYZProfileTHR","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsSetColorSpace","cmsGetTagSignature","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetWide","cmsMLUfree","cmsBuildGamma","cmsCreateLinearizationDeviceLink","cmsCreateProofingTransform","cmsIT8LoadFromFile","cmsIT8SetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8SaveToFile","cmsStageAllocCLut16bit","cmsStageSampleCLut16bit","cmsStageFree","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsCloseIOhandler","cmsDeleteContext"],"visited":1}
/**/


// Dummy sampler function for cmsStageSampleCLut16bit
static int  id_000028_dummy_sampler16(const cmsUInt16Number* in, cmsUInt16Number* out, void* user_data) {
    (void)in;
    (void)out;
    (void)user_data;
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
	return 0;
	}
	
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create IO handler from stream
    cmsIOHANDLER* io_handler = cmsOpenIOhandlerFromStream(context, in_file);
    
    // Create profiles
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    const cmsCIExyY* d50 = cmsD50_xyY();
    cmsHPROFILE lab_profile = cmsCreateLab4ProfileTHR(context, d50);
    
    // Set color space for lab profile
    cmsSetColorSpace(lab_profile, cmsSigLabData);
    
    // Get tag signature
    if (xyz_profile) {
        cmsTagSignature first_tag = cmsGetTagSignature(xyz_profile, 0);
        (void)first_tag; // Prevent unused variable warning
    }
    
    // Create MLU and set ASCII string
    cmsMLU* mlu = cmsMLUalloc(context, 1);
    if (mlu) {
        const char lang[] = "en";
        const char country[] = "US";
        const char* ascii_str = "Test ASCII";
        cmsMLUsetASCII(mlu, lang, country, ascii_str);
        
        // Get wide string from MLU
        wchar_t wide_buffer[256];
        cmsUInt32Number wide_len = cmsMLUgetWide(mlu, lang, country, wide_buffer, sizeof(wide_buffer)/sizeof(wchar_t));
        (void)wide_len; // Prevent unused variable warning
        cmsMLUfree(mlu);
    }
    
    // Create linearization device link
    cmsToneCurve* identity_curve = cmsBuildGamma(context, 1.0);
    cmsToneCurve* transfer_funcs[3] = {identity_curve, identity_curve, identity_curve};
    cmsHPROFILE device_link = cmsCreateLinearizationDeviceLink(cmsSigRgbData, transfer_funcs);
    
    // Create proofing transform
    cmsUInt32Number input_format = TYPE_Lab_16;
    cmsUInt32Number output_format = TYPE_XYZ_16;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number proofing_intent = INTENT_ABSOLUTE_COLORIMETRIC;
    cmsUInt32Number flags = cmsFLAGS_SOFTPROOFING;
    
    cmsHTRANSFORM transform = NULL;
    if (xyz_profile && lab_profile && device_link) {
        transform = cmsCreateProofingTransform(
            lab_profile, input_format,
            xyz_profile, output_format,
            device_link,
            intent, proofing_intent, flags
        );
    }
    
    // Create IT8 handle and set properties
    cmsHANDLE it8 = NULL;
    if (context) {
        // Write data to file for IT8 loading
        FILE* it8_file = fopen("input_file", "wb");
        if (it8_file) {
            fwrite(data, 1, size, it8_file);
            assert_file_closed(&it8_file);;
            
            // Load IT8 from file
            it8 = cmsIT8LoadFromFile(context, "input_file");
            if (it8) {
                // Set properties
                cmsIT8SetPropertyDbl(it8, "TestProp", 123.456);
                cmsIT8SetDataRowCol(it8, 0, 0, "TestData");
                
                // Save IT8 to file
                cmsIT8SaveToFile(it8, "output_file");
            }
        }
    }
    
    // Create and sample CLUT stage
    if (context) {
        cmsStage* clut_stage = cmsStageAllocCLut16bit(context, 2, 3, NULL, 0);
        if (clut_stage) {
            cmsStageSampleCLut16bit(clut_stage,  id_000028_dummy_sampler16, NULL, 0);
            cmsStageFree(clut_stage);
        }
    }
    
    // Cleanup
    if (transform) cmsDeleteTransform(transform);
    if (device_link) cmsCloseProfile(device_link);
    if (identity_curve) cmsFreeToneCurve(identity_curve);
    if (lab_profile) cmsCloseProfile(lab_profile);
    if (xyz_profile) cmsCloseProfile(xyz_profile);
    if (it8) cmsIT8Free(it8);
    if (io_handler) cmsCloseIOhandler(io_handler);
    if (in_file) assert_file_closed(&in_file);;
    if (context) cmsDeleteContext(context);
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}