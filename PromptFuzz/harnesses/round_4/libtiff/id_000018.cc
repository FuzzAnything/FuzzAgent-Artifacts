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
//<ID> 667
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 57
//<Quality> {"density":8,"unique_branches":{"TIFFWriteRawStrip":[[38,6,38,43,1,0,4,1],[38,47,38,79,1,0,4,1]],"InitCCITTFax3":[[1558,9,1558,32,0,0,4,1]],"_TIFFgetMode":[[64,17,64,31,0,0,4,0]],"cmpTIFFEntryOffsetAndLength":[[4233,9,4233,32,0,0,4,0],[4233,9,4233,32,0,0,4,1],[4235,14,4235,37,0,0,4,0],[4235,14,4235,37,0,0,4,1]],"ChopUpSingleUncompressedStrip":[[8017,27,8017,52,0,0,4,0],[8056,9,8056,34,0,0,4,1]],"TIFFReadDirectory":[[4694,35,4694,58,0,0,4,0],[4695,35,4695,54,0,0,4,0],[4695,35,4695,54,0,0,4,1],[4695,58,4695,76,0,0,4,0],[4695,58,4695,76,0,0,4,1],[4696,35,4696,66,0,0,4,0],[4723,35,4723,58,0,0,4,0],[4724,35,4724,54,0,0,4,0],[4724,35,4724,54,0,0,4,1],[4724,58,4724,76,0,0,4,0],[4724,58,4724,76,0,0,4,1],[4725,35,4725,66,0,0,4,0],[5015,9,5015,32,0,0,4,0],[5016,9,5016,56,0,0,4,0],[5016,9,5016,56,0,0,4,1],[5017,9,5017,58,0,0,4,0],[5017,9,5017,58,0,0,4,1],[5018,9,5018,57,0,0,4,0],[5018,9,5018,57,0,0,4,1],[5019,9,5019,70,0,0,4,0],[5020,9,5020,59,0,0,4,1]],"ByteCountLooksBad":[[4156,9,4156,34,0,0,4,1]],"CalcFinalIFDdatasizeReading":[[4253,9,4253,34,0,0,4,1],[4263,9,4263,41,0,0,4,0],[4274,17,4274,57,0,0,4,0],[4274,17,4274,57,0,0,4,1],[4277,13,4277,35,0,0,4,0],[4277,13,4277,35,0,0,4,1],[4282,18,4282,44,0,0,4,1],[4296,9,4296,33,0,0,4,0],[4296,9,4296,33,0,0,4,1],[4298,13,4298,52,0,0,4,1],[4306,13,4306,35,0,0,4,0],[4306,13,4306,35,0,0,4,1],[4314,9,4314,41,0,0,4,0]],"TIFFClientOpenExt":[[421,26,421,37,0,0,4,0]],"TryChopUpUncompressedBigTiff":[[8095,48,8095,73,0,0,4,0],[8152,9,8152,34,0,0,4,1]],"LogLuv32toXYZ":[[1185,9,1185,16,0,0,4,1]],"LogL16toY":[[806,9,806,12,0,0,4,1],[809,13,809,28,0,0,4,0],[809,13,809,28,0,0,4,1]],"_TIFFVSetField":[[447,21,447,46,0,0,4,0],[460,21,460,46,0,0,4,0]],"TIFFGetBitRevTable":[[307,13,307,21,0,0,4,1]],"allocChoppedUpStripArrays":[[7933,9,7933,38,0,0,4,0],[7936,13,7936,33,0,0,4,0]]},"library_calls":["TIFFOpen","TIFFFileno","TIFFCreateDirectory","TIFFGetBitRevTable","TIFFWriteRawStrip","TIFFGetStrileOffsetWithErr","TIFFTileSize","TIFFReadEncodedTile","LogLuv32toXYZ","TIFFCleanup"],"critical_calls":["TIFFOpen","TIFFFileno","TIFFCreateDirectory","TIFFGetBitRevTable","TIFFWriteRawStrip","TIFFGetStrileOffsetWithErr","TIFFTileSize","TIFFReadEncodedTile","LogLuv32toXYZ","TIFFCleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
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

    // Open the input file with libtiff
    TIFF *tif = TIFFOpen("input_file", fuzz_str_1);
    if (!tif) {
        // Clean up and exit if file cannot be opened
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Get file descriptor using TIFFFileno (assuming it's available from context)
    int fd = TIFFFileno(tif);

    // Create a new directory
    TIFFCreateDirectory(tif);

    // Use bit reversal table
    const unsigned char *bit_rev_table = TIFFGetBitRevTable(fuzz_int32_t_2);

    // Prepare data for raw strip writing
    uint32_t strip_index = 0;
    tmsize_t write_size = size > 1024 ? 1024 : size;
    void *write_data = malloc(write_size);
    if (write_data) {
        memcpy(write_data, data, write_size);
        // Write raw strip to the current directory
        TIFFWriteRawStrip(tif, fuzz_uint32_t_3, write_data, write_size);
        free(write_data);
    }

    // Get strile offset with error checking
    int err = 0;
    uint64_t strile_offset = TIFFGetStrileOffsetWithErr(tif, fuzz_uint32_t_4, &err);

    // Read encoded tile
    uint32_t tile_index = 0;
    tmsize_t tile_size = TIFFTileSize(tif);
    if (tile_size > 0) {
        void *tile_buf = malloc(tile_size);
        if (tile_buf) {
            TIFFReadEncodedTile(tif, fuzz_uint32_t_5, tile_buf, tile_size);
            free(tile_buf);
        }
    }

    // Convert LogLuv32 to XYZ
    if (size >= 4) {
        uint32_t logluv32;
        memcpy(&logluv32, data, 4);
        float xyz[3];
        LogLuv32toXYZ(logluv32, xyz);
    }

    // Clean up
    TIFFCleanup(tif);
    remove("input_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}