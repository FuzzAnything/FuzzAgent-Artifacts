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
//<ID> 692
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 34
//<Quality> {"density":1,"unique_branches":{"TIFFFlushData1":[[957,13,959,50,0,0,4,0]],"TIFFFetchNormalTag":[[6953,21,6953,49,0,0,4,1]],"EstimateStripByteCounts":[[5666,13,5667,56,0,0,4,0]],"OkToChangeTag":[[1104,9,1104,13,0,0,4,0],[1110,39,1110,74,0,0,4,0],[1111,9,1111,31,0,0,4,0],[280,24,280,36,2,0,4,1]],"OJPEGReadBufferFill":[[2235,17,2235,23,0,0,4,0]],"TIFFWriteEncodedTile":[[40,6,40,43,1,0,4,1],[40,47,40,79,1,0,4,1]],"TIFFVSetField":[[1195,12,1195,35,0,0,4,1]],"TIFFReadRawStrip":[[742,9,742,31,0,0,4,1],[744,9,744,32,0,0,4,1],[751,9,751,40,0,0,4,0],[751,9,751,40,0,0,4,1],[759,9,759,31,0,0,4,0],[759,35,759,64,0,0,4,0],[759,35,759,64,0,0,4,1],[763,9,763,24,0,0,4,0],[763,9,763,24,0,0,4,1]],"TIFFSwabArrayOfLong":[[123,12,123,19,0,0,4,0],[123,12,123,19,0,0,4,1]],"TIFFFlush":[[35,9,35,28,0,0,4,0]],"TIFFReadEncodedTile":[[989,9,989,25,0,0,4,1],[1005,14,1005,29,0,0,4,0]],"TIFFReadDirEntryArrayWithLimit":[[1306,13,1306,32,0,0,4,1]],"OJPEGReadHeaderInfoSec":[[1546,13,1546,33,0,0,4,0]],"_TIFFVSetField":[[479,17,479,35,0,0,4,0]],"TIFFWriteTile":[[392,9,392,40,0,0,4,1]],"TIFFComputeTile":[[52,9,52,16,0,0,4,1]],"TIFFClientOpenExt":[[516,21,516,34,0,0,4,0]],"TIFFAppendToStrip":[[775,9,775,37,0,0,4,0]],"TIFFReadDirEntryFloatArray":[[3019,21,3019,34,0,0,4,0],[3021,26,3021,40,0,0,4,0]]},"library_calls":["TIFFStreamOpen","_TIFFmalloc","TIFFSwabArrayOfLong","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteTile","TIFFSetField","TIFFWriteScanline","_TIFFfree","TIFFIsCODECConfigured","TIFFCreateEXIFDirectory","TIFFSetField","TIFFSetField","TIFFFlush","TIFFCreateDirectory","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteScanline","_TIFFmalloc","TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFGetField","TIFFReadEncodedTile","TIFFReadRawStrip","TIFFClose","_TIFFfree","TIFFClose"],"critical_calls":["_TIFFfree","TIFFClose","TIFFReadRawStrip","TIFFReadEncodedTile","TIFFGetField","TIFFGetField","TIFFGetField","TIFFStreamOpen","_TIFFmalloc","TIFFWriteScanline","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFCreateDirectory","TIFFFlush","TIFFSetField","TIFFSetField","TIFFCreateEXIFDirectory","TIFFIsCODECConfigured","_TIFFfree","TIFFWriteScanline","TIFFSetField","TIFFWriteTile","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSwabArrayOfLong","_TIFFmalloc","TIFFStreamOpen","TIFFClose"],"visited":0}
/*Here's a fuzz driver that uses the specified libtiff APIs to achieve the event of reading TIFF data from a user buffer, manipulating it with byte swapping, checking codec configuration, creating EXIF directory, and writing custom directory:

*/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create input file from the fuzzer data
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
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output stream for TIFF
    std::ofstream out_stream("output_file", std::ios::binary);
    if (!out_stream.is_open()) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    
    TIFF *tif = nullptr;
    int ret = 0;
    
    try {
        // 1. Create a TIFF stream for reading
        // We'll create an input stream from the file
        std::ifstream in_stream("input_file", std::ios::binary);
        if (!in_stream.is_open()) {
            throw std::runtime_error("Failed to open input stream");
        }
        
        // Create output TIFF using stream API
        tif = TIFFStreamOpen("output_file", &out_stream);
        if (!tif) {
            throw std::runtime_error("Failed to create TIFF");
        }
        
        // 2. Use TIFFSwabArrayOfLong on input data
        // Convert input data to array of uint32_t for byte swapping
        if (size >= sizeof(uint32_t)) {
            size_t num_longs = size / sizeof(uint32_t);
            uint32_t *long_array = static_cast<uint32_t*>(_TIFFmalloc(num_longs * sizeof(uint32_t)));
            if (long_array) {
                // Copy and swap bytes
                memcpy(long_array, data, num_longs * sizeof(uint32_t));
                TIFFSwabArrayOfLong(long_array, num_longs);
                
                // Write the swapped data as a tile (for demonstration)
                if (TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, static_cast<uint32_t>(1)) &&
                    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, static_cast<uint32_t>(1)) &&
                    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32) &&
                    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1) &&
                    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK) &&
                    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG) &&
                    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE)) {
                    
                    // Write as tile if tiled
                    TIFFSetField(tif, TIFFTAG_TILEWIDTH, 1);
                    TIFFSetField(tif, TIFFTAG_TILELENGTH, 1);
                    
                    if (TIFFWriteTile(tif, long_array, 0, 0, 0, 0) < 0) {
                        // Try scanline instead
                        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
                        TIFFWriteScanline(tif, long_array, 0, 0);
                    }
                }
                
                _TIFFfree(long_array);
            }
        }
        
        // 3. Check if LZW codec is configured (using common codec values)
        int codec_configured = 0;
        uint16_t codecs_to_test[] = {
            COMPRESSION_NONE,      // 1
            COMPRESSION_CCITTRLE,  // 2
            COMPRESSION_CCITTFAX3, // 3
            COMPRESSION_CCITTFAX4, // 4
            COMPRESSION_LZW,       // 5
            COMPRESSION_JPEG,      // 7
            COMPRESSION_DEFLATE,   // 8
        };
        
        for (auto codec : codecs_to_test) {
            if (TIFFIsCODECConfigured(codec)) {
                codec_configured = 1;
                break;
            }
        }
        
        // 4. Create EXIF directory
        ret = TIFFCreateEXIFDirectory(tif);
        if (ret == 0) {
            // Try to set some EXIF tags if directory creation was successful
            TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, "Fuzzed Image");
            TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, "Created by fuzzer");
        }
        
        // 5. Write custom directory
        // Note: TIFFWriteCustomDirectory is not in the provided API list
        // Using available APIs to simulate directory operations
        TIFFFlush(tif);
        
        // Create a new directory
        if (TIFFCreateDirectory(tif) == 1) {
            // Set some fields in the new directory
            TIFFSetField(tif, TIFFTAG_SUBFILETYPE, 0);
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 64);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 64);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, codecs_to_test[0]);
            
            // Write a simple pattern
            std::vector<uint8_t> buffer(64 * 64, 128);
            for (uint32_t row = 0; row < 64; ++row) {
                TIFFWriteScanline(tif, buffer.data(), row, 0);
            }
        }
        
        // 6. Read data using available APIs (simulating TIFFReadFromUserBuffer)
        // Since TIFFReadFromUserBuffer isn't available, use TIFFReadEncodedTile/Strip
        if (size > 100) {
            // Allocate buffers for reading
            tmsize_t read_size = static_cast<tmsize_t>(std::min(size, static_cast<size_t>(1024)));
            void *read_buf = _TIFFmalloc(read_size);
            if (read_buf) {
                // Try to read from the input file using standard TIFF APIs
                // We need to create a TIFF from input file for reading
                assert_file_closed(&in_file);;
                in_file = fopen("input_file", "rb");
                if (in_file) {
                    // Create input TIFF
                    std::ifstream in_stream2("input_file", std::ios::binary);
                    TIFF *tif_in = TIFFStreamOpen("input_file", &in_stream2);
                    if (tif_in) {
                        // Try to read basic information
                        uint32_t width, height;
                        if (TIFFGetField(tif_in, TIFFTAG_IMAGEWIDTH, &width)) {
                            // Image has valid width
                        }
                        
                        // Try reading a tile if possible
                        uint32_t tile_width, tile_height;
                        TIFFGetField(tif_in, TIFFTAG_TILEWIDTH, &tile_width);
                        TIFFGetField(tif_in, TIFFTAG_TILELENGTH, &tile_height);
                        
                        if (tile_width > 0 && tile_height > 0) {
                            // Try to read first tile
                            TIFFReadEncodedTile(tif_in, 0, read_buf, read_size);
                        } else {
                            // Try to read first strip
                            TIFFReadRawStrip(tif_in, 0, read_buf, read_size);
                        }
                        
                        TIFFClose(tif_in);
                    }
                }
                _TIFFfree(read_buf);
            }
        }
        
    } catch (const std::exception& e) {
        // Handle exceptions silently during fuzzing
    }
    
    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }
    
    // Close file handles
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	return 0;
}