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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"TIFFErrorExtR":[[125,9,125,29,0,0,4,0],[128,33,128,36,0,0,4,0]]},"library_calls":["TIFFSetErrorHandlerExt","TIFFClientOpen","TIFFNumberOfDirectories","TIFFReadCustomDirectory","TIFFRGBAImageOK","TIFFVStripSize64","TIFFTileSize64","TIFFRawStripSize64","LogLuv32fromXYZ","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBAImage","_TIFFfree","TIFFCleanup","TIFFSetErrorHandlerExt"],"critical_calls":["TIFFSetErrorHandlerExt","TIFFClientOpen","TIFFNumberOfDirectories","TIFFReadCustomDirectory","TIFFRGBAImageOK","TIFFVStripSize64","TIFFTileSize64","TIFFRawStripSize64","LogLuv32fromXYZ","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBAImage","_TIFFfree","TIFFCleanup","TIFFSetErrorHandlerExt"],"visited":0}
/*Here's a fuzz driver that uses the specified libtiff APIs to process TIFF data:

*/


// Custom error handler
void  id_000011_CustomErrorHandler(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    // Just ignore errors during fuzzing
    return;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
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
    
    // Set custom error handler
    TIFFErrorHandlerExt prevHandler = TIFFSetErrorHandlerExt( id_000011_CustomErrorHandler);
    
    // Open the input file as TIFF
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create TIFF object from file
    TIFF* tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
                               (thandle_t)in_file,
                               NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    
    if (!tif) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use TIFFNumberOfDirectories
    tdir_t numDirs = TIFFNumberOfDirectories(tif);
    
    if (numDirs > 0) {
        // Try to read custom directory (starting from first directory)
        int ret = TIFFReadCustomDirectory(tif, fuzz_uint64_t_3, NULL);
        
        // Check if RGBA image conversion is possible
        char errorMsg[1024];
        int rgbaOk = TIFFRGBAImageOK(tif, errorMsg);
        
        // Calculate various sizes
        if (rgbaOk) {
            // Calculate vertical strip size for 1 row
            uint64_t vStripSize = TIFFVStripSize64(tif, fuzz_uint32_t_4);
            
            // Calculate tile size (if tiled)
            uint64_t tileSize = TIFFTileSize64(tif);
            
            // Calculate raw strip size for first strip
            uint64_t rawStripSize = TIFFRawStripSize64(tif, fuzz_uint32_t_5);
        }
        
        // Convert a sample XYZ color to LogLuv32
        float xyz[3] = {0.5f, 0.5f, 0.5f};  // Sample XYZ values
        uint32_t logLuvValue = LogLuv32fromXYZ(xyz, fuzz_int32_t_6);
        
        // Try to read RGBA image if possible
        if (rgbaOk) {
            // Get image dimensions
            uint32_t width = 0, height = 0;
            if (TIFFGetField(tif, fuzz_uint32_t_7, &width) &&
                TIFFGetField(tif, fuzz_uint32_t_8, &height)) {
                
                // Allocate buffer for RGBA data
                uint32_t* raster = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
                if (raster) {
                    // Try to read RGBA image
                    int readResult = TIFFReadRGBAImage(tif, fuzz_uint32_t_9, fuzz_uint32_t_10, raster, fuzz_int32_t_11);
                    _TIFFfree(raster);
                }
            }
        }
    }
    
    // Clean up
    TIFFCleanup(tif);
    assert_file_closed(&in_file);;
    
    // Restore original error handler
    TIFFSetErrorHandlerExt(prevHandler);
    
    // Clean up temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}