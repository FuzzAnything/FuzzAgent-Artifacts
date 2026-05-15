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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 130, nr_unique_branch: 9
//<Quality> {"density":26,"unique_branches":{"gz_write":[[245,17,245,26,0,0,4,0],[246,24,246,36,0,0,4,1]],"gz_comp":[[116,21,116,29,0,0,4,0],[117,25,117,40,0,0,4,1],[117,44,117,64,0,0,4,1]],"gzfwrite":[[291,37,291,55,0,0,4,0],[291,59,291,72,0,0,4,0]],"gzclose_w":[[685,9,685,39,0,0,4,0],[696,9,696,31,0,0,4,0]]},"library_calls":["zlibCompileFlags","compressBound","compress2","zError","deflateInit_","deflateBound","deflateBound_z","deflateParams","deflate","deflateReset","deflateResetKeep","deflateEnd","inflateInit_","inflate","inflateMark","inflateReset","inflateEnd","adler32","crc32","adler32_z","crc32_z","adler32_combine","crc32_combine","gzdopen","gzsetparams","gzread","gztell64","gzclose","gzdopen","gzwrite","gzfwrite","gzclose_w"],"critical_calls":["zlibCompileFlags","compressBound","compress2","zError","deflateInit_","deflateBound","deflateBound_z","deflateParams","deflate","deflateReset","deflateResetKeep","deflateEnd","inflateInit_","inflate","inflateMark","inflateReset","inflateEnd","adler32","crc32","adler32_z","crc32_z","adler32_combine","crc32_combine","gzdopen","gzsetparams","gzread","gztell64","gzclose","gzdopen","gzwrite","gzfwrite","gzclose_w"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Return early if input is too small for meaningful processing
    if (size < 4) {
	return 0;
	}
	
    
    // Variables for tracking resources
    z_stream deflate_strm = {0};
    z_stream inflate_strm = {0};
    gzFile gz_file = nullptr;
    FILE *tmp_file = nullptr;
    int fd = -1;
    
    // 1. Get zlib compile flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Prevent unused variable warning
    
    // 2. Calculate maximum compressed size for original data
    uLong max_compressed = compressBound((uLong)size);
    
    // 3. Allocate buffers for compression/decompression
    Bytef *compressed = (Bytef*)malloc(max_compressed);
    Bytef *decompressed = (Bytef*)malloc(size);
    uLongf compressed_len = max_compressed;
    uLongf decompressed_len = size;
    
    if (!compressed || !decompressed) {
        free(compressed);
        free(decompressed);
        return 0;
    }
    
    // 4. Compress input data using compress2
    int compress_result = compress2(compressed, &compressed_len, 
                                   (const Bytef*)data, (uLong)size, 
                                   Z_DEFAULT_COMPRESSION);
    
    if (compress_result != Z_OK) {
        const char *err_str = zError(compress_result);
        (void)err_str; // Use error string
    }
    
    // 5. Initialize deflate stream
    int deflate_init = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, 
                                   ZLIB_VERSION, sizeof(z_stream));
    
    if (deflate_init == Z_OK) {
        // 6. Calculate deflate bound
        uLong deflate_bound = deflateBound(&deflate_strm, (uLong)size);
        
        // 7. Calculate deflate bound with z_size_t
        z_size_t source_len_z = (z_size_t)size;
        z_size_t deflate_bound_z = deflateBound_z(&deflate_strm, source_len_z);
        (void)deflate_bound_z; // Prevent unused warning
        
        // 8. Set deflate parameters
        deflateParams(&deflate_strm, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
        
        // 9. Perform deflation
        deflate_strm.next_in = (Bytef*)data;
        deflate_strm.avail_in = (uInt)size;
        deflate_strm.next_out = compressed;
        deflate_strm.avail_out = (uInt)max_compressed;
        
        int deflate_result = deflate(&deflate_strm, Z_FINISH);
        (void)deflate_result; // Use result
        
        // 10. Reset deflate stream
        deflateReset(&deflate_strm);
        
        // 11. Reset keep deflate stream
        deflateResetKeep(&deflate_strm);
        
        // 12. End deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // 13. Initialize inflate stream
    int inflate_init = inflateInit_(&inflate_strm, ZLIB_VERSION, sizeof(z_stream));
    
    if (inflate_init == Z_OK) {
        // 14. Perform inflation
        inflate_strm.next_in = compressed;
        inflate_strm.avail_in = (uInt)compressed_len;
        inflate_strm.next_out = decompressed;
        inflate_strm.avail_out = (uInt)size;
        
        int inflate_result = inflate(&inflate_strm, Z_FINISH);
        (void)inflate_result; // Use result
        
        // 15. Get inflate mark
        long mark = inflateMark(&inflate_strm);
        (void)mark; // Prevent unused warning
        
        // 16. Reset inflate stream
        inflateReset(&inflate_strm);
        
        // 17. End inflate stream
        inflateEnd(&inflate_strm);
    }
    
    // 18. Calculate checksums
    uLong adler = adler32(0L, (const Bytef*)data, (uInt)size);
    uLong crc = crc32(0L, (const Bytef*)data, (uInt)size);
    
    // 19. Calculate checksums with z_size_t
    uLong adler_z = adler32_z(0L, (const Bytef*)data, (z_size_t)size);
    uLong crc_z = crc32_z(0L, (const Bytef*)data, (z_size_t)size);
    (void)adler_z; // Prevent unused warnings
    (void)crc_z;
    
    // 20. Combine checksums (using dummy values for demonstration)
    uLong combined_adler = adler32_combine(adler, crc, 100);
    uLong combined_crc = crc32_combine(crc, adler, 100);
    (void)combined_adler;
    (void)combined_crc;
    
    // 21. Create temporary file for gz operations
    tmp_file = fmemopen((void*)data, size, "rb");
    if (tmp_file) {
        fd = fuzz_fileno(tmp_file);
        
        // 22. Open gz file from file descriptor
        gz_file = gzdopen(fd, "rb");
        if (gz_file) {
            // 23. Set gz parameters
            gzsetparams(gz_file, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
            
            // 24. Read from gz file
            Bytef *read_buf = (Bytef*)malloc(size);
            if (read_buf) {
                unsigned int read_len = gzread(gz_file, read_buf, (unsigned int)size);
                (void)read_len; // Use read length
                
                // 25. Get gz file position
                off64_t pos = gztell64(gz_file);
                (void)pos; // Prevent unused warning
                
                free(read_buf);
            }
            
            // 26. Close gz file for reading
            gzclose(gz_file);
            gz_file = nullptr;
        }
        
        // 27. Re-open for writing
        gz_file = gzdopen(fd, "wb");
        if (gz_file) {
            // 28. Write to gz file
            unsigned int write_len = gzwrite(gz_file, (voidpc)data, (unsigned int)size);
            (void)write_len; // Use write length
            
            // 29. Write using gzfwrite
            z_size_t items_written = gzfwrite((voidpc)data, 1, (z_size_t)size, gz_file);
            (void)items_written; // Prevent unused warning
            
            // 30. Close gz file for writing
            gzclose_w(gz_file);
        }
        
        assert_file_closed(&tmp_file);;
    }
    
    // 31. Clean up allocated resources
    free(compressed);
    free(decompressed);
    
    assert_file_closed(&tmp_file);
	return 0;
}