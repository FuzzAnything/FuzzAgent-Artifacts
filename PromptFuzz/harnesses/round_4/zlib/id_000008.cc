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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 35, nr_unique_branch: 4
//<Quality> {"density":7,"unique_branches":{"deflateStateCheck":[[541,9,541,38,0,0,4,0]],"gzflush":[[612,9,612,32,0,0,4,0]],"gzsetparams":[[641,9,641,32,0,0,4,0]],"deflateSetHeader":[[715,9,715,32,0,0,4,0]]},"library_calls":["zlibVersion","gzdopen","gzsetparams","gzfread","deflateSetHeader","gzflush","gzrewind","gzfread","gzclose","deflateEnd"],"critical_calls":["zlibVersion","gzdopen","gzsetparams","gzfread","deflateSetHeader","gzflush","gzrewind","gzfread","gzclose","deflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs with the input data:

*/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for meaningful operations

    // 1. Get zlib version
    const char *version = zlibVersion();
    (void)version;  // Use version to avoid unused variable warning

    // 2. Create input file from the data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 3. Create gzFile from the file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Set compression parameters (even though we're reading)
    // We'll use a subset of the data to determine parameters
    int level = data[0] % 10;  // 0-9 compression level
    int strategy = data[1] % 4;  // 0-3 strategy
    gzsetparams(gz_in, level, strategy);

    // 5. Prepare buffer for reading
    size_t buf_size = 4096;
    if (size < buf_size) buf_size = size;
    std::vector<char> buffer(buf_size);
    
    // 6. Read data using gzfread
    // Read in chunks: nitems * size = total bytes
    size_t items_to_read = 1;
    size_t item_size = buf_size;
    
    // Adjust based on available data
    if (size < item_size) {
        item_size = size;
    }
    
    z_size_t bytes_read = gzfread(buffer.data(), fuzz_uint64_t_2, items_to_read, gz_in);
    (void)bytes_read;  // Use result to avoid warning

    // 7. Create z_stream for deflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // 8. Prepare gz_header
    gz_header header;
    memset(&header, 0, sizeof(header));
    
    // Use some data for header fields
    header.text = data[2] & 1;
    header.time = *reinterpret_cast<const uLong*>(data + 3);
    header.xflags = data[7];
    header.os = data[8];
    
    // 9. Set deflate header
    deflateSetHeader(&strm, &header);

    // 10. Flush the gzFile (even though reading, we can try)
    // This might fail for read-only files, but we call it anyway
    gzflush(gz_in, fuzz_int32_t_3);

    // 11. Try to read more data
    if (size > 2048) {
        // Move to a different position in the file
        gzrewind(gz_in);
        
        // Read with different parameters
        size_t second_read_size = (size / 2) < 1024 ? size / 2 : 1024;
        if (second_read_size > 0) {
            std::vector<char> buffer2(second_read_size);
            z_size_t bytes_read2 = gzfread(buffer2.data(), fuzz_uint64_t_4, second_read_size, gz_in);
            (void)bytes_read2;
        }
    }

    // 12. Clean up
    gzclose(gz_in);  // This will also close the underlying file descriptor
    // Note: gzdopen duplicates the fd, so we shouldn't close in_file separately
    // The gzclose will handle closing the underlying FILE*
    
    // 13. Additional cleanup for z_stream
    deflateEnd(&strm);

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}