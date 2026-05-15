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
//<ID> 393
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFClientOpenExt","TIFFWarningExtR","TIFFGetSizeProc","TIFFSetDirectory","TIFFGetField","TIFFGetField","TIFFGetField","TIFFScanlineSize","_TIFFmemset","TIFFReadScanline","TIFFVTileSize64","TIFFUnlinkDirectory","TIFFStreamOpen","TIFFClose","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFClientOpenExt","TIFFWarningExtR","TIFFGetSizeProc","TIFFSetDirectory","TIFFGetField","TIFFGetField","TIFFGetField","TIFFScanlineSize","_TIFFmemset","TIFFReadScanline","TIFFVTileSize64","TIFFUnlinkDirectory","TIFFStreamOpen","TIFFClose","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create temporary input file with provided data
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Reopen input file for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Create TIFFOpenOptions and set max single memory allocation
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts) {
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 1024 * 1024); // 1MB
        
        // Open input TIFF file using custom client open with file descriptor
        TIFF *tif_in = TIFFClientOpenExt("input_tiff", "r", 
                                         reinterpret_cast<thandle_t>(fd),
                                         NULL, NULL, NULL, NULL,
                                         NULL, NULL, NULL, opts);
        
        if (tif_in) {
            // Set warning handler
            TIFFWarningExtR(tif_in, "fuzzer", "Fuzzing warning test");
            
            // Get size procedure
            TIFFSizeProc size_proc = TIFFGetSizeProc(tif_in);
            
            // Try to read the first directory
            if (TIFFSetDirectory(tif_in, 0)) {
                // Get image dimensions
                uint32_t width = 0, height = 0;
                TIFFGetField(tif_in, TIFFTAG_IMAGEWIDTH, &width);
                TIFFGetField(tif_in, TIFFTAG_IMAGELENGTH, &height);
                
                // Calculate buffer for scanline
                uint16_t samplesperpixel = 1;
                TIFFGetField(tif_in, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
                tmsize_t scanline_size = TIFFScanlineSize(tif_in);
                
                if (scanline_size > 0 && height > 0) {
                    // Allocate buffer for scanline
                    std::vector<uint8_t> buffer(scanline_size);
                    
                    // Clear buffer using _TIFFmemset
                    _TIFFmemset(buffer.data(), 0, scanline_size);
                    
                    // Try to read first scanline
                    for (uint32_t row = 0; row < height; row++) {
                        if (TIFFReadScanline(tif_in, buffer.data(), row, 0) < 0) {
                            break;
                        }
                    }
                    
                    // Calculate tile size for 1 row
                    uint64_t tile_size = TIFFVTileSize64(tif_in, 1);
                    
                    // Try to unlink directory (if not the only one)
                    TIFFUnlinkDirectory(tif_in, 0);
                }
            }
            
            // Create output stream for TIFF
            std::ofstream out_stream("output_file", std::ios::binary);
            if (out_stream) {
                // Open output TIFF stream
                TIFF *tif_out = TIFFStreamOpen("output_tiff", &out_stream);
                if (tif_out) {
                    // Clean up output TIFF
                    TIFFClose(tif_out);
                }
                out_stream.close();
            }
            
            // Clean up input TIFF
            TIFFClose(tif_in);
        }
        
        // Free options
        TIFFOpenOptionsFree(opts);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}