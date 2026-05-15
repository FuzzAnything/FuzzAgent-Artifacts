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
//<ID> 939
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 1
//<Quality> {"density":23,"unique_branches":{"TIFFClientOpenExt":[[346,9,346,51,0,0,4,0]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFOpenOptionsFree","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFIsBigTIFF","TIFFPrintDirectory","TIFFNumberOfStrips","TIFFStripSize64","TIFFRawStripSize","_TIFFmalloc","TIFFReadEncodedStrip","_TIFFmalloc","TIFFReadRGBATileExt","_TIFFfree","_TIFFmalloc","TIFFReadFromUserBuffer","_TIFFfree","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteScanline","TIFFClose","_TIFFfree","TIFFCurrentRow","TIFFSwabLong","TIFFSwabFloat","TIFFSwabArrayOfLong8","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFClientOpenExt","TIFFIsBigTIFF","TIFFPrintDirectory","TIFFNumberOfStrips","TIFFStripSize64","TIFFRawStripSize","_TIFFmalloc","TIFFReadEncodedStrip","_TIFFmalloc","TIFFReadRGBATileExt","_TIFFfree","_TIFFmalloc","TIFFReadFromUserBuffer","_TIFFfree","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteScanline","TIFFClose","_TIFFfree","TIFFCurrentRow","TIFFSwabLong","TIFFSwabFloat","TIFFSwabArrayOfLong8","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs to process TIFF data:

*/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<86) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
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
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_19, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_20, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Open the input file for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Allocate and configure TIFF open options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set memory allocation limits
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, size * 2);
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, size);
    
    // Get file descriptor for TIFF client open
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        TIFFOpenOptionsFree(opts);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open TIFF file using the input file descriptor
    TIFF *tif = TIFFClientOpenExt("input_file", fuzz_str_1, 
                                  (thandle_t)(intptr_t)fd,
                                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, opts);
    
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // Check if it's BigTIFF
    int isBigTIFF = TIFFIsBigTIFF(tif);
    
    // Print directory information
    FILE *out_file = fopen("output_file", "w");
    if (out_file) {
        TIFFPrintDirectory(tif, out_file, fuzz_int64_t_2);
        assert_file_closed(&out_file);;
    }
    
    // Get number of strips
    uint32_t stripCount = TIFFNumberOfStrips(tif);
    
    // Process strips if available
    if (stripCount > 0) {
        // Allocate buffer for reading
        uint64_t stripSize64 = TIFFStripSize64(tif);
        tmsize_t stripSize = TIFFRawStripSize(tif, fuzz_uint32_t_3);
        
        void *readBuffer = _TIFFmalloc(stripSize > 0 ? stripSize : 1024);
        if (readBuffer) {
            // Read first strip
            tmsize_t bytesRead = TIFFReadEncodedStrip(tif, fuzz_uint32_t_4, readBuffer, 
                                                     stripSize > 0 ? stripSize : 1024);
            
            // Try to read RGBA tile if image is tiled
            uint32_t *rgbaBuffer = (uint32_t*)_TIFFmalloc(256 * 256 * sizeof(uint32_t));
            if (rgbaBuffer) {
                int rgbaResult = TIFFReadRGBATileExt(tif, fuzz_uint32_t_5, fuzz_uint32_t_6, rgbaBuffer, fuzz_int32_t_7);
                _TIFFfree(rgbaBuffer);
            }
            
            // Test reading from user buffer
            if (bytesRead > 0) {
                void *outBuffer = _TIFFmalloc(bytesRead);
                if (outBuffer) {
                    int userBufResult = TIFFReadFromUserBuffer(tif, fuzz_uint32_t_8, 
                                                              readBuffer, bytesRead,
                                                              outBuffer, bytesRead);
                    _TIFFfree(outBuffer);
                }
            }
            
            // Test writing a scanline (create new file for writing)
            TIFF *outTif = TIFFOpen("output_file.tif", fuzz_str_9);
            if (outTif) {
                // Set basic parameters for output TIFF
                TIFFSetField(outTif, fuzz_uint32_t_10, 100);
                TIFFSetField(outTif, fuzz_uint32_t_11, 100);
                TIFFSetField(outTif, fuzz_uint32_t_12, 8);
                TIFFSetField(outTif, fuzz_uint32_t_13, 3);
                TIFFSetField(outTif, fuzz_uint32_t_14, 1);
                TIFFSetField(outTif, fuzz_uint32_t_15, PLANARCONFIG_CONTIG);
                TIFFSetField(outTif, fuzz_uint32_t_16, PHOTOMETRIC_RGB);
                TIFFSetField(outTif, fuzz_uint32_t_17, COMPRESSION_NONE);
                
                // Write a scanline
                uint8_t scanline[300];
                memset(scanline, 0, sizeof(scanline));
                int writeResult = TIFFWriteScanline(outTif, scanline, fuzz_uint32_t_18, fuzz_uint16_t_19);
                
                TIFFClose(outTif);
            }
            
            _TIFFfree(readBuffer);
        }
    }
    
    // Get current row
    uint32_t currentRow = TIFFCurrentRow(tif);
    
    // Test swab functions
    uint32_t longValue = 0x12345678;
    TIFFSwabLong(&longValue);
    
    float floatValue = 1.234f;
    TIFFSwabFloat(&floatValue);
    
    uint64_t long8Array[4] = {0x123456789ABCDEF0, 0xFEDCBA9876543210};
    TIFFSwabArrayOfLong8(long8Array, fuzz_int64_t_20);
    
    // Clean up
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    remove("output_file.tif");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file.tif");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}