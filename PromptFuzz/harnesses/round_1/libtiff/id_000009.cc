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
//<ID> 513
//<Prompt> []
/*<Combination>: [
*/
//<score> 28.333334, nr_unique_branch: 5
//<Quality> {"density":17,"unique_branches":{"_tiffWriteProc":[[116,13,116,34,0,0,4,0]],"TIFFHashSetRemoveInternal":[[543,13,543,46,0,0,4,1],[545,17,545,21,0,0,4,0]],"TIFFWriteRawTile":[[548,13,548,62,0,0,4,1]],"TIFFAppendToStrip":[[841,34,841,52,0,0,4,0]]},"library_calls":["TIFFOpen","TIFFIsBigEndian","TIFFLastDirectory","TIFFGetTagListEntry","TIFFRasterScanlineSize","TIFFFieldWithTag","TIFFFieldSetGetSize","TIFFSwabArrayOfLong8","TIFFReadEXIFDirectory","TIFFNumberOfDirectories","TIFFUnlinkDirectory","TIFFGetField","TIFFGetField","TIFFTileSize","_TIFFmalloc","TIFFWriteRawTile","_TIFFfree","TIFFClose"],"critical_calls":["TIFFOpen","TIFFIsBigEndian","TIFFLastDirectory","TIFFGetTagListEntry","TIFFRasterScanlineSize","TIFFFieldWithTag","TIFFFieldSetGetSize","TIFFSwabArrayOfLong8","TIFFReadEXIFDirectory","TIFFNumberOfDirectories","TIFFUnlinkDirectory","TIFFGetField","TIFFGetField","TIFFTileSize","_TIFFmalloc","TIFFWriteRawTile","_TIFFfree","TIFFClose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
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
    
    TIFF *tif = nullptr;
    int ret = 0;
    
    // Open the input file as a TIFF
    tif = TIFFOpen("input_file", fuzz_str_1);
    if (!tif) {
        // Clean up and exit
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // API 5: Check if TIFF is Big Endian
    int is_big_endian = TIFFIsBigEndian(tif);
    
    // API 7: Check if we're at the last directory
    int is_last_dir = TIFFLastDirectory(tif);
    
    // API 6: Get first tag from tag list (index 0)
    uint32_t first_tag = TIFFGetTagListEntry(tif, fuzz_int32_t_2);
    
    // API 8: Get raster scanline size
    tmsize_t scanline_size = TIFFRasterScanlineSize(tif);
    
    // API 9: Get field size (need to get a field first)
    // Try to get a field for a common tag like ImageWidth (256)
    const TIFFField* field = TIFFFieldWithTag(tif, fuzz_uint32_t_3);
    if (field) {
        int field_size = TIFFFieldSetGetSize(field);
    }
    
    // API 1: Swab an array of Long8 values
    // Create a small array to swab
    uint64_t long8_array[2] = {0x1122334455667788ULL, 0x8877665544332211ULL};
    TIFFSwabArrayOfLong8(long8_array, 2);
    
    // API 3: Try to read EXIF directory
    // Try reading from offset 0, which might work if EXIF data exists
    TIFFReadEXIFDirectory(tif, 0);
    
    // API 2: Try to unlink a directory if we have multiple
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    if (num_dirs > 1) {
        // Try to unlink the second directory (index 1)
        TIFFUnlinkDirectory(tif, fuzz_uint32_t_4);
    }
    
    // API 4: Write a raw tile if the image is tiled
    uint32_t tile_width = 0, tile_length = 0;
    if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) &&
        TIFFGetField(tif, fuzz_uint32_t_5, &tile_length)) {
        
        // Create buffer for tile data
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0) {
            uint8_t* tile_data = (uint8_t*)_TIFFmalloc(tile_size);
            if (tile_data) {
                // Fill with some data (could use part of input)
                size_t copy_size = tile_size < 1024 ? tile_size : 1024;
                if (copy_size > size) copy_size = size;
                if (copy_size > 0) {
                    memcpy(tile_data, data, copy_size);
                }
                
                // Write tile 0
                TIFFWriteRawTile(tif, fuzz_uint32_t_6, tile_data, tile_size);
                _TIFFfree(tile_data);
            }
        }
    }
    
    // Clean up
    TIFFClose(tif);
    
    // Remove temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}