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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["gzdopen","gzdirect","inflateInit2_","gzclose","inflateEnd","gzclose","gzopen","inflateEnd","gzclose","gzread","inflate","gzerror","gzwrite","gzputc","inflateSyncPoint","gzeof","inflateCopy","inflateEnd","inflateReset","deflateInit2_","inflateEnd","gzclose","gzclose","deflateGetDictionary","deflateBound","compress2","gzseek64","deflateEnd","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdirect","inflateInit2_","gzopen","gzread","inflate","gzerror","gzwrite","gzputc","inflateSyncPoint","gzeof","inflateCopy","inflateEnd","inflateReset","deflateInit2_","deflateGetDictionary","deflateBound","compress2","gzseek64","deflateEnd","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Use first byte to control various parameters
    const uint8_t control = data[0];
    const uint8_t *input_data = data + 1;
    size_t input_size = size - 1;

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)input_data, input_size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    const char *mode = (control & 0x01) ? "rb9" : "rb";
    gzFile gz_input = gzdopen(fd, mode);
    if (!gz_input) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Check if file is direct
    int direct = gzdirect(gz_input);

    // Set up inflation stream for decompression
    z_stream inf_stream;
    memset(&inf_stream, 0, sizeof(inf_stream));
    
    int windowBits = (control & 0x02) ? 15 + 32 : 15; // Add 32 for gzip auto-detection
    if (inflateInit2_(&inf_stream, windowBits, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create output FILE
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        inflateEnd(&inf_stream);
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Get output file descriptor and open as gzFile for writing
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_output = gzopen("output_file", (control & 0x04) ? "wb9" : "wb");
    if (!gz_output) {
        assert_file_closed(&out_file);;
        inflateEnd(&inf_stream);
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Create buffers
    unsigned char read_buf[4096];
    unsigned char inflate_buf[8192];
    unsigned char compress_buf[16384];
    
    // Process input data
    int total_read = 0;
    int bytes_read;
    int inflate_result;
    int flush;
    
    while ((bytes_read = gzread(gz_input, read_buf, sizeof(read_buf))) > 0) {
        total_read += bytes_read;
        
        // Set up inflation stream
        inf_stream.next_in = read_buf;
        inf_stream.avail_in = bytes_read;
        
        flush = (bytes_read < (int)sizeof(read_buf)) ? Z_FINISH : Z_NO_FLUSH;
        
        do {
            inf_stream.next_out = inflate_buf;
            inf_stream.avail_out = sizeof(inflate_buf);
            
            inflate_result = inflate(&inf_stream, flush);
            
            if (inflate_result == Z_STREAM_ERROR || 
                inflate_result == Z_DATA_ERROR ||
                inflate_result == Z_MEM_ERROR) {
                // Handle error
                int errnum;
                const char *err = gzerror(gz_input, &errnum);
                (void)err; // Suppress unused variable warning
                break;
            }
            
            size_t have = sizeof(inflate_buf) - inf_stream.avail_out;
            
            // Write decompressed data to gzFile
            if (have > 0) {
                gzwrite(gz_output, inflate_buf, have);
                
                // Also write individual characters occasionally
                if (control & 0x08) {
                    for (size_t i = 0; i < have && i < 10; i++) {
                        gzputc(gz_output, inflate_buf[i]);
                    }
                }
            }
            
            // Check sync point
            if (inflateSyncPoint(&inf_stream)) {
                // Found sync point
            }
            
        } while (inf_stream.avail_out == 0);
        
        // Check for EOF on input
        if (gzeof(gz_input)) {
            break;
        }
    }

    // Make a copy of inflation stream
    z_stream inf_stream_copy;
    if (inflateCopy(&inf_stream_copy, &inf_stream) == Z_OK) {
        inflateEnd(&inf_stream_copy);
    }

    // Reset inflation stream
    inflateReset(&inf_stream);

    // Set up deflation stream for recompression
    z_stream def_stream;
    memset(&def_stream, 0, sizeof(def_stream));
    
    int level = (control >> 4) & 0x09; // 0-9
    int strategy = (control >> 2) & 0x03;
    
    if (deflateInit2_(&def_stream, level, Z_DEFLATED, windowBits, 8, 
                     strategy, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&inf_stream);
        gzclose(gz_output);
        gzclose(gz_input);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Get dictionary from deflate stream (if any)
    Bytef dict_buf[256];
    uInt dict_len = sizeof(dict_buf);
    deflateGetDictionary(&def_stream, dict_buf, &dict_len);

    // Calculate compression bound for some data
    uLong bound = deflateBound(&def_stream, total_read);

    // Use compress2 on some data
    uLongf destLen = sizeof(compress_buf);
    if (total_read > 100) {
        compress2(compress_buf, &destLen, inflate_buf, 
                 (total_read < 100) ? total_read : 100, level);
    }

    // Seek in gzFile
    if (control & 0x10) {
        gzseek64(gz_output, 0, SEEK_SET);
    }

    // Clean up deflation
    deflateEnd(&def_stream);

    // Clean up inflation
    inflateEnd(&inf_stream);

    // Close all file handles
    gzclose(gz_output);
    gzclose(gz_input);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    // Remove temporary output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}