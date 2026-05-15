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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["lou_logFile","lou_logPrint","lou_listTables","lou_logPrint","lou_logEnd","lou_findTables","lou_logPrint","lou_freeTableFiles","lou_logEnd","lou_logPrint","lou_logPrint","lou_logPrint","lou_checkTable","lou_logPrint","lou_freeTableFiles","lou_freeTableFiles","lou_logEnd","lou_logPrint","lou_freeTableFiles","lou_freeTableFiles","lou_logEnd","lou_charToDots","lou_logPrint","lou_hyphenate","lou_logPrint","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_translateString","lou_logPrint","lou_translate","lou_logPrint","lou_freeTableFiles","lou_freeTableFiles","lou_logEnd"],"critical_calls":["lou_logFile","lou_logPrint","lou_listTables","lou_findTables","lou_logPrint","lou_logPrint","lou_logPrint","lou_checkTable","lou_charToDots","lou_logPrint","lou_hyphenate","lou_logPrint","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_translateString","lou_logPrint","lou_translate","lou_logPrint","lou_freeTableFiles","lou_freeTableFiles","lou_logEnd"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<23) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Early exit if data is too small for meaningful processing
    if (size < 4) {
	return 0;
	}
	
    
    // Set up logging to capture any potential errors
    lou_logFile(fuzz_str_1);
    lou_logPrint("=== Starting liblouis fuzzing iteration ===\n");
    
    // Get list of available tables
    char **table_list = lou_listTables();
    if (!table_list) {
        lou_logPrint("No tables available\n");
        lou_logEnd();
        return 0;
    }
    
    // Find tables using a query constructed from input data
    // Use the first 32 bytes (or less if data is smaller) as query
    size_t query_len = size < 32 ? size : 32;
    std::string query(reinterpret_cast<const char*>(data), query_len);
    
    // Replace any null bytes in query to make it a valid C string
    for (size_t i = 0; i < query.length(); ++i) {
        if (query[i] == '\0') query[i] = ' ';
    }
    
    char **found_tables = lou_findTables(query.c_str());
    if (!found_tables || !found_tables[0]) {
        lou_logPrint("No tables found with query\n");
        lou_freeTableFiles(table_list);
        lou_logEnd();
        return 0;
    }
    
    // Use the first found table for processing
    const char* table_to_use = found_tables[0];
    lou_logPrint("Using table: ");
    lou_logPrint(table_to_use);
    lou_logPrint("\n");
    
    // Check if the table is valid
    int table_check = lou_checkTable(table_to_use);
    if (table_check != 0) {
        lou_logPrint("Table check failed\n");
        lou_freeTableFiles(found_tables);
        lou_freeTableFiles(table_list);
        lou_logEnd();
        return 0;
    }
    
    // Convert input data to widechar format for liblouis processing
    // Use remaining data after query (if any)
    const uint8_t* text_data = data + query_len;
    size_t text_size = size - query_len;
    
    if (text_size < 2) {
        lou_logPrint("Insufficient data for text processing\n");
        lou_freeTableFiles(found_tables);
        lou_freeTableFiles(table_list);
        lou_logEnd();
        return 0;
    }
    
    // Convert bytes to widechars (simple approach: each 2 bytes = 1 widechar)
    size_t widechar_len = text_size / 2;
    std::vector<widechar> inbuf(widechar_len);
    
    for (size_t i = 0; i < widechar_len; ++i) {
        inbuf[i] = (text_data[2*i] << 8) | text_data[2*i + 1];
    }
    
    // Test 1: charToDots conversion
    std::vector<widechar> dots_buf(widechar_len);
    int dots_result = lou_charToDots(table_to_use, inbuf.data(), dots_buf.data(), 
                                     widechar_len, 0);
    if (dots_result == 0) {
        lou_logPrint("charToDots failed\n");
    }
    
    // Test 2: hyphenation
    std::vector<char> hyphens_buf(widechar_len);
    int hyphen_result = lou_hyphenate(table_to_use, inbuf.data(), widechar_len, 
                                      hyphens_buf.data(), fuzz_int32_t_2);
    if (hyphen_result == 0) {
        lou_logPrint("hyphenate failed\n");
    }
    
    // Test 3: translateString (simpler translation API)
    int inlen = widechar_len;
    int outlen = widechar_len * 4;  // Allocate extra space for output
    std::vector<widechar> outbuf(outlen);
    std::vector<formtype> typeform_buf(widechar_len);
    std::vector<char> spacing_buf(widechar_len);
    
    // Initialize typeform (using emphasis class if available)
    const char** emph_classes = lou_getEmphClasses(table_to_use);
    if (emph_classes && emph_classes[0]) {
        formtype typeform_val = lou_getTypeformForEmphClass(table_to_use, emph_classes[0]);
        std::fill(typeform_buf.begin(), typeform_buf.end(), typeform_val);
        lou_freeEmphClasses(emph_classes);
    }
    
    int trans_result = lou_translateString(table_to_use, inbuf.data(), &inlen, 
                                           outbuf.data(), &outlen, typeform_buf.data(), 
                                           spacing_buf.data(), fuzz_int32_t_3);
    if (trans_result == 0) {
        lou_logPrint("translateString failed\n");
    }
    
    // Test 4: translate (more comprehensive translation API)
    std::vector<int> outputPos(outlen);
    std::vector<int> inputPos(widechar_len);
    std::vector<int> cursorPos(widechar_len);
    
    // Reset lengths for second translation attempt
    inlen = widechar_len;
    outlen = widechar_len * 4;
    std::fill(outbuf.begin(), outbuf.end(), 0);
    
    int trans_full_result = lou_translate(table_to_use, inbuf.data(), &inlen, 
                                          outbuf.data(), &outlen, typeform_buf.data(), 
                                          spacing_buf.data(), outputPos.data(), 
                                          inputPos.data(), cursorPos.data(), fuzz_int32_t_4);
    if (trans_full_result == 0) {
        lou_logPrint("translate failed\n");
    }
    
    // Create a FILE* from input data for any potential file operations
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Example: Use file descriptor if needed (though not directly used by listed APIs)
        int fd = fuzz_fileno(in_file);
        (void)fd;  // Prevent unused variable warning
        
        // Close the file
        assert_file_closed(&in_file);;
    }
    
    // Create output file for logging or other purposes
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some processed data to output file
        if (outlen > 0) {
            fwrite(outbuf.data(), sizeof(widechar), outlen, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    lou_freeTableFiles(found_tables);
    lou_freeTableFiles(table_list);
    lou_logEnd();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}