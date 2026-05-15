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
//<ID> 1265
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"TIFFWarningExtR":[[92,9,92,28,0,0,4,1]]},"library_calls":["TIFFSetWarningHandler","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenOptionsSetWarnAboutUnknownTags","TIFFClientOpenExt","TIFFOpen","TIFFGetSeekProc","TIFFPrintDirectory","TIFFCurrentStrip","TIFFTileRowSize","TIFFFieldWithTag","TIFFFieldDataType","TIFFGetField","TIFFGetField","TIFFGetField","TIFFReadRGBATile","TIFFTileSize","TIFFReadTile","TIFFSetField","TIFFSwabArrayOfShort","TIFFNumberOfTiles","TIFFWriteEncodedTile","TIFFTileSize","TIFFWarningExtR","TIFFClose","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFSetWarningHandler","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenOptionsSetWarnAboutUnknownTags","TIFFClientOpenExt","TIFFOpen","TIFFGetSeekProc","TIFFPrintDirectory","TIFFCurrentStrip","TIFFTileRowSize","TIFFFieldWithTag","TIFFFieldDataType","TIFFGetField","TIFFGetField","TIFFGetField","TIFFReadRGBATile","TIFFTileSize","TIFFReadTile","TIFFSetField","TIFFSwabArrayOfShort","TIFFNumberOfTiles","TIFFWriteEncodedTile","TIFFTileSize","TIFFWarningExtR","TIFFClose","TIFFClose","TIFFOpenOptionsFree"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<129) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_18, fuzz_str_sz_18, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_19, fuzz_str_sz_19, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
	return 0;
	}
	
    
    // 1. Write input data to file for TIFF to read
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // 2. Set up warning handler
    TIFFErrorHandler prev_warning = TIFFSetWarningHandler(nullptr);
    
    // 3. Create and configure TIFFOpenOptions
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, size * 2);
    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, fuzz_int32_t_1);
    
    // 4. Open TIFF file with custom options
    TIFF *tif = TIFFClientOpenExt(fuzz_str_2, fuzz_str_3, 
        (thandle_t)0,  // Simplified handle
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, opts);
    
    // Also try regular TIFFOpen for comparison
    TIFF *tif2 = TIFFOpen("input_file", fuzz_str_4);
    
    if (tif) {
        // 5. Get various TIFF information
        TIFFGetSeekProc(tif);  // Get seek procedure
        
        // 6. Print directory to output file
        FILE *out_file = fopen("output_file", "w");
        if (out_file) {
            TIFFPrintDirectory(tif, out_file, fuzz_int64_t_5);
            assert_file_closed(&out_file);;
        }
        
        // 7. Try to get current strip
        uint32_t current_strip = TIFFCurrentStrip(tif);
        
        // 8. Get tile row size
        tmsize_t tile_row_size = TIFFTileRowSize(tif);
        
        // 9. Try to get a field by tag
        const TIFFField *field = TIFFFieldWithTag(tif, fuzz_uint32_t_6);
        if (field) {
            TIFFDataType data_type = TIFFFieldDataType(field);
        }
        
        // 10. Try to get field value
        uint32_t width = 0;
        TIFFGetField(tif, fuzz_uint32_t_7, &width);
        
        // 11. Try to read RGB tile if image is tiled
        uint32_t tile_width = 0, tile_height = 0;
        if (TIFFGetField(tif, fuzz_uint32_t_8, &tile_width) &&
            TIFFGetField(tif, fuzz_uint32_t_9, &tile_height)) {
            
            // Allocate buffer for RGBA tile
            std::vector<uint32_t> rgba_buffer(tile_width * tile_height);
            if (TIFFReadRGBATile(tif, fuzz_uint32_t_10, fuzz_uint32_t_11, rgba_buffer.data())) {
                // Process tile if needed
            }
            
            // 12. Try to read raw tile
            std::vector<uint8_t> tile_buffer(TIFFTileSize(tif));
            if (TIFFReadTile(tif, tile_buffer.data(), fuzz_uint32_t_12, fuzz_uint32_t_13, fuzz_uint32_t_14, fuzz_uint16_t_15) > 0) {
                // Process tile data
            }
        }
        
        // 13. Try to set a field
        TIFFSetField(tif, fuzz_uint32_t_16, PLANARCONFIG_CONTIG);
        
        // 14. Byte swapping example
        if (size >= 4) {
            uint16_t short_array[2];
            memcpy(short_array, data, 4);
            TIFFSwabArrayOfShort(short_array, 2);
        }
        
        // 15. Try to write encoded tile if we have tile data
        std::vector<uint8_t> write_buffer(1024);
        if (size > 1024) {
            memcpy(write_buffer.data(), data, 1024);
            // Only attempt if we have a valid tile number
            if (TIFFNumberOfTiles(tif) > 0) {
                TIFFWriteEncodedTile(tif, fuzz_uint32_t_17, write_buffer.data(), 
                    std::min((tmsize_t)1024, TIFFTileSize(tif)));
            }
        }
        
        // 16. Issue a warning (for demonstration)
        TIFFWarningExtR(tif, fuzz_str_18, fuzz_str_19);
        
        TIFFClose(tif);
    }
    
    if (tif2) {
        TIFFClose(tif2);
    }
    
    // 17. Clean up options
    TIFFOpenOptionsFree(opts);
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}