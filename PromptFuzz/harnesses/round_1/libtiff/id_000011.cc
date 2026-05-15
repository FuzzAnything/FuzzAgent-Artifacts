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
//<ID> 637
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 42
//<Quality> {"density":15,"unique_branches":{"TIFFWriteCheck":[[617,23,617,28,0,0,4,0]],"TIFFAdvanceDirectory":[[1951,17,1951,28,0,0,4,0]],"TIFFFieldWithTag":[[933,9,933,13,0,0,4,0]],"TIFFWriteRawTile":[[40,47,40,79,1,0,4,1]],"TIFFFetchDirectory":[[6181,13,6181,50,0,0,4,0]],"TIFFUnlinkDirectory":[[2237,9,2237,34,0,0,4,0],[2267,24,2267,29,0,0,4,0],[2269,13,2269,25,0,0,4,1],[2274,13,2274,68,0,0,4,1],[2317,9,2317,18,0,0,4,1]],"TIFFReadFromUserBuffer":[[1573,9,1573,34,0,0,4,1],[1578,9,1578,40,0,0,4,1],[1593,9,1593,21,0,0,4,0],[1593,9,1593,21,0,0,4,1],[1594,9,1594,46,0,0,4,0],[1599,9,1599,25,0,0,4,0],[1599,9,1599,25,0,0,4,1],[1601,13,1601,40,0,0,4,0],[1601,13,1601,40,0,0,4,1],[1605,17,1605,23,0,0,4,0],[1608,18,1610,65,0,0,4,0],[1608,18,1610,65,0,0,4,1],[1619,13,1619,46,0,0,4,0],[1619,13,1619,46,0,0,4,1],[1621,13,1621,30,0,0,4,1],[1630,17,1630,45,0,0,4,1],[1637,22,1639,62,0,0,4,0],[1637,22,1639,62,0,0,4,1],[1645,9,1645,12,0,0,4,0],[1645,9,1645,12,0,0,4,1],[1650,9,1650,21,0,0,4,0],[1650,9,1650,21,0,0,4,1],[1651,9,1651,46,0,0,4,0],[321,7,321,45,6,0,4,0],[321,7,321,45,6,0,4,1]],"TIFFWriteDirectorySec":[[527,13,527,31,0,0,4,0],[527,35,527,75,0,0,4,0],[529,17,529,37,0,0,4,1]],"TIFFComputeStrip":[[48,9,48,53,0,0,4,0],[50,13,50,45,0,0,4,1]],"_TIFFRewriteField":[[3519,13,3519,20,0,0,4,0]],"TIFFForceStrileArrayWriting":[[121,27,121,34,0,0,4,0]]},"library_calls":["TIFFOpen","TIFFOpen","TIFFNumberOfDirectories","TIFFReadEXIFDirectory","TIFFGetTagListEntry","TIFFFieldWithTag","TIFFFieldSetGetSize","TIFFGetField","TIFFGetField","TIFFComputeStrip","TIFFRasterScanlineSize","TIFFTileSize","_TIFFmalloc","TIFFWriteRawTile","TIFFReadFromUserBuffer","_TIFFmalloc","TIFFSwabArrayOfLong8","_TIFFfree","TIFFUnlinkDirectory","_TIFFfree","TIFFClose"],"critical_calls":["TIFFOpen","TIFFOpen","TIFFNumberOfDirectories","TIFFReadEXIFDirectory","TIFFGetTagListEntry","TIFFFieldWithTag","TIFFFieldSetGetSize","TIFFGetField","TIFFGetField","TIFFComputeStrip","TIFFRasterScanlineSize","TIFFTileSize","_TIFFmalloc","TIFFWriteRawTile","TIFFReadFromUserBuffer","_TIFFmalloc","TIFFSwabArrayOfLong8","_TIFFfree","TIFFUnlinkDirectory","_TIFFfree","TIFFClose"],"visited":1}
/**/


extern "C" {
}

// We need to define the missing function prototype for TIFFOpenOptionsFree
extern "C" void TIFFOpenOptionsFree(TIFFOpenOptions*);

