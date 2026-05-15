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
//<ID> 191
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"inflate":[[495,10,495,33,0,0,4,0],[495,37,495,56,0,0,4,1]]},"library_calls":["gzdopen","gzdirect","gzerror","gzseek64","gzoffset","gzungetc","inflateInit2_","gzclose","inflateGetHeader","inflatePrime","inflateSetDictionary","inflate","deflateInit2_","deflateBound","deflate","gzeof","deflateEnd","inflateEnd","inflateInit2_","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gzerror","gzseek64","gzoffset","gzungetc","inflateInit2_","inflateGetHeader","inflatePrime","inflateSetDictionary","inflate","deflateInit2_","deflateBound","deflate","gzeof","deflateEnd","inflateEnd","inflateInit2_","inflateEnd","gzclose"],"visited":0}
/*Here's a comprehensive fuzz driver that uses all the specified zlib APIs to test the scenario of reading and processing compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t* f_data, size_t f_size) {
	if(f_size<92) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful testing

    // Create temporary files for reading and writing
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

    int fd = fuzz_fileno(in_file);
    if (fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 1. Open compressed file using gzdopen
    gzFile gzfile = gzdopen(fd, fuzz_str_1);
    if (gzfile == Z_NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Check if file is in gzip format
    int direct = gzdirect(gzfile);

    // 3. Check for errors
    int errnum;
    const char *errmsg = gzerror(gzfile, &errnum);

    // 4. Seek in the compressed file
    off64_t seek_pos = gzseek64(gzfile, size/2, fuzz_int32_t_2);
    
    // 5. Get current offset
    off_t offset = gzoffset(gzfile);

    // 6. Push a byte back
    int c = gzgetc(gzfile);
    if (c != -1) {
        gzungetc(c, gzfile);
    }

    // 7. Initialize inflate stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int ret = inflateInit2_(&strm, MAX_WBITS + 16, fuzz_str_3, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 8. Set up header for gzip extraction
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 0;
    header.time = 0;
    header.xflags = 0;
    header.os = 0;
    header.extra = Z_NULL;
    header.extra_len = 0;
    header.name = Z_NULL;
    header.comment = Z_NULL;
    header.hcrc = 0;
    header.done = 0;
    
    inflateGetHeader(&strm, &header);

    // 9. Prime inflate stream
    if (size > 0) {
        inflatePrime(&strm, fuzz_int32_t_4, data[0]);
    }

    // 10. Set dictionary for inflation
    uInt dictSize = std::min<uInt>(size, 256);
    if (dictSize > 0) {
        inflateSetDictionary(&strm, data, dictSize);
    }

    // Prepare buffers for inflation
    std::vector<Bytef> in_buffer(std::min<size_t>(size, 4096));
    std::vector<Bytef> out_buffer(4096);
    
    size_t bytes_processed = 0;
    int flush = Z_NO_FLUSH;

    // 11. Inflate (decompress) the data
    do {
        // Read from gzfile for inflation input
        size_t to_read = std::min(in_buffer.size(), size - bytes_processed);
        if (to_read > 0) {
            // Use the original data for inflation
            memcpy(in_buffer.data(), data + bytes_processed, to_read);
            strm.avail_in = static_cast<uInt>(to_read);
            strm.next_in = in_buffer.data();
            bytes_processed += to_read;
        } else {
            strm.avail_in = 0;
            strm.next_in = Z_NULL;
        }

        if (bytes_processed >= size) {
            flush = Z_FINISH;
        }

        do {
            strm.avail_out = static_cast<uInt>(out_buffer.size());
            strm.next_out = out_buffer.data();
            
            ret = inflate(&strm, fuzz_int32_t_5);
            
            if (ret == Z_STREAM_ERROR || ret == Z_NEED_DICT || ret == Z_DATA_ERROR || 
                ret == Z_MEM_ERROR) {
                break;
            }
            
            uInt have = static_cast<uInt>(out_buffer.size()) - strm.avail_out;
            if (have > 0) {
                fwrite(out_buffer.data(), 1, have, out_file);
            }
        } while (strm.avail_out == 0);

        if (ret == Z_STREAM_END) break;
    } while (strm.avail_in != 0);

    // 12. Initialize deflate stream for compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    ret = deflateInit2_(&deflate_strm, fuzz_int32_t_7, fuzz_int32_t_8,
                       MAX_WBITS + 16, fuzz_int32_t_9, fuzz_int32_t_10,
                       fuzz_str_6, sizeof(z_stream));
    if (ret == Z_OK) {
        // 13. Compute bound for compressed size
        uLong bound = deflateBound(&deflate_strm, strm.total_out);
        
        // Compress the decompressed data
        std::vector<Bytef> deflate_out(bound);
        deflate_strm.avail_in = static_cast<uInt>(strm.total_out);
        deflate_strm.next_in = out_buffer.data();
        deflate_strm.avail_out = static_cast<uInt>(deflate_out.size());
        deflate_strm.next_out = deflate_out.data();
        
        ret = deflate(&deflate_strm, fuzz_int32_t_11);
        
        // 14. Check for end of file in gzfile
        int eof_check = gzeof(gzfile);
        
        // 15. Clean up deflate stream
        deflateEnd(&deflate_strm);
    }

    // 16. Clean up inflate stream
    inflateEnd(&strm);

    // 17. Clean up inflate stream again (as per API list, though redundant)
    // Note: Actually calling inflateEnd twice is wrong, but keeping as per requirement
    // We'll just reset and reinitialize to call inflateEnd again
    memset(&strm, 0, sizeof(strm));
    inflateInit2_(&strm, fuzz_int32_t_13, fuzz_str_12, sizeof(z_stream));
    inflateEnd(&strm);

    // 18. Close gzfile
    gzclose(gzfile);

    // Clean up file handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}