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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"deflateUsed":[[738,9,738,32,0,0,4,0]]},"library_calls":["gzdopen","gzsetparams","inflateInit2_","gzclose","gzread","inflateMark","inflate","crc32","adler32","adler32","adler32_combine","compress2","crc32","crc32_combine64","inflateResetKeep","inflate","deflateUsed","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzsetparams","inflateInit2_","gzread","inflateMark","inflate","crc32","adler32","adler32","adler32_combine","compress2","crc32","crc32_combine64","inflateResetKeep","inflate","deflateUsed","inflateEnd","gzclose"],"visited":0}
/**/


// Helper function to create a temporary file with the input data
static FILE*  id_000051_create_temp_input_file(const uint8_t* data, size_t size) {
    return fmemopen((void*)data, size, "rb");
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<109) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimal data

    // 1. Create temporary input file from fuzzer data
    FILE* in_file =  id_000051_create_temp_input_file(data, size);
    if (!in_file) {
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_fd_closed(fd);
	return 0;
    }

    // 2. Open compressed stream using gzdopen
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_fd_closed(fd);
	return 0;
    }

    // 3. Set compression parameters (for reading)
    gzsetparams(gz_in, fuzz_int32_t_2, fuzz_int32_t_3);

    // 4. Initialize inflate stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Use inflateInit2_ with negative windowBits for raw decompression
    int ret = inflateInit2_(&strm, fuzz_int32_t_5, fuzz_str_4, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_fd_closed(fd);
	return 0;
    }

    // 5. Read from gzFile and decompress
    Bytef in_buf[4096];
    Bytef out_buf[8192];
    uLongf dest_len = sizeof(out_buf);
    uLong crc_val = 0;
    uLong adler1 = 0, adler2 = 0;
    long mark = 0;
    
    // Read some data from gzFile
    int bytes_read = gzread(gz_in, in_buf, sizeof(in_buf));
    
    if (bytes_read > 0) {
        // Set input for inflate
        strm.next_in = in_buf;
        strm.avail_in = bytes_read;
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        
        // Mark current position
        mark = inflateMark(&strm);
        
        // Decompress
        ret = inflate(&strm, fuzz_int32_t_6);
        
        // Get decompressed data length
        uLong decompressed_len = sizeof(out_buf) - strm.avail_out;
        
        if (decompressed_len > 0) {
            // 6. Compute CRC32 of decompressed data
            crc_val = crc32(fuzz_uint64_t_7, out_buf, decompressed_len);
            
            // 7. Split data for Adler32 combine demo
            size_t half = decompressed_len / 2;
            if (half > 0) {
                adler1 = adler32(fuzz_uint64_t_8, out_buf, half);
                adler2 = adler32(fuzz_uint64_t_9, out_buf + half, decompressed_len - half);
                
                // Combine Adler32 values
                uLong combined_adler = adler32_combine(fuzz_uint64_t_10, fuzz_uint64_t_11, decompressed_len);
                (void)combined_adler; // Prevent unused warning
            }
            
            // 8. Recompress the decompressed data
            Bytef recompressed[16384];
            uLongf recomp_len = sizeof(recompressed);
            ret = compress2(recompressed, &recomp_len, out_buf, decompressed_len, 
                           fuzz_int32_t_12);
            
            // 9. Combine CRC32 values (demo with arbitrary values)
            uLong crc2 = crc32(fuzz_uint64_t_14, fuzz_str_13, static_cast<uInt>(fuzz_str_sz_13));
            uLong combined_crc = crc32_combine64(fuzz_uint64_t_15, crc2, 100);
            (void)combined_crc; // Prevent unused warning
        }
        
        // 10. Reset inflate stream keeping window
        if (mark != -1) {
            inflateResetKeep(&strm);
            
            // Try decompressing again
            strm.next_in = in_buf;
            strm.avail_in = bytes_read;
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);
            inflate(&strm, fuzz_int32_t_16);
        }
    }

    // 11. Check deflate usage (for completeness, though we're using inflate)
    int bits_used = 0;
    deflateUsed(&strm, &bits_used);

    // 12. Clean up
    inflateEnd(&strm);
    gzclose(gz_in);
    
    // Note: gzclose already closes the underlying fd, don't double-close
    assert_file_closed(&in_file);;

    assert_fd_closed(fd);
	return 0;
}