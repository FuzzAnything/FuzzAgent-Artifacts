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
//<ID> 533
//<Prompt> []
/*<Combination>: [
*/
//<score> 136, nr_unique_branch: 17
//<Quality> {"density":17,"unique_branches":{"Fax3VGetField":[[1434,9,1434,35,0,0,4,0],[1437,9,1437,33,0,0,4,0]],"TIFFWriteDirectorySec":[[849,21,851,58,0,0,4,1],[236,34,236,71,75,0,4,0]],"TIFFWriteRawTile":[[40,47,40,79,1,0,4,1]],"TIFFFieldSetGetSize":[[775,9,775,37,0,0,4,0]],"_TIFFVSetField":[[808,21,808,55,0,0,4,0],[809,21,809,61,0,0,4,0]],"TIFFWriteCheck":[[614,9,614,24,0,0,4,0],[617,23,617,28,0,0,4,0]],"TIFFAppendToStrip":[[794,45,794,65,0,0,4,0],[799,13,799,45,0,0,4,0],[800,13,800,59,0,0,4,0],[800,13,800,59,0,0,4,1],[809,17,809,24,0,0,4,1],[847,9,847,35,0,0,4,0],[847,39,847,64,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFOpenExt","TIFFOpenOptionsFree","TIFFFileName","TIFFCurrentDirectory","TIFFFieldWithTag","TIFFFieldDataType","TIFFDefaultTileSize","TIFFGetReadProc","TIFFSwabShort","TIFFWriteRawTile","TIFFCurrentDirOffset","TIFFSetWriteOffset","TIFFRewriteDirectory","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFOpenExt","TIFFFileName","TIFFCurrentDirectory","TIFFFieldWithTag","TIFFFieldDataType","TIFFDefaultTileSize","TIFFGetReadProc","TIFFSwabShort","TIFFWriteRawTile","TIFFCurrentDirOffset","TIFFSetWriteOffset","TIFFRewriteDirectory","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/


// Include libtiff headers

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 1: Create a temporary file and write input data to it
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Step 2: Allocate and configure TIFFOpenOptions
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Set a warning handler (using NULL for simplicity)
    TIFFOpenOptionsSetWarningHandlerExtR(opts, NULL, NULL);
    
    // Step 3: Open the TIFF file
    TIFF *tif = TIFFOpenExt("input_file", "r+", opts);
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 4: Use various TIFF APIs as specified
    
    // Get filename
    const char *filename = TIFFFileName(tif);
    
    // Get current directory
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    
    // Get field with a specific tag (using TIFFTAG_IMAGEWIDTH as example)
    const TIFFField *field = TIFFFieldWithTag(tif, TIFFTAG_IMAGEWIDTH);
    if (field) {
        // Get field data type
        TIFFDataType data_type = TIFFFieldDataType(field);
    }
    
    // Get default tile size
    uint32_t tile_width = 0, tile_height = 0;
    TIFFDefaultTileSize(tif, &tile_width, &tile_height);
    
    // Get read procedure
    TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
    
    // Prepare tile data (using a portion of input data)
    uint32_t tile_index = 0;
    tmsize_t tile_data_size = size > 1024 ? 1024 : size;
    void *tile_data = malloc(tile_data_size);
    if (tile_data && size > 0) {
        memcpy(tile_data, data, tile_data_size);
        
        // Byte-swap a short for demonstration
        if (tile_data_size >= 2) {
            TIFFSwabShort((uint16_t*)tile_data);
        }
        
        // Write raw tile
        TIFFWriteRawTile(tif, tile_index, tile_data, tile_data_size);
        
        free(tile_data);
    }
    
    // Set write offset (to current position)
    toff_t current_offset = TIFFCurrentDirOffset(tif);
    TIFFSetWriteOffset(tif, current_offset);
    
    // Rewrite directory
    TIFFRewriteDirectory(tif);
    
    // Step 5: Clean up resources
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    // Step 6: Create output file and verify we can write to it
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output file
        if (size > 0) {
            fwrite(data, 1, size > 100 ? 100 : size, out_file);
        }
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // Clean up input file
    remove("input_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}