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
//<ID> 466
//<Prompt> []
/*<Combination>: [
*/
//<score> 53.333332, nr_unique_branch: 21
//<Quality> {"density":20,"unique_branches":{"TIFFReadRGBAImageOriented":[[640,9,640,50,0,0,4,0]],"TIFFVGetFieldDefaulted":[[276,9,276,34,0,0,4,0]],"setorientation":[[677,22,677,65,0,0,4,1],[678,22,678,65,0,0,4,1]],"TIFFRGBAImageGet":[[592,9,592,25,0,0,4,1],[598,9,598,29,0,0,4,1],[607,9,607,29,0,0,4,0],[607,33,607,72,0,0,4,0],[610,13,610,19,0,0,4,1]],"TIFFReadEncodedStripGetStripSize":[[517,9,517,32,0,0,4,0]],"_TIFFReadEncodedStripAndAllocBuffer":[[609,9,609,21,0,0,4,1],[615,9,615,43,0,0,4,0]],"gtStripContig":[[1221,26,1221,40,0,0,4,1],[1222,19,1222,26,0,0,4,0],[1227,17,1227,37,0,0,4,0],[1229,13,1229,44,0,0,4,0],[1229,13,1229,44,0,0,4,1],[1233,13,1233,25,0,0,4,1],[1239,13,1245,31,0,0,4,0],[1246,14,1246,25,0,0,4,0],[339,24,339,33,1,0,4,1]]},"library_calls":["TIFFOpen","TIFFGetField","TIFFGetField","TIFFCreateDirectory","TIFFSetDirectory","TIFFNumberOfDirectories","TIFFStripSize","TIFFNumberOfStrips","_TIFFmalloc","TIFFWriteEncodedStrip","_TIFFfree","TIFFFlushData","_TIFFmalloc","TIFFReadRGBAImageOriented","_TIFFfree","TIFFIsTiled","_TIFFmalloc","TIFFTileSize","TIFFReadRGBATileExt","_TIFFfree","TIFFError","TIFFClose"],"critical_calls":["TIFFOpen","TIFFGetField","TIFFGetField","TIFFCreateDirectory","TIFFSetDirectory","TIFFNumberOfDirectories","TIFFStripSize","TIFFNumberOfStrips","_TIFFmalloc","TIFFWriteEncodedStrip","_TIFFfree","TIFFFlushData","_TIFFmalloc","TIFFReadRGBAImageOriented","_TIFFfree","TIFFIsTiled","_TIFFmalloc","TIFFTileSize","TIFFReadRGBATileExt","_TIFFfree","TIFFError","TIFFClose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<65) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file
    FILE *input_file = fopen("input_file", "wb");
    if (!input_file) {
	assert_file_closed(&input_file);
	return 0;
	}
	
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;
    
    // Open the TIFF file from the input data
    input_file = fopen("input_file", "rb");
    if (!input_file) {
	assert_file_closed(&input_file);
	assert_file_closed(&input_file);
	return 0;
	}
	
    
    TIFF *tif = TIFFOpen("input_file", fuzz_str_1);
    if (!tif) {
        assert_file_closed(&input_file);;
        assert_file_closed(&input_file);
	assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Get image dimensions for RGBA reading
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    // Call each required API at least once
    
    // 1. TIFFCreateDirectory - create a new directory
    if (TIFFCreateDirectory(tif) != 0) {
        // Switch to the new directory
        TIFFSetDirectory(tif, TIFFNumberOfDirectories(tif) - 1);
    }
    
    // 2. TIFFStripSize - get strip size
    tmsize_t strip_size = TIFFStripSize(tif);
    
    // 3. TIFFWriteEncodedStrip - write data to a strip
    // We'll write to strip 0 if it exists
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    if (strip_count > 0 && strip_size > 0 && size > 0) {
        // Use a portion of input data for writing
        tmsize_t write_size = strip_size;
        if (write_size > (tmsize_t)size) {
            write_size = size;
        }
        // Allocate buffer for strip data
        void *strip_data = _TIFFmalloc(write_size);
        if (strip_data) {
            memcpy(strip_data, data, write_size);
            TIFFWriteEncodedStrip(tif, fuzz_uint32_t_2, strip_data, write_size);
            _TIFFfree(strip_data);
        }
    }
    
    // 4. TIFFFlushData - flush data to file
    TIFFFlushData(tif);
    
    // 5. TIFFReadRGBAImageOriented - read RGBA image (if we have valid dimensions)
    if (width > 0 && height > 0) {
        uint32_t *raster = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
        if (raster) {
            TIFFReadRGBAImageOriented(tif, fuzz_uint32_t_3, fuzz_uint32_t_4, raster, fuzz_int32_t_5, fuzz_int32_t_6);
            _TIFFfree(raster);
        }
    }
    
    // 6. TIFFReadRGBATileExt - read a tile as RGBA (if tiled image)
    if (TIFFIsTiled(tif) && width > 0 && height > 0) {
        uint32_t *tile_raster = (uint32_t*)_TIFFmalloc(TIFFTileSize(tif));
        if (tile_raster) {
            // Read tile at position (0,0)
            TIFFReadRGBATileExt(tif, fuzz_uint32_t_7, fuzz_uint32_t_8, tile_raster, fuzz_int32_t_9);
            _TIFFfree(tile_raster);
        }
    }
    
    // 7. TIFFError - report an error (example usage)
    if (size == 0) {
        TIFFError(fuzz_str_10, fuzz_str_11);
    }
    
    // Cleanup
    TIFFClose(tif);
    assert_file_closed(&input_file);;
    
    // Clean up temporary files
    remove("input_file");
    
    assert_file_closed(&input_file);
	assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}