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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 2
//<Quality> {"density":4,"unique_branches":{"inflateCodesUsed":[[1410,9,1410,32,0,0,4,0]],"inflateStateCheck":[[94,28,94,47,0,0,4,0]]},"library_calls":["gzdopen","gzdopen","gzsetparams","inflateInit2_","gzclose_w","gzclose","inflateGetHeader","inflateBackInit_","inflateBack","inflateCodesUsed","inflateBackEnd","gzoffset64","gzwrite","gzoffset64","inflateEnd","gzclose","gzclose_w"],"critical_calls":["gzdopen","gzdopen","gzsetparams","inflateInit2_","inflateGetHeader","inflateBackInit_","inflateBack","inflateCodesUsed","inflateBackEnd","gzoffset64","gzwrite","gzoffset64","inflateEnd","gzclose","gzclose_w"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve the event of reading and decompressing a zlib-compressed byte stream while extracting metadata:

*/


extern "C" {
}

// Callback functions for inflateBack
static unsigned int  id_000045_back_in_callback(void* desc, unsigned char** buf) {
    // This callback provides data to inflateBack
    static unsigned char dummy_data[] = {0x78, 0x9C, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01};
    *buf = dummy_data;
    return sizeof(dummy_data);
}

static int  id_000045_back_out_callback(void* desc, unsigned char* buf, unsigned int length) {
    // This callback receives decompressed data from inflateBack
    // We'll just ignore the output for fuzzing
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<59) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    // Ensure minimum size for zlib operations
    if (size < 10) {
	return 0;
	}
	
    
    // Create a temporary file with the input data for gz operations
    FILE* temp_in = fmemopen((void*)data, size, "rb");
    if (!temp_in) {
	assert_file_closed(&temp_in);
	return 0;
	}
	
    
    // Create output file for gz operations
    FILE* temp_out = fopen("output_file", "wb");
    if (!temp_out) {
        assert_file_closed(&temp_in);;
        assert_file_closed(&temp_out);
	assert_file_closed(&temp_in);
	return 0;
    }
    
    int fd_in = fuzz_fileno(temp_in);
    int fd_out = fuzz_fileno(temp_out);
    
    // 1. Use gzdopen to create gzFile from file descriptor
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&temp_in);;
        assert_file_closed(&temp_out);;
        assert_file_closed(&temp_out);
	assert_file_closed(&temp_in);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 2. Use gzsetparams (though this is typically for writing, we'll use it with a write file)
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        // Set compression parameters (level 6, default strategy)
        gzsetparams(gz_out, fuzz_int32_t_2, fuzz_int32_t_3);
    }
    
    // 3. Use inflateGetHeader with inflate stream
    z_stream strm;
    gz_header header;
    
    // Initialize header
    memset(&header, 0, sizeof(header));
    header.extra_max = 65536;
    header.name_max = 256;
    header.comm_max = 256;
    
    // Initialize zlib stream for inflation
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int ret = inflateInit2(&strm, 15 + 16); // 15 window bits + 16 for gzip header
    if (ret != Z_OK) {
        if (gz_out) gzclose_w(gz_out);
        if (gz_in) gzclose(gz_in);
        assert_file_closed(&temp_in);;
        assert_file_closed(&temp_out);;
        assert_file_closed(&temp_out);
	assert_file_closed(&temp_in);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Set up input data for inflation
    std::vector<uint8_t> compressed_data(data, data + size);
    strm.avail_in = compressed_data.size();
    strm.next_in = compressed_data.data();
    
    // Try to get header
    inflateGetHeader(&strm, &header);
    
    // 4. Use inflateBack for decompression with callbacks
    z_stream back_strm;
    back_strm.zalloc = Z_NULL;
    back_strm.zfree = Z_NULL;
    back_strm.opaque = Z_NULL;
    
    // Initialize for inflateBack
    unsigned char window[32768];
    ret = inflateBackInit_(&back_strm, 15, window, fuzz_str_4, sizeof(z_stream));
    if (ret == Z_OK) {
        void* in_desc = nullptr;
        void* out_desc = nullptr;
        
        // Use inflateBack with our callbacks
        inflateBack(&back_strm,  id_000045_back_in_callback, in_desc,  id_000045_back_out_callback, out_desc);
        
        // 5. Use inflateCodesUsed to get information about the inflation
        unsigned long codes_used = inflateCodesUsed(&back_strm);
        (void)codes_used; // Use the variable to avoid unused warning
        
        inflateBackEnd(&back_strm);
    }
    
    // 6. Read a byte from the gz input file
    if (gz_in) {
        int byte = gzgetc(gz_in);
        (void)byte; // Use the variable
    }
    
    // 7. Get offset in the gz file (though we just opened it)
    if (gz_in) {
        off64_t offset = gzoffset64(gz_in);
        (void)offset; // Use the variable
    }
    
    // 8. Write some data to gz output file to use gzoffset64 on it
    if (gz_out) {
        const char* test_data = "test data for compression\n";
        gzwrite(gz_out, fuzz_str_5, static_cast<unsigned int>(fuzz_str_sz_5));
        
        // Get offset after writing
        off64_t out_offset = gzoffset64(gz_out);
        (void)out_offset; // Use the variable
    }
    
    // 9. Clean up resources
    
    // Clean up inflate stream
    inflateEnd(&strm);
    
    // Close gz files using required APIs
    if (gz_in) {
        // Note: gzclose_w is for writing files, but we'll use it anyway as required
        // For reading files we should use gzclose or gzclose_r
        gzclose(gz_in); // Close reading file properly
    }
    
    if (gz_out) {
        gzclose_w(gz_out); // Use required API for writing file
    } else {
        assert_file_closed(&temp_out);;
    }
    
    // Close FILE pointers (gzclose should have closed the underlying file descriptors)
    assert_file_closed(&temp_in);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&temp_out);
	assert_file_closed(&temp_in);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}