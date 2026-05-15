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
//<ID> 1066
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"TIFFErrorExt":[[66,9,66,26,0,0,4,0],[72,9,72,29,0,0,4,1]]},"library_calls":["TIFFClientOpen","TIFFIsCODECConfigured","TIFFGetConfiguredCODECs","TIFFSetDirectory","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFReadRGBATile","TIFFGetMapFileProc","TIFFSwabArrayOfTriples","TIFFSwabArrayOfLong","TIFFErrorExt","_TIFFfree","TIFFClose","TIFFErrorExt"],"critical_calls":["TIFFClientOpen","TIFFIsCODECConfigured","TIFFGetConfiguredCODECs","TIFFSetDirectory","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFReadRGBATile","TIFFGetMapFileProc","TIFFSwabArrayOfTriples","TIFFSwabArrayOfLong","TIFFErrorExt","_TIFFfree","TIFFClose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<89) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a file so libtiff can read it
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Reopen for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for TIFFOpen
    int fd = fuzz_fileno(in_file);
    
    // Open TIFF using file descriptor
    TIFF* tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, (thandle_t)(intptr_t)fd,
                               NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    
    if (tif) {
        // Call TIFFDeferStrileArrayWriting if available
        // Note: This function may not exist in all libtiff versions
        // We'll try to use it if TIFF supports it
        #ifdef TIFFDeferStrileArrayWriting
        TIFFDeferStrileArrayWriting(tif);
        #endif
        
        // Check if LZW codec is configured (example)
        int codec_configured = TIFFIsCODECConfigured(fuzz_uint16_t_3);
        
        // Get configured codecs
        TIFFCodec* codecs = TIFFGetConfiguredCODECs();
        
        // Try to set to first directory
        TIFFSetDirectory(tif, fuzz_uint32_t_4);
        
        // Check if image is tiled
        if (TIFFIsTiled(tif)) {
            uint32_t tile_width, tile_height;
            TIFFGetField(tif, fuzz_uint32_t_5, &tile_width);
            TIFFGetField(tif, fuzz_uint32_t_6, &tile_height);
            
            // Allocate buffer for RGBA tile
            std::vector<uint32_t> rgba_buffer(tile_width * tile_height);
            
            // Read first tile if coordinates are valid
            if (tile_width > 0 && tile_height > 0) {
                TIFFReadRGBATile(tif, fuzz_uint32_t_7, fuzz_uint32_t_8, rgba_buffer.data());
            }
        }
        
        // Get map file procedure
        TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);
        
        // Prepare arrays for byte swapping
        const size_t array_size = 100;
        
        // Test TIFFSwabArrayOfTriples
        std::vector<uint8_t> triples(array_size * 3);
        if (triples.size() > 0) {
            memcpy(triples.data(), data, std::min(triples.size(), size));
            TIFFSwabArrayOfTriples(triples.data(), array_size);
        }
        
        // Test TIFFSwabArrayOfLong
        std::vector<uint32_t> longs(array_size);
        if (longs.size() * sizeof(uint32_t) > 0) {
            memcpy(longs.data(), data, 
                   std::min(longs.size() * sizeof(uint32_t), size));
            TIFFSwabArrayOfLong(longs.data(), array_size);
        }
        
        // Use TIFFErrorExt for error reporting
        if (size == 0) {
            TIFFErrorExt((thandle_t)tif, fuzz_str_9, 
                        "Empty input data");
        }
        
        // Clean up codecs
        if (codecs) {
            _TIFFfree(codecs);
        }
        
        // Clean up TIFF
        TIFFClose(tif);
    } else {
        // Use TIFFErrorExt when opening fails
        TIFFErrorExt(NULL, fuzz_str_10, 
                    "Failed to open TIFF from input data");
    }
    
    // Close file
    assert_file_closed(&in_file);;
    
    // Remove temporary file
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}