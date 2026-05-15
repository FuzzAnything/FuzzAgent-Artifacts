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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 4
//<Quality> {"density":17,"unique_branches":{"TIFFError":[[49,9,49,26,0,0,4,0],[55,9,55,29,0,0,4,1]],"TIFFClientOpenExt":[[356,17,356,50,0,0,4,0],[357,9,357,54,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFCurrentDirOffset","TIFFGetField","_TIFFmalloc","TIFFYCbCrToRGBInit","TIFFFieldWithTag","TIFFFieldSetGetCountSize","TIFFDefaultTileSize","TIFFGetField","TIFFStripSize","_TIFFmalloc","TIFFReadEncodedStrip","TIFFSwabLong","TIFFSwabArrayOfTriples","_TIFFfree","TIFFError","TIFFCleanup","TIFFClose","TIFFError","_TIFFfree","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFCurrentDirOffset","TIFFGetField","_TIFFmalloc","TIFFYCbCrToRGBInit","TIFFFieldWithTag","TIFFFieldSetGetCountSize","TIFFDefaultTileSize","TIFFGetField","TIFFStripSize","_TIFFmalloc","TIFFReadEncodedStrip","TIFFSwabLong","TIFFSwabArrayOfTriples","_TIFFfree","TIFFCleanup","TIFFClose","_TIFFfree","TIFFOpenOptionsFree"],"visited":0}
/**/


// Include necessary libtiff headers
extern "C" {
    #include "tiffio.h"
    #include "tiff.h"
    #include "tiffio.hxx"
}

// Define the missing function prototype that was in the API list
extern "C" void TIFFSwabArrayOfTriples(uint8_t* tp, tmsize_t n);

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file
    FILE* in_file = fopen("input_file", "wb");
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
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    TIFF* tif = nullptr;
    TIFFOpenOptions* opts = nullptr;
    TIFFYCbCrToRGB* ycbcr = nullptr;
    
    // 1. Create and configure TIFFOpenOptions
    opts = TIFFOpenOptionsAlloc();
    if (opts) {
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 100 * 1024 * 1024); // 100MB limit
    }
    
    // 2. Open the TIFF file
    tif = TIFFClientOpenExt("input_file", "r", (thandle_t)(intptr_t)fd,
                            TIFFReadWriteProc(nullptr), TIFFReadWriteProc(nullptr),
                            TIFFSeekProc(nullptr), TIFFCloseProc(nullptr),
                            TIFFSizeProc(nullptr), TIFFMapFileProc(nullptr),
                            TIFFUnmapFileProc(nullptr), opts);
    
    if (tif) {
        // 3. Check TIFF properties and manipulate data
        
        // Get current directory offset
        uint64_t dir_offset = TIFFCurrentDirOffset(tif);
        
        // Try to read a field to check if it's a valid TIFF
        uint32_t width = 0;
        if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width)) {
            // 4. Initialize YCbCr to RGB conversion
            ycbcr = (TIFFYCbCrToRGB*)_TIFFmalloc(sizeof(TIFFYCbCrToRGB));
            if (ycbcr) {
                float luma[3] = {0.299f, 0.587f, 0.114f};
                float refwhite[3] = {255.0f, 255.0f, 255.0f};
                int init_result = TIFFYCbCrToRGBInit(ycbcr, luma, refwhite);
                
                // 5. Get field information
                const TIFFField* field = TIFFFieldWithTag(tif, TIFFTAG_IMAGEWIDTH);
                if (field) {
                    int count_size = TIFFFieldSetGetCountSize(field);
                }
                
                // 6. Set default tile size
                uint32_t tile_width = 0, tile_height = 0;
                TIFFDefaultTileSize(tif, &tile_width, &tile_height);
                
                // 7. Read and process some image data if possible
                uint32_t rowsperstrip = 0;
                if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip) && rowsperstrip > 0) {
                    tmsize_t stripsize = TIFFStripSize(tif);
                    if (stripsize > 0) {
                        uint8_t* strip_buffer = (uint8_t*)_TIFFmalloc(stripsize);
                        if (strip_buffer) {
                            // Try to read first strip
                            tmsize_t read_bytes = TIFFReadEncodedStrip(tif, 0, strip_buffer, stripsize);
                            
                            if (read_bytes > 0) {
                                // 8. Perform byte swapping operations
                                
                                // Swap a single long value
                                uint32_t sample_value = 0x12345678;
                                TIFFSwabLong(&sample_value);
                                
                                // Swap array of triples if we have enough data
                                if (read_bytes >= 3) {
                                    TIFFSwabArrayOfTriples(strip_buffer, 
                                        static_cast<tmsize_t>(read_bytes / 3));
                                }
                            }
                            
                            _TIFFfree(strip_buffer);
                        }
                    }
                }
            }
        } else {
            // 9. Use TIFFError for error reporting
            TIFFError("input_file", "Failed to get image width");
        }
        
        // 10. Clean up
        TIFFCleanup(tif);
        TIFFClose(tif);
    } else {
        TIFFError("input_file", "Failed to open TIFF file");
    }
    
    // Clean up allocated resources
    if (ycbcr) {
        _TIFFfree(ycbcr);
    }
    
    if (opts) {
        TIFFOpenOptionsFree(opts);
    }
    
    assert_file_closed(&in_file);;
    
    // Clean up temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}