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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 2.5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["lou_registerLogCallback","lou_registerTableResolver","lou_indexTables","lou_findTables","lou_getTableInfo","lou_freeTableInfo","lou_compileString","lou_getTable","lou_translatePrehyphenated","lou_free"],"critical_calls":["lou_registerLogCallback","lou_registerTableResolver","lou_indexTables","lou_findTables","lou_getTableInfo","lou_freeTableInfo","lou_compileString","lou_getTable","lou_translatePrehyphenated","lou_free"],"visited":1}
/**/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Early exit if input is too small for meaningful processing
    if (size < 4) {
	return 0;
	}
	

    // 1. Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Register a log callback (simple one that does nothing)
    lou_registerLogCallback([](logLevels level, const char *message) {
        // Empty callback to capture logs
    });

    // 3. Register a table resolver (simple one that returns NULL)
    lou_registerTableResolver([](const char *table, const char *base) -> char** {
        return nullptr;
    });

    // 4. Parse input to extract table list and translation data
    // First, read a null-terminated table list from the input
    std::vector<char> tableList;
    int ch;
    while ((ch = fgetc(in_file)) != EOF && ch != 0) {
        tableList.push_back(static_cast<char>(ch));
    }
    tableList.push_back('\0'); // Ensure null termination
    
    if (tableList.size() <= 1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0; // No table list found
    }

    // 5. Index tables using the table list
    const char *tables_array[] = {tableList.data(), nullptr};
    lou_indexTables(tables_array);

    // 6. Try to find tables using a query (use first few bytes as query)
    char query[256];
    size_t query_len = std::min(size_t(255), size);
    fseek(in_file, 0, SEEK_SET);
    fread(query, 1, query_len, in_file);
    query[query_len] = '\0';
    
    char **found_tables = lou_findTables(query);
    if (found_tables) {
        // Free the found tables list
        for (int i = 0; found_tables[i] != nullptr; i++) {
            free(found_tables[i]);
        }
        free(found_tables);
    }

    // 7. Get table info for the first table
    char *table_info = lou_getTableInfo(tableList.data(), fuzz_str_1);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }

    // 8. Compile a table string (use a subset of input as table definition)
    fseek(in_file, 0, SEEK_SET);
    std::vector<char> tableString;
    size_t table_string_size = std::min(size_t(1024), size);
    tableString.resize(table_string_size + 1);
    fread(tableString.data(), 1, table_string_size, in_file);
    tableString[table_string_size] = '\0';
    
    lou_compileString(tableList.data(), tableString.data());

    // 9. Get the table handle
    const void *table_handle = lou_getTable(tableList.data());

    // 10. Prepare translation buffers using remaining input data
    fseek(in_file, 0, SEEK_END);
    long remaining = ftell(in_file);
    fseek(in_file, -remaining, SEEK_CUR);
    
    // Read input buffer for translation
    std::vector<widechar> inbuf;
    while (remaining > 0 && (ch = fgetc(in_file)) != EOF) {
        inbuf.push_back(static_cast<widechar>(ch));
        remaining--;
    }
    
    if (inbuf.empty()) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Prepare output buffers
    int inlen = static_cast<int>(inbuf.size());
    int outlen = inlen * 2; // Conservative estimate
    std::vector<widechar> outbuf(outlen);
    std::vector<formtype> typeform(inbuf.size(), 0);
    std::vector<char> spacing(inbuf.size(), 0);
    std::vector<int> outputPos(outlen);
    std::vector<int> inputPos(inbuf.size());
    std::vector<int> cursorPos(1, 0);
    std::vector<char> inputHyphens(inbuf.size(), 0);
    std::vector<char> outputHyphens(outlen);

    // 11. Perform translation with hyphens
    lou_translatePrehyphenated(
        tableList.data(),
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
        fuzz_int32_t_2  // Default mode
    );

    // 12. Clean up
    assert_file_closed(&in_file);;
    
    // Free any remaining resources
    lou_free();

    assert_file_closed(&in_file);
	return 0;
}