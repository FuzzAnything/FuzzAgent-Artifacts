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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"gzgets":[[586,9,586,20,0,0,4,0],[586,24,586,44,0,0,4,1]]},"library_calls":["zlibCompileFlags","gzdopen","gzbuffer","inflateInit2_","gzclose","inflateGetHeader","inflateCopy","deflateInit_","deflateSetDictionary","deflateEnd","inflateEnd","gzseek64","gzopen","gzprintf","gzclose","gzseek64","gzgets","inflateBackInit_","inflateBackEnd","adler32","adler32","adler32","adler32","adler32_combine","inflate","inflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","gzbuffer","inflateInit2_","inflateGetHeader","inflateCopy","deflateInit_","deflateSetDictionary","deflateEnd","inflateEnd","gzseek64","gzopen","gzprintf","gzclose","gzseek64","gzgets","inflateBackInit_","inflateBackEnd","adler32","adler32","adler32","adler32","adler32_combine","inflate","inflateEnd","gzclose"],"visited":0}
/**/


// Custom output callback structure for inflateBack
struct OutCallbackData {
    FILE* out_file;
    size_t total_written;
};

// Input callback for inflateBack
static unsigned int  id_000071_in_callback(void* desc, unsigned char** buf) {
    // Just return 0 to indicate no more input
    // In a real scenario, this would provide data
    return 0;
}

// Output callback for inflateBack
static int  id_000071_out_callback(void* desc, unsigned char* buf, unsigned int size) {
    OutCallbackData* data = (OutCallbackData*)desc;
    if (data && data->out_file) {
        size_t written = fwrite(buf, 1, size, data->out_file);
        data->total_written += written;
        return 0; // Success
    }
    return 1; // Error
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // Get zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Use to avoid unused variable warning

    // Create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Open output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    
    // Open gzFile from file descriptor
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set buffer size for gzFile
    gzbuffer(gz_in, 8192);

    // Allocate and initialize z_stream structures
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));
    
    // Initialize inflation with gzip/zlib header detection
    int windowBits = 15 + 16; // 15 window bits + 16 for gzip decoding
    if (inflateInit2_(&strm1, windowBits, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Setup header for gzip extraction
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.extra_max = 65536;
    header.name_max = 65536;
    header.comm_max = 65536;
    
    // Set header extraction
    inflateGetHeader(&strm1, &header);

    // Copy the stream
    if (inflateCopy(&strm2, &strm1) == Z_OK) {
        // Set dictionary on deflate stream (even though we're inflating)
        // We'll create a dummy deflate stream for this
        z_stream def_strm;
        memset(&def_strm, 0, sizeof(z_stream));
        deflateInit(&def_strm, Z_DEFAULT_COMPRESSION);
        
        // Use part of input as dictionary
        uInt dict_len = size > 1024 ? 1024 : (uInt)size;
        if (dict_len > 0) {
            deflateSetDictionary(&def_strm, data, dict_len);
        }
        deflateEnd(&def_strm);
        
        // Clean up copied stream
        inflateEnd(&strm2);
    }

    // Read from gzFile using gzgetc
    if (gzseek64(gz_in, 0, SEEK_SET) == 0) {
        int ch;
        int count = 0;
        while ((ch = gzgetc(gz_in)) != EOF && count < 100) {
            // Write character using gzprintf (to different file)
            gzFile gz_out = gzopen("output_file2", "wb");
            if (gz_out) {
                gzprintf(gz_out, "%c", ch);
                gzclose(gz_out);
            }
            count++;
        }
    }

    // Try to seek and read more
    if (gzseek64(gz_in, size/2, SEEK_SET) >= 0) {
        char buf[100];
        if (gzgets(gz_in, buf, sizeof(buf))) {
            // Do something with buf
        }
    }

    // Prepare for inflateBack (alternative inflation method)
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(z_stream));
    
    unsigned char back_window[32768]; // 32K window
    
    if (inflateBackInit_(&back_strm, 15, back_window, 
                        ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        OutCallbackData out_data = {out_file, 0};
        
        // We can't easily use the fuzzer input with inflateBack
        // without proper callback setup, so just initialize and end
        inflateBackEnd(&back_strm);
    }

    // Calculate Adler-32 checksums and combine them
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, Z_NULL, 0);
    
    if (size > 20) {
        // Calculate checksum for first half
        size_t half1 = size / 2;
        adler1 = adler32(adler1, data, (uInt)half1);
        
        // Calculate checksum for second half
        adler2 = adler32(adler2, data + half1, (uInt)(size - half1));
        
        // Combine the checksums
        uLong combined = adler32_combine(adler1, adler2, size);
        (void)combined; // Use to avoid warning
    }

    // Process input data with inflation
    if (size > 0) {
        Bytef out_buffer[4096];
        strm1.next_in = (Bytef*)data;
        strm1.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
        strm1.next_out = out_buffer;
        strm1.avail_out = sizeof(out_buffer);
        
        // Try to inflate
        int ret = inflate(&strm1, Z_NO_FLUSH);
        
        // If we got some output, write it
        if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
            size_t have = sizeof(out_buffer) - strm1.avail_out;
            if (have > 0 && out_file) {
                fwrite(out_buffer, 1, have, out_file);
            }
        }
    }

    // Clean up all resources
    inflateEnd(&strm1);
    gzclose(gz_in);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove temporary output files
    remove("output_file");
    remove("output_file2");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}