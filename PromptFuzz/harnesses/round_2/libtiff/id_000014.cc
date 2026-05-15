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
//<ID> 785
//<Prompt> []
/*<Combination>: [
*/
//<score> 112, nr_unique_branch: 20
//<Quality> {"density":16,"unique_branches":{"TIFFReadTile":[[961,9,961,31,0,0,4,1],[961,35,961,66,0,0,4,1]],"TIFFWriteDirectorySec":[[497,9,497,34,0,0,4,0]],"Fax3PrematureEOF":[[282,22,282,62,1,0,4,0],[282,22,282,62,2,0,4,0]],"OJPEGReadHeaderInfoSec":[[1517,17,1519,32,0,0,4,0]],"TIFFWarningExtR":[[98,9,98,31,0,0,4,0],[101,35,101,38,0,0,4,0]],"XYZtoRGB24":[[874,24,874,33,0,0,4,0],[874,24,874,33,0,0,4,1],[874,40,874,49,0,0,4,0],[874,40,874,49,0,0,4,1],[875,24,875,33,0,0,4,0],[875,24,875,33,0,0,4,1],[875,40,875,49,0,0,4,0],[875,40,875,49,0,0,4,1],[876,24,876,33,0,0,4,0],[876,24,876,33,0,0,4,1],[876,40,876,49,0,0,4,0],[876,40,876,49,0,0,4,1]]},"library_calls":["TIFFSetWarningHandlerExt","TIFFOpenOptionsAlloc","TIFFOpen","TIFFOpenOptionsFree","TIFFGetField","TIFFGetField","TIFFGetField","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFTileSize","_TIFFmalloc","TIFFReadTile","TIFFIsByteSwapped","TIFFSwabArrayOfFloat","XYZtoRGB24","TIFFCheckpointDirectory","_TIFFfree","TIFFSetField","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFSetWarningHandlerExt","TIFFOpenOptionsAlloc","TIFFOpen","TIFFGetField","TIFFGetField","TIFFGetField","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFTileSize","_TIFFmalloc","TIFFReadTile","TIFFIsByteSwapped","TIFFSwabArrayOfFloat","XYZtoRGB24","TIFFCheckpointDirectory","_TIFFfree","TIFFSetField","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs to achieve a TIFF processing event:

*/


// Include libtiff headers

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file for libtiff to process
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Set up custom warning handler
    TIFFErrorHandlerExt warning_handler = [](thandle_t, const char* module, const char* fmt, va_list ap) {
        // Custom warning handler - do nothing in fuzzing
    };
    TIFFSetWarningHandlerExt(warning_handler);
    
    // Open the TIFF file for reading
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    TIFF* tif = TIFFOpen("input_file", "rm");
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Get tile information
    uint32_t width = 0, height = 0, tileWidth = 0, tileHeight = 0;
    uint16_t samplesPerPixel = 1;
    
    // Use TIFFVSetField via TIFFSetField (which internally uses TIFFVSetField)
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) &&
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel)) {
        
        // Check if image is tiled
        if (TIFFIsTiled(tif)) {
            TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tileWidth);
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tileHeight);
            
            if (tileWidth > 0 && tileHeight > 0) {
                // Allocate buffer for tile data (assuming XYZ float format)
                tmsize_t tileSize = TIFFTileSize(tif);
                if (tileSize > 0) {
                    uint8_t* tileBuffer = (uint8_t*)_TIFFmalloc(tileSize);
                    if (tileBuffer) {
                        // Read a tile using TIFFReadTile
                        uint32_t tileX = 0;
                        uint32_t tileY = 0;
                        uint32_t tileZ = 0;
                        uint16_t sample = 0;
                        
                        if (TIFFReadTile(tif, tileBuffer, tileX, tileY, tileZ, sample) > 0) {
                            // Check if we need to swap byte order
                            if (TIFFIsByteSwapped(tif)) {
                                // Assuming tile contains float data
                                TIFFSwabArrayOfFloat((float*)tileBuffer, tileSize / sizeof(float));
                            }
                            
                            // Prepare for XYZ to RGB conversion
                            // We'll process the first pixel in the tile
                            if (tileSize >= sizeof(float) * 3) {
                                float xyz[3];
                                uint8_t rgb[3];
                                
                                // Copy XYZ values (simplified - assuming tile contains XYZ data)
                                memcpy(xyz, tileBuffer, sizeof(float) * 3);
                                
                                // Convert XYZ to RGB24
                                XYZtoRGB24(xyz, rgb);
                                
                                // Write the converted RGB back to buffer
                                memcpy(tileBuffer, rgb, 3);
                            }
                            
                            // Checkpoint directory changes
                            TIFFCheckpointDirectory(tif);
                        }
                        
                        _TIFFfree(tileBuffer);
                    }
                }
            }
        }
    }
    
    // Set field using TIFFVSetField via va_args
    uint32_t orientation = ORIENTATION_TOPLEFT;
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    
    // Write processed data to output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}