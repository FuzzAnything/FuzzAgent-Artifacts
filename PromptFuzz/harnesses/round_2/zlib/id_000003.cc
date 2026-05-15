#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <zlib.h>
}
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["gzopen","gzclearerr","gzrewind","gzoffset","gzread","deflateInit_","gzclose","deflateTune","deflateBound","deflateGetDictionary","compress2_z","deflateEnd","gzdopen","gzread","gzclose","gzclose"],"critical_calls":["gzopen","gzclearerr","gzrewind","gzoffset","gzread","deflateInit_","deflateTune","deflateBound","deflateGetDictionary","compress2_z","deflateEnd","gzdopen","gzread","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	
    
    // Write input to temporary file
    FILE *tmp_file = fopen("input_file", "wb");
    if (!tmp_file) {
	assert_file_closed(&tmp_file);
	return 0;
	}
	
    fwrite(data, 1, size, tmp_file);
    assert_file_closed(&tmp_file);;
    
    // 1. Open compressed file with gzopen
    gzFile gz = gzopen("input_file", "rb");
    if (!gz) {
        unlink("input_file");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 2. Clear any error state
    gzclearerr(gz);
    
    // 3. Rewind to beginning
    gzrewind(gz);
    
    // 4. Get current offset (should be 0 after rewind)
    off_t offset = gzoffset(gz);
    
    // Read some data from gz file
    char buffer[4096];
    int bytes_read = gzread(gz, buffer, sizeof(buffer));
    
    if (bytes_read > 0) {
        // 5. Prepare for compression
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        // Initialize deflate with default parameters
        if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
            gzclose(gz);
            unlink("input_file");
            assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
        }
        
        // 6. Tune compression parameters
        deflateTune(&strm, 8, 9, 8, 4096);
        
        // 7. Get compression bound
        uLong bound = deflateBound(&strm, bytes_read);
        
        // 8. Try to get dictionary (may not be set)
        Bytef dict[4096];
        uInt dictLength = 0;
        deflateGetDictionary(&strm, dict, &dictLength);
        
        // 9. Use compress2_z with the read data
        Bytef compressed[8192];
        z_size_t compressed_len = sizeof(compressed);
        
        compress2_z(compressed, &compressed_len, 
                   reinterpret_cast<const Bytef*>(buffer), 
                   bytes_read, Z_DEFAULT_COMPRESSION);
        
        // Clean up deflate stream
        deflateEnd(&strm);
    }
    
    // 10. Get file descriptor and use gzdopen
    FILE *file_for_fd = fopen("input_file", "rb");
    if (file_for_fd) {
        int fd = fuzz_fileno(file_for_fd);
        gzFile gz2 = gzdopen(fd, "rb");
        if (gz2) {
            // Read a bit from the new gzFile
            char buf2[1024];
            gzread(gz2, buf2, sizeof(buf2));
            gzclose(gz2);
        } else {
            // If gzdopen fails, close the fd manually
            close(fd);
        }
        assert_file_closed(&file_for_fd);;
    }
    
    // Clean up
    gzclose(gz);
    unlink("input_file");
    
    assert_file_closed(&file_for_fd);
	assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}