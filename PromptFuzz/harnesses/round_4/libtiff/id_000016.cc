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
//<score> 483, nr_unique_branch: 103
//<Quality> {"density":23,"unique_branches":{"TIFFWriteEncodedTile":[[426,9,426,31,0,0,4,1],[447,9,447,55,0,0,4,1],[460,9,460,23,0,0,4,1],[467,9,467,23,0,0,4,1],[474,9,474,48,0,0,4,0],[476,13,476,42,0,0,4,1],[487,9,487,15,0,0,4,1],[487,19,487,41,0,0,4,1],[491,9,491,47,0,0,4,0],[496,13,496,25,0,0,4,1],[500,13,500,19,0,0,4,0],[500,23,500,73,0,0,4,1],[40,6,40,43,1,0,4,1],[40,47,40,79,1,0,4,0],[42,7,42,44,2,0,4,0],[42,48,42,64,2,0,4,0],[314,6,314,54,4,0,4,0],[314,6,314,54,5,0,4,0]],"EvaluateIFDdatasizeWrite":[[2142,9,2142,69,0,0,4,0],[2146,13,2146,27,0,0,4,1]],"TIFFWriteDirectoryTagData":[[3116,9,3116,70,0,0,4,1],[3128,13,3128,45,0,0,4,0],[3130,13,3130,22,0,0,4,1],[3130,26,3130,43,0,0,4,1],[3135,13,3135,20,0,0,4,1],[3140,13,3140,39,0,0,4,1],[3147,13,3147,21,0,0,4,1],[3153,13,3153,33,0,0,4,1],[3155,13,3155,45,0,0,4,0],[3159,17,3159,43,0,0,4,1]],"TIFFFlushData":[[148,9,148,49,0,0,4,1],[150,9,150,41,0,0,4,1]],"TIFFCleanup":[[67,29,67,61,0,0,4,1]],"TIFFWriteDirectorySec":[[723,21,723,29,0,0,4,1],[733,25,736,62,0,0,4,1],[750,21,750,29,0,0,4,1],[772,25,775,59,0,0,4,1],[1441,9,1441,77,0,0,4,0]],"TIFFWriteDirectoryTagLongLong8Array":[[2002,9,2002,18,0,0,4,0],[2002,22,2002,52,0,0,4,1],[2006,14,2006,23,0,0,4,0],[2006,27,2006,56,0,0,4,0],[2006,27,2006,56,0,0,4,1],[2010,9,2010,22,0,0,4,1],[2052,13,2052,22,0,0,4,1],[2058,41,2058,51,0,0,4,0],[2058,41,2058,51,0,0,4,1],[2060,17,2060,29,0,0,4,1]],"TIFFAppendToStrip":[[775,9,775,37,0,0,4,1],[780,9,780,40,0,0,4,1],[785,9,785,25,0,0,4,1],[791,9,791,29,0,0,4,0],[794,9,794,41,0,0,4,0],[798,13,798,48,0,0,4,1],[839,9,839,41,0,0,4,0],[841,9,841,30,0,0,4,1],[841,34,841,52,0,0,4,1],[847,9,847,35,0,0,4,1],[930,9,930,17,0,0,4,1],[939,9,939,66,0,0,4,0]],"_WriteAsType":[[1919,9,1919,40,0,0,4,0]],"_TIFFRand":[[781,9,781,18,0,0,4,0],[781,9,781,18,0,0,4,1]],"LogL16fromY":[[818,9,818,26,0,0,4,0],[818,9,818,26,0,0,4,1],[820,9,820,27,0,0,4,0],[820,9,820,27,0,0,4,1],[822,9,822,26,0,0,4,0],[822,9,822,26,0,0,4,1],[824,9,824,27,0,0,4,0],[824,9,824,27,0,0,4,1]],"TIFFFlushData1":[[952,9,952,27,0,0,4,1]],"tiff_itrunc":[[792,9,792,35,0,0,4,1]],"TIFFWriteBufferSetup":[[683,9,683,25,0,0,4,1],[692,9,692,31,0,0,4,1],[706,9,706,19,0,0,4,1]],"TIFFFlush":[[43,9,43,43,0,0,4,0],[44,9,44,45,0,0,4,1]],"LogLuv32fromXYZ":[[1214,9,1214,12,0,0,4,0],[1214,9,1214,12,0,0,4,1],[1214,16,1214,23,0,0,4,0],[1214,16,1214,23,0,0,4,1],[1224,9,1224,16,0,0,4,0],[1224,9,1224,16,0,0,4,1],[1228,9,1228,17,0,0,4,0],[1228,9,1228,17,0,0,4,1],[1230,9,1230,16,0,0,4,0],[1230,9,1230,16,0,0,4,1],[1234,9,1234,17,0,0,4,0],[1234,9,1234,17,0,0,4,1]],"_TIFFReserveLargeEnoughWriteBuffer":[[194,9,194,51,0,0,4,1]],"TIFFWriteCheck":[[634,9,634,22,0,0,4,1],[640,9,640,46,0,0,4,0],[640,50,640,71,0,0,4,1],[650,13,650,35,0,0,4,1],[656,9,656,35,0,0,4,1],[660,9,660,56,0,0,4,1],[282,22,282,62,4,0,4,0]],"LogL10fromY":[[896,9,896,20,0,0,4,0],[896,9,896,20,0,0,4,1],[898,14,898,28,0,0,4,0],[898,14,898,28,0,0,4,1]],"_TIFFCheckDirNumberAndOffset":[[5762,9,5762,20,0,0,4,0]]},"library_calls":["TIFFOpen","TIFFCreateDirectory","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFTileSize","TIFFClose","_TIFFmalloc","TIFFClose","TIFFWriteBufferSetup","_TIFFfree","TIFFClose","TIFFWriteEncodedTile","LogL10fromY","LogLuv32fromXYZ","TIFFGetStrileByteCount","TIFFGetStrileByteCountWithErr","TIFFClose","_TIFFfree"],"critical_calls":["TIFFOpen","TIFFCreateDirectory","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFTileSize","_TIFFmalloc","TIFFWriteBufferSetup","TIFFWriteEncodedTile","LogL10fromY","LogLuv32fromXYZ","TIFFGetStrileByteCount","TIFFGetStrileByteCountWithErr","TIFFClose","_TIFFfree"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create input and output files
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        remove("input_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    TIFF *tif = nullptr;
    int err = 0;
    uint64_t byte_count = 0;
    
    // Open TIFF file for writing
    tif = TIFFOpen("output_file", fuzz_str_1);
    if (!tif) {
        assert_file_closed(&out_file);;
        remove("input_file");
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // Set up basic TIFF directory structure
    TIFFCreateDirectory(tif);
    
    // Set required tags for tile-based TIFF
    uint32_t width = 256;
    uint32_t height = 256;
    uint16_t samplesperpixel = 3;
    uint16_t bitspersample = 8;
    uint32_t tilewidth = 64;
    uint32_t tileheight = 64;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    // Set tile parameters
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tilewidth);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tileheight);
    
    // Calculate tile size
    tmsize_t tile_size = TIFFTileSize(tif);
    if (tile_size == 0) {
        TIFFClose(tif);
        assert_file_closed(&out_file);;
        remove("input_file");
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // Allocate buffer for tile data
    void *tile_buffer = _TIFFmalloc(tile_size);
    if (!tile_buffer) {
        TIFFClose(tif);
        assert_file_closed(&out_file);;
        remove("input_file");
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // Set up write buffer
    if (TIFFWriteBufferSetup(tif, tile_buffer, tile_size) == 0) {
        _TIFFfree(tile_buffer);
        TIFFClose(tif);
        assert_file_closed(&out_file);;
        remove("input_file");
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // Prepare tile data from input
    size_t data_to_use = size > tile_size ? tile_size : size;
    if (data_to_use > 0) {
        memcpy(tile_buffer, data, data_to_use);
    }
    
    // Write encoded tile
    tmsize_t written = TIFFWriteEncodedTile(tif, fuzz_uint32_t_2, tile_buffer, tile_size);
    
    // Use LogL10fromY with sample data
    if (size >= sizeof(double)) {
        double Y = *reinterpret_cast<const double*>(data);
        int ciel = 1; // Standard ciel parameter
        int logl10_result = LogL10fromY(Y, fuzz_int32_t_3);
        (void)logl10_result; // Use result to avoid unused variable warning
    }
    
    // Use LogLuv32fromXYZ with sample data
    if (size >= 3 * sizeof(float)) {
        float XYZ[3];
        memcpy(XYZ, data, 3 * sizeof(float));
        int em = 1; // Standard em parameter
        uint32_t logluv_result = LogLuv32fromXYZ(XYZ, em);
        (void)logluv_result; // Use result to avoid unused variable warning
    }
    
    // Get strile byte count
    byte_count = TIFFGetStrileByteCount(tif, fuzz_uint32_t_4);
    
    // Get strile byte count with error
    TIFFGetStrileByteCountWithErr(tif, fuzz_uint32_t_5, &err);
    
    // Clean up
    TIFFClose(tif);
    assert_file_closed(&out_file);;
    _TIFFfree(tile_buffer);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	return 0;
}