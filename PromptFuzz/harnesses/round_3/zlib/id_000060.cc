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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 3
//<Quality> {"density":6,"unique_branches":{"gzerror":[[520,9,520,31,0,0,4,0],[520,35,520,58,0,0,4,1]],"inflatePrime":[[231,22,231,51,0,0,4,0]]},"library_calls":["compressBound_z","uncompress_z","gzdopen","inflateInit2_","inflate","inflatePrime","inflateCopy","inflateEnd","inflateEnd","gzoffset","gzoffset64","gzdopen","gzprintf","gzerror","gzclose","gzclose"],"critical_calls":["compressBound_z","uncompress_z","gzdopen","inflateInit2_","inflate","inflatePrime","inflateCopy","inflateEnd","inflateEnd","gzoffset","gzoffset64","gzdopen","gzprintf","gzerror","gzclose","gzclose"],"visited":1}
/**/


// Helper function to create a temporary file with input data
static FILE*  id_000060_create_temp_file(const uint8_t* data, size_t size, const char* mode) {
    FILE* file = tmpfile();
    if (file && size > 0) {
        fwrite(data, 1, size, file);
        rewind(file);
    }
    return file;
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data for testing
    
    // 1. First use compressBound_z to estimate maximum compressed size
    z_size_t max_compressed = compressBound_z(size);
    if (max_compressed == 0) {
	return 0;
	}
	
    
    // 2. Try to uncompress the data (if it's valid compressed data)
    Bytef* uncompressed_buf = (Bytef*)malloc(max_compressed);
    if (!uncompressed_buf) {
	return 0;
	}
	
    
    z_size_t uncompressed_len = max_compressed;
    int uncompress_result = uncompress_z(uncompressed_buf, &uncompressed_len, 
                                         data, size);
    
    // 3. Create a gzFile for reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        free(uncompressed_buf);
        assert_file_closed(&in_file);
	return 0;
    }
	int fuzz_fd_1 = fuzz_fileno(in_file);

    
    // 4. Open the gzFile with gzopen
    gzFile gz = gzdopen(fuzz_fd_1, fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        free(uncompressed_buf);
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }
    
    // 5. Use inflate functions with the data
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));
    
    // Initialize inflation stream
    if (inflateInit2(&strm1, 15 + 16) == Z_OK) {
        // Set up input
        strm1.next_in = (Bytef*)data;
        strm1.avail_in = size > 100 ? 100 : size; // Use first 100 bytes
        
        // Allocate output buffer
        Bytef* out_buf = (Bytef*)malloc(1024);
        if (out_buf) {
            strm1.next_out = out_buf;
            strm1.avail_out = 1024;
            
            // Try to inflate a bit
            inflate(&strm1, fuzz_int32_t_2);
            
            // 6. Use inflatePrime to add bits
            inflatePrime(&strm1, fuzz_int32_t_3, fuzz_int32_t_4); // Add 2 bits with value 1
            
            // 7. Try to copy the stream
            if (inflateCopy(&strm2, &strm1) == Z_OK) {
                // Use the copied stream briefly
                inflateEnd(&strm2);
            }
            
            free(out_buf);
        }
        inflateEnd(&strm1);
    }
    
    // 8. Try to get gzFile offset using both gzoffset and gzoffset64
    off_t offset = gzoffset(gz);
    off64_t offset64 = gzoffset64(gz);
    
    // 9. Try to write to output file using gzprintf
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
	int fuzz_fd_0 = fuzz_fileno(out_file);

        gzFile gz_out = gzdopen(fuzz_fd_0, fuzz_str_5);
        if (gz_out) {
            // Write a simple format string
            gzprintf(gz_out, "Processed %lu bytes", (unsigned long)size);
            
            // 10. Check for errors with gzerror
            int errnum = 0;
            const char* errmsg = gzerror(gz_out, &errnum);
            
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // 11. Cleanup
    gzclose(gz); // This also closes the underlying file descriptor
    // Note: gzclose will close the file descriptor, so we don't need to assert_file_closed(&in_file);
    // Trying to fclose after gzclose would cause issues
    
    free(uncompressed_buf);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
}