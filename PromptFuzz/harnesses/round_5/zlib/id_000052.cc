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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 166
//<Quality> {"density":9,"unique_branches":{"inflateBack":[[228,9,228,18,0,0,4,1],[230,17,230,28,0,0,4,0],[239,13,239,19,0,0,4,0],[239,13,239,19,0,0,4,1],[244,13,244,19,0,0,4,0],[244,13,244,19,0,0,4,1],[250,13,250,19,0,0,4,0],[250,13,250,19,0,0,4,1],[255,13,255,20,0,0,4,0],[255,13,255,20,0,0,4,1],[262,9,262,20,0,0,4,0],[266,17,266,59,0,0,4,0],[266,17,266,59,0,0,4,1],[277,20,277,38,0,0,4,0],[277,20,277,38,0,0,4,1],[281,21,281,32,0,0,4,0],[282,21,282,32,0,0,4,0],[282,21,282,32,0,0,4,1],[294,9,294,19,0,0,4,0],[304,17,304,34,0,0,4,0],[304,17,304,34,0,0,4,1],[304,38,304,55,0,0,4,0],[304,38,304,55,0,0,4,1],[315,20,315,46,0,0,4,0],[315,20,315,46,0,0,4,1],[320,20,320,36,0,0,4,0],[320,20,320,36,0,0,4,1],[327,17,327,20,0,0,4,0],[327,17,327,20,0,0,4,1],[336,20,336,60,0,0,4,0],[336,20,336,60,0,0,4,1],[339,25,339,54,0,0,4,0],[339,25,339,54,0,0,4,1],[342,21,342,34,0,0,4,0],[342,21,342,34,0,0,4,1],[347,25,347,39,0,0,4,0],[347,25,347,39,0,0,4,1],[350,29,350,45,0,0,4,0],[350,29,350,45,0,0,4,1],[360,30,360,44,0,0,4,0],[360,30,360,44,0,0,4,1],[374,25,374,72,0,0,4,0],[374,25,374,72,0,0,4,1],[380,28,380,34,0,0,4,0],[380,28,380,34,0,0,4,1],[386,17,386,35,0,0,4,0],[386,17,386,35,0,0,4,1],[389,17,389,38,0,0,4,0],[389,17,389,38,0,0,4,1],[404,17,404,20,0,0,4,0],[404,17,404,20,0,0,4,1],[413,17,413,20,0,0,4,0],[422,9,422,17,0,0,4,0],[424,17,424,26,0,0,4,0],[424,17,424,26,0,0,4,1],[424,30,424,41,0,0,4,0],[424,30,424,41,0,0,4,1],[434,21,434,50,0,0,4,0],[434,21,434,50,0,0,4,1],[437,17,437,24,0,0,4,0],[437,17,437,24,0,0,4,1],[437,28,437,49,0,0,4,1],[451,17,451,29,0,0,4,0],[451,17,451,29,0,0,4,1],[463,17,463,29,0,0,4,0],[463,17,463,29,0,0,4,1],[470,17,470,29,0,0,4,0],[470,17,470,29,0,0,4,1],[478,17,478,34,0,0,4,0],[478,17,478,34,0,0,4,1],[488,21,488,50,0,0,4,0],[488,21,488,50,0,0,4,1],[491,17,491,38,0,0,4,1],[502,17,502,29,0,0,4,0],[502,17,502,29,0,0,4,1],[511,17,511,34,0,0,4,0],[511,17,511,34,0,0,4,1],[516,17,517,58,0,0,4,0],[516,17,517,58,0,0,4,1],[516,49,516,76,0,0,4,0],[528,21,528,32,0,0,4,1],[536,21,536,41,0,0,4,0],[536,21,536,41,0,0,4,1],[541,26,541,32,0,0,4,0],[541,26,541,32,0,0,4,1],[542,22,542,40,0,0,4,0],[542,22,542,40,0,0,4,1],[545,9,545,18,0,0,4,0],[550,9,550,17,0,0,4,0],[562,9,562,28,0,0,4,0],[563,13,563,62,0,0,4,0],[564,13,564,32,0,0,4,0],[564,13,564,32,0,0,4,1],[126,16,126,36,7,0,4,1],[126,16,126,36,13,0,4,0],[126,16,126,36,13,0,4,1],[101,13,101,22,15,0,4,0],[101,13,101,22,15,0,4,1],[103,17,103,26,15,0,4,0],[103,17,103,26,15,0,4,1],[153,13,153,22,16,0,4,0],[153,13,153,22,16,0,4,1],[157,17,157,41,16,0,4,0],[126,16,126,36,18,0,4,0],[126,16,126,36,18,0,4,1],[126,16,126,36,25,0,4,0],[126,16,126,36,25,0,4,1],[126,16,126,36,31,0,4,0],[126,16,126,36,31,0,4,1],[126,16,126,36,35,0,4,0],[126,16,126,36,35,0,4,1],[126,16,126,36,39,0,4,0],[126,16,126,36,39,0,4,1],[153,13,153,22,51,0,4,0],[153,13,153,22,51,0,4,1],[157,17,157,41,51,0,4,0],[126,16,126,36,52,0,4,0],[126,16,126,36,52,0,4,1],[126,16,126,36,61,0,4,0],[126,16,126,36,61,0,4,1],[153,13,153,22,64,0,4,0],[153,13,153,22,64,0,4,1],[157,17,157,41,64,0,4,0],[101,13,101,22,77,0,4,0],[101,13,101,22,77,0,4,1],[103,17,103,26,77,0,4,0],[103,17,103,26,77,0,4,1],[101,13,101,22,81,0,4,0],[101,13,101,22,81,0,4,1],[103,17,103,26,81,0,4,0],[103,17,103,26,81,0,4,1],[101,13,101,22,85,0,4,0],[101,13,101,22,85,0,4,1],[103,17,103,26,85,0,4,0],[103,17,103,26,85,0,4,1],[101,13,101,22,89,0,4,1],[103,17,103,26,89,0,4,1],[101,13,101,22,90,0,4,0],[101,13,101,22,90,0,4,1],[103,17,103,26,90,0,4,0],[103,17,103,26,90,0,4,1],[101,13,101,22,91,0,4,0],[101,13,101,22,91,0,4,1],[103,17,103,26,91,0,4,0],[103,17,103,26,91,0,4,1],[101,13,101,22,92,0,4,0],[101,13,101,22,92,0,4,1],[103,17,103,26,92,0,4,0],[103,17,103,26,92,0,4,1],[101,13,101,22,95,0,4,0],[101,13,101,22,95,0,4,1],[103,17,103,26,95,0,4,0],[101,13,101,22,96,0,4,0],[101,13,101,22,96,0,4,1],[103,17,103,26,96,0,4,0],[101,13,101,22,97,0,4,0],[101,13,101,22,97,0,4,1],[103,17,103,26,97,0,4,0],[101,13,101,22,102,0,4,0],[101,13,101,22,102,0,4,1],[103,17,103,26,102,0,4,0],[103,17,103,26,102,0,4,1],[101,13,101,22,107,0,4,0],[101,13,101,22,107,0,4,1],[103,17,103,26,107,0,4,0],[103,17,103,26,107,0,4,1]]},"library_calls":["zlibCompileFlags","gzdopen","deflateInit_","gzclose","deflateTune","deflateEnd","gzclose","deflateCopy","deflateEnd","gzclose","inflateBackInit_","deflateEnd","deflateEnd","gzclose","inflateBack","gzeof","inflateBackEnd","deflateEnd","deflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","deflateInit_","deflateTune","deflateCopy","inflateBackInit_","inflateBack","gzeof","inflateBackEnd","deflateEnd","deflateEnd","gzclose"],"visited":0}
/**/


