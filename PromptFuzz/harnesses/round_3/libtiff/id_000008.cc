#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tiffio.hxx>
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 54
//<Quality> {"density":2,"unique_branches":{"TIFFAppendToStrip":[[930,9,930,17,0,0,4,1],[939,9,939,66,0,0,4,0]],"TIFFPrintDirectory":[[351,13,351,14,0,0,4,0],[625,21,625,30,0,0,4,1],[721,9,721,37,0,0,4,1],[724,9,724,35,0,0,4,1],[236,34,236,71,1,0,4,1],[236,34,236,71,5,0,4,1],[236,34,236,71,7,0,4,1],[236,34,236,71,9,0,4,1],[236,34,236,71,14,0,4,1],[236,34,236,71,15,0,4,1],[236,34,236,71,16,0,4,1],[236,34,236,71,23,0,4,0],[236,34,236,71,24,0,4,1],[236,34,236,71,28,0,4,1],[236,34,236,71,32,0,4,1],[236,34,236,71,33,0,4,1],[236,34,236,71,34,0,4,1],[236,34,236,71,38,0,4,1],[236,34,236,71,41,0,4,1],[236,34,236,71,42,0,4,1],[236,34,236,71,45,0,4,1],[236,34,236,71,46,0,4,1],[236,34,236,71,48,0,4,1],[236,34,236,71,49,0,4,1],[236,34,236,71,50,0,4,1],[236,34,236,71,51,0,4,1],[236,34,236,71,52,0,4,1],[236,34,236,71,54,0,4,1],[236,34,236,71,56,0,4,1],[236,34,236,71,59,0,4,1],[236,34,236,71,60,0,4,1],[236,34,236,71,62,0,4,1],[236,34,236,71,63,0,4,1],[236,34,236,71,65,0,4,1]],"TIFFFlushData1":[[957,13,959,50,0,0,4,1]],"TIFFWriteScanline":[[109,9,109,35,0,0,4,1],[38,6,38,43,1,0,4,0],[42,7,42,44,2,0,4,0],[42,48,42,64,2,0,4,0]],"LogL10toY":[[885,9,885,17,0,0,4,1]],"TIFFWriteDirectorySec":[[509,13,509,45,0,0,4,0],[512,17,512,45,0,0,4,1],[527,13,527,31,0,0,4,0],[527,35,527,75,0,0,4,0],[529,17,529,37,0,0,4,1]],"PickContigCase":[[3105,16,3105,37,0,0,4,1]],"TIFFCIELabToRGBInit":[[149,17,149,43,0,0,4,0],[149,17,149,43,0,0,4,1],[159,17,159,43,0,0,4,0],[159,17,159,43,0,0,4,1],[169,17,169,43,0,0,4,0],[169,17,169,43,0,0,4,1]]},"library_calls":["TIFFSetWarningHandler","TIFFSetErrorHandler","TIFFSetTagExtender","TIFFStreamOpen","TIFFCIELabToRGBInit","TIFFCreateDirectory","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteScanline","LogL10toY","TIFFRGBAImageBegin","TIFFWriteDirectory","TIFFPrintDirectory","TIFFRGBAImageEnd","TIFFClose","TIFFSetWarningHandler","TIFFSetErrorHandler"],"critical_calls":["TIFFSetWarningHandler","TIFFSetErrorHandler","TIFFSetTagExtender","TIFFRGBAImageEnd","TIFFPrintDirectory","TIFFWriteDirectory","TIFFRGBAImageBegin","LogL10toY","TIFFWriteScanline","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFCreateDirectory","TIFFCIELabToRGBInit","TIFFStreamOpen","TIFFClose","TIFFSetWarningHandler","TIFFSetErrorHandler"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Create input file from fuzzer data
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // 2. Create output stream for TIFF operations
    std::ostringstream oss;
    TIFF* tiff = nullptr;
    
    // 3. Set up error handlers
    TIFFErrorHandler old_warning_handler = TIFFSetWarningHandler(nullptr);
    TIFFErrorHandler old_error_handler = TIFFSetErrorHandler(nullptr);
    
    // 4. Create a custom tag extender
    TIFFExtendProc default_extender = TIFFSetTagExtender(nullptr);
    
    try {
        // 5. Open TIFF stream for writing
        tiff = TIFFStreamOpen(fuzz_str_1, &oss);
        if (!tiff) {
            throw std::runtime_error("Failed to open TIFF stream");
        }
        
        // 6. Initialize display parameters for CIELab conversion
        TIFFDisplay display;
        memset(&display, 0, sizeof(TIFFDisplay));
        display.d_mat[0][0] = 1.0f;
        display.d_mat[1][1] = 1.0f;
        display.d_mat[2][2] = 1.0f;
        display.d_YCR = 1.0f;
        display.d_YCG = 1.0f;
        display.d_YCB = 1.0f;
        display.d_Vrwr = 100;
        display.d_Vrwg = 100;
        display.d_Vrwb = 100;
        display.d_Y0R = 0.0f;
        display.d_Y0G = 0.0f;
        display.d_Y0B = 0.0f;
        display.d_gammaR = 1.0f;
        display.d_gammaG = 1.0f;
        display.d_gammaB = 1.0f;
        
        float whitePoint[3] = {1.0f, 1.0f, 1.0f};
        TIFFCIELabToRGB cielabToRGB;
        int cielab_init_result = TIFFCIELabToRGBInit(&cielabToRGB, &display, whitePoint);
        
        // 7. Create a minimal valid TIFF structure
        TIFFCreateDirectory(tiff);
        
        // Set required basic fields
        TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, (uint32_t)256);
        TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, (uint32_t)256);
        TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, (uint16_t)8);
        TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)1);
        TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, (uint16_t)PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, (uint16_t)PLANARCONFIG_CONTIG);
        TIFFSetField(tiff, TIFFTAG_ORIENTATION, (uint16_t)ORIENTATION_TOPLEFT);
        TIFFSetField(tiff, TIFFTAG_COMPRESSION, (uint16_t)COMPRESSION_NONE);
        
        // 8. Write some dummy pixel data
        std::vector<uint8_t> pixel_data(256 * 256, 128);
        if (size > 0) {
            size_t copy_size = std::min(size, pixel_data.size());
            memcpy(pixel_data.data(), data, copy_size);
        }
        
        for (uint32_t row = 0; row < 256; ++row) {
            TIFFWriteScanline(tiff, pixel_data.data() + row * 256, row, fuzz_uint16_t_2);
        }
        
        // 9. Use LogL10 conversion
        double y_value = LogL10toY(fuzz_int32_t_3);
        
        // 10. Prepare for RGBA image processing
        TIFFRGBAImage rgba_img;
        char emsg[1024];
        int rgba_init_result = TIFFRGBAImageBegin(&rgba_img, tiff, fuzz_int32_t_4, emsg);
        
        // 11. Write directory and close
        TIFFWriteDirectory(tiff);
        
        // 12. Print directory information to file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            TIFFPrintDirectory(tiff, out_file, fuzz_int64_t_5);
            assert_file_closed(&out_file);;
        }
        
        // 13. Clean up RGBA image if initialized
        if (rgba_init_result == 1) {
            TIFFRGBAImageEnd(&rgba_img);
        }
        
    } catch (...) {
        // Exception handling
    }
    
    // 14. Clean up TIFF structure
    if (tiff) {
        TIFFClose(tiff);
    }
    
    // 15. Restore original handlers
    TIFFSetWarningHandler(old_warning_handler);
    TIFFSetErrorHandler(old_error_handler);
    
    // 16. Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}