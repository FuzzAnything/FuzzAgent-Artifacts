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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"deflate":[[990,9,990,33,0,0,4,0]]},"library_calls":["zlibCompileFlags","gzopen64","gzoffset","inflateInit2_","gzclose","inflateReset2","inflateReset","inflate","inflateEnd","gzclose","compress","inflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzopen64","gzoffset","inflateInit2_","inflateReset2","inflateReset","inflate","compress","inflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve decompression and compression operations:

*/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
        return 0;
    }

    // 1. Get zlib compile flags
    uLong flags = zlibCompileFlags();

    // 2. Create temporary input file with the compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Open compressed data as gzip file
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzopen64("input_file", "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Get current offset in gzip file
    off_t offset = gzoffset(gz_in);

    // 5. Set up z_stream for decompression
    z_stream decomp_stream;
    memset(&decomp_stream, 0, sizeof(decomp_stream));
    
    // Initialize with default parameters
    if (inflateInit2(&decomp_stream, MAX_WBITS + 16) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // 6. Reset inflate stream using both required APIs
    inflateReset2(&decomp_stream, MAX_WBITS + 16);
    inflateReset(&decomp_stream);

    // 7. Prepare decompression buffers
    std::vector<uint8_t> decompressed(size * 4);  // Allocate enough space
    decomp_stream.next_in = (Bytef *)data;
    decomp_stream.avail_in = size;
    decomp_stream.next_out = decompressed.data();
    decomp_stream.avail_out = decompressed.size();

    // 8. Perform decompression
    int decomp_result = inflate(&decomp_stream, Z_FINISH);
    
    if (decomp_result != Z_STREAM_END && decomp_result != Z_OK) {
        inflateEnd(&decomp_stream);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    size_t decompressed_size = decomp_stream.total_out;

    // 9. Prepare for compression
    std::vector<uint8_t> recompressed(decompressed_size * 2);
    uLongf recompressed_len = recompressed.size();
    
    // 10. Use compress API on decompressed data
    int compress_result = compress(
        recompressed.data(), 
        &recompressed_len,
        decompressed.data(), 
        decompressed_size
    );

    // 11. Clean up decompression stream
    inflateEnd(&decomp_stream);
    
    // 12. Clean up file resources
    gzclose(gz_in);
    assert_file_closed(&in_file);;

    // 13. Verify compression was successful
    if (compress_result != Z_OK) {
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // 14. Test with the recompressed data
    if (recompressed_len > 0) {
        // Open output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(recompressed.data(), 1, recompressed_len, out_file);
            assert_file_closed(&out_file);;
            
            // Remove the temporary output file
            remove("output_file");
        }
    }

    // 15. Remove temporary input file
    remove("input_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}