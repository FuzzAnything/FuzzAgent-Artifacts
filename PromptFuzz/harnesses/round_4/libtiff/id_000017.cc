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
//<ID> 664
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 2
//<Quality> {"density":23,"unique_branches":{"TIFFErrorExtR":[[125,9,125,29,0,0,4,0],[128,33,128,36,0,0,4,0]]},"library_calls":["TIFFSetErrorHandlerExt","TIFFClientOpen","TIFFRGBAImageOK","TIFFReadDirectory","TIFFGetStrileOffset","TIFFNumberOfStrips","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBAStripExt","TIFFClientOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteScanline","TIFFYCbCrToRGBInit","TIFFYCbCrtoRGB","TIFFRewriteDirectory","TIFFClose","_TIFFfree","TIFFClose"],"critical_calls":["TIFFSetErrorHandlerExt","TIFFClientOpen","TIFFRGBAImageOK","TIFFReadDirectory","TIFFGetStrileOffset","TIFFNumberOfStrips","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBAStripExt","TIFFClientOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteScanline","TIFFYCbCrToRGBInit","TIFFYCbCrtoRGB","TIFFRewriteDirectory","TIFFClose","_TIFFfree","TIFFClose"],"visited":0}
/**/


// Custom error handler to suppress warnings/errors during fuzzing
static void  id_000017_errorHandler(thandle_t, const char* module, const char* fmt, va_list ap) {
    // Do nothing - suppress all TIFF errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<87) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_18, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_19, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_20, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Write input data to a file for TIFF to read
    FILE* in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // 2. Set up error handler
    TIFFSetErrorHandlerExt((TIFFErrorHandlerExt) id_000017_errorHandler);
    
    // 3. Open input TIFF file
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Create TIFF handle using file descriptor
    thandle_t handle = (thandle_t)in_file;
    TIFF* tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, handle,
        [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
            return fread(buf, 1, size, (FILE*)handle);
        },
        nullptr, // No write function for read-only
        [](thandle_t handle, toff_t offset, int whence) -> toff_t {
            fseek((FILE*)handle, (long)offset, whence);
            return ftell((FILE*)handle);
        },
        [](thandle_t handle) -> int {
            fclose((FILE*)handle);
            return 0;
        },
        [](thandle_t handle) -> toff_t {
            fseek((FILE*)handle, 0, SEEK_END);
            return ftell((FILE*)handle);
        },
        nullptr, // No map function
        nullptr  // No unmap function
    );
    
    if (!tif) {
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 4. Check if TIFF is suitable for RGBA image
    char errormsg[1024] = {0};
    int rgba_ok = TIFFRGBAImageOK(tif, errormsg);
    
    // 5. Read TIFF directory
    TIFFReadDirectory(tif);
    
    // 6. Get strile offset (for first strip)
    uint64_t strile_offset = TIFFGetStrileOffset(tif, fuzz_uint32_t_3);
    (void)strile_offset; // Use result to avoid unused variable warning
    
    // 7. Try to read RGBA strip if possible
    uint32_t* rgba_buffer = nullptr;
    if (rgba_ok && TIFFNumberOfStrips(tif) > 0) {
        // Allocate buffer for RGBA strip
        uint32_t width = 0, height = 0;
        TIFFGetField(tif, fuzz_uint32_t_4, &width);
        TIFFGetField(tif, fuzz_uint32_t_5, &height);
        
        if (width > 0 && height > 0 && width < 10000 && height < 10000) {
            size_t buffer_size = width * 4; // RGBA for one row
            rgba_buffer = (uint32_t*)_TIFFmalloc(buffer_size);
            if (rgba_buffer) {
                // Read first strip
                TIFFReadRGBAStripExt(tif, fuzz_uint32_t_6, rgba_buffer, fuzz_int32_t_7);
            }
        }
    }
    
    // 8. Create output TIFF file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        thandle_t out_handle = (thandle_t)out_file;
        TIFF* out_tif = TIFFClientOpen(fuzz_str_8, fuzz_str_9, out_handle,
            nullptr, // No read function for write-only
            [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
                return fwrite(buf, 1, size, (FILE*)handle);
            },
            [](thandle_t handle, toff_t offset, int whence) -> toff_t {
                fseek((FILE*)handle, (long)offset, whence);
                return ftell((FILE*)handle);
            },
            [](thandle_t handle) -> int {
                fclose((FILE*)handle);
                return 0;
            },
            [](thandle_t handle) -> toff_t {
                fseek((FILE*)handle, 0, SEEK_END);
                return ftell((FILE*)handle);
            },
            nullptr,
            nullptr
        );
        
        if (out_tif) {
            // Set basic TIFF tags for a small test image
            uint32_t width = 10;
            uint32_t height = 10;
            
            TIFFSetField(out_tif, fuzz_uint32_t_10, width);
            TIFFSetField(out_tif, fuzz_uint32_t_11, height);
            TIFFSetField(out_tif, fuzz_uint32_t_12, 8);
            TIFFSetField(out_tif, fuzz_uint32_t_13, 3);
            TIFFSetField(out_tif, fuzz_uint32_t_14, PLANARCONFIG_CONTIG);
            TIFFSetField(out_tif, fuzz_uint32_t_15, PHOTOMETRIC_RGB);
            TIFFSetField(out_tif, fuzz_uint32_t_16, ORIENTATION_TOPLEFT);
            TIFFSetField(out_tif, fuzz_uint32_t_17, COMPRESSION_NONE);
            TIFFSetField(out_tif, fuzz_uint32_t_18, height);
            
            // 9. Write a scanline
            uint8_t scanline[30] = {0}; // 10 pixels * 3 bytes
            // Fill with some data from input
            size_t data_to_copy = size < sizeof(scanline) ? size : sizeof(scanline);
            memcpy(scanline, data, data_to_copy);
            
            TIFFWriteScanline(out_tif, scanline, fuzz_uint32_t_19, fuzz_uint16_t_20);
            
            // 10. Use YCbCr to RGB conversion
            TIFFYCbCrToRGB ycbcr_to_rgb;
            float ref_white[3] = {1.0f, 1.0f, 1.0f};
            float ref_black[3] = {0.0f, 0.0f, 0.0f};
            
            if (TIFFYCbCrToRGBInit(&ycbcr_to_rgb, ref_white, ref_black) == 0) {
                uint32_t r, g, b;
                // Convert a sample YCbCr pixel
                TIFFYCbCrtoRGB(&ycbcr_to_rgb, 
                              (uint32_t)(data[0] % 256),  // Y
                              (int32_t)(data[1] % 256) - 128,  // Cb
                              (int32_t)(data[2] % 256) - 128,  // Cr
                              &r, &g, &b);
                (void)r; (void)g; (void)b; // Use results
            }
            
            // 11. Rewrite directory
            TIFFRewriteDirectory(out_tif);
            
            TIFFClose(out_tif);
        }
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // 12. Clean up
    if (rgba_buffer) {
        _TIFFfree(rgba_buffer);
    }
    
    TIFFClose(tif);
    remove("input_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}