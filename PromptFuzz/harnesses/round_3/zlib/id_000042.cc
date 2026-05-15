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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 14
//<Quality> {"density":9,"unique_branches":{"inflate":[[568,17,568,38,0,0,4,0],[577,17,577,38,0,0,4,0],[586,17,586,38,0,0,4,0],[599,21,599,42,0,0,4,0],[605,22,605,43,0,0,4,0],[614,25,614,46,0,0,4,0],[615,25,615,53,0,0,4,1],[639,25,639,46,0,0,4,0],[640,29,640,56,0,0,4,1],[650,22,650,43,0,0,4,0],[661,25,661,46,0,0,4,0],[662,29,662,59,0,0,4,1],[672,22,672,43,0,0,4,0],[686,17,686,38,0,0,4,0]]},"library_calls":["inflateInit2_","inflateGetHeader","inflateEnd","inflateEnd","inflate","crc32","crc32","crc32_combine_gen","inflateReset","deflateInit_","inflateEnd","deflateParams","gzdopen","deflateEnd","inflateEnd","gzfwrite","gztell64","gzclose","deflateEnd","inflateEnd"],"critical_calls":["inflateInit2_","inflateGetHeader","inflate","crc32","crc32","crc32_combine_gen","inflateReset","deflateInit_","deflateParams","gzdopen","gzfwrite","gztell64","gzclose","deflateEnd","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 32) {
	return 0;
	}
	

    // Create input FILE* from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output FILE* for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize decompression stream with gzip/zlib format auto-detection
    z_stream d_stream;
    memset(&d_stream, 0, sizeof(z_stream));
    
    // Initialize inflation with auto header detection (windowBits = 15 + 32)
    if (inflateInit2(&d_stream, 15 + 32) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Setup gzip header structure
    gz_header header;
    memset(&header, 0, sizeof(header));
    
    // Request header collection during inflation
    if (inflateGetHeader(&d_stream, &header) != Z_OK) {
        inflateEnd(&d_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Prepare buffers for decompression
    size_t out_buf_size = 65536;
    uint8_t *out_buf = (uint8_t *)malloc(out_buf_size);
    if (!out_buf) {
        inflateEnd(&d_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Decompress input data
    uint8_t *in_buf = (uint8_t *)data;
    size_t in_remaining = size;
    int ret = Z_OK;
    uint32_t crc1 = 0, crc2 = 0;
    size_t decompressed_bytes = 0;

    while (in_remaining > 0 && ret != Z_STREAM_END) {
        d_stream.next_in = in_buf;
        d_stream.avail_in = (uInt)(in_remaining > UINT_MAX ? UINT_MAX : in_remaining);
        in_buf += d_stream.avail_in;
        in_remaining -= d_stream.avail_in;

        do {
            d_stream.next_out = out_buf;
            d_stream.avail_out = (uInt)out_buf_size;

            ret = inflate(&d_stream, fuzz_int32_t_1);
            if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
                break;
            }

            size_t have = out_buf_size - d_stream.avail_out;
            if (have > 0) {
                // Calculate CRC for first half of decompressed data
                if (decompressed_bytes < have) {
                    crc1 = crc32(fuzz_uint64_t_2, out_buf, (uInt)(have / 2));
                    crc2 = crc32(fuzz_uint64_t_3, out_buf + (have / 2), (uInt)(have - (have / 2)));
                }
                decompressed_bytes += have;
            }
        } while (d_stream.avail_out == 0);
    }

    // Generate CRC combine value using decompressed bytes count
    if (decompressed_bytes > 0) {
        off_t offset = (off_t)decompressed_bytes;
        uLong combine_gen = crc32_combine_gen(offset);
        // Use the combine value (could be used to combine crc1 and crc2)
        (void)combine_gen; // Prevent unused variable warning
    }

    // Reset stream for possible re-use
    inflateReset(&d_stream);

    // Initialize compression stream
    z_stream c_stream;
    memset(&c_stream, 0, sizeof(z_stream));
    
    if (deflateInit(&c_stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(out_buf);
        inflateEnd(&d_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Change compression parameters using fuzzer data
    int level = data[0] % 10;  // 0-9 compression level
    int strategy = data[1] % 4; // 0-3 strategy
    if (deflateParams(&c_stream, level, strategy) != Z_OK) {
        // Continue anyway
    }

    // Open gzip file for writing compressed output
    gzFile gz_out = gzdopen(out_fd, fuzz_str_4);
    if (!gz_out) {
        deflateEnd(&c_stream);
        free(out_buf);
        inflateEnd(&d_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Write some data to gzip file using gzfwrite
    // Use part of decompressed data if available, otherwise use input data
    const void *write_data = decompressed_bytes > 0 ? out_buf : data;
    z_size_t write_size = decompressed_bytes > 0 ? 
                         (z_size_t)(decompressed_bytes < 1024 ? decompressed_bytes : 1024) : 
                         (z_size_t)(size < 1024 ? size : 1024);
    
    z_size_t items_written = gzfwrite(write_data, write_size, 1, gz_out);
    (void)items_written; // Prevent unused variable warning

    // Get current position in gzip file
    off64_t position = gztell64(gz_out);
    (void)position; // Prevent unused variable warning

    // Cleanup
    gzclose(gz_out);  // This also closes the underlying file descriptor
    deflateEnd(&c_stream);
    free(out_buf);
    inflateEnd(&d_stream);
    
    // Note: in_file is still open, close it
    assert_file_closed(&in_file);;
    // out_file is closed by gzclose, so don't close it again
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}