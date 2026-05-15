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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 0
//<Quality> {"density":26,"unique_branches":{},"library_calls":["zlibCompileFlags","gzopen64","crc32","adler32","adler32","gzread","crc32","adler32","gzrewind","gzread","adler32","adler32_combine","adler32_combine64","crc32_combine_gen64","deflateInit_","gzclose","inflateInit_","deflateEnd","gzclose","gzrewind","deflateBound_z","inflateEnd","deflateEnd","gzclose","gzread","deflate","deflateParams","inflateReset2","inflate","inflateEnd","deflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzopen64","crc32","adler32","adler32","gzread","crc32","adler32","gzrewind","gzread","adler32","adler32_combine","adler32_combine64","crc32_combine_gen64","deflateInit_","inflateInit_","gzrewind","deflateBound_z","gzread","deflate","deflateParams","inflateReset2","inflate","inflateEnd","deflateEnd","gzclose"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* f_data, size_t f_size) {
	if(f_size<87) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	

    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Get zlib compile flags
    uLong compile_flags = zlibCompileFlags();

    // Open the file with gzopen64 for reading
    gzFile gz_in = gzopen64("input_file", fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Read decompressed data using gzread
    const size_t read_buf_size = 1024;
    Bytef read_buf[read_buf_size];
    int bytes_read = 0;
    
    // Initialize checksums
    uLong crc_val = crc32(fuzz_uint64_t_2, Z_NULL, 0);
    uLong adler1 = adler32(fuzz_uint64_t_3, Z_NULL, 0);
    uLong adler2 = adler32(fuzz_uint64_t_4, Z_NULL, 0);
    
    // Read and compute checksums
    while ((bytes_read = gzread(gz_in, read_buf, read_buf_size)) > 0) {
        crc_val = crc32(crc_val, read_buf, bytes_read);
        adler1 = adler32(adler1, read_buf, bytes_read);
    }

    // Reset to beginning for second pass
    gzrewind(gz_in);
    
    // Second read for another checksum
    while ((bytes_read = gzread(gz_in, read_buf, read_buf_size)) > 0) {
        adler2 = adler32(adler2, read_buf, bytes_read);
    }

    // Combine Adler32 checksums
    off_t dummy_offset = 100;
    off64_t dummy_offset64 = 100;
    uLong combined_adler = adler32_combine(adler1, adler2, dummy_offset);
    uLong combined_adler64 = adler32_combine64(adler1, adler2, dummy_offset64);

    // Generate CRC32 combine operator
    uLong crc_combine_op = crc32_combine_gen64(dummy_offset64);

    // Initialize zlib streams
    z_stream def_stream;
    z_stream inf_stream;
    
    memset(&def_stream, 0, sizeof(def_stream));
    memset(&inf_stream, 0, sizeof(inf_stream));

    // Initialize deflate stream
    if (deflateInit_(&def_stream, fuzz_int32_t_6, 
                     fuzz_str_5, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize inflate stream
    if (inflateInit_(&inf_stream, fuzz_str_7, sizeof(z_stream)) != Z_OK) {
        deflateEnd(&def_stream);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Reset to beginning again
    gzrewind(gz_in);
    
    // Prepare compression buffers
    const size_t comp_buf_size = deflateBound_z(&def_stream, read_buf_size);
    Bytef *comp_buf = (Bytef *)malloc(comp_buf_size);
    Bytef *decomp_buf = (Bytef *)malloc(read_buf_size);
    
    if (!comp_buf || !decomp_buf) {
        free(comp_buf);
        free(decomp_buf);
        inflateEnd(&inf_stream);
        deflateEnd(&def_stream);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Process data in chunks
    while ((bytes_read = gzread(gz_in, read_buf, read_buf_size)) > 0) {
        // Set up deflate stream
        def_stream.next_in = read_buf;
        def_stream.avail_in = bytes_read;
        def_stream.next_out = comp_buf;
        def_stream.avail_out = comp_buf_size;

        // Compress with Z_SYNC_FLUSH
        if (deflate(&def_stream, fuzz_int32_t_8) != Z_OK) {
            break;
        }

        // Change compression parameters
        deflateParams(&def_stream, fuzz_int32_t_9, fuzz_int32_t_10);

        // Reset inflate stream with different window bits
        inflateReset2(&inf_stream, fuzz_int32_t_11);

        // Set up inflate stream
        inf_stream.next_in = comp_buf;
        inf_stream.avail_in = comp_buf_size - def_stream.avail_out;
        inf_stream.next_out = decomp_buf;
        inf_stream.avail_out = read_buf_size;

        // Decompress
        inflate(&inf_stream, fuzz_int32_t_12);
    }

    // Clean up
    free(comp_buf);
    free(decomp_buf);
    inflateEnd(&inf_stream);
    deflateEnd(&def_stream);
    gzclose(gz_in);
    assert_file_closed(&in_file);;

    // Remove temporary files
    remove("input_file");
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}