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
//<ID> 534
//<Prompt> []
/*<Combination>: [
*/
//<score> 180, nr_unique_branch: 21
//<Quality> {"density":20,"unique_branches":{"TIFFForceStrileArrayWriting":[[81,9,81,34,0,0,4,1],[86,9,86,29,0,0,4,0]],"TIFFWriteCheck":[[661,9,661,58,0,0,4,0],[662,9,662,57,0,0,4,1]],"_TIFFErrorEarly":[[84,9,84,13,0,0,4,0],[84,17,84,35,0,0,4,1]],"TIFFAppendToStrip":[[791,9,791,29,0,0,4,1],[794,45,794,65,0,0,4,1],[799,13,799,45,0,0,4,1],[841,34,841,52,0,0,4,0]],"TIFFFlush":[[44,9,44,45,0,0,4,0],[44,49,44,72,0,0,4,0],[46,13,46,45,0,0,4,1],[51,9,51,35,0,0,4,0]],"TIFFWriteDirectorySec":[[606,21,608,78,0,0,4,0]],"TIFFClampDoubleToUInt16":[[1510,9,1510,16,0,0,4,0],[1510,9,1510,16,0,0,4,1],[1512,9,1512,20,0,0,4,0]],"TIFFWriteDirectoryTagSampleformatArray":[[1589,22,1589,57,0,0,4,0],[1591,29,1591,38,0,0,4,0],[1591,29,1591,38,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFOpenExt","TIFFOpenOptionsFree","TIFFFileName","TIFFCurrentDirectory","TIFFFieldWithTag","TIFFFieldDataType","TIFFDefaultTileSize","TIFFSwabShort","TIFFGetReadProc","TIFFSetWriteOffset","TIFFIsTiled","TIFFNumberOfTiles","TIFFNumberOfTiles","TIFFTileSize","_TIFFmalloc","TIFFWriteRawTile","_TIFFfree","TIFFRewriteDirectory","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFOpenExt","TIFFFileName","TIFFCurrentDirectory","TIFFFieldWithTag","TIFFFieldDataType","TIFFDefaultTileSize","TIFFSwabShort","TIFFGetReadProc","TIFFSetWriteOffset","TIFFIsTiled","TIFFNumberOfTiles","TIFFNumberOfTiles","TIFFTileSize","_TIFFmalloc","TIFFWriteRawTile","_TIFFfree","TIFFRewriteDirectory","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 1: Write input data to a temporary file
    FILE *temp_in = fopen("input_file", "wb");
    if (!temp_in) {
	assert_file_closed(&temp_in);
	return 0;
	}
	
    fwrite(data, 1, size, temp_in);
    assert_file_closed(&temp_in);;
    
    // Step 2: Allocate TIFF open options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove("input_file");
        assert_file_closed(&temp_in);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 3: Set warning handler (using NULL handler for simplicity)
    TIFFOpenOptionsSetWarningHandlerExtR(opts, nullptr, nullptr);
    
    // Step 4: Open TIFF file with options
    TIFF *tif = TIFFOpenExt("input_file", "r+", opts);
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        remove("input_file");
        assert_file_closed(&temp_in);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 5: Get file name (call API)
    const char *filename = TIFFFileName(tif);
    
    // Step 6: Get current directory
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    
    // Step 7: Get field by tag (using a common tag - ImageWidth = 256)
    const TIFFField *field = TIFFFieldWithTag(tif, 256);
    if (field) {
        // Step 8: Get field data type
        TIFFDataType data_type = TIFFFieldDataType(field);
    }
    
    // Step 9: Get default tile size
    uint32_t tile_width = 0, tile_height = 0;
    TIFFDefaultTileSize(tif, &tile_width, &tile_height);
    
    // Step 10: Byte swap example
    if (size >= 2) {
        uint16_t sample = static_cast<uint16_t>(data[0] | (data[1] << 8));
        TIFFSwabShort(&sample);
    }
    
    // Step 11: Get read procedure
    TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
    
    // Step 12: Set write offset (use current position)
    if (size >= 8) {
        uint64_t offset;
        memcpy(&offset, data, sizeof(uint64_t));
        offset %= 1024 * 1024; // Limit to reasonable range
        TIFFSetWriteOffset(tif, static_cast<toff_t>(offset));
    }
    
    // Step 13: Write raw tile if data available
    if (size > 0 && TIFFIsTiled(tif)) {
        uint32_t tile_index = 0;
        if (TIFFNumberOfTiles(tif) > 0) {
            tile_index = current_dir % TIFFNumberOfTiles(tif);
            
            // Prepare tile data (use input data, limited to reasonable size)
            tmsize_t tile_size = TIFFTileSize(tif);
            if (tile_size > 0) {
                void *tile_data = _TIFFmalloc(tile_size);
                if (tile_data) {
                    size_t copy_size = (size < static_cast<size_t>(tile_size)) ? 
                                       size : static_cast<size_t>(tile_size);
                    memcpy(tile_data, data, copy_size);
                    
                    // Write raw tile
                    TIFFWriteRawTile(tif, tile_index, tile_data, tile_size);
                    
                    _TIFFfree(tile_data);
                }
            }
        }
    }
    
    // Step 14: Rewrite directory
    TIFFRewriteDirectory(tif);
    
    // Step 15: Close TIFF
    TIFFClose(tif);
    
    // Step 16: Free options
    TIFFOpenOptionsFree(opts);
    
    // Cleanup temporary file
    remove("input_file");
    
    assert_file_closed(&temp_in);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}