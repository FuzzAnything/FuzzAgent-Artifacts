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
//<ID> 1516
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 5
//<Quality> {"density":16,"unique_branches":{"TIFFGetBitRevTable":[[307,13,307,21,0,0,4,1]],"PickSeparateCase":[[3301,9,3301,29,0,0,4,0],[3345,46,3345,73,0,0,4,1]],"TIFFSwabArrayOfTriples":[[105,12,105,19,0,0,4,0],[105,12,105,19,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenExt","TIFFOpenOptionsFree","TIFFReadDirectory","TIFFGetField","TIFFIsTiled","TIFFTileSize","_TIFFmalloc","TIFFCheckTile","TIFFReadTile","TIFFGetBitRevTable","TIFFSwabArrayOfTriples","_TIFFfree","TIFFRGBAImageBegin","TIFFRGBAImageEnd","TIFFCheckpointDirectory","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenExt","TIFFReadDirectory","TIFFGetField","TIFFIsTiled","TIFFTileSize","_TIFFmalloc","TIFFCheckTile","TIFFReadTile","TIFFGetBitRevTable","TIFFSwabArrayOfTriples","_TIFFfree","TIFFRGBAImageBegin","TIFFRGBAImageEnd","TIFFCheckpointDirectory","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs to process TIFF files from byte streams:

*/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
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

    // 1. Allocate TIFFOpenOptions
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 2. Open TIFF file with options
    TIFF *tif = TIFFOpenExt("input_file", fuzz_str_1, opts);
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 3. Read first directory
    TIFFReadDirectory(tif);

    // 4. Try to get a field (ImageWidth = 256)
    uint32_t width = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);

    // 5. Check if tiled and process tiles if available
    if (TIFFIsTiled(tif)) {
        // Get tile size
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0) {
            // Allocate buffer for tile
            void *tile_buf = _TIFFmalloc(tile_size);
            if (tile_buf) {
                // Read first tile (x=0, y=0, z=0, sample=0)
                if (TIFFCheckTile(tif, fuzz_uint32_t_2, fuzz_uint32_t_3, fuzz_uint32_t_4, fuzz_uint16_t_5)) {
                    TIFFReadTile(tif, tile_buf, fuzz_uint32_t_6, fuzz_uint32_t_7, fuzz_uint32_t_8, fuzz_uint16_t_9);
                    
                    // Demonstrate bit reversal table usage
                    const unsigned char *bit_rev_table = TIFFGetBitRevTable(fuzz_int32_t_10);
                    if (bit_rev_table) {
                        // Create a small test array for swabbing
                        uint8_t triples[9];
                        if (tile_size >= 9) {
                            memcpy(triples, tile_buf, 9);
                            TIFFSwabArrayOfTriples(triples, 3);
                        }
                    }
                }
                _TIFFfree(tile_buf);
            }
        }
    }

    // 6. Setup RGBA image processing
    TIFFRGBAImage rgba;
    char emsg[1024];
    if (TIFFRGBAImageBegin(&rgba, tif, fuzz_int32_t_11, emsg)) {
        // Image processing could go here
        // For fuzzing, we just verify we can begin
        TIFFRGBAImageEnd(&rgba);
    }

    // 7. Checkpoint directory
    TIFFCheckpointDirectory(tif);

    // 8. Cleanup
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    // Remove temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}