// Function prototypes for APIs we need to use
extern "C" {
    uint32_t TIFFComputeStrip(TIFF*, uint32_t, uint16_t);
    int TIFFReadFromUserBuffer(TIFF* tif, uint32_t strile, void* inbuf, tmsize_t insize, void* outbuf, tmsize_t outsize);
    int TIFFFieldSetGetSize(const TIFFField*);
    void TIFFSwabArrayOfLong8(uint64_t* lp, tmsize_t n);
    int TIFFUnlinkDirectory(TIFF*, tdir_t);
}

// Custom fuzz driver following the requirements
extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // Write input data to a file for TIFF to read
    FILE* in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    TIFF* tif = nullptr;
    void* malloc_buffer = nullptr;
    uint64_t* long8_array = nullptr;
    
    // Open the TIFF file for reading and writing
    tif = TIFFOpen("input_file", fuzz_str_1);
    if (!tif) {
        // Try opening as read-only if read-write fails
        tif = TIFFOpen("input_file", fuzz_str_2);
    }
    
    if (tif) {
        // Get number of directories
        tdir_t num_dirs = TIFFNumberOfDirectories(tif);
        
        // Try to read EXIF directory if possible
        if (num_dirs > 0) {
            // Try reading EXIF directory at offset 0 (common location)
            TIFFReadEXIFDirectory(tif, 0);
        }
        
        // Get tag list entry
        uint32_t tag_entry = TIFFGetTagListEntry(tif, fuzz_int32_t_3);
        
        // Get field with specific tag
        const TIFFField* field = TIFFFieldWithTag(tif, tag_entry);
        if (field) {
            // Get field size
            int field_size = TIFFFieldSetGetSize(field);
        }
        
        // Get image dimensions if available
        uint32_t width = 0, height = 0;
        if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
            
            // Compute strip for a specific row
            if (height > 0) {
                uint16_t sample = 0;  // Assuming first sample
                uint32_t strip = TIFFComputeStrip(tif, fuzz_uint32_t_4, fuzz_uint16_t_5);
                
                // Get raster scanline size
                tmsize_t scanline_size = TIFFRasterScanlineSize(tif);
                
                // Get tile size
                tmsize_t tile_size = TIFFTileSize(tif);
                
                // Allocate buffers for reading/writing
                if (tile_size > 0 && tile_size < 1000000) {  // Reasonable size limit
                    malloc_buffer = _TIFFmalloc(tile_size);
                    
                    if (malloc_buffer && size >= tile_size) {
                        // Prepare some data from the input
                        size_t copy_size = (size < tile_size) ? size : tile_size;
                        memcpy(malloc_buffer, data, copy_size);
                        
                        // Try to write a tile (tile 0)
                        TIFFWriteRawTile(tif, fuzz_uint32_t_6, malloc_buffer, tile_size);
                        
                        // Try to read from user buffer
                        // We'll use a small buffer for testing
                        char small_inbuf[100];
                        char small_outbuf[100];
                        if (size >= 100) {
                            memcpy(small_inbuf, data, 100);
                            TIFFReadFromUserBuffer(tif, fuzz_uint32_t_7, small_inbuf, 100, small_outbuf, 100);
                        }
                    }
                }
            }
        }
        
        // Process some long8 data from input
        if (size >= sizeof(uint64_t) * 4) {
            long8_array = (uint64_t*)_TIFFmalloc(sizeof(uint64_t) * 4);
            if (long8_array) {
                memcpy(long8_array, data, sizeof(uint64_t) * 4);
                TIFFSwabArrayOfLong8(long8_array, 4);
                _TIFFfree(long8_array);
            }
        }
        
        // Try to unlink a directory if there are multiple
        if (num_dirs > 1) {
            TIFFUnlinkDirectory(tif, num_dirs - 1);  // Try to unlink last directory
        }
        
        // Clean up
        if (malloc_buffer) {
            _TIFFfree(malloc_buffer);
        }
        
        TIFFClose(tif);
    }
    
    // Clean up temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}