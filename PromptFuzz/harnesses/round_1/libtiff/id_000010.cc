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
//<ID> 524
//<Prompt> []
/*<Combination>: [
*/
//<score> 18.666666, nr_unique_branch: 2
//<Quality> {"density":28,"unique_branches":{"TIFFClientOpenExt":[[345,17,345,47,0,0,4,0],[346,9,346,51,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFGetVersion","TIFFIsBigTIFF","TIFFIsUpSampled","TIFFIsCODECConfigured","TIFFIsCODECConfigured","TIFFIsCODECConfigured","TIFFFieldWithTag","TIFFFieldIsAnonymous","TIFFGetField","TIFFGetField","TIFFStripSize","_TIFFcalloc","TIFFReadEncodedStrip","TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteEncodedStrip","TIFFClose","TIFFOpenOptionsFree","_TIFFfree","TIFFCIELabToRGBInit","LogL10fromY","TIFFClose"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFGetVersion","TIFFIsBigTIFF","TIFFIsUpSampled","TIFFIsCODECConfigured","TIFFIsCODECConfigured","TIFFIsCODECConfigured","TIFFFieldWithTag","TIFFFieldIsAnonymous","TIFFGetField","TIFFGetField","TIFFStripSize","_TIFFcalloc","TIFFReadEncodedStrip","TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteEncodedStrip","TIFFClose","TIFFOpenOptionsFree","_TIFFfree","TIFFCIELabToRGBInit","LogL10fromY","TIFFClose"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs with the input data:

*/


// libtiff headers

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<103) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_18, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_19, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a file first
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Now try to read it as a TIFF file
    FILE *reopen_file = fopen("input_file", "rb");
    if (!reopen_file) {
	assert_file_closed(&reopen_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(reopen_file);
    if (fd < 0) {
        assert_file_closed(&reopen_file);;
        assert_file_closed(&reopen_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Create TIFFOpenOptions for opening
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        assert_file_closed(&reopen_file);;
        assert_file_closed(&reopen_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set memory allocation limits
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 10 * 1024 * 1024); // 10MB
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 100 * 1024 * 1024); // 100MB
    
    // Open TIFF file using the file descriptor
    TIFF *tif = TIFFClientOpenExt(fuzz_str_1, fuzz_str_2, 
                                 reinterpret_cast<thandle_t>(fd),
                                 TIFFReadWriteProc(0), TIFFReadWriteProc(0),
                                 TIFFSeekProc(0), TIFFCloseProc(0),
                                 TIFFSizeProc(0), TIFFMapFileProc(0),
                                 TIFFUnmapFileProc(0), opts);
    
    // Free options
    TIFFOpenOptionsFree(opts);
    
    if (!tif) {
        assert_file_closed(&reopen_file);;
        assert_file_closed(&reopen_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Get and use the library version
    const char *version = TIFFGetVersion();
    if (version) {
        // Version string is available, we could log it if needed
    }
    
    // Check if this is a BigTIFF file
    int is_bigtiff = TIFFIsBigTIFF(tif);
    
    // Check if the image is upsampled
    int is_upsampled = TIFFIsUpSampled(tif);
    
    // Check if specific CODECs are configured
    int is_codec_configured_ccitt = TIFFIsCODECConfigured(fuzz_uint16_t_3);
    int is_codec_configured_lzw = TIFFIsCODECConfigured(fuzz_uint16_t_4);
    int is_codec_configured_packbits = TIFFIsCODECConfigured(fuzz_uint16_t_5);
    
    // Find a field and check if it's anonymous
    const TIFFField *field = TIFFFieldWithTag(tif, fuzz_uint32_t_6);
    if (field) {
        int is_anonymous = TIFFFieldIsAnonymous(field);
    }
    
    // Try to read and write some data if possible
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, fuzz_uint32_t_7, &width) &&
        TIFFGetField(tif, fuzz_uint32_t_8, &height)) {
        
        // Try to get strip size
        tmsize_t strip_size = TIFFStripSize(tif);
        if (strip_size > 0) {
            // Allocate buffer for strip data
            void *strip_data = _TIFFcalloc(fuzz_int64_t_9, strip_size);
            if (strip_data) {
                // Read first strip
                tmsize_t read_bytes = TIFFReadEncodedStrip(tif, fuzz_uint32_t_10, strip_data, strip_size);
                
                if (read_bytes > 0) {
                    // Create output file for writing
                    FILE *out_file = fopen("output_file", "wb");
                    if (out_file) {
                        int out_fd = fuzz_fileno(out_file);
                        
                        // Open output TIFF file
                        TIFFOpenOptions *out_opts = TIFFOpenOptionsAlloc();
                        if (out_opts) {
                            TIFF *out_tif = TIFFClientOpenExt(fuzz_str_11, fuzz_str_12,
                                                             reinterpret_cast<thandle_t>(out_fd),
                                                             TIFFReadWriteProc(0), TIFFReadWriteProc(0),
                                                             TIFFSeekProc(0), TIFFCloseProc(0),
                                                             TIFFSizeProc(0), TIFFMapFileProc(0),
                                                             TIFFUnmapFileProc(0), out_opts);
                            
                            if (out_tif) {
                                // Copy basic tags from input to output
                                TIFFSetField(out_tif, fuzz_uint32_t_13, width);
                                TIFFSetField(out_tif, fuzz_uint32_t_14, height);
                                TIFFSetField(out_tif, fuzz_uint32_t_15, 8);
                                TIFFSetField(out_tif, fuzz_uint32_t_16, 1);
                                TIFFSetField(out_tif, fuzz_uint32_t_17, 1);
                                TIFFSetField(out_tif, fuzz_uint32_t_18, PLANARCONFIG_CONTIG);
                                TIFFSetField(out_tif, fuzz_uint32_t_19, PHOTOMETRIC_MINISBLACK);
                                
                                // Write the strip data
                                tmsize_t written_bytes = TIFFWriteEncodedStrip(out_tif, fuzz_uint32_t_20, strip_data, read_bytes);
                                
                                // Clean up output TIFF
                                TIFFClose(out_tif);
                            }
                            TIFFOpenOptionsFree(out_opts);
                        }
                        assert_file_closed(&out_file);;
                    }
                }
                
                // Free strip data
                _TIFFfree(strip_data);
            }
        }
    }
    
    // Test CIELab to RGB conversion initialization
    TIFFCIELabToRGB cielab_to_rgb;
    TIFFDisplay display;
    float whitepoint[3] = {1.0f, 1.0f, 1.0f};
    
    // Initialize display with default values
    memset(&display, 0, sizeof(display));
    display.d_mat[0][0] = 1.0f;
    display.d_mat[1][1] = 1.0f;
    display.d_mat[2][2] = 1.0f;
    display.d_gammaR = 1.0f;
    display.d_gammaG = 1.0f;
    display.d_gammaB = 1.0f;
    
    // Initialize CIELab to RGB converter
    int cielab_init_result = TIFFCIELabToRGBInit(&cielab_to_rgb, &display, whitepoint);
    
    // Test LogL10 conversion
    double luminance = 0.5;
    int logl10_result = LogL10fromY(luminance, fuzz_int32_t_21);
    
    // Clean up
    TIFFClose(tif);
    assert_file_closed(&reopen_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&reopen_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}