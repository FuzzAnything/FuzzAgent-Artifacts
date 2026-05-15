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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 16
//<Quality> {"density":8,"unique_branches":{"deflate_fast":[[1941,9,1941,26,0,0,4,1],[1945,9,1945,20,0,0,4,1]],"deflate_stored":[[1704,34,1704,42,0,0,4,0],[1704,46,1704,63,0,0,4,1],[1745,13,1745,16,0,0,4,1],[1760,9,1760,13,0,0,4,1],[1786,9,1786,36,0,0,4,1]],"_tr_align":[[276,7,276,40,1,0,4,1],[276,7,276,40,7,0,4,1]],"deflate_rle":[[2142,9,2142,26,0,0,4,1],[2146,9,2146,20,0,0,4,1]],"deflate":[[1239,17,1239,41,0,0,4,0],[1246,21,1246,42,0,0,4,0],[1248,25,1248,42,0,0,4,0]],"deflate_huff":[[2178,9,2178,26,0,0,4,1],[2182,9,2182,20,0,0,4,1]]},"library_calls":["zlibVersion","gzdopen","gzclose","gzclose","gzdopen","gzclose","gzsetparams","deflateInit2_","deflateSetHeader","deflateEnd","gzclose","gzclose","gzfread","gzflush","gzclose","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzdopen","gzsetparams","deflateInit2_","deflateSetHeader","deflateEnd","gzfread","gzflush","gzclose","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve the event of reading compressed data, setting compression parameters, and manipulating gzip headers:

*/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}




    // Ensure we have enough data for basic operations
    if (size < 100) {
        return 0;
    }

    // Get zlib version (API 1)
    const char* version = zlibVersion();
    (void)version; // Use version to avoid unused variable warning

    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor and create gzFile for reading
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create output file for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        // in_file is already closed by gzdopen taking ownership
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    gzFile gz_out = gzdopen(out_fd, fuzz_str_2);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Set compression parameters on output (API 4)
    // Use different levels and strategies from the input data
    int level = data[0] % 10; // 0-9
    int strategy = data[1] % 4; // 0-3
    if (gzsetparams(gz_out, level, strategy) != Z_OK) {
        // Continue even if this fails
    }

    // Create and set gzip header (API 5)
    z_stream strm;
    gz_header header;
    
    // Initialize the header structure
    memset(&header, 0, sizeof(header));
    
    // Set some header fields using input data
    header.text = data[2] & 1;
    header.time = (static_cast<uLong>(data[3]) << 24) | 
                  (static_cast<uLong>(data[4]) << 16) | 
                  (static_cast<uLong>(data[5]) << 8) | 
                  data[6];
    header.xflags = data[7];
    header.os = data[8];
    
    // Allocate and set extra field if we have enough data
    if (size > 50) {
        size_t extra_len = data[9] % 20; // Up to 20 bytes
        if (extra_len > 0 && 10 + extra_len < size) {
            header.extra = (Bytef*)malloc(extra_len);
            if (header.extra) {
                memcpy(header.extra, data + 10, extra_len);
                header.extra_len = static_cast<uInt>(extra_len);
                header.extra_max = header.extra_len;
            }
        }
    }
    
    // Set header name if we have enough data
    if (size > 80) {
        size_t name_len = data[30] % 20; // Up to 20 bytes
        if (name_len > 0 && 31 + name_len < size) {
            header.name = (Bytef*)malloc(name_len + 1);
            if (header.name) {
                memcpy(header.name, data + 31, name_len);
                header.name[name_len] = '\0';
                header.name_max = static_cast<uInt>(name_len);
            }
        }
    }

    // Initialize z_stream for deflateSetHeader
    // We need to get the z_stream from gzFile, but gzFile is opaque
    // So we'll create a separate z_stream for demonstration
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Initialize deflate with gzip wrapper
    if (deflateInit2(&deflate_strm, level, Z_DEFLATED, 15 + 16, 8, 
                     strategy) == Z_OK) {
        // Set the header (API 5)
        deflateSetHeader(&deflate_strm, &header);
        
        // Clean up deflate stream
        deflateEnd(&deflate_strm);
    }

    // Read data from input gzFile (API 1)
    const size_t buf_size = 4096;
    Bytef* buffer = (Bytef*)malloc(buf_size);
    if (!buffer) {
        // Cleanup
        if (header.extra) free(header.extra);
        if (header.name) free(header.name);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    z_size_t total_read = 0;
    while (total_read < size) {
        // Read using gzfread (API 1)
        z_size_t items_to_read = 1;
        z_size_t bytes_per_item = buf_size;
        
        // Adjust read size based on remaining data
        if (size - total_read < buf_size) {
            bytes_per_item = size - total_read;
        }
        
        z_size_t read_result = gzfread(buffer, fuzz_uint64_t_3, items_to_read, gz_in);
        
        if (read_result == 0) {
            // End of file or error
            break;
        }
        
        // Write to output file (simplified - in real case would write processed data)
        // For this fuzzer, we're mainly testing the APIs
        
        total_read += read_result * bytes_per_item;
    }

    // Flush the output gzFile (API 2)
    // Try different flush modes
    int flush_modes[] = {Z_NO_FLUSH, Z_PARTIAL_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH};
    int flush_idx = data[size % size] % 5;
    gzflush(gz_out, flush_modes[flush_idx]);

    // Clean up
    free(buffer);
    
    if (header.extra) free(header.extra);
    if (header.name) free(header.name);
    
    // Close gzFiles (these will also close the underlying FILEs)
    gzclose(gz_out);
    gzclose(gz_in);
    
    // Remove the output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}