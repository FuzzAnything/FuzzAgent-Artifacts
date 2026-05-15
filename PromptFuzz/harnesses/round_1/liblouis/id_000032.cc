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
#include <liblouis/liblouis.h>
}
//<ID> 641
//<Prompt> []
/*<Combination>: [
*/
//<score> 1.5, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["lou_getDataPath","lou_registerLogCallback","lou_hyphenate","lou_getEmphClasses","lou_freeEmphClasses","lou_findTable","lou_freeTableFile","lou_compileString","lou_logPrint","lou_logEnd"],"critical_calls":["lou_getDataPath","lou_registerLogCallback","lou_hyphenate","lou_getEmphClasses","lou_freeEmphClasses","lou_findTable","lou_freeTableFile","lou_compileString","lou_logPrint","lou_logEnd"],"visited":1}
/**/


extern "C" {
}

// Log callback function
static void  id_000032_log_callback(logLevels level, const char *message) {
    // Empty callback - just needed for registration
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    if (size < 4) {
        return 0;  // Need at least some data
    }

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE* for output data
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
    (void)in_fd;  // Mark as used
    (void)out_fd;  // Mark as used

    // 1. Get data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        // Could use data_path here
        free(data_path);
    }

    // 2. Register log callback
    lou_registerLogCallback( id_000032_log_callback);

    // 3. Process input data for hyphenation
    // We'll use the input data as text to hyphenate
    // First, convert data to widechars (assuming UTF-16 like encoding)
    std::vector<widechar> inbuf;
    size_t max_chars = size / sizeof(widechar);
    if (max_chars > 100) max_chars = 100;  // Limit to prevent excessive allocation
    
    for (size_t i = 0; i < max_chars && i * sizeof(widechar) + 1 < size; i++) {
        widechar wc = 0;
        if (i * sizeof(widechar) + 1 < size) {
            wc = (data[i * sizeof(widechar)] << 8) | data[i * sizeof(widechar) + 1];
        } else {
            wc = data[i * sizeof(widechar)];
        }
        inbuf.push_back(wc);
    }
    
    if (inbuf.empty()) {
        inbuf.push_back('t');
        inbuf.push_back('e');
        inbuf.push_back('s');
        inbuf.push_back('t');
    }
    
    // 4. Hyphenate the input
    std::vector<char> hyphens(inbuf.size() + 1, 0);
    int mode = 0;
    
    // Use a simple table list for testing
    const char *tableList = "en-us-g2.ctb";
    int hyphen_result = lou_hyphenate(fuzz_str_1, inbuf.data(), inbuf.size(), 
                                     hyphens.data(), fuzz_int32_t_2);

    // 5. Get emphasis classes for the table
    const char **emph_classes = lou_getEmphClasses(fuzz_str_3);
    if (emph_classes) {
        // Process emphasis classes if available
        for (int i = 0; emph_classes[i] != nullptr; i++) {
            // Could use emph_classes[i] here
        }
        lou_freeEmphClasses(emph_classes);
    }

    // 6. Find a table using part of input data as query
    // Convert first few bytes to null-terminated string for query
    size_t query_len = size > 50 ? 50 : size;
    std::vector<char> query_buf(query_len + 1);
    memcpy(query_buf.data(), data, query_len);
    query_buf[query_len] = '\0';
    
    char *found_table = lou_findTable(query_buf.data());
    if (found_table) {
        // Could use found_table here
        lou_freeTableFile(found_table);
    }

    // 7. Compile a table string using part of input data
    // Use another part of input as table definition
    size_t compile_len = size > 100 ? 100 : size;
    if (compile_len > query_len + 10) {
        // Make sure we use different part of data
        std::vector<char> compile_buf(compile_len + 1);
        size_t offset = query_len + 10;
        if (offset + compile_len > size) {
            offset = size - compile_len;
        }
        if (offset < size) {
            memcpy(compile_buf.data(), data + offset, compile_len);
            compile_buf[compile_len] = '\0';
            
            int compile_result = lou_compileString("test", compile_buf.data());
            // Result can be ignored for fuzzing
        }
    }

    // 8. Log something using logPrint
    lou_logPrint("Fuzzing iteration completed");

    // Write some results to output file
    if (out_file) {
        fwrite(hyphens.data(), 1, hyphens.size(), out_file);
        fflush(out_file);
    }

    // Clean up
    lou_logEnd();  // Ensure log is ended
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}