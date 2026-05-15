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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["gzdopen","gzclose_r","gzread","gzungetc","gztell","gzseek64","inflateInit2_","gzclose_r","inflateEnd","gzclose_r","inflate","inflateSync","inflateReset2","gzerror","get_crc_table","inflateEnd","gzread","gztell","gzclose_r"],"critical_calls":["gzdopen","gzread","gzungetc","gztell","gzseek64","inflateInit2_","inflate","inflateSync","inflateReset2","gzerror","get_crc_table","inflateEnd","gzread","gztell","gzclose_r"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Need enough data for meaningful testing
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Open gzFile for reading
    gzFile gz = gzdopen(fd, "rb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Allocate buffer for reading
    const size_t buf_size = 4096;
    unsigned char *buffer = (unsigned char *)malloc(buf_size);
    if (!buffer) {
        gzclose_r(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Read some data
    int bytes_read = gzread(gz, buffer, buf_size / 2);
    
    // Try to unget a character if we read something
    if (bytes_read > 0) {
        // Get the last character read
        int last_char = buffer[bytes_read - 1];
        // Push it back
        int unget_result = gzungetc(last_char, gz);
        (void)unget_result; // Result intentionally unused
    }

    // Get current position
    off_t current_pos = gztell(gz);
    
    // Seek to a different position using 64-bit seek
    // Seek to 1/4 of the size or 100 bytes, whichever is smaller
    off64_t seek_offset = (off64_t)(size / 4);
    if (seek_offset > 100) seek_offset = 100;
    off64_t seek_result = gzseek64(gz, seek_offset, fuzz_int32_t_1);
    (void)seek_result; // Result intentionally unused

    // Create z_stream for inflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize for decompression with automatic header detection
    int init_result = inflateInit2(&strm, 15 + 32); // 32 enables automatic header detection
    if (init_result != Z_OK) {
        free(buffer);
        gzclose_r(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set up input for inflate
    strm.next_in = (Bytef *)data;
    strm.avail_in = size > 100 ? 100 : (uInt)size; // Use first 100 bytes for inflate
    
    // Allocate output buffer for inflate
    unsigned char *out_buffer = (unsigned char *)malloc(buf_size);
    if (!out_buffer) {
        inflateEnd(&strm);
        free(buffer);
        gzclose_r(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    strm.next_out = out_buffer;
    strm.avail_out = buf_size;

    // Perform inflation
    int inflate_result = inflate(&strm, fuzz_int32_t_2);
    
    // Try to sync if we got an error (but not Z_STREAM_END)
    if (inflate_result != Z_STREAM_END && inflate_result != Z_OK) {
        inflateSync(&strm);
    }

    // Reset with different window bits
    inflateReset2(&strm, fuzz_int32_t_3); // Reset with raw deflate format

    // Get error information from gzFile
    int gz_errnum = 0;
    const char *gz_error_msg = gzerror(gz, &gz_errnum);
    (void)gz_error_msg; // Intentionally unused

    // Get CRC table (for CRC32 calculations)
    const z_crc_t *crc_table = get_crc_table();
    (void)crc_table; // Intentionally unused - just demonstrating usage

    // Clean up inflate resources
    inflateEnd(&strm);
    free(out_buffer);

    // Read a bit more after seek
    if (bytes_read > 0) {
        gzread(gz, buffer, buf_size / 4);
    }

    // Get position again after operations
    current_pos = gztell(gz);

    // Clean up
    free(buffer);
    gzclose_r(gz);
    assert_file_closed(&in_file);; // Note: gzclose_r already closed the underlying fd

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}