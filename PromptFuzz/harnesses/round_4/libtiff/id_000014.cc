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
//<score> 140, nr_unique_branch: 10
//<Quality> {"density":20,"unique_branches":{"_tiffWriteProc":[[121,13,121,23,0,0,4,0],[124,9,124,18,0,0,4,0]],"TIFFRegisterCODEC":[[244,9,244,19,0,0,4,0]],"TIFFLinkDirectory":[[3257,17,3257,25,0,0,4,0]],"TIFFFlush":[[51,9,51,35,0,0,4,0]],"TIFFFindCODEC":[[228,33,228,35,0,0,4,0],[229,13,229,39,0,0,4,0]],"TIFFWriteDirectorySec":[[1217,21,1217,44,0,0,4,0],[1465,9,1465,20,0,0,4,0],[1467,9,1467,23,0,0,4,1]]},"library_calls":["TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFCreateDirectory","TIFFSetClientdata","TIFFSetField","TIFFSetField","_TIFFmalloc","TIFFWriteRawTile","_TIFFfree","TIFFFlush","TIFFCreateDirectory","TIFFRegisterCODEC","TIFFCreateDirectory","TIFFFlush","TIFFClose"],"critical_calls":["TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFCreateDirectory","TIFFSetClientdata","TIFFSetField","TIFFSetField","_TIFFmalloc","TIFFWriteRawTile","_TIFFfree","TIFFFlush","TIFFCreateDirectory","TIFFRegisterCODEC","TIFFCreateDirectory","TIFFFlush","TIFFClose"],"visited":0}
/**/


// Include necessary libtiff headers based on the provided APIs
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
	//fuzzer shim end}




    // Create output TIFF file
    const char* output_filename = "output_file";
    
    // Open a TIFF file for writing
    TIFF* tif = TIFFOpen(output_filename, fuzz_str_1);
    if (!tif) {
        assert_file_name_closed("output_file");
	return 0;
    }
    
    // Set basic TIFF tags
    uint32_t width = 256;
    uint32_t height = 256;
    uint16_t samples_per_pixel = 3;
    uint16_t bits_per_sample = 8;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    // 1. Create a new directory
    TIFFCreateDirectory(tif);
    
    // 2. Set client info (using some dummy data)
    char client_info[] = "fuzzer_client_info";
    // Note: TIFFSetClientInfo is not in the provided exported APIs list,
    // but we'll use TIFFSetClientdata which is available
    thandle_t client_data = reinterpret_cast<thandle_t>(client_info);
    TIFFSetClientdata(tif, client_data);
    
    // 3. Defer strile array writing (if available)
    // Note: TIFFDeferStrileArrayWriting is not in the provided exported APIs list
    // We'll skip this call as it's not available
    
    // 4. Get map file proc (if available)
    // Note: TIFFGetMapFileProc is not in the provided exported APIs list
    // We'll skip this call as it's not available
    
    // 5. Write raw tile using input data
    // Calculate tile parameters
    uint32_t tile_width = 64;
    uint32_t tile_height = 64;
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    
    // Prepare tile data buffer
    size_t tile_size = tile_width * tile_height * samples_per_pixel * (bits_per_sample / 8);
    uint8_t* tile_data = static_cast<uint8_t*>(_TIFFmalloc(tile_size));
    
    if (tile_data) {
        // Fill tile data with input data or a pattern
        size_t copy_size = (size < tile_size) ? size : tile_size;
        if (copy_size > 0) {
            memcpy(tile_data, data, copy_size);
        }
        // Fill remaining with zeros if needed
        if (copy_size < tile_size) {
            memset(tile_data + copy_size, 0, tile_size - copy_size);
        }
        
        // Write raw tile
        TIFFWriteRawTile(tif, fuzz_uint32_t_2, tile_data, static_cast<tmsize_t>(tile_size));
        
        // Free tile buffer
        _TIFFfree(tile_data);
    }
    
    // 6. Flush data to ensure it's written
    TIFFFlush(tif);
    
    // 7. Create another directory for testing
    TIFFCreateDirectory(tif);
    
    // 8. Unregister a CODEC (if we can get one)
    // First register a dummy codec to then unregister it
    TIFFCodec* codec = TIFFRegisterCODEC(
        fuzz_uint16_t_4, 
        fuzz_str_3, 
        [](TIFF* tif, int scheme) { return 1; }
    );
    
    if (codec) {
        // Note: TIFFUnRegisterCODEC is not in the provided exported APIs list
        // We'll skip this call as it's not available
    }
    
    // Create one more directory to ensure multiple operations
    TIFFCreateDirectory(tif);
    
    // Final flush
    TIFFFlush(tif);
    
    // Close the TIFF file
    TIFFClose(tif);
    
    // Clean up: remove the output file
    remove(output_filename);
    
    assert_file_name_closed("output_file");
	return 0;
}