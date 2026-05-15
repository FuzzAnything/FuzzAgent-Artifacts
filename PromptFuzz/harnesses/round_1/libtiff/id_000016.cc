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
//<ID> 1197
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["TIFFClientOpen","TIFFErrorExt","TIFFCreateCustomDirectory","_TIFFmalloc","TIFFReadBufferSetup","TIFFGetField","TIFFGetField","TIFFComputeStrip","TIFFGetStrileByteCountWithErr","_TIFFmalloc","TIFFReadRawStrip","_TIFFmalloc","TIFFReadRGBAStrip","_TIFFfree","_TIFFfree","LogLuv24fromXYZ","_TIFFfree","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFCreateCustomDirectory","_TIFFmalloc","TIFFReadBufferSetup","TIFFGetField","TIFFGetField","TIFFComputeStrip","TIFFGetStrileByteCountWithErr","_TIFFmalloc","TIFFReadRawStrip","_TIFFmalloc","TIFFReadRGBAStrip","_TIFFfree","_TIFFfree","LogLuv24fromXYZ","_TIFFfree","TIFFClose"],"visited":0}
/**/


// Include libtiff headers

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_8, fdp);
	//fuzzer shim end}




    // Ensure we have enough data for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    // Open input file for reading
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for TIFFOpen
    int fd = fuzz_fileno(in_file);
    
    // Open TIFF from file descriptor
    TIFF *tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
                               reinterpret_cast<thandle_t>(fd),
                               nullptr, nullptr, nullptr, nullptr,
                               nullptr, nullptr, nullptr);
    
    if (!tif) {
        // Use TIFFErrorExt to report error
        TIFFErrorExt(reinterpret_cast<thandle_t>(fd), 
                    fuzz_str_3, 
                    "Failed to open TIFF from file descriptor");
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Try to create custom directory (even if it fails, we still call it)
    int custom_dir_result = TIFFCreateCustomDirectory(tif, nullptr);
    
    // Setup read buffer
    const size_t BUFFER_SIZE = 4096;
    void *read_buffer = _TIFFmalloc(fuzz_int64_t_4);
    if (read_buffer) {
        TIFFReadBufferSetup(tif, read_buffer, BUFFER_SIZE);
    }
    
    // Get basic image information
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, fuzz_uint32_t_5, &width);
    TIFFGetField(tif, fuzz_uint32_t_6, &height);
    
    if (width > 0 && height > 0) {
        // Compute strip for first row
        uint32_t strip = TIFFComputeStrip(tif, fuzz_uint32_t_7, fuzz_uint16_t_8);
        
        // Try to get strip byte count
        int byte_count_err = 0;
        uint64_t strip_byte_count = TIFFGetStrileByteCountWithErr(tif, strip, &byte_count_err);
        
        if (strip_byte_count > 0 && strip_byte_count < 1048576) { // Limit to 1MB
            // Allocate buffer for raw strip reading
            void *raw_strip_buffer = _TIFFmalloc(strip_byte_count);
            if (raw_strip_buffer) {
                // Read raw strip data
                tmsize_t read_bytes = TIFFReadRawStrip(tif, strip, raw_strip_buffer, strip_byte_count);
                
                // Try to read RGBA strip if image has appropriate format
                uint32_t *rgba_buffer = nullptr;
                if (read_bytes > 0) {
                    rgba_buffer = (uint32_t*)_TIFFmalloc(width * sizeof(uint32_t));
                    if (rgba_buffer) {
                        TIFFReadRGBAStrip(tif, strip, rgba_buffer);
                    }
                }
                
                // Clean up RGBA buffer
                _TIFFfree(rgba_buffer);
                _TIFFfree(raw_strip_buffer);
            }
        }
        
        // Test LogLuv24fromXYZ with sample data
        float xyz[3] = {0.5f, 0.5f, 0.5f};
        uint32_t logluv_value = LogLuv24fromXYZ(xyz, 1);
        (void)logluv_value; // Suppress unused warning
    }
    
    // Clean up
    _TIFFfree(read_buffer);
    TIFFClose(tif);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}