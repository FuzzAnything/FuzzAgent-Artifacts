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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"TIFFClientOpenExt":[[402,22,402,32,0,0,4,0]]},"library_calls":["TIFFSetErrorHandlerExt","TIFFSetErrorHandlerExt","TIFFClientOpen","TIFFNumberOfDirectories","TIFFReadCustomDirectory","TIFFRGBAImageOK","TIFFVStripSize64","TIFFTileSize64","TIFFRawStripSize64","LogLuv32fromXYZ","TIFFErrorExt","TIFFCleanup","TIFFSetErrorHandlerExt"],"critical_calls":["TIFFSetErrorHandlerExt","TIFFClientOpen","TIFFNumberOfDirectories","TIFFReadCustomDirectory","TIFFRGBAImageOK","TIFFVStripSize64","TIFFTileSize64","TIFFRawStripSize64","LogLuv32fromXYZ","TIFFErrorExt","TIFFCleanup","TIFFSetErrorHandlerExt"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create temporary input file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Set up error handler
    TIFFErrorHandlerExt old_handler = TIFFSetErrorHandlerExt(nullptr);
    
    // Open the TIFF file for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        // Clean up and return
        remove("input_file");
        TIFFSetErrorHandlerExt(old_handler);
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Use TIFFClientOpen to open the file
    thandle_t handle = (thandle_t)in_file;
    TIFFReadWriteProc read_proc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fread(buf, 1, size, (FILE*)handle);
    };
    
    TIFFSeekProc seek_proc = [](thandle_t handle, toff_t off, int whence) -> toff_t {
        fseek((FILE*)handle, off, whence);
        return ftell((FILE*)handle);
    };
    
    TIFFCloseProc close_proc = [](thandle_t handle) -> int {
        fclose((FILE*)handle);
        return 0;
    };
    
    TIFFSizeProc size_proc = [](thandle_t handle) -> toff_t {
        long curr = ftell((FILE*)handle);
        fseek((FILE*)handle, 0, SEEK_END);
        long size = ftell((FILE*)handle);
        fseek((FILE*)handle, curr, SEEK_SET);
        return size;
    };
    
    // Null map/unmap procs
    TIFFMapFileProc map_proc = [](thandle_t, void** base, toff_t* size) -> int { return 0; };
    TIFFUnmapFileProc unmap_proc = [](thandle_t, void* base, toff_t size) {};
    
    TIFF* tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, handle, 
                               read_proc, nullptr,  // No write proc for read-only
                               seek_proc, close_proc, 
                               size_proc, map_proc, unmap_proc);
    
    if (tif) {
        // Get number of directories
        tdir_t num_dirs = TIFFNumberOfDirectories(tif);
        
        if (num_dirs > 0) {
            // Read custom directory (use default field array by passing nullptr)
            toff_t dir_offset = 0;  // First directory
            int read_result = TIFFReadCustomDirectory(tif, fuzz_uint64_t_3, nullptr);
            
            // Check if RGBA conversion is possible
            char error_msg[1024];
            int rgba_ok = TIFFRGBAImageOK(tif, error_msg);
            
            // Calculate strip and tile sizes
            uint32_t nrows = 1;
            uint64_t vstrip_size = TIFFVStripSize64(tif, fuzz_uint32_t_4);
            uint64_t tile_size = TIFFTileSize64(tif);
            
            // Get raw strip size for first strip
            uint32_t strip = 0;
            uint64_t raw_strip_size = TIFFRawStripSize64(tif, fuzz_uint32_t_5);
            
            // Use LogLuv32fromXYZ with dummy data
            float xyz[3] = {0.5f, 0.5f, 0.5f};
            uint32_t logluv_value = LogLuv32fromXYZ(xyz, fuzz_int32_t_6);
            
            // Try to trigger an error condition
            if (num_dirs > 10) {  // Unlikely case
                TIFFErrorExt(handle, fuzz_str_7, fuzz_str_8);
            }
        }
        
        // Clean up TIFF
        TIFFCleanup(tif);
    } else {
        // Close file manually if TIFF open failed
        close_proc(handle);
    }
    
    // Restore original error handler
    TIFFSetErrorHandlerExt(old_handler);
    
    // Clean up temporary file
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}