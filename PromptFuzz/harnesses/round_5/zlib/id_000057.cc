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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"inflate":[[1119,9,1119,18,0,0,4,0]]},"library_calls":["gzopen","gzclose","inflateInit_","gzclose","gztell64","gzread","inflate","inflateSync","inflateSyncPoint","inflatePrime","deflateInit_","deflateCopy","gzdopen","gzputc","gzerror","gzclose_w","deflateEnd","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzopen","inflateInit_","gztell64","gzread","inflate","inflateSync","inflateSyncPoint","inflatePrime","deflateInit_","deflateCopy","gzdopen","gzputc","gzerror","gzclose_w","deflateEnd","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a temporary file for compressed data
    FILE *tmp_file = fopen("tmp_compressed.z", "wb");
    if (!tmp_file) {
        assert_file_closed(&tmp_file);
	return 0;
    }
    fwrite(data, 1, size, tmp_file);
    assert_file_closed(&tmp_file);;
    
    // 1. Open the compressed file for reading
    gzFile gz_in = gzopen("tmp_compressed.z", "rb");
    if (!gz_in) {
        remove("tmp_compressed.z");
        assert_file_closed(&tmp_file);
	return 0;
    }
    
    // Create output file for decompressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        remove("tmp_compressed.z");
        assert_file_closed(&out_file);
	assert_file_closed(&tmp_file);
	return 0;
    }
    
    // 2. Initialize zlib streams
    z_stream strm_in, strm_out, strm_copy;
    memset(&strm_in, 0, sizeof(z_stream));
    memset(&strm_out, 0, sizeof(z_stream));
    memset(&strm_copy, 0, sizeof(z_stream));
    
    // Initialize inflate stream
    if (inflateInit(&strm_in) != Z_OK) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        remove("tmp_compressed.z");
        assert_file_closed(&out_file);
	assert_file_closed(&tmp_file);
	return 0;
    }
    
    // 3. Use gztell64 to get current position
    off64_t pos = gztell64(gz_in);
    
    // 4. Read and decompress data
    Bytef in_buf[1024];
    Bytef out_buf[1024];
    int ret = Z_OK;
    
    while (ret != Z_STREAM_END) {
        // Read from gz file
        int bytes_read = gzread(gz_in, in_buf, sizeof(in_buf));
        if (bytes_read <= 0) {
            break;
        }
        
        strm_in.avail_in = bytes_read;
        strm_in.next_in = in_buf;
        
        do {
            strm_in.avail_out = sizeof(out_buf);
            strm_in.next_out = out_buf;
            
            ret = inflate(&strm_in, Z_NO_FLUSH);
            
            if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                // 5. Try to synchronize on error
                int sync_ret = inflateSync(&strm_in);
                if (sync_ret == Z_OK) {
                    // 6. Check if at sync point
                    int is_sync_point = inflateSyncPoint(&strm_in);
                    if (is_sync_point == 1) {
                        // 7. Prime the stream with some bits
                        inflatePrime(&strm_in, 8, 0xFF);
                    }
                }
            }
            
            size_t have = sizeof(out_buf) - strm_in.avail_out;
            if (have > 0) {
                fwrite(out_buf, 1, have, out_file);
            }
        } while (strm_in.avail_out == 0);
    }
    
    // 8. Initialize deflate stream for copying
    if (deflateInit(&strm_out, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // 9. Copy the deflate stream
        int copy_ret = deflateCopy(&strm_copy, &strm_out);
        if (copy_ret == Z_OK) {
	int fuzz_fd_0 = fuzz_fileno(out_file);

            // Write a single character using gzputc
            gzFile gz_out = gzdopen(fuzz_fd_0, "wb");
            if (gz_out) {
                gzputc(gz_out, 'X');
                // 10. Get error information
                int errnum = 0;
                const char *err_msg = gzerror(gz_out, &errnum);
                // 11. Close write file properly
                gzclose_w(gz_out);
            } else {
                assert_file_closed(&out_file);;
            }
            
            // Clean up copied stream
            deflateEnd(&strm_copy);
        }
        deflateEnd(&strm_out);
    }
    
    // 12. Clean up
    inflateEnd(&strm_in);
    gzclose(gz_in);
    remove("tmp_compressed.z");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&tmp_file);
	assert_file_name_closed("output_file");
	return 0;
}