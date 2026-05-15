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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.3333335, nr_unique_branch: 13
//<Quality> {"density":2,"unique_branches":{"lou_indexTables":[[960,6,960,17,0,0,4,0]],"lou_compileString":[[5450,6,5450,12,0,0,4,0]],"lou_findTables":[[1118,6,1118,17,0,0,4,0],[1121,23,1121,24,0,0,4,1],[1131,6,1131,13,0,0,4,1]],"lou_translatePrehyphenated":[[1416,6,1416,26,0,0,4,0],[1417,7,1417,28,0,0,4,1],[1418,7,1418,23,0,0,4,1],[1424,6,1425,42,0,0,4,1],[1443,6,1443,28,0,0,4,1]],"analyzeTable":[[713,7,713,12,0,0,4,0]],"_lou_translate":[[1198,6,1198,19,0,0,4,0]],"compileFile":[[4913,6,4913,44,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_registerTableResolver","lou_readCharFromFile","lou_compileString","lou_indexTables","lou_findTables","lou_freeTableFiles","lou_getTableInfo","lou_freeTableInfo","lou_getTable","lou_free","lou_free","lou_translatePrehyphenated","lou_free"],"critical_calls":["lou_registerLogCallback","lou_registerTableResolver","lou_readCharFromFile","lou_compileString","lou_indexTables","lou_findTables","lou_freeTableFiles","lou_getTableInfo","lou_freeTableInfo","lou_getTable","lou_translatePrehyphenated","lou_free"],"visited":2}
/**/


extern "C" {
}

// Custom log callback for testing
static void  id_000001_test_log_callback(logLevels level, const char* message) {
    // Empty callback for fuzzing
    (void)level;
    (void)message;
}

// Custom table resolver for testing
static char**  id_000001_test_table_resolver(const char* table, const char* base) {
    (void)base;
    // Return minimal table data
    static char* table_data[] = {
        (char*)"table test",
        (char*)"include test.cti",
        (char*)"noback correct",
        NULL
    };
    return table_data;
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<59) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
        return 0;
    }

    // 1. Register log callback
    lou_registerLogCallback( id_000001_test_log_callback);
    
    // 2. Register table resolver
    lou_registerTableResolver( id_000001_test_table_resolver);
    
    // 3. Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Read and compile table from input
    int mode = 0;
    int table_result = lou_readCharFromFile("input_file", &mode);
    
    // Use first part of data as table string
    size_t table_len = size / 2;
    char* table_string = (char*)malloc(table_len + 1);
    if (!table_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    memcpy(table_string, data, table_len);
    table_string[table_len] = '\0';
    
    // 5. Compile table string
    int compile_result = lou_compileString("test_table", table_string);
    free(table_string);
    
    // 6. Index tables
    const char* tables[] = {"test_table", "en-us-comp8.ctb", NULL};
    lou_indexTables(tables);
    
    // 7. Find tables
    char** found_tables = lou_findTables(fuzz_str_1);
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }
    
    // 8. Get table info
    char* table_info = lou_getTableInfo(fuzz_str_2, fuzz_str_3);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    // 9. Get table handle
    const void* table_handle = lou_getTable(fuzz_str_4);
    
    // 10. Prepare translation buffers using second half of data
    size_t trans_len = size - (size / 2);
    if (trans_len < sizeof(widechar) * 2) {
        assert_file_closed(&in_file);;
        lou_free();
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Convert input to widechar buffer
    const uint8_t* trans_data = data + (size / 2);
    size_t widechar_count = trans_len / sizeof(widechar);
    if (widechar_count < 2) {
        assert_file_closed(&in_file);;
        lou_free();
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), trans_data, widechar_count * sizeof(widechar));
    
    int inlen = static_cast<int>(widechar_count);
    std::vector<widechar> outbuf(widechar_count * 2);
    int outlen = static_cast<int>(outbuf.size());
    
    std::vector<formtype> typeform(widechar_count, 0);
    std::vector<char> spacing(widechar_count, 0);
    std::vector<int> outputPos(outbuf.size(), 0);
    std::vector<int> inputPos(inbuf.size(), 0);
    std::vector<int> cursorPos(1, 0);
    std::vector<char> inputHyphens(inbuf.size(), 0);
    std::vector<char> outputHyphens(outbuf.size(), 0);
    
    // 11. Perform translation with hyphenation
    int translate_result = lou_translatePrehyphenated(
        fuzz_str_5,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform.data(),
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        cursorPos.data(),
        inputHyphens.data(),
        outputHyphens.data(),
        fuzz_int32_t_6
    );
    
    // 12. Clean up
    assert_file_closed(&in_file);;
    lou_free();
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}