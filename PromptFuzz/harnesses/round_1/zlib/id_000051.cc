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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"deflateGetDictionary":[[630,9,630,32,0,0,4,0]]},"library_calls":["gzdopen","gzseek64","gzeof","inflateInit2_","gzclose","compressBound","inflate","inflateCodesUsed","inflateCopy","deflateGetDictionary","compressBound","compress_z","deflateInit_","deflateParams","deflateCopy","deflateEnd","deflateEnd","adler32","adler32","adler32","adler32","adler32_combine64","inflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzseek64","gzeof","inflateInit2_","compressBound","inflate","inflateCodesUsed","inflateCopy","deflateGetDictionary","compressBound","compress_z","deflateInit_","deflateParams","deflateCopy","deflateEnd","deflateEnd","adler32","adler32","adler32","adler32","adler32_combine64","inflateEnd","inflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




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
    
    // 2. Create gzFile handle from file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Use gzseek64 to move to beginning (just to demonstrate usage)
    off64_t seek_result = gzseek64(gz_in, 0, SEEK_SET);
    if (seek_result < 0) {
        // Handle error if needed
    }
    
    // 4. Read the compressed data using gzgetc and check for EOF
    std::vector<Bytef> compressed_buffer;
    int c;
    while ((c = gzgetc(gz_in)) != -1) {
        compressed_buffer.push_back(static_cast<Bytef>(c));
        // Check if we've reached EOF
        if (gzeof(gz_in)) {
            break;
        }
    }
    
    // 5. Prepare for decompression
    z_stream inf_strm, inf_strm2;
    memset(&inf_strm, 0, sizeof(z_stream));
    memset(&inf_strm2, 0, sizeof(z_stream));
    
    // 6. Initialize inflation stream
    if (inflateInit2(&inf_strm, 15 + 32) != Z_OK) { // 15+32 for gzip decoding
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Set up input buffer for inflation
    inf_strm.next_in = compressed_buffer.data();
    inf_strm.avail_in = compressed_buffer.size();
    
    // 8. Create output buffer for decompressed data
    std::vector<Bytef> decompressed_buffer(compressBound(compressed_buffer.size()));
    inf_strm.next_out = decompressed_buffer.data();
    inf_strm.avail_out = decompressed_buffer.size();
    
    // 9. Perform inflation
    int inflate_result = inflate(&inf_strm, Z_FINISH);
    
    // 10. Get inflation codes usage
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    
    // 11. Copy the inflation stream
    int copy_result = inflateCopy(&inf_strm2, &inf_strm);
    
    // 12. Get dictionary from stream (if any)
    uInt dict_length = 0;
    Bytef dictionary_buffer[1024];
    int dict_result = deflateGetDictionary(&inf_strm, dictionary_buffer, &dict_length);
    
    // 13. Prepare for compression using compress_z
    std::vector<Bytef> recompressed_buffer(compressBound(inf_strm.total_out));
    z_size_t recompressed_len = recompressed_buffer.size();
    
    // 14. Use compress_z on the decompressed data
    int compress_result = compress_z(
        recompressed_buffer.data(), 
        &recompressed_len,
        decompressed_buffer.data(),
        inf_strm.total_out
    );
    
    // 15. Set up deflate stream for parameter testing
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    
    if (deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // 16. Set deflate parameters
        deflateParams(&def_strm, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
        
        // 17. Copy deflate stream
        z_stream def_strm_copy;
        memset(&def_strm_copy, 0, sizeof(z_stream));
        int def_copy_result = deflateCopy(&def_strm_copy, &def_strm);
        
        // Clean up copied stream
        if (def_copy_result == Z_OK) {
            deflateEnd(&def_strm_copy);
        }
        
        // Clean up original deflate stream
        deflateEnd(&def_strm);
    }
    
    // 18. Compute Adler-32 checksums and combine them
    uLong adler1 = adler32(0L, Z_NULL, 0);
    if (decompressed_buffer.size() > 0) {
        size_t half = decompressed_buffer.size() / 2;
        adler1 = adler32(adler1, decompressed_buffer.data(), half);
        
        uLong adler2 = adler32(0L, Z_NULL, 0);
        adler2 = adler32(adler2, decompressed_buffer.data() + half, 
                        decompressed_buffer.size() - half);
        
        // Combine the two checksums
        uLong combined = adler32_combine64(adler1, adler2, decompressed_buffer.size() - half);
    }
    
    // 19. Write recompressed data to output file
    if (compress_result == Z_OK) {
        fwrite(recompressed_buffer.data(), 1, recompressed_len, out_file);
    }
    
    // 20. Clean up all resources
    inflateEnd(&inf_strm);
    inflateEnd(&inf_strm2);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}