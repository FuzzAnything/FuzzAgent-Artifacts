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
//<ID> 920
//<Prompt> []
/*<Combination>: [
*/
//<score> 2, nr_unique_branch: 2
//<Quality> {"density":1,"unique_branches":{"TIFFErrorExt":[[66,9,66,26,0,0,4,0],[72,9,72,29,0,0,4,1]]},"library_calls":["TIFFClientOpen","TIFFErrorExt","TIFFWarning","TIFFReadCustomDirectory","TIFFLastDirectory","TIFFWarningExt","TIFFGetField","TIFFGetField","TIFFReadRGBATile","LogLuv24toXYZ","TIFFXYZToRGB","TIFFClientOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFDefaultStripSize","TIFFWriteRawStrip","TIFFErrorExt","TIFFErrorExt","TIFFCleanup","TIFFCleanup"],"critical_calls":["TIFFErrorExt","TIFFErrorExt","TIFFWriteRawStrip","TIFFSetField","TIFFDefaultStripSize","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFClientOpen","TIFFXYZToRGB","LogLuv24toXYZ","TIFFReadRGBATile","TIFFGetField","TIFFGetField","TIFFWarningExt","TIFFLastDirectory","TIFFReadCustomDirectory","TIFFWarning","TIFFErrorExt","TIFFClientOpen","TIFFCleanup","TIFFCleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs:

*/



extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
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
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    TIFF *tif = nullptr;
    TIFF *tif_out = nullptr;
    TIFFCIELabToRGB cielab = {};
    std::vector<uint32_t> rgba_buffer;
    std::vector<uint8_t> raw_strip_buffer;
    
    try {
        // Open input TIFF file
        in_file = fopen("input_file", "rb");
        if (!in_file) goto cleanup;
        
        // Create TIFF handle from file descriptor
        tif = TIFFClientOpen("input_tiff", "r", 
                            reinterpret_cast<thandle_t>(in_file),
                            nullptr, nullptr, nullptr, nullptr,
                            nullptr, nullptr, nullptr);
        
        if (!tif) {
            // Use error and warning functions
            TIFFErrorExt(reinterpret_cast<thandle_t>(in_file), 
                        "LLVMFuzzerTestOneInput_16", 
                        "Failed to open input TIFF");
            TIFFWarning("LLVMFuzzerTestOneInput_16", 
                       "Input data may not be valid TIFF");
            goto cleanup;
        }
        
        // Read custom directory (try multiple offsets)
        for (int i = 0; i < 3; i++) {
            if (TIFFReadCustomDirectory(tif, i * 1024, nullptr)) {
                break;
            }
        }
        
        // Check if last directory
        int is_last = TIFFLastDirectory(tif);
        if (is_last) {
            TIFFWarningExt(reinterpret_cast<thandle_t>(tif), 
                          "LLVMFuzzerTestOneInput_16",
                          "This is the last directory in TIFF");
        }
        
        // Try to read RGBA tile if possible
        uint32_t tile_width = 0, tile_height = 0;
        
        // Attempt to get tile dimensions from TIFF tags
        uint32_t image_width = 0, image_height = 0;
        if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &image_width) &&
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &image_height)) {
            
            // Try to read a tile at position (0,0)
            tile_width = (image_width > 256) ? 256 : image_width;
            tile_height = (image_height > 256) ? 256 : image_height;
            
            rgba_buffer.resize(tile_width * tile_height);
            if (TIFFReadRGBATile(tif, 0, 0, rgba_buffer.data())) {
                // Try color conversion if we have valid RGBA data
                float xyz[3];
                uint32_t logluv_pixel = rgba_buffer[0] & 0x00FFFFFF; // Use first pixel
                LogLuv24toXYZ(logluv_pixel, xyz);
                
                uint32_t r, g, b;
                // Initialize CIELab converter with default values
                cielab.range = 100;
                cielab.rstep = cielab.gstep = cielab.bstep = 1.0f;
                cielab.X0 = cielab.Y0 = cielab.Z0 = 0.0f;
                
                TIFFXYZToRGB(&cielab, xyz[0], xyz[1], xyz[2], &r, &g, &b);
            }
        }
        
        // Try to write raw strip data
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            tif_out = TIFFClientOpen("output_tiff", "w",
                                    reinterpret_cast<thandle_t>(out_file),
                                    nullptr, nullptr, nullptr, nullptr,
                                    nullptr, nullptr, nullptr);
            
            if (tif_out) {
                // Set basic TIFF fields for output
                TIFFSetField(tif_out, TIFFTAG_IMAGEWIDTH, 64);
                TIFFSetField(tif_out, TIFFTAG_IMAGELENGTH, 64);
                TIFFSetField(tif_out, TIFFTAG_BITSPERSAMPLE, 8);
                TIFFSetField(tif_out, TIFFTAG_SAMPLESPERPIXEL, 3);
                TIFFSetField(tif_out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
                TIFFSetField(tif_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
                TIFFSetField(tif_out, TIFFTAG_ROWSPERSTRIP, 
                            TIFFDefaultStripSize(tif_out, 0));
                
                // Create some dummy strip data
                raw_strip_buffer.resize(64 * 3); // 64 pixels * 3 channels
                for (size_t i = 0; i < raw_strip_buffer.size(); i++) {
                    raw_strip_buffer[i] = static_cast<uint8_t>(i % 256);
                }
                
                // Write raw strip
                tmsize_t written = TIFFWriteRawStrip(tif_out, 0, 
                                                    raw_strip_buffer.data(), 
                                                    raw_strip_buffer.size());
                if (written < 0) {
                    TIFFErrorExt(reinterpret_cast<thandle_t>(tif_out),
                                "LLVMFuzzerTestOneInput_16",
                                "Failed to write raw strip");
                }
            }
        }
        
    } catch (...) {
        // Catch any unexpected exceptions
        TIFFErrorExt(nullptr, "LLVMFuzzerTestOneInput_16", 
                    "Unexpected exception occurred");
    }
    
cleanup:
    // Cleanup TIFF handles
    if (tif) TIFFCleanup(tif);
    if (tif_out) TIFFCleanup(tif_out);
    
    // Close file handles
    if (in_file) assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}