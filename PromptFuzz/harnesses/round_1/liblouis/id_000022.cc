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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["lou_setLogLevel","lou_compileString","lou_getDataPath","lou_findTable","lou_findTables","lou_freeTableFiles","lou_getTableInfo","lou_getEmphClasses","lou_freeEmphClasses","lou_free","lou_hyphenate","lou_backTranslateString","lou_translatePrehyphenated","lou_free"],"critical_calls":["lou_setLogLevel","lou_compileString","lou_getDataPath","lou_findTable","lou_findTables","lou_freeTableFiles","lou_getTableInfo","lou_getEmphClasses","lou_freeEmphClasses","lou_hyphenate","lou_backTranslateString","lou_translatePrehyphenated","lou_free"],"visited":0}
/**/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<105) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
	return 0;
	}
	
    
    // Initialize liblouis
    lou_setLogLevel(LOU_LOG_ERROR);
    
    // 1. First, try to compile a table from input
    // Use first part of data as table content
    size_t table_size = size > 1024 ? 1024 : size;
    std::string table_content(reinterpret_cast<const char*>(data), table_size);
    const char* tableList = "fuzz_table";
    
    // Try to compile table (may fail - that's OK for fuzzing)
    lou_compileString(tableList, table_content.c_str());
    
    // 2. Get data path
    char* data_path = lou_getDataPath();
    if (data_path) {
        // Could use data_path for something
        free(data_path);
    }
    
    // 3. Try to find tables using part of input as query
    size_t query_size = size > 128 ? 128 : size;
    std::string query(reinterpret_cast<const char*>(data), query_size);
    char* found_table = lou_findTable(query.c_str());
    if (found_table) {
        free(found_table);
    }
    
    // 4. Try to find multiple tables
    char** found_tables = lou_findTables(query.c_str());
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }
    
    // 5. Get table info if we have a valid-looking table
    const char* test_table = "en-us-comp8.ctb";  // Common table
    char* table_info = lou_getTableInfo(fuzz_str_1, fuzz_str_2);
    if (table_info) {
        free(table_info);
    }
    
    // 6. Get emphasis classes
    const char** emph_classes = lou_getEmphClasses(fuzz_str_3);
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    // 7. Now process the input as widechar data for translation/hyphenation
    // Convert input to widechar buffer
    size_t widechar_len = size / sizeof(widechar);
    if (widechar_len == 0) {
        lou_free();
        return 0;
    }
    
    std::vector<widechar> inbuf(widechar_len);
    memcpy(inbuf.data(), data, widechar_len * sizeof(widechar));
    
    // 8. Try hyphenation
    std::vector<char> hyphens(widechar_len + 1);
    int mode = 0;
    lou_hyphenate(fuzz_str_4, inbuf.data(), static_cast<int>(widechar_len), 
                  hyphens.data(), fuzz_int32_t_5);
    
    // 9. Try back translation
    std::vector<widechar> outbuf(widechar_len * 2);  // Allow expansion
    int outlen = static_cast<int>(outbuf.size());
    int inlen = static_cast<int>(widechar_len);
    
    lou_backTranslateString(fuzz_str_6, inbuf.data(), &inlen, 
                           outbuf.data(), &outlen, nullptr, nullptr, mode);
    
    // 10. Try translatePrehyphenated with the hyphens we generated
    std::vector<char> inputHyphens(widechar_len + 1);
    std::vector<char> outputHyphens(widechar_len * 2 + 1);
    
    // Initialize with some pattern from data
    for (size_t i = 0; i < widechar_len && i < size; i++) {
        inputHyphens[i] = (data[i] % 2) ? '1' : '0';
    }
    
    // Reset lengths
    inlen = static_cast<int>(widechar_len);
    outlen = static_cast<int>(outbuf.size());
    
    lou_translatePrehyphenated(fuzz_str_7, inbuf.data(), &inlen,
                               outbuf.data(), &outlen, nullptr, nullptr,
                               nullptr, nullptr, nullptr,
                               inputHyphens.data(), outputHyphens.data(), mode);
    
    // 11. Try to use a FILE* with the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Could read from file if needed
        assert_file_closed(&in_file);;
    }
    
    // 12. Also try output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Could write some output
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    lou_free();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}