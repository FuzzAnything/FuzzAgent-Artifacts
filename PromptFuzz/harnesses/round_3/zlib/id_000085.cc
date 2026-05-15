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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 12
//<Quality> {"density":14,"unique_branches":{"inflateBack":[[563,13,563,62,0,0,4,0],[564,13,564,32,0,0,4,0],[564,13,564,32,0,0,4,1],[157,17,157,41,16,0,4,0],[157,17,157,41,51,0,4,0],[157,17,157,41,64,0,4,0],[103,17,103,26,77,0,4,1],[103,17,103,26,90,0,4,1],[103,17,103,26,91,0,4,1],[103,17,103,26,92,0,4,1],[103,17,103,26,95,0,4,1],[103,17,103,26,96,0,4,1]]},"library_calls":["zlibVersion","zlibCompileFlags","gzdopen","gzdopen","gzclose","gzclose","inflateInit2_","gzclose","gzclose","deflateInit2_","inflateEnd","gzclose","gzclose","inflateBackInit_","inflateBack","inflateBackEnd","gzungetc","gzread","inflateValidate","inflate","inflateCodesUsed","compressBound_z","compress2_z","gzwrite","gzerror","deflatePrime","deflatePending","inflateEnd","deflateEnd","gzclose","gzclose"],"critical_calls":["zlibVersion","zlibCompileFlags","gzdopen","gzdopen","inflateInit2_","deflateInit2_","inflateBackInit_","inflateBack","inflateBackEnd","gzungetc","gzread","inflateValidate","inflate","inflateCodesUsed","compressBound_z","compress2_z","gzwrite","gzerror","deflatePrime","deflatePending","inflateEnd","deflateEnd","gzclose","gzclose"],"visited":0}
/**/


// Callback functions for inflateBack
static unsigned char  id_000085_in_buf[1024];
static unsigned char  id_000085_out_buf[1024];

static unsigned  id_000085_in(void* desc, unsigned char** buf) {
    static size_t consumed = 0;
    FILE* file = (FILE*)desc;
    size_t n = fread( id_000085_in_buf, 1, sizeof( id_000085_in_buf), file);
    *buf =  id_000085_in_buf;
    consumed += n;
    return (unsigned)n;
}

static int  id_000085_out(void* desc, unsigned char* buf, unsigned len) {
    FILE* file = (FILE*)desc;
    return (int)fwrite(buf, 1, len, file);
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}




    // 1. Get zlib version and compile flags
    const char* version = zlibVersion();
    uLong flags = zlibCompileFlags();
    
    // 2. Create FILE* from input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create output FILE* for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 5. Create gzFile handles
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    gzFile gz_out = gzdopen(fd_out, fuzz_str_2);
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
    
    // 6. Initialize z_stream for decompression
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    int ret = inflateInit2_(&strm_inflate, fuzz_int32_t_3, version, sizeof(z_stream));
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
    
    // 7. Initialize z_stream for compression
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    ret = deflateInit2_(&strm_deflate, fuzz_int32_t_4, fuzz_int32_t_5,
                       fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8, version, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
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
    
    // 8. Initialize inflateBack stream
    z_stream strm_back;
    memset(&strm_back, 0, sizeof(z_stream));
    unsigned char window[32768];
    ret = inflateBackInit_(&strm_back, MAX_WBITS, window, version, sizeof(z_stream));
    if (ret == Z_OK) {
        // Reset input file position for inflateBack
        fseek(in_file, 0, SEEK_SET);
        ret = inflateBack(&strm_back,  id_000085_in, in_file,  id_000085_out, out_file);
        inflateBackEnd(&strm_back);
    }
    
    // 9. Process data through gzFile interface
    fseek(in_file, 0, SEEK_SET);
    unsigned char buf[4096];
    int bytes_read;
    
    // Read using gzgetc and gzungetc
    int c = gzgetc(gz_in);
    if (c != -1) {
        gzungetc(c, gz_in);
    }
    
    // Read  id_000085_in chunks
    while ((bytes_read = gzread(gz_in, buf, sizeof(buf))) > 0) {
        // Validate during decompression
        inflateValidate(&strm_inflate, fuzz_int32_t_9);
        
        // Decompress the chunk
        strm_inflate.next_in = buf;
        strm_inflate.avail_in = bytes_read;
        
        unsigned char decomp_buf[8192];
        do {
            strm_inflate.next_out = decomp_buf;
            strm_inflate.avail_out = sizeof(decomp_buf);
            ret = inflate(&strm_inflate, fuzz_int32_t_10);
            
            // Check codes used
            unsigned long codes_used = inflateCodesUsed(&strm_inflate);
            
            // Compress the decompressed data
            if (ret == Z_OK || ret == Z_STREAM_END) {
                size_t decomp_len = sizeof(decomp_buf) - strm_inflate.avail_out;
                if (decomp_len > 0) {
                    // Use compressBound_z to get bound
                    z_size_t bound = compressBound_z(decomp_len);
                    
                    // Use compress2_z
                    Bytef comp_buf[8192];
                    z_size_t comp_len = sizeof(comp_buf);
                    compress2_z(comp_buf, &comp_len, decomp_buf, decomp_len, fuzz_int32_t_11);
                    
                    // Write compressed data to gzFile
                    gzwrite(gz_out, comp_buf, (unsigned)comp_len);
                }
            }
        } while (strm_inflate.avail_out == 0);
    }
    
    // 10. Get error from gzFile
    int errnum;
    const char* error_msg = gzerror(gz_in, &errnum);
    
    // 11. Use deflatePrime and deflatePending
    deflatePrime(&strm_deflate, fuzz_int32_t_12, fuzz_int32_t_13);
    unsigned int pending;
    int bits;
    deflatePending(&strm_deflate, &pending, &bits);
    
    // 12. Clean up
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    gzclose(gz_in);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 13. Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}