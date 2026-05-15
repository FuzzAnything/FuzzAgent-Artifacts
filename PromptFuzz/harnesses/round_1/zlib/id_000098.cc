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
//<ID> 322
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 4
//<Quality> {"density":26,"unique_branches":{"deflate_huff":[[2178,9,2178,26,0,0,4,1],[2182,9,2182,20,0,0,4,0],[1644,8,1644,31,6,0,4,1],[1631,24,1631,44,10,0,4,0]]},"library_calls":["zlibVersion","gzdopen","gzdopen","gzclose","gzclose","gzsetparams","deflateInit2_","gzclose","gzclose","inflateInit2_","deflateEnd","gzclose","gzclose","deflateSetDictionary","inflateSetDictionary","deflateGetDictionary","deflateBound_z","compressBound","inflateEnd","deflateEnd","gzclose","gzclose","deflate","deflatePending","deflateResetKeep","deflateParams","deflate","deflateEnd","inflate","inflateEnd","gzclose","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzdopen","gzsetparams","deflateInit2_","inflateInit2_","deflateSetDictionary","inflateSetDictionary","deflateGetDictionary","deflateBound_z","compressBound","deflate","deflatePending","deflateResetKeep","deflateParams","deflate","deflateEnd","inflate","inflateEnd","gzclose","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to create a scenario where compressed data is processed with dictionary support and parameter tuning:

*/


extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for very small inputs
    if (size < 100) {
	return 0;
	}
	
    
    // Split input data for different purposes
    size_t dict_size = size / 4;
    size_t compress_size = size / 2;
    size_t remaining = size - dict_size - compress_size;
    
    if (dict_size == 0 || compress_size == 0 || remaining == 0) {
	return 0;
	}
	
    
    const Bytef* dict_data = data;
    const Bytef* compress_data = data + dict_size;
    const Bytef* remaining_data = data + dict_size + compress_size;
    
    // Create temporary files for input/output
    FILE *in_file = fmemopen((void *)compress_data, compress_size, "rb");
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
    
    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Version string
    const char* version = zlibVersion();
    
    // Event: Process compressed data with dictionary support and parameter tuning
    // This simulates a compression/decompression pipeline with dynamic parameter adjustment
    
    // 1. Create gzFile handles for input/output
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 2. Set parameters for gzFile output
    int level = Z_DEFAULT_COMPRESSION;
    int strategy = Z_DEFAULT_STRATEGY;
    if (remaining_data[0] % 3 == 0) {
        level = Z_BEST_COMPRESSION;
        strategy = Z_FILTERED;
    } else if (remaining_data[0] % 3 == 1) {
        level = Z_BEST_SPEED;
        strategy = Z_HUFFMAN_ONLY;
    }
    
    gzsetparams(gz_out, level, strategy);
    
    // 3. Initialize deflate stream for compression
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    int windowBits = 15; // 32K window
    int memLevel = 8;    // Default memory level
    int method = Z_DEFLATED;
    
    int ret = deflateInit2_(&def_strm, level, method, windowBits, 
                           memLevel, strategy, version, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Initialize inflate stream for decompression
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    
    ret = inflateInit2_(&inf_strm, windowBits, version, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&def_strm);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Set dictionary for both compression and decompression
    if (dict_size > 0) {
        deflateSetDictionary(&def_strm, dict_data, (uInt)dict_size);
        inflateSetDictionary(&inf_strm, dict_data, (uInt)dict_size);
    }
    
    // 6. Get dictionary back (verification)
    Bytef* retrieved_dict = (Bytef*)malloc(dict_size);
    uInt retrieved_len = dict_size;
    if (retrieved_dict) {
        deflateGetDictionary(&def_strm, retrieved_dict, &retrieved_len);
        free(retrieved_dict);
    }
    
    // 7. Calculate compression bounds
    z_size_t source_len = compress_size;
    z_size_t bound_z = deflateBound_z(&def_strm, source_len);
    uLong bound = compressBound((uLong)source_len);
    
    // Use the bounds to allocate output buffers
    size_t out_buf_size = (size_t)bound_z > (size_t)bound ? (size_t)bound_z : (size_t)bound;
    Bytef* out_buf = (Bytef*)malloc(out_buf_size);
    if (!out_buf) {
        inflateEnd(&inf_strm);
        deflateEnd(&def_strm);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 8. Perform compression
    def_strm.next_in = (Bytef*)compress_data;
    def_strm.avail_in = (uInt)compress_size;
    def_strm.next_out = out_buf;
    def_strm.avail_out = (uInt)out_buf_size;
    
    int flush = Z_FINISH;
    if (remaining_data[1] % 2 == 0) {
        flush = Z_SYNC_FLUSH;
    }
    
    ret = deflate(&def_strm, flush);
    
    // 9. Check for pending data
    unsigned int pending;
    int bits;
    deflatePending(&def_strm, &pending, &bits);
    
    // 10. Reset deflate stream keeping internal state
    deflateResetKeep(&def_strm);
    
    // 11. Try compression again with different parameters
    if (remaining_data[2] % 2 == 0) {
        deflateParams(&def_strm, Z_BEST_SPEED, Z_DEFAULT_STRATEGY);
    }
    
    def_strm.next_in = (Bytef*)remaining_data;
    def_strm.avail_in = (uInt)(remaining < 1000 ? remaining : 1000);
    def_strm.next_out = out_buf;
    def_strm.avail_out = (uInt)out_buf_size;
    
    ret = deflate(&def_strm, Z_FINISH);
    
    // 12. Clean up compression
    deflateEnd(&def_strm);
    
    // 13. Test inflate with the compressed data
    inf_strm.next_in = out_buf;
    inf_strm.avail_in = (uInt)(out_buf_size < def_strm.total_out ? out_buf_size : def_strm.total_out);
    
    Bytef* inflate_buf = (Bytef*)malloc(compress_size * 2);
    if (inflate_buf) {
        inf_strm.next_out = inflate_buf;
        inf_strm.avail_out = (uInt)(compress_size * 2);
        
        ret = inflate(&inf_strm, Z_FINISH);
        
        free(inflate_buf);
    }
    
    // 14. Clean up inflation
    inflateEnd(&inf_strm);
    
    // 15. Close gzFiles
    gzclose(gz_in);
    gzclose(gz_out);
    
    // Clean up
    free(out_buf);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}