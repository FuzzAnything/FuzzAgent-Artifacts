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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzbuffer","gzgets","crc32_z","gzfwrite","crc32_z","crc32_combine_gen64","crc32_combine64","deflateInit_","deflate","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzbuffer","gzgets","crc32_z","gzfwrite","crc32_z","crc32_combine_gen64","crc32_combine64","deflateInit_","deflate","deflateEnd","gzclose","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to process zlib-compressed input data:

*/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;  // Too small for meaningful testing
    }

    // 1. Create FILE* handles for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get file descriptors and create gzFile handles
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 3. Set buffer size for gz_out using gzbuffer
    gzbuffer(gz_out, 8192);  // Use 8KB buffer

    // 4. Read from gzFile using gzgets
    char buffer[1024];
    char *read_result = gzgets(gz_in, buffer, sizeof(buffer));
    
    if (read_result) {
        size_t read_len = strlen(buffer);
        
        // 5. Calculate CRC32 using crc32_z
        uLong crc1 = crc32_z(0L, (const Bytef*)buffer, read_len);
        
        // 6. Write to output using gzfwrite
        z_size_t written = gzfwrite(buffer, 1, read_len, gz_out);
        
        if (written > 0) {
            // 7. Calculate another CRC32 for comparison
            uLong crc2 = crc32_z(0L, (const Bytef*)data, size > 100 ? 100 : size);
            
            // 8. Generate CRC combine generator
            off64_t len2 = size > 100 ? 100 : size;
            uLong crc_gen = crc32_combine_gen64(len2);
            
            // 9. Combine the two CRCs
            uLong combined_crc = crc32_combine64(crc1, crc2, crc_gen);
            
            // 10. Use deflate for compression
            z_stream strm;
            Bytef out_buffer[1024];
            
            // Initialize deflate stream
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            
            if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
                strm.next_in = (Bytef*)buffer;
                strm.avail_in = (uInt)read_len;
                strm.next_out = out_buffer;
                strm.avail_out = sizeof(out_buffer);
                
                // Call deflate with Z_FINISH flush
                int ret = deflate(&strm, Z_FINISH);
                
                if (ret == Z_STREAM_END || ret == Z_OK) {
                    // Successfully compressed
                    size_t compressed_size = sizeof(out_buffer) - strm.avail_out;
                    
                    // Write compressed data to output
                    fwrite(out_buffer, 1, compressed_size, out_file);
                }
                
                // Clean up deflate stream
                deflateEnd(&strm);
            }
        }
    }

    // 11. Clean up
    gzclose(gz_in);  // This also closes the file descriptor
    gzclose(gz_out); // This also closes the file descriptor
    
    // Note: fclose is not needed because gzdopen took ownership of the file descriptors
    // The FILE* handles are already closed by gzclose
    
    remove("output_file");  // Clean up output file
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}