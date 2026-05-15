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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["gzdopen","gzoffset","gzrewind","gzclearerr","gzread","deflateInit_","gzclose","deflateTune","deflateGetDictionary","deflateBound","compress2_z","inflateInit2_","inflateSyncPoint","inflateGetDictionary","inflateGetHeader","gzopen","gzread","gzclose","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzoffset","gzrewind","gzclearerr","gzread","deflateInit_","deflateTune","deflateGetDictionary","deflateBound","compress2_z","inflateInit2_","inflateSyncPoint","inflateGetDictionary","inflateGetHeader","gzopen","gzread","gzclose","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create a temporary file to store the compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    
    // 2. Open compressed data using gzdopen
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Get offset before reading
    off_t offset = gzoffset(gz_in);
    
    // 4. Rewind to beginning
    gzrewind(gz_in);
    
    // 5. Clear any existing errors
    gzclearerr(gz_in);
    
    // 6. Read decompressed data
    const size_t BUFFER_SIZE = 65536;
    Bytef *decompressed = new Bytef[BUFFER_SIZE];
    unsigned int bytes_read = 0;
    int total_read = 0;
    
    while ((bytes_read = gzread(gz_in, decompressed + total_read, 
                                 BUFFER_SIZE - total_read)) > 0) {
        total_read += bytes_read;
        if (total_read >= BUFFER_SIZE) break;
    }
    
    // 7. Initialize z_stream for deflation
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    // 8. Call deflateInit_
    int init_result = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION,
                                   ZLIB_VERSION, sizeof(z_stream));
    if (init_result != Z_OK) {
        delete[] decompressed;
        gzclose(gz_in);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 9. Tune deflate parameters
    deflateTune(&deflate_strm, 8, 9, 8, 4096);
    
    // 10. Get dictionary from deflate stream
    Bytef dict_buffer[32768];
    uInt dict_length = sizeof(dict_buffer);
    deflateGetDictionary(&deflate_strm, dict_buffer, &dict_length);
    
    // 11. Calculate deflate bound for compression
    uLong bound = deflateBound(&deflate_strm, total_read);
    
    // 12. Allocate compression buffers
    Bytef *compressed = new Bytef[bound];
    Bytef *compressed2 = new Bytef[bound];
    uLongf compressed_len = bound;
    z_size_t compressed_len_z = bound;
    z_size_t source_len_z = total_read;
    
    // 13. Perform compression using compress2_z
    int compress_result = compress2_z(compressed, &compressed_len_z,
                                     decompressed, source_len_z,
                                     Z_DEFAULT_COMPRESSION);
    
    // 14. Set up inflate stream for checking compressed data
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    // 15. Initialize inflate stream
    inflate_strm.next_in = compressed;
    inflate_strm.avail_in = compressed_len_z;
    inflate_strm.next_out = decompressed; // Reuse buffer
    inflate_strm.avail_out = BUFFER_SIZE;
    
    inflateInit2_(&inflate_strm, MAX_WBITS, ZLIB_VERSION, sizeof(z_stream));
    
    // 16. Check sync points
    inflateSyncPoint(&inflate_strm);
    
    // 17. Get dictionary from inflate stream
    Bytef inflate_dict[32768];
    uInt inflate_dict_len = sizeof(inflate_dict);
    inflateGetDictionary(&inflate_strm, inflate_dict, &inflate_dict_len);
    
    // 18. Get header from inflate stream
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&inflate_strm, &header);
    
    // 19. Create output file for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(compressed, 1, compressed_len_z, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 20. Open the output file with gzopen
    gzFile gz_out = gzopen("output_file", "rb");
    if (gz_out) {
        // Read a small amount to verify it works
        Bytef verify_buffer[1024];
        gzread(gz_out, verify_buffer, sizeof(verify_buffer));
        gzclose(gz_out);
    }
    
    // 21. Clean up
    delete[] decompressed;
    delete[] compressed;
    delete[] compressed2;
    
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    gzclose(gz_in);
    
    // Remove temporary output file
    unlink("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}