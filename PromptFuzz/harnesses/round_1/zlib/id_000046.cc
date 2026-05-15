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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"compress2_z":[[61,14,61,25,0,0,4,0]]},"library_calls":["gzdopen","gzdirect","gzungetc","gzclose","inflateInit2_","gzclose","inflateCopy","inflateSyncPoint","inflate","deflateInit2_","inflateEnd","inflateEnd","gzclose","deflateSetDictionary","deflateGetDictionary","deflateBound","compress2","adler32","adler32","adler32","adler32_combine64","gzerror","deflateEnd","inflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gzungetc","inflateInit2_","inflateCopy","inflateSyncPoint","inflate","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateBound","compress2","adler32","adler32","adler32","adler32_combine64","gzerror","deflateEnd","inflateEnd","inflateEnd","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs in a coherent scenario:

*/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for meaningful operations
    if (size < 50) {
	return 0;
	}
	
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Open a gzFile for reading
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 2. Check if file is in direct mode
    int is_direct = gzdirect(gz_in);
    
    // 3. Read some data using gzgetc
    int first_char = gzgetc(gz_in);
    
    // 4. Put a character back using gzputc (simulating ungetc)
    if (first_char != EOF) {
        gzungetc(first_char, gz_in);
    }
    
    // 5. Read data for inflation
    Bytef compressed_buf[1024];
    uLongf compressed_size = sizeof(compressed_buf);
    
    // Read compressed data (treating fuzzer input as compressed data)
    size_t read_size = fread(compressed_buf, 1, compressed_size, in_file);
    if (read_size == 0) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Setup inflate stream
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    inflate_strm.next_in = compressed_buf;
    inflate_strm.avail_in = read_size;
    
    Bytef decompressed_buf[2048];
    inflate_strm.next_out = decompressed_buf;
    inflate_strm.avail_out = sizeof(decompressed_buf);
    
    if (inflateInit2(&inflate_strm, 15 + 32) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Create a copy of inflate stream
    z_stream inflate_strm_copy;
    memset(&inflate_strm_copy, 0, sizeof(inflate_strm_copy));
    int copy_result = inflateCopy(&inflate_strm_copy, &inflate_strm);
    
    // 8. Try to find sync point
    int sync_result = inflateSyncPoint(&inflate_strm);
    
    // 9. Perform inflation on original stream
    int inflate_result = inflate(&inflate_strm, Z_SYNC_FLUSH);
    
    // 10. Setup deflate stream for compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    if (deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                    15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        inflateEnd(&inflate_strm);
        inflateEnd(&inflate_strm_copy);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 11. Set dictionary for deflate (use part of input as dictionary)
    uInt dict_len = size < 100 ? size : 100;
    int dict_result = deflateSetDictionary(&deflate_strm, data, dict_len);
    
    // 12. Get dictionary back
    Bytef retrieved_dict[100];
    uInt retrieved_len = sizeof(retrieved_dict);
    int get_dict_result = deflateGetDictionary(&deflate_strm, retrieved_dict, &retrieved_len);
    
    // 13. Calculate bound for compressed size
    uLong bound_size = deflateBound(&deflate_strm, inflate_strm.total_out);
    
    // 14. Compress using compress2 API
    Bytef dest_buf[4096];
    uLongf dest_len = sizeof(dest_buf);
    
    // Use decompressed data from inflate if available, otherwise use input
    const Bytef* source_for_compress;
    uLong source_len;
    
    if (inflate_result == Z_OK || inflate_result == Z_STREAM_END) {
        source_for_compress = decompressed_buf;
        source_len = inflate_strm.total_out;
    } else {
        source_for_compress = data;
        source_len = size < 4096 ? size : 4096;
    }
    
    int compress_result = compress2(dest_buf, &dest_len, 
                                   source_for_compress, source_len,
                                   Z_DEFAULT_COMPRESSION);
    
    // 15. Combine Adler-32 checksums
    uLong adler1 = adler32(0L, Z_NULL, 0);
    if (source_len > 0) {
        size_t half = source_len / 2;
        adler1 = adler32(adler1, source_for_compress, half);
        
        uLong adler2 = adler32(0L, source_for_compress + half, source_len - half);
        uLong combined_adler = adler32_combine64(adler1, adler2, source_len);
        
        // Use the combined Adler value
        (void)combined_adler;
    }
    
    // 16. Check for gzFile errors
    int gz_errnum;
    const char* gz_error_msg = gzerror(gz_in, &gz_errnum);
    (void)gz_error_msg; // Suppress unused variable warning
    
    // 17. Cleanup
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    inflateEnd(&inflate_strm_copy);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}