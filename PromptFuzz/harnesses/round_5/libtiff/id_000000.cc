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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenOptionsFree","TIFFOpenOptionsFree","TIFFSetClientdata","TIFFRasterScanlineSize64","_TIFFmalloc","_TIFFrealloc","TIFFGetFieldDefaulted","TIFFGetFieldDefaulted","TIFFGetFieldDefaulted","TIFFGetFieldDefaulted","TIFFReadScanline","TIFFSwabArrayOfShort","_TIFFfree","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFSetClientdata","TIFFRasterScanlineSize64","_TIFFmalloc","_TIFFrealloc","TIFFGetFieldDefaulted","TIFFGetFieldDefaulted","TIFFGetFieldDefaulted","TIFFGetFieldDefaulted","TIFFReadScanline","TIFFSwabArrayOfShort","_TIFFfree","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Create TIFFOpenOptions and set memory limit
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 100 * 1024 * 1024); // 100MB limit
    
    // Open the TIFF file
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        TIFFOpenOptionsFree(opts);
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for libtiff
    int fd = fuzz_fileno(in_file);
    TIFF *tif = TIFFFdOpen(NULL, "input_file", "r");
    assert_file_closed(&in_file);;
    
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set client data
    char client_data[] = "fuzz_client";
    thandle_t old_clientdata = TIFFSetClientdata(tif, (thandle_t)client_data);
    
    // Get raster scanline size
    uint64_t raster_size = TIFFRasterScanlineSize64(tif);
    
    // Allocate buffer for scanline using _TIFFrealloc
    void *scanline_buf = nullptr;
    if (raster_size > 0) {
        scanline_buf = _TIFFmalloc(raster_size);
        if (scanline_buf) {
            // Try to reallocate with different size
            tmsize_t new_size = raster_size * 2;
            void *new_buf = _TIFFrealloc(scanline_buf, new_size);
            if (new_buf) {
                scanline_buf = new_buf;
            }
        }
    }
    
    // Get field values using TIFFGetFieldDefaulted
    uint32_t width = 0, height = 0;
    uint16_t samples_per_pixel = 0, bits_per_sample = 0;
    
    TIFFGetFieldDefaulted(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetFieldDefaulted(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    
    // Process some data with TIFFSwabArrayOfShort if we have 16-bit data
    if (bits_per_sample == 16 && scanline_buf && raster_size > 0) {
        // Read a scanline
        if (TIFFReadScanline(tif, scanline_buf, 0, 0) >= 0) {
            // Swap byte order of 16-bit data
            size_t short_count = raster_size / 2;
            if (short_count > 0) {
                TIFFSwabArrayOfShort((uint16_t*)scanline_buf, short_count);
            }
        }
    }
    
    // Cleanup
    if (scanline_buf) {
        _TIFFfree(scanline_buf);
    }
    
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    // Remove temporary file
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}