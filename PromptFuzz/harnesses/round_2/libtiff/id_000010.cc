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
//<ID> 536
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 8
//<Quality> {"density":17,"unique_branches":{"TIFFWriteCheck":[[614,9,614,24,0,0,4,0],[617,23,617,28,0,0,4,0]],"TIFFGetClientInfo":[[69,12,69,26,0,0,4,0],[69,30,69,61,0,0,4,1],[72,9,72,23,0,0,4,0]],"TIFFClientOpenExt":[[515,13,515,21,0,0,4,0],[516,21,516,34,0,0,4,1],[542,13,542,20,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFSetClientInfo","TIFFWriteCheck","TIFFGetField","TIFFSetDirectory","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFNumberOfStrips","TIFFGetStrileOffsetWithErr","TIFFRawStripSize","TIFFReadRGBAStripExt","TIFFGetField","LogLuv24toXYZ","TIFFGetClientInfo","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFSetClientInfo","TIFFWriteCheck","TIFFGetField","TIFFSetDirectory","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFNumberOfStrips","TIFFGetStrileOffsetWithErr","TIFFRawStripSize","TIFFReadRGBAStripExt","TIFFGetField","LogLuv24toXYZ","TIFFGetClientInfo","TIFFClose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Too small for meaningful TIFF processing

    // Write input data to a file for TIFF to read
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Create an output stream for TIFFStreamOpen
    std::ofstream out_stream("output_file", std::ios::binary);
    if (!out_stream.is_open()) {
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Open TIFF using the stream interface
    TIFF *tif = TIFFStreamOpen("input_file", &out_stream);
    if (!tif) {
        out_stream.close();
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	return 0;
    }

    // Set client info for later retrieval
    int client_data = 42;
    TIFFSetClientInfo(tif, &client_data, "test_client");

    // Check if we can write to this TIFF
    int write_check = TIFFWriteCheck(tif, 1, "test");
    
    // Get number of directories
    uint16_t dir_count = 0;
    if (TIFFGetField(tif, TIFFTAG_PAGENUMBER, &dir_count, NULL)) {
        // Process first few directories
        for (uint16_t dir = 0; dir < std::min(dir_count, static_cast<uint16_t>(3)); dir++) {
            TIFFSetDirectory(tif, dir);
            
            // Get basic image parameters
            uint32_t width = 0, height = 0;
            uint16_t samples_per_pixel = 0;
            uint16_t bits_per_sample = 0;
            
            if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
                TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) &&
                TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel) &&
                TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample)) {
                
                // Process strips if image is organized in strips
                uint32_t rows_per_strip = 0;
                if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip)) {
                    uint32_t strip_count = TIFFNumberOfStrips(tif);
                    
                    for (uint32_t strip = 0; strip < std::min(strip_count, 5U); strip++) {
                        // Get strip offset with error checking
                        int offset_err = 0;
                        uint64_t offset = TIFFGetStrileOffsetWithErr(tif, strip, &offset_err);
                        
                        if (offset_err == 0 && offset > 0) {
                            // Get raw strip size
                            tmsize_t raw_size = TIFFRawStripSize(tif, strip);
                            
                            if (raw_size > 0) {
                                // Read RGBA strip data
                                std::vector<uint32_t> rgba_buffer(width * rows_per_strip);
                                int read_result = TIFFReadRGBAStripExt(tif, strip, 
                                                                       rgba_buffer.data(), 0);
                                
                                // Process some pixels through LogLuv24toXYZ if possible
                                if (read_result == 1 && rgba_buffer.size() > 0) {
                                    // Check if this might be LogLuv data
                                    uint16_t photometric = 0;
                                    if (TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric)) {
                                        if (photometric == PHOTOMETRIC_LOGLUV) {
                                            // Convert first pixel (if valid)
                                            float xyz[3];
                                            LogLuv24toXYZ(rgba_buffer[0], xyz);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Retrieve client info to verify it was set correctly
    void *retrieved_client_info = TIFFGetClientInfo(tif, "test_client");
    
    // Cleanup
    TIFFClose(tif);
    out_stream.close();
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	return 0;
}