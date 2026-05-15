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
//<ID> 557
//<Prompt> []
/*<Combination>: [
*/
//<score> 76, nr_unique_branch: 2
//<Quality> {"density":38,"unique_branches":{"OptimizeByResampling":[[680,50,680,79,0,0,4,0],[680,83,680,111,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenIOhandlerFromStream","cmsDeleteContext","cmsCreateXYZProfileTHR","cmsCloseIOhandler","cmsDeleteContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext","cmsSetColorSpace","cmsSetColorSpace","cmsGetTagSignature","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetWide","cmsMLUfree","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8SetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8SaveToFile","cmsIT8Free","cmsBuildGamma","cmsStageAllocCLut16bit","cmsStageSampleCLut16bit","cmsStageFree","cmsFreeToneCurve","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromStream","cmsCreateXYZProfileTHR","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsSetColorSpace","cmsSetColorSpace","cmsGetTagSignature","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetWide","cmsMLUfree","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8SetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8SaveToFile","cmsIT8Free","cmsBuildGamma","cmsStageAllocCLut16bit","cmsStageSampleCLut16bit","cmsStageFree","cmsFreeToneCurve","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs to process input data:

*/


extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
	return 0;
	}
	

    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 2. Create input stream from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Create IO handler from stream
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromStream(context, in_file);
    if (!io_handler) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Create XYZ profile
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    if (!xyz_profile) {
        cmsCloseIOhandler(io_handler);
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Get D50 white point and create Lab profile
    const cmsCIExyY *white_point = cmsD50_xyY();
    cmsHPROFILE lab_profile = cmsCreateLab4ProfileTHR(context, white_point);
    if (!lab_profile) {
        cmsCloseProfile(xyz_profile);
        cmsCloseIOhandler(io_handler);
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Set color space
    cmsSetColorSpace(lab_profile, cmsSigLabData);
    cmsSetColorSpace(xyz_profile, cmsSigXYZData);

    // 7. Get tag signature from profile
    cmsTagSignature tag_sig = cmsGetTagSignature(xyz_profile, 0);

    // 8. MLU operations
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        const char lang_code[] = "en";
        const char country_code[] = "US";
        const char ascii_str[] = "Test String";
        
        // Set ASCII string
        cmsMLUsetASCII(mlu, lang_code, country_code, ascii_str);
        
        // Get as wide string
        wchar_t wide_buffer[256];
        cmsMLUgetWide(mlu, lang_code, country_code, wide_buffer, 256);
        
        cmsMLUfree(mlu);
    }

    // 9. IT8 operations
    // First, write input data to file for IT8 loading
    FILE *it8_input = fopen("input_file", "wb");
    if (it8_input) {
        fwrite(data, 1, size, it8_input);
        assert_file_closed(&it8_input);;
        
        // Load IT8 from file
        cmsHANDLE it8 = cmsIT8LoadFromFile(context, "input_file");
        if (it8) {
            // Set index column
            cmsIT8SetIndexColumn(it8, "R");
            
            // Get table count
            cmsUInt32Number table_count = cmsIT8TableCount(it8);
            
            // Set property as double
            cmsIT8SetPropertyDbl(it8, "PROPERTY", 1.23);
            
            // Set data at row 0, column 0
            cmsIT8SetDataRowCol(it8, 0, 0, "42");
            
            // Save to file
            cmsIT8SaveToFile(it8, "output_file");
            
            cmsIT8Free(it8);
        }
        remove("input_file");
        remove("output_file");
    }

    // 10. Create tone curve for CLUT
    cmsToneCurve *curve = cmsBuildGamma(context, 2.2);
    if (curve) {
        // 11. CLUT stage operations
        // Create a simple 2x2x2 CLUT
        const cmsUInt32Number grid_points = 2;
        const cmsUInt32Number input_chan = 3;
        const cmsUInt32Number output_chan = 3;
        cmsUInt16Number table[2*2*2*3] = {0};
        
        // Fill table with some data
        for (unsigned i = 0; i < sizeof(table)/sizeof(table[0]); i++) {
            table[i] = (i * 1000) % 65535;
        }
        
        cmsStage *clut_stage = cmsStageAllocCLut16bit(context, grid_points, 
                                                      input_chan, output_chan, 
                                                      table);
        if (clut_stage) {
            // Sample CLUT
            cmsSAMPLER16 sampler = [](const cmsUInt16Number *in, 
                                      cmsUInt16Number *out, 
                                      void *cargo) {
                for (int i = 0; i < 3; i++) {
                    out[i] = in[i];
                }
                return 1;
            };
            
            cmsStageSampleCLut16bit(clut_stage, sampler, nullptr, 0);
            cmsStageFree(clut_stage);
        }
        
        cmsFreeToneCurve(curve);
    }

    // 12. Create and delete transform
    // Create a simple transform between profiles
    cmsUInt32Number input_format = TYPE_Lab_16;
    cmsUInt32Number output_format = TYPE_XYZ_16;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM transform = cmsCreateTransform(lab_profile, input_format,
                                                 xyz_profile, output_format,
                                                 intent, flags);
    
    if (transform) {
        // Use the transform with some test data
        cmsUInt16Number input_pixel[3] = {32768, 32768, 32768};
        cmsUInt16Number output_pixel[3];
        
        // This would normally transform colors, but we just need to call it
        cmsDoTransform(transform, input_pixel, output_pixel, 1);
        
        cmsDeleteTransform(transform);
    }

    // 13. Cleanup - in reverse order of creation
    cmsCloseProfile(lab_profile);
    cmsCloseProfile(xyz_profile);
    cmsCloseIOhandler(io_handler);
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;

    assert_file_closed(&it8_input);
	assert_file_closed(&in_file);
	return 0;
}