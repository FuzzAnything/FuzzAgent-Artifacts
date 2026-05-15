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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 5
//<Quality> {"density":9,"unique_branches":{"gzclose":[[15,9,15,21,0,0,4,0]],"gzeof":[[502,9,502,21,0,0,4,0]],"gzerror":[[517,9,517,21,0,0,4,0]],"gzseek64":[[373,9,373,21,0,0,4,0]],"gzdirect":[[631,9,631,21,0,0,4,0]]},"library_calls":["gzdopen","gzopen","gzclose","gzwrite","gzputc","gzclose","gzopen","gzdirect","inflateInit2_","gzclose","gzclose","gzread","inflate","inflateSyncPoint","inflateCopy","deflateInit2_","deflateGetDictionary","deflateBound","compress2","deflateEnd","gzseek64","gzeof","gzerror","inflateEnd","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzopen","gzwrite","gzputc","gzclose","gzopen","gzdirect","inflateInit2_","gzread","inflate","inflateSyncPoint","inflateCopy","deflateInit2_","deflateGetDictionary","deflateBound","compress2","deflateEnd","gzseek64","gzeof","gzerror","inflateEnd","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
	return 0;
	}
	  // Not enough data for meaningful fuzzing

    // Create temporary files for reading/writing
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

    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    // 1. Open gzip file using gzdopen and gzopen
    gzFile gz_fd = gzdopen(fd_in, "rb");
    if (!gz_fd) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    gzFile gz_file = gzopen("input_file", "wb");
    if (!gz_file) {
        gzclose(gz_fd);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 2. Write some data to gz_file using gzwrite and gzputc
    // First, write a portion of input data
    unsigned int write_len = size > 100 ? 100 : (unsigned int)size;
    if (gzwrite(gz_file, data, write_len) <= 0) {
        // Handle write error
    }
    gzputc(gz_file, 'X');  // Add a marker byte

    gzclose(gz_file);
    gz_file = gzopen("input_file", "rb");

    // 3. Check if file is in direct mode
    int is_direct = gzdirect(gz_file);

    // 4. Initialize inflate stream
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    
    // Use both inflateInit2_ versions (windowBits 15+32 for gzip decoding)
    int ret = inflateInit2_(&inflate_strm, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_fd);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 5. Read from gzip file using gzread
    uint8_t buffer[4096];
    unsigned int bytes_read = gzread(gz_fd, buffer, sizeof(buffer));
    
    // 6. Use inflate to decompress data
    inflate_strm.next_in = buffer;
    inflate_strm.avail_in = bytes_read;
    
    uint8_t out_buffer[8192];
    inflate_strm.next_out = out_buffer;
    inflate_strm.avail_out = sizeof(out_buffer);
    
    ret = inflate(&inflate_strm, Z_FINISH);
    
    // 7. Check sync point
    int sync_result = inflateSyncPoint(&inflate_strm);
    
    // 8. Create copy of inflate stream
    z_stream inflate_copy_strm;
    memset(&inflate_copy_strm, 0, sizeof(inflate_copy_strm));
    ret = inflateCopy(&inflate_copy_strm, &inflate_strm);

    // 9. Initialize deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    ret = deflateInit2_(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15 + 16, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    
    if (ret == Z_OK) {
        // 10. Get dictionary from deflate stream
        Bytef dict[256];
        uInt dict_length = sizeof(dict);
        deflateGetDictionary(&deflate_strm, dict, &dict_length);
        
        // 11. Calculate bound for compression
        uLong bound = deflateBound(&deflate_strm, size);
        
        // 12. Use compress2
        Bytef* compressed = new Bytef[bound];
        uLongf compressed_len = bound;
        
        // Use remaining input data for compression
        size_t compress_data_size = size > 500 ? 500 : size;
        ret = compress2(compressed, &compressed_len, data, 
                       (uLong)compress_data_size, Z_BEST_COMPRESSION);
        
        delete[] compressed;
        
        // 13. End deflate stream
        deflateEnd(&deflate_strm);
    }

    // 14. Seek in gzip file
    off64_t seek_pos = gzseek64(gz_file, 0, SEEK_SET);
    
    // 15. Check EOF
    int eof_check = gzeof(gz_file);
    
    // 16. Get any errors
    int err_num;
    const char* error_msg = gzerror(gz_file, &err_num);
    
    // 17. End inflate streams
    inflateEnd(&inflate_strm);
    if (inflate_copy_strm.state) {
        inflateEnd(&inflate_copy_strm);
    }

    // 18. Close all files
    gzclose(gz_fd);
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Clean up temporary file
    remove("input_file");
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}