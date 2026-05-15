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
//<ID> 500
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 2
//<Quality> {"density":6,"unique_branches":{"cmpMatches":[[1105,6,1105,43,0,0,4,0],[1105,6,1105,43,0,0,4,1]]},"library_calls":["lou_logFile","lou_logEnd","lou_free","lou_logEnd","lou_free","lou_logEnd","lou_free","lou_logEnd","lou_free","lou_checkTable","lou_compileString","lou_logEnd","lou_free","lou_logEnd","lou_free","lou_logEnd","lou_free","lou_charToDots","lou_translatePrehyphenated","lou_getEmphClasses","lou_freeEmphClasses","lou_findTables","lou_freeTableFiles","lou_findTable","lou_freeTableFile","lou_logEnd","lou_free"],"critical_calls":["lou_logFile","lou_checkTable","lou_compileString","lou_charToDots","lou_translatePrehyphenated","lou_getEmphClasses","lou_freeEmphClasses","lou_findTables","lou_freeTableFiles","lou_findTable","lou_freeTableFile","lou_logEnd","lou_free"],"visited":0}
/**/


// Include liblouis headers
extern "C" {
}

// Custom types from liblouis
typedef unsigned short formtype;
typedef unsigned short widechar;

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<62) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    // Early exit for too small inputs
    if (size < 4) {
        return 0;
    }

    // Step 1: Set up logging
    lou_logFile(fuzz_str_1);
    
    // Step 2: Create a temporary file from the input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        lou_logEnd();
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Read table list from the beginning of input
    // First 4 bytes as length of table list string
    uint32_t table_len = 0;
    if (fread(&table_len, sizeof(uint32_t), 1, in_file) != 1) {
        assert_file_closed(&in_file);;
        lou_logEnd();
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Validate table length
    table_len = std::min(table_len, (uint32_t)256); // Limit to reasonable size
    if (table_len == 0 || table_len > size - sizeof(uint32_t)) {
        assert_file_closed(&in_file);;
        lou_logEnd();
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read table list string
    std::vector<char> table_buf(table_len + 1);
    if (fread(table_buf.data(), 1, table_len, in_file) != table_len) {
        assert_file_closed(&in_file);;
        lou_logEnd();
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }
    table_buf[table_len] = '\0';
    const char *tableList = table_buf.data();
    
    // Step 4: Check if table is valid
    int table_check = lou_checkTable(tableList);
    if (table_check != 0) {
        // Table not valid, try to compile a simple table
        const char *simple_table = "include en-us-g2.ctb\n";
        lou_compileString(tableList, fuzz_str_2);
    }
    
    // Step 5: Read input data for processing
    // Determine how much data we have left
    long remaining = size - ftell(in_file);
    if (remaining <= 0) {
        assert_file_closed(&in_file);;
        lou_logEnd();
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read remaining data as wide characters
    size_t widechar_count = remaining / sizeof(widechar);
    if (widechar_count == 0) {
        assert_file_closed(&in_file);;
        lou_logEnd();
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }
    
    std::vector<widechar> inbuf(widechar_count);
    size_t read_count = fread(inbuf.data(), sizeof(widechar), widechar_count, in_file);
    assert_file_closed(&in_file);;
    
    if (read_count == 0) {
        lou_logEnd();
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }
    
    int inlen = static_cast<int>(read_count);
    
    // Step 6: Call lou_charToDots
    std::vector<widechar> dots_out(inlen);
    int dots_result = lou_charToDots(tableList, inbuf.data(), dots_out.data(), inlen, fuzz_int32_t_3);
    
    // Step 7: Prepare for lou_translatePrehyphenated
    int outlen = inlen * 2; // Conservative estimate
    std::vector<widechar> outbuf(outlen);
    std::vector<formtype> typeform(inlen, 0);
    std::vector<char> spacing(inlen, 0);
    std::vector<int> outputPos(inlen);
    std::vector<int> inputPos(inlen);
    std::vector<int> cursorPos(1, 0);
    std::vector<char> inputHyphens(inlen, 0);
    std::vector<char> outputHyphens(outlen, 0);
    
    // Step 8: Call lou_translatePrehyphenated
    int translate_result = lou_translatePrehyphenated(
        tableList,
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
        fuzz_int32_t_4
    );
    
    // Step 9: Get emphasis classes and free them
    const char **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        lou_freeEmphClasses(emphClasses);
    }
    
    // Step 10: Find and free table files
    char **tables = lou_findTables(tableList);
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // Step 11: Find and free a single table file
    char *singleTable = lou_findTable(fuzz_str_5);
    if (singleTable) {
        lou_freeTableFile(singleTable);
    }
    
    // Step 12: Clean up logging
    lou_logEnd();
    
    // Step 13: Free library resources
    lou_free();
    
    assert_file_closed(&in_file);
	return 0;
}