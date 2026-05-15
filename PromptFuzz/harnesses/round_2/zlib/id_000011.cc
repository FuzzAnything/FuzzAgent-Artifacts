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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"gzseek64":[[391,19,391,30,0,0,4,1]]},"library_calls":["gzopen64","gzclose","gzdopen","gzseek64","gzfread","adler32_z","gzerror","zError","gzclose"],"critical_calls":["gzopen64","gzclose","gzdopen","gzseek64","gzfread","adler32_z","gzerror","zError","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
        // Not enough data for meaningful fuzzing
        return 0;
    }

    // Create a FILE* from the input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor from the FILE*
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Open the gzip file using the file descriptor
    gzFile gz = gzopen64("input_file", "rb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Reopen the gzFile with the actual file descriptor for reading
    // We need to close and reopen with proper handling
    gzclose(gz);
    gz = gzdopen(fd, "rb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Attempt to seek within the gzip file
    // Use part of the input to determine seek parameters safely
    off64_t seek_offset = static_cast<off64_t>(data[0] % 100);  // Small offset for safety
    int seek_whence = (data[1] % 2 == 0) ? SEEK_SET : SEEK_CUR;
    
    off64_t seek_result = gzseek64(gz, seek_offset, seek_whence);
    
    // Read some data from the gzip file to process
    const size_t buf_size = 1024;
    Bytef buffer[buf_size];
    z_size_t bytes_read = 0;
    
    // Attempt to read up to 100 bytes
    size_t read_size = std::min(static_cast<size_t>(100), size);
    if (read_size > 0) {
        bytes_read = gzfread(buffer, 1, read_size, gz);
    }

    // Calculate Adler-32 checksum on the read data
    uLong adler = 0L;
    if (bytes_read > 0) {
        adler = adler32_z(adler, buffer, bytes_read);
    }

    // Check for errors using gzerror
    int errnum = 0;
    const char *error_msg = gzerror(gz, &errnum);
    
    // Also use zError to get error string for comparison
    const char *zlib_error_msg = NULL;
    if (errnum != Z_OK && errnum != Z_STREAM_END) {
        zlib_error_msg = zError(errnum);
    }

    // Clean up: close gzFile (which also closes the underlying file descriptor)
    gzclose(gz);
    
    // Note: gzclose already closed the file descriptor, so we don't need to assert_file_closed(&in_file);
    // Attempting to assert_file_closed(&in_file); after gzclose would cause double-free
    
    // Release any other resources
    // The buffer is stack-allocated, so no need to free
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}