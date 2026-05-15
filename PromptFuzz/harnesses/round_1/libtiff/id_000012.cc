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
//<ID> 663
//<Prompt> []
/*<Combination>: [
*/
//<score> 144, nr_unique_branch: 99
//<Quality> {"density":32,"unique_branches":{"TIFFVSetField":[[1195,12,1195,35,0,0,4,1]],"TIFFWriteDirectorySec":[[1200,13,1200,24,0,0,4,0],[236,34,236,71,7,0,4,1],[236,34,236,71,25,0,4,1]],"oog_encode":[[914,9,914,21,0,0,4,0],[918,27,918,30,0,0,4,0],[918,27,918,30,0,0,4,1],[920,27,920,31,0,0,4,0],[920,27,920,31,0,0,4,1],[924,17,924,33,0,0,4,0],[924,17,924,33,0,0,4,1],[924,37,924,44,0,0,4,0],[924,37,924,44,0,0,4,1],[924,48,924,58,0,0,4,1],[926,43,926,50,0,0,4,0],[926,43,926,50,0,0,4,1],[932,21,932,34,0,0,4,0],[932,21,932,34,0,0,4,1],[939,27,939,30,0,0,4,0],[939,27,939,30,0,0,4,1],[940,17,940,29,0,0,4,0],[940,17,940,29,0,0,4,1],[943,30,943,46,0,0,4,0],[944,25,944,54,0,0,4,0],[944,25,944,54,0,0,4,1],[946,30,946,46,0,0,4,0],[947,25,947,64,0,0,4,0],[947,25,947,64,0,0,4,1],[949,21,949,28,0,0,4,0],[949,21,949,28,0,0,4,1]],"LogLuv24toXYZ":[[1038,9,1038,16,0,0,4,0],[1038,9,1038,16,0,0,4,1],[1045,9,1045,34,0,0,4,0],[1045,9,1045,34,0,0,4,1]],"_TIFFGetStrileOffsetOrByteCountValue":[[8489,13,8489,18,0,0,4,1]],"uv_decode":[[1003,9,1003,14,0,0,4,1],[1003,18,1003,31,0,0,4,0],[1003,18,1003,31,0,0,4,1],[1007,12,1007,29,0,0,4,0],[1007,12,1007,29,0,0,4,1],[1011,13,1011,19,0,0,4,0],[1011,13,1011,19,0,0,4,1],[1013,18,1013,24,0,0,4,0],[1013,18,1013,24,0,0,4,1]],"TIFFRawStripSize64":[[181,9,181,23,0,0,4,0]],"TIFFWriteBufferSetup":[[683,9,683,25,0,0,4,0],[685,13,685,43,0,0,4,0],[282,22,282,62,3,0,4,0]],"JPEGSetupEncode":[[1936,9,1936,44,0,0,4,1],[1957,9,1957,51,0,0,4,0],[1960,13,1960,49,0,0,4,1],[1973,18,1973,62,0,0,4,0],[1973,18,1973,62,0,0,4,1],[1974,18,1974,62,0,0,4,1],[1975,17,1975,44,0,0,4,0],[1977,22,1977,59,0,0,4,1],[1980,22,1980,65,0,0,4,1],[1992,9,1992,35,0,0,4,1],[1998,9,1998,35,0,0,4,1],[2016,9,2016,31,0,0,4,1],[2054,9,2054,33,0,0,4,1],[2055,9,2055,30,0,0,4,1],[2061,9,2061,16,0,0,4,0],[2079,9,2079,48,0,0,4,0]],"TIFFWriteEncodedTile":[[426,9,426,31,0,0,4,1],[447,9,447,55,0,0,4,1],[460,9,460,23,0,0,4,1],[467,9,467,23,0,0,4,1],[474,9,474,48,0,0,4,0],[476,13,476,42,0,0,4,0],[476,13,476,42,0,0,4,1],[487,9,487,15,0,0,4,0],[491,9,491,47,0,0,4,0],[496,13,496,25,0,0,4,1],[500,13,500,19,0,0,4,0],[500,23,500,73,0,0,4,1],[40,6,40,43,1,0,4,1],[40,47,40,79,1,0,4,0],[42,7,42,44,2,0,4,1],[43,6,43,53,2,0,4,0],[314,6,314,54,4,0,4,0],[314,6,314,54,5,0,4,0]],"TIFFWriteTile":[[392,9,392,40,0,0,4,1]],"uv_encode":[[979,9,979,22,0,0,4,0],[982,9,982,21,0,0,4,0],[984,9,984,30,0,0,4,0],[987,9,987,29,0,0,4,0]],"TIFFRawStripSize":[[197,9,197,28,0,0,4,0]],"_TIFFReserveLargeEnoughWriteBuffer":[[194,9,194,51,0,0,4,0],[200,13,200,63,0,0,4,0],[200,13,200,63,0,0,4,1],[202,17,204,71,0,0,4,1]],"JPEGInitializeLibJPEG":[[2783,9,2783,19,0,0,4,1],[2790,13,2790,42,0,0,4,1],[2799,13,2799,51,0,0,4,1]],"OkToChangeTag":[[1110,39,1110,74,0,0,4,0],[1111,9,1111,31,0,0,4,0]],"LogL10toY":[[885,9,885,17,0,0,4,0],[885,9,885,17,0,0,4,1]]},"library_calls":["TIFFOpen","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFTileSize","_TIFFmalloc","TIFFWriteTile","_TIFFfree","TIFFCreateDirectory","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteCustomDirectory","TIFFFieldWithTag","TIFFFieldReadCount","TIFFFileName","uv_encode","LogLuv24toXYZ","TIFFNumberOfStrips","TIFFRawStripSize","_TIFFmalloc","TIFFWriteRawStrip","_TIFFfree","TIFFIsTiled","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBATile","_TIFFfree","TIFFClose"],"critical_calls":["TIFFOpen","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFTileSize","_TIFFmalloc","TIFFWriteTile","_TIFFfree","TIFFCreateDirectory","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteCustomDirectory","TIFFFieldWithTag","TIFFFieldReadCount","TIFFFileName","uv_encode","LogLuv24toXYZ","TIFFNumberOfStrips","TIFFRawStripSize","_TIFFmalloc","TIFFWriteRawStrip","_TIFFfree","TIFFIsTiled","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBATile","_TIFFfree","TIFFClose"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<79) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_18, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_19, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_20, fdp);
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

    // Open TIFF file for reading and writing
    TIFF *tif = TIFFOpen("input_file", fuzz_str_1);
    if (!tif) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Get basic image information
    uint32_t width = 0, height = 0;
    uint16_t samples = 0, bits = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);

    // 1. TIFFWriteTile - Write a tile if image is tiled
    if (TIFFIsTiled(tif)) {
        uint32_t tile_width = 0, tile_height = 0;
        TIFFGetField(tif, fuzz_uint32_t_2, &tile_width);
        TIFFGetField(tif, fuzz_uint32_t_3, &tile_height);
        
        // Allocate buffer for tile data
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size <= size) {
            void *tile_buf = _TIFFmalloc(tile_size);
            if (tile_buf) {
                // Use some data for tile buffer
                memcpy(tile_buf, data, tile_size > size ? size : tile_size);
                TIFFWriteTile(tif, tile_buf, fuzz_uint32_t_4, fuzz_uint32_t_5, fuzz_uint32_t_6, fuzz_uint16_t_7);
                _TIFFfree(tile_buf);
            }
        }
    }

    // 2. TIFFWriteCustomDirectory - Write a custom directory
    uint64_t dir_offset = 0;
    if (TIFFCreateDirectory(tif)) {
        // Set some fields in the new directory
        if (width > 0 && height > 0) {
            TIFFSetField(tif, fuzz_uint32_t_8, width);
            TIFFSetField(tif, fuzz_uint32_t_9, height);
            TIFFSetField(tif, fuzz_uint32_t_10, samples > 0 ? samples : 1);
            TIFFSetField(tif, fuzz_uint32_t_11, bits > 0 ? bits : 8);
        }
        TIFFWriteCustomDirectory(tif, &dir_offset);
    }

    // 3. TIFFFieldWithName - Look up a field by name (not in provided list, using TIFFFieldWithTag instead)
    const TIFFField *field = TIFFFieldWithTag(tif, fuzz_uint32_t_12);
    if (field) {
        // 7. TIFFFieldReadCount - Get read count of field
        int read_count = TIFFFieldReadCount(field);
        (void)read_count; // Prevent unused variable warning
    }

    // 4. TIFFFileName - Get file name
    const char *filename = TIFFFileName(tif);
    (void)filename; // Prevent unused variable warning

    // 5. uv_encode - Encode chromaticity values
    double u = 0.0, v = 0.0;
    int encoding = 0;
    if (size >= 2) {
        u = static_cast<double>(data[0]) / 255.0;
        v = static_cast<double>(data[1]) / 255.0;
    }
    uv_encode(u, v, fuzz_int32_t_13);

    // 6. LogLuv24toXYZ - Convert LogLuv24 to XYZ
    if (size >= 4) {
        uint32_t logluv_pixel = 0;
        memcpy(&logluv_pixel, data, 4);
        float xyz[3];
        LogLuv24toXYZ(fuzz_uint32_t_14, xyz);
    }

    // 8. TIFFWriteRawStrip - Write raw strip data
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    if (strip_count > 0) {
        uint32_t strip = 0;
        tmsize_t strip_size = TIFFRawStripSize(tif, fuzz_uint32_t_15);
        if (strip_size > 0 && strip_size <= size) {
            void *strip_buf = _TIFFmalloc(strip_size);
            if (strip_buf) {
                memcpy(strip_buf, data, strip_size > size ? size : strip_size);
                TIFFWriteRawStrip(tif, fuzz_uint32_t_16, strip_buf, strip_size);
                _TIFFfree(strip_buf);
            }
        }
    }

    // 9. TIFFReadRGBATile - Read RGBA tile data
    if (TIFFIsTiled(tif) && width > 0 && height > 0) {
        uint32_t tile_width = 0, tile_height = 0;
        TIFFGetField(tif, fuzz_uint32_t_17, &tile_width);
        TIFFGetField(tif, fuzz_uint32_t_18, &tile_height);
        
        if (tile_width > 0 && tile_height > 0) {
            uint32_t *raster = static_cast<uint32_t*>(_TIFFmalloc(tile_width * tile_height * sizeof(uint32_t)));
            if (raster) {
                // Read tile at position (0, 0)
                TIFFReadRGBATile(tif, fuzz_uint32_t_19, fuzz_uint32_t_20, raster);
                _TIFFfree(raster);
            }
        }
    }

    // Cleanup
    TIFFClose(tif);
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}