// Callback function for inflateBack input
static unsigned int  id_000052_input_callback(void* desc, unsigned char** buf) {
    // We'll use the FILE* from fmemopen as our descriptor
    FILE* in_file = (FILE*)desc;
    static unsigned char local_buf[1024];
    
    size_t bytes_read = fread(local_buf, 1, sizeof(local_buf), in_file);
    *buf = local_buf;
    return (unsigned int)bytes_read;
}

// Callback function for inflateBack output
static int  id_000052_output_callback(void* desc, unsigned char* buf, unsigned int size) {
    // We'll use the FILE* for output as our descriptor
    FILE* out_file = (FILE*)desc;
    return (int)fwrite(buf, 1, size, out_file);
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;  // Need minimum data for meaningful fuzzing
    }

    // Create input and output files
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // 1. First, get zlib compile flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;  // Use the value to avoid unused variable warning

    // 2. Create a gzFile from the input file descriptor
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

    // 3. Create compression stream for deflate operations
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    // Initialize deflate with default parameters
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 4. Tune the deflate parameters using deflateTune
    // Use some values from the input data for tuning parameters
    int good_length = data[0] % 32 + 8;      // 8-40
    int max_lazy = data[1] % 32 + 8;         // 8-40
    int nice_length = data[2] % 128 + 32;    // 32-160
    int max_chain = data[3] % 256 + 64;      // 64-320
    
    if (deflateTune(&deflate_strm, good_length, max_lazy, nice_length, max_chain) != Z_OK) {
        deflateEnd(&deflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 5. Create another deflate stream and copy the first one
    z_stream deflate_strm_copy;
    memset(&deflate_strm_copy, 0, sizeof(z_stream));
    
    if (deflateCopy(&deflate_strm_copy, &deflate_strm) != Z_OK) {
        deflateEnd(&deflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 6. Create decompression stream for inflateBack
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    // Initialize inflate for back interface
    unsigned char window[1 << 15];  // 32KB window
    if (inflateBackInit(&inflate_strm, 15, window) != Z_OK) {
        deflateEnd(&deflate_strm_copy);
        deflateEnd(&deflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 7. Use inflateBack to decompress data
    // We need to rewind the input file for inflateBack
    fseek(in_file, 0, SEEK_SET);
    
    // The input data should be zlib compressed data for inflateBack
    // We'll try to decompress it
    int inflate_result = inflateBack(&inflate_strm,  id_000052_input_callback, in_file, 
                                      id_000052_output_callback, out_file);
    
    // Check if we reached end of file in gzFile
    int is_eof = gzeof(gz_in);
    (void)is_eof;  // Use the value to avoid unused variable warning

    // 8. Clean up all resources
    inflateBackEnd(&inflate_strm);
    deflateEnd(&deflate_strm_copy);
    deflateEnd(&deflate_strm);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove the output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}