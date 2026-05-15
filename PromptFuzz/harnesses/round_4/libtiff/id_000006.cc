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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetErrorHandlerExtR","TIFFClientOpenExt","TIFFSetWarningHandlerExt","_TIFFmalloc","TIFFReadBufferSetup","TIFFReadRawStrip","TIFFCheckpointDirectory","_TIFFfree","TIFFClose"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetErrorHandlerExtR","TIFFClientOpenExt","TIFFSetWarningHandlerExt","_TIFFmalloc","TIFFReadBufferSetup","TIFFReadRawStrip","TIFFCheckpointDirectory","_TIFFfree","TIFFClose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Open the file for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create TIFFOpenOptions and set error handler
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts) {
        TIFFOpenOptionsSetErrorHandlerExtR(opts, nullptr, nullptr);
    }
    
    // Open TIFF file using file descriptor
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        if (opts) free(opts);
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // Define custom I/O functions
    auto readproc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fread(buf, 1, size, reinterpret_cast<FILE*>(handle));
    };
    
    auto writeproc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fwrite(buf, 1, size, reinterpret_cast<FILE*>(handle));
    };
    
    auto seekproc = [](thandle_t handle, toff_t off, int whence) -> toff_t {
        return fseek(reinterpret_cast<FILE*>(handle), off, whence);
    };
    
    auto closeproc = [](thandle_t handle) -> int {
        return fclose(reinterpret_cast<FILE*>(handle));
    };
    
    auto sizeproc = [](thandle_t handle) -> toff_t {
        FILE* f = reinterpret_cast<FILE*>(handle);
        long pos = ftell(f);
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, pos, SEEK_SET);
        return size;
    };
    
    // Open TIFF with custom I/O
    TIFF* tif = TIFFClientOpenExt("input_file", fuzz_str_1, 
                                  reinterpret_cast<thandle_t>(in_file),
                                  readproc, writeproc,
                                  seekproc, closeproc,
                                  sizeproc, nullptr, nullptr,
                                  opts);
    
    if (tif) {
        // Set warning handler
        TIFFSetWarningHandlerExt(nullptr);
        
        // Allocate buffer for reading
        tmsize_t buf_size = 4096;
        void* buffer = _TIFFmalloc(fuzz_int64_t_2);
        
        if (buffer) {
            // Set up read buffer
            TIFFReadBufferSetup(tif, buffer, fuzz_int64_t_3);
            
            // Try to read raw strips if they exist
            uint32_t strip = 0;
            tmsize_t read_size = TIFFReadRawStrip(tif, fuzz_uint32_t_4, buffer, fuzz_int64_t_5);
            
            // Checkpoint directory
            TIFFCheckpointDirectory(tif);
            
            // Clean up buffer
            _TIFFfree(buffer);
        }
        
        // Close TIFF
        TIFFClose(tif);
    }
    
    // Clean up
    if (opts) free(opts);
    
    // Note: in_file is closed by TIFFClose via closeproc
    // Remove temporary file
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}