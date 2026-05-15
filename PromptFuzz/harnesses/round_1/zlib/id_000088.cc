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
//<ID> 253
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["gzdopen","gzerror","gzdopen","gzclose","inflateBackInit_","gzclose","gzclose","inflateBack","inflateBackEnd","gzrewind","crc32","crc32","gzread","crc32","gzfwrite","gzseek64","gzseek","gzread","crc32","crc32_combine_gen","crc32_combine_op","deflateInit_","deflateReset","deflateBound","deflatePrime","compressBound_z","compress_z","compressBound_z","deflateEnd","gzclose","gzclose","inflateInit_","inflateReset","inflateEnd"],"critical_calls":["gzdopen","gzerror","gzdopen","inflateBackInit_","inflateBack","inflateBackEnd","gzrewind","crc32","crc32","gzread","crc32","gzfwrite","gzseek64","gzseek","gzread","crc32","crc32_combine_gen","crc32_combine_op","deflateInit_","deflateReset","deflateBound","deflatePrime","compressBound_z","compress_z","compressBound_z","deflateEnd","gzclose","gzclose","inflateInit_","inflateReset","inflateEnd"],"visited":1}
/*I'll create a comprehensive fuzz driver that uses the specified zlib APIs to implement a complex decompression and recompression workflow.

*/


// Include zlib headers

// Structure for inflateBack callbacks
struct BackStreamData {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Input function for inflateBack
unsigned int  id_000088_in_func_callback(void* desc, unsigned char** buf) {
    BackStreamData* bs = (BackStreamData*)desc;
    
    if (bs->pos >= bs->size) {
        return 0; // No more data
    }
    
    *buf = (unsigned char*)(bs->data + bs->pos);
    size_t available = bs->size - bs->pos;
    
    // Return amount to read (at most 4096 bytes at a time)
    size_t to_read = std::min(available, (size_t)4096);
    bs->pos += to_read;
    
    return (unsigned int)to_read;
}

// Output function for inflateBack
int  id_000088_out_func_callback(void* desc, unsigned char* buf, unsigned int length) {
    // Just consume the output - we'll compute CRC32
    BackStreamData* bs = (BackStreamData*)desc;
    // We can compute CRC here if needed, but for now just return success
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        // Need enough data for meaningful fuzzing
        return 0;
    }

    // 1. Create FILE* variables for input and output
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
    
    // 2. Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 3. Create gzFile using gzdopen
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Use gzerror to check for errors
    int errnum;
    const char* error_msg = gzerror(gz_in, &errnum);
    (void)error_msg; // Suppress unused variable warning
    
    // 5. Create gzFile for output
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Buffer for reading/writing
    std::vector<Bytef> buffer(4096);
    
    // 7. Initialize z_stream for inflateBack
    z_stream strm_inflate_back;
    memset(&strm_inflate_back, 0, sizeof(z_stream));
    
    // 8. Initialize inflateBack with custom window
    unsigned char window[32768]; // 32KB window
    int ret = inflateBackInit_(&strm_inflate_back, 15, window, 
                              ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 9. Set up data for inflateBack callbacks
    BackStreamData back_data;
    back_data.data = data;
    back_data.size = size;
    back_data.pos = 0;
    
    // 10. Perform inflateBack
    ret = inflateBack(&strm_inflate_back,  id_000088_in_func_callback, &back_data,
                      id_000088_out_func_callback, &back_data);
    
    // 11. End inflateBack
    inflateBackEnd(&strm_inflate_back);
    
    // 12. Reset position in input file
    rewind(in_file);
    
    // 13. Use gzrewind
    gzrewind(gz_in);
    
    // 14. Read some data with gzFile
    int bytes_read;
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    // Read first chunk and compute CRC
    bytes_read = gzread(gz_in, buffer.data(), (unsigned int)std::min(buffer.size(), size/2));
    if (bytes_read > 0) {
        crc1 = crc32(crc1, buffer.data(), (uInt)bytes_read);
        
        // Write to output gzFile using gzfwrite
        z_size_t items_written = gzfwrite(buffer.data(), (z_size_t)bytes_read, 1, gz_out);
        (void)items_written; // Suppress unused warning
    }
    
    // 15. Use gzseek64 and gzseek
    if (size > 100) {
        // Seek to different positions
        off64_t pos64 = gzseek64(gz_in, 50, SEEK_SET);
        (void)pos64;
        
        off_t pos = gzseek(gz_in, 0, SEEK_CUR);
        (void)pos;
    }
    
    // Read second chunk and compute CRC
    bytes_read = gzread(gz_in, buffer.data(), (unsigned int)std::min(buffer.size(), size/2));
    if (bytes_read > 0) {
        crc2 = crc32(crc2, buffer.data(), (uInt)bytes_read);
    }
    
    // 16. Combine CRC values using crc32_combine_gen and crc32_combine_op
    uLong combined_crc = 0;
    if (crc1 != 0 && crc2 != 0) {
        off_t len = (off_t)bytes_read;
        uLong op = crc32_combine_gen(len);
        combined_crc = crc32_combine_op(crc1, crc2, op);
    }
    
    // 17. Now test deflate functions with the data we have
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    // 18. Initialize deflate
    ret = deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION, 
                      ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // 19. Reset deflate stream
        deflateReset(&strm_deflate);
        
        // 20. Compute deflateBound
        uLong bound = deflateBound(&strm_deflate, (uLong)size);
        (void)bound;
        
        // 21. Use deflatePrime (add some bits)
        if (size > 10) {
            deflatePrime(&strm_deflate, 4, data[0] & 0x0F);
        }
        
        // 22. Prepare buffers for compression
        std::vector<Bytef> compressed_buffer(compressBound_z((z_size_t)size));
        z_size_t compressed_len = compressed_buffer.size();
        
        // 23. Use compress_z
        ret = compress_z(compressed_buffer.data(), &compressed_len, 
                        data, (z_size_t)std::min(size, (size_t)1024));
        
        // 24. Use compressBound_z
        z_size_t bound_z = compressBound_z((z_size_t)size);
        (void)bound_z;
        
        // 25. Clean up deflate
        deflateEnd(&strm_deflate);
    }
    
    // 26. Clean up - close all resources
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    // 27. Reset inflate stream (even though we're not using it directly)
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    inflateInit_(&strm_inflate, ZLIB_VERSION, (int)sizeof(z_stream));
    inflateReset(&strm_inflate);
    inflateEnd(&strm_inflate);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}