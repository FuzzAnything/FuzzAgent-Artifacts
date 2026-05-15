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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 2
//<Quality> {"density":21,"unique_branches":{"deflate":[[990,9,990,33,0,0,4,0]],"compress2_z":[[32,29,32,41,0,0,4,1]]},"library_calls":["inflateInit2_","inflate","inflateEnd","inflateGetDictionary","inflateEnd","crc32","crc32","deflateInit2_","deflateSetHeader","inflateSetDictionary","deflateBound","deflate","deflateEnd","deflateEnd","compress","compress2","compress_z","compress2_z","gzdopen","gzwrite","gzclose_w","gzdopen","gzclose_r","crc32","crc32","crc32_combine","crc32_combine64","crc32_combine_gen64"],"critical_calls":["inflateInit2_","inflate","inflateGetDictionary","inflateEnd","crc32","crc32","deflateInit2_","deflateSetHeader","inflateSetDictionary","deflateBound","deflate","deflateEnd","compress","compress2","compress_z","compress2_z","gzdopen","gzwrite","gzclose_w","gzdopen","gzclose_r","crc32","crc32","crc32_combine","crc32_combine64","crc32_combine_gen64"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<114) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_17, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	

    // Initialize decompression stream for zlib/gzip format
    z_stream d_stream;
    memset(&d_stream, 0, sizeof(d_stream));
    d_stream.zalloc = Z_NULL;
    d_stream.zfree = Z_NULL;
    d_stream.opaque = Z_NULL;
    
    // Use inflateInit2_ with windowBits = 47 to handle both zlib and gzip
    int ret = inflateInit2_(&d_stream, fuzz_int32_t_2, fuzz_str_1, sizeof(z_stream));
    if (ret != Z_OK) {
	return 0;
	}
	

    // Setup decompression input
    std::vector<Bytef> decompressed(size * 2);
    d_stream.next_in = (Bytef*)data;
    d_stream.avail_in = (uInt)size;
    d_stream.next_out = decompressed.data();
    d_stream.avail_out = (uInt)decompressed.size();

    // Perform decompression
    ret = inflate(&d_stream, fuzz_int32_t_3);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        inflateEnd(&d_stream);
        return 0;
    }
    size_t decompressed_size = decompressed.size() - d_stream.avail_out;

    // Try to get dictionary from decompression stream
    std::vector<Bytef> dict_buf(32768);
    uInt dict_len = 0;
    inflateGetDictionary(&d_stream, dict_buf.data(), &dict_len);

    // Clean up decompression stream
    inflateEnd(&d_stream);

    // Calculate CRC32 of decompressed data
    uLong decompressed_crc = crc32(fuzz_uint64_t_4, Z_NULL, 0);
    decompressed_crc = crc32(decompressed_crc, 
                           decompressed.data(), 
                           (uInt)decompressed_size);

    // Initialize compression stream
    z_stream c_stream;
    memset(&c_stream, 0, sizeof(c_stream));
    c_stream.zalloc = Z_NULL;
    c_stream.zfree = Z_NULL;
    c_stream.opaque = Z_NULL;

    // Use deflateInit2_ for gzip compression
    ret = deflateInit2_(&c_stream, fuzz_int32_t_6, fuzz_int32_t_7,
                        fuzz_int32_t_8, fuzz_int32_t_9, fuzz_int32_t_10,
                        fuzz_str_5, sizeof(z_stream));
    if (ret != Z_OK) {
	return 0;
	}
	

    // Set gzip header
    gz_header gzheader;
    memset(&gzheader, 0, sizeof(gzheader));
    gzheader.text = 1;
    gzheader.time = 1234567890;
    gzheader.os = 255;
    deflateSetHeader(&c_stream, &gzheader);

    // Set dictionary if we got one
    if (dict_len > 0) {
        inflateSetDictionary(&c_stream, dict_buf.data(), dict_len);
    }

    // Calculate maximum compressed size
    uLong max_compressed = deflateBound(&c_stream, (uLong)decompressed_size);
    std::vector<Bytef> compressed(max_compressed);

    // Setup compression
    c_stream.next_in = decompressed.data();
    c_stream.avail_in = (uInt)decompressed_size;
    c_stream.next_out = compressed.data();
    c_stream.avail_out = (uInt)compressed.size();

    // Perform compression
    ret = deflate(&c_stream, fuzz_int32_t_11);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        deflateEnd(&c_stream);
        return 0;
    }
    size_t compressed_size = compressed.size() - c_stream.avail_out;

    // Clean up compression stream
    deflateEnd(&c_stream);

    // Test various compression functions
    std::vector<Bytef> comp1(decompressed_size * 2);
    uLongf comp1_len = (uLongf)comp1.size();
    compress(comp1.data(), &comp1_len, decompressed.data(), (uLong)decompressed_size);

    std::vector<Bytef> comp2(decompressed_size * 2);
    uLongf comp2_len = (uLongf)comp2.size();
    compress2(comp2.data(), &comp2_len, decompressed.data(), 
              (uLong)decompressed_size, fuzz_int32_t_12);

    std::vector<Bytef> comp_z(decompressed_size * 2);
    z_size_t comp_z_len = (z_size_t)comp_z.size();
    compress_z(comp_z.data(), &comp_z_len, decompressed.data(), 
               (z_size_t)decompressed_size);

    std::vector<Bytef> comp2_z(decompressed_size * 2);
    z_size_t comp2_z_len = (z_size_t)comp2_z.size();
    compress2_z(comp2_z.data(), &comp2_z_len, decompressed.data(), 
                (z_size_t)decompressed_size, fuzz_int32_t_13);

    // Create gzFile from file descriptor
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
	assert_file_closed(&out_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd, "wb9");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Write decompressed data to gzFile
    gzwrite(gz_out, decompressed.data(), (unsigned int)decompressed_size);

    // Close gzFile (both reading and writing variants)
    gzclose_w(gz_out);
    assert_file_closed(&out_file);;

    // Also test gzclose_r for completeness
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        int fd_in = fuzz_fileno(in_file);
        gzFile gz_in = gzdopen(fd_in, "rb");
        if (gz_in) {
            gzclose_r(gz_in);
        }
        assert_file_closed(&in_file);;
    }

    // Test CRC combine functions
    uLong crc1 = crc32(fuzz_uint64_t_15, fuzz_str_14, static_cast<uInt>(fuzz_str_sz_14));
    uLong crc2 = crc32(fuzz_uint64_t_17, fuzz_str_16, static_cast<uInt>(fuzz_str_sz_16));
    uLong crc_combined = crc32_combine(crc1, crc2, 5);
    
    uLong crc_combined64 = crc32_combine64(crc1, crc2, (off64_t)5);
    uLong crc_gen = crc32_combine_gen64((off64_t)5);

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
}