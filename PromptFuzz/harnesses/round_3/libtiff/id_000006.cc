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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 2
//<Quality> {"density":20,"unique_branches":{"TIFFClientOpenExt":[[356,17,356,50,0,0,4,0],[357,9,357,54,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenOptionsFree","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFIsMSB2LSB","TIFFGetTagListCount","TIFFGetTagListEntry","TIFFNumberOfStrips","TIFFRawStripSize","_TIFFmalloc","TIFFReadRawStrip","_TIFFfree","TIFFIsTiled","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBAStripExt","_TIFFfree","TIFFDefaultTileSize","uv_encode","TIFFDeferStrileArrayWriting","TIFFWarning","TIFFClose","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","_TIFFmalloc","TIFFWriteScanline","_TIFFfree","TIFFClose"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFIsMSB2LSB","TIFFGetTagListCount","TIFFGetTagListEntry","TIFFNumberOfStrips","TIFFRawStripSize","_TIFFmalloc","TIFFReadRawStrip","_TIFFfree","TIFFIsTiled","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBAStripExt","_TIFFfree","TIFFDefaultTileSize","uv_encode","TIFFDeferStrileArrayWriting","TIFFWarning","TIFFClose","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","_TIFFmalloc","TIFFWriteScanline","_TIFFfree","TIFFClose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<123) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_18, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_19, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_20, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_21, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_22, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // 2. Create TIFFOpenOptions and set memory limits
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts) {
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 100 * 1024 * 1024); // 100MB limit
    }
    
    // 3. Open input TIFF file
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        if (opts) TIFFOpenOptionsFree(opts);
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }
	int fuzz_fd_0 = fuzz_fileno(in_file);

    
    TIFF *tif = TIFFClientOpenExt(fuzz_str_1, fuzz_str_2, 
                                  (thandle_t)fuzz_fd_0,
                                  TIFFReadWriteProc(0), TIFFReadWriteProc(0),
                                  TIFFSeekProc(0), TIFFCloseProc(0),
                                  TIFFSizeProc(0), TIFFMapFileProc(0),
                                  TIFFUnmapFileProc(0), opts);
    if (!tif) {
        assert_file_closed(&in_file);;
        if (opts) TIFFOpenOptionsFree(opts);
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // 4. Use TIFFIsMSB2LSB to check fill order
    int is_msb2lsb = TIFFIsMSB2LSB(tif);
    
    // 5. Get tag list entries
    int tag_count = TIFFGetTagListCount(tif);
    for (int i = 0; i < tag_count && i < 10; i++) {
        uint32_t tag = TIFFGetTagListEntry(tif, fuzz_int32_t_3);
        // Just access the tag, could be used for further operations
    }
    
    // 6. Read raw strip data
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    if (strip_count > 0) {
        tmsize_t strip_size = TIFFRawStripSize(tif, fuzz_uint32_t_4);
        if (strip_size > 0 && strip_size < 10 * 1024 * 1024) {
            void *strip_buffer = _TIFFmalloc(strip_size);
            if (strip_buffer) {
                tmsize_t bytes_read = TIFFReadRawStrip(tif, fuzz_uint32_t_5, strip_buffer, strip_size);
                _TIFFfree(strip_buffer);
            }
        }
    }
    
    // 7. Read RGBA strip if image is stripped (not tiled)
    if (!TIFFIsTiled(tif) && strip_count > 0) {
        uint32_t width, height;
        TIFFGetField(tif, fuzz_uint32_t_6, &width);
        TIFFGetField(tif, fuzz_uint32_t_7, &height);
        
        if (width > 0 && height > 0 && width < 10000 && height < 10000) {
            uint32_t *raster = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
            if (raster) {
                // Read first strip as RGBA
                int result = TIFFReadRGBAStripExt(tif, fuzz_uint32_t_8, raster, fuzz_int32_t_9);
                _TIFFfree(raster);
            }
        }
    }
    
    // 8. Get default tile size (even for non-tiled images)
    uint32_t tile_width = 0, tile_height = 0;
    TIFFDefaultTileSize(tif, &tile_width, &tile_height);
    
    // 9. Use uv_encode with some test values
    int uv_result = uv_encode(0.5, 0.5, fuzz_int32_t_10);
    
    // 10. Enable defer strile array writing (for potential write operations)
    TIFFDeferStrileArrayWriting(tif);
    
    // 11. Create warning using TIFFWarning
    TIFFWarning(fuzz_str_11, fuzz_str_12);
    
    // 12. Cleanup
    TIFFClose(tif);
    assert_file_closed(&in_file);;
    
    // 13. Create output file for testing write operations
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Create output TIFF for testing write operations
        TIFF *out_tif = TIFFOpen("output_file", fuzz_str_13);
        if (out_tif) {
            // Set some basic parameters
            TIFFSetField(out_tif, fuzz_uint32_t_14, 64);
            TIFFSetField(out_tif, fuzz_uint32_t_15, 64);
            TIFFSetField(out_tif, fuzz_uint32_t_16, 8);
            TIFFSetField(out_tif, fuzz_uint32_t_17, 3);
            TIFFSetField(out_tif, fuzz_uint32_t_18, PHOTOMETRIC_RGB);
            TIFFSetField(out_tif, fuzz_uint32_t_19, PLANARCONFIG_CONTIG);
            TIFFSetField(out_tif, fuzz_uint32_t_20, COMPRESSION_NONE);
            
            // Write some dummy data
            unsigned char *buffer = (unsigned char*)_TIFFmalloc(64 * 64 * 3);
            if (buffer) {
                memset(buffer, 128, 64 * 64 * 3);
                for (uint32_t row = 0; row < 64; row++) {
                    TIFFWriteScanline(out_tif, buffer + row * 64 * 3, fuzz_uint32_t_21, fuzz_uint16_t_22);
                }
                _TIFFfree(buffer);
            }
            
            TIFFClose(out_tif);
        }
        assert_file_closed(&out_file);;
    }
    
    // 14. Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fuzz_fd_0);
	return 0;
}