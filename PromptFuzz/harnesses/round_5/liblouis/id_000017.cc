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
//<ID> 440
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["lou_setDataPath","lou_listTables","lou_checkTable","lou_freeTableFiles","lou_getTable","lou_getTableInfo","lou_findTable","lou_getTypeformForEmphClass","lou_freeTableInfo","lou_freeTableFile","lou_freeTableFiles","lou_dotsToChar","lou_backTranslateString","lou_backTranslate","lou_freeTableInfo","lou_freeTableFile","lou_freeTableFiles","lou_free"],"critical_calls":["lou_setDataPath","lou_listTables","lou_checkTable","lou_getTable","lou_getTableInfo","lou_findTable","lou_getTypeformForEmphClass","lou_dotsToChar","lou_backTranslateString","lou_backTranslate","lou_freeTableInfo","lou_freeTableFile","lou_freeTableFiles","lou_free"],"visited":0}
/**/


// Include liblouis headers (assuming they're available)
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for basic operations
    if (size < sizeof(widechar) * 2) {
	return 0;
	}
	

    // 1. Set data path first (using lou_setDataPath)
    char *old_path = lou_setDataPath(nullptr);  // Reset to default
    if (old_path) free(old_path);

    // 2. List available tables (using lou_listTables)
    char **tables = lou_listTables();
    if (!tables) {
	return 0;
	}
	
    
    // Find a valid table to use
    const char *table_name = nullptr;
    for (int i = 0; tables[i] != nullptr; i++) {
        if (lou_checkTable(tables[i]) == 0) {
            table_name = tables[i];
            break;
        }
    }
    
    if (!table_name) {
        lou_freeTableFiles(tables);
        return 0;
    }

    // 3. Get table handle (using lou_getTable)
    const void *table_handle = lou_getTable(table_name);

    // 4. Get table information (using lou_getTableInfo)
    char *table_locale = lou_getTableInfo(table_name, "locale");
    char *table_name_copy = strdup(table_name);  // Copy for lou_findTable

    // 5. Find table by query (using lou_findTable)
    char *found_table = lou_findTable(table_locale ? table_locale : "en");
    
    // Use found_table if available, otherwise use original
    const char *table_to_use = found_table ? found_table : table_name;

    // 6. Get typeform for emphasis class (using lou_getTypeformForEmphClass)
    formtype italic_typeform = lou_getTypeformForEmphClass(table_to_use, "italic");

    // 7. Convert input data to widechar array for processing
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count == 0) {
        lou_freeTableInfo(table_locale);
        free(table_name_copy);
        lou_freeTableFile(found_table);
        lou_freeTableFiles(tables);
        return 0;
    }

    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), data, widechar_count * sizeof(widechar));

    // 8. Perform dots-to-char conversion (using lou_dotsToChar)
    std::vector<widechar> dots_outbuf(widechar_count);
    int dots_result = lou_dotsToChar(table_to_use, inbuf.data(), 
                                     dots_outbuf.data(), 
                                     static_cast<int>(widechar_count), 0);

    // 9. Prepare for back-translation operations
    int inlen = static_cast<int>(widechar_count);
    int outlen = inlen * 2;  // Allow expansion
    std::vector<widechar> outbuf(outlen);
    std::vector<formtype> typeform_array(inlen, italic_typeform);
    std::vector<char> spacing(inlen, 0);
    std::vector<int> outputPos(inlen);
    std::vector<int> inputPos(inlen);
    std::vector<int> cursorPos(inlen);

    // 10. First back-translation (using lou_backTranslateString)
    int inlen1 = inlen;
    int outlen1 = outlen;
    int result1 = lou_backTranslateString(table_to_use, 
                                         inbuf.data(), &inlen1,
                                         outbuf.data(), &outlen1,
                                         typeform_array.data(),
                                         spacing.data(), 0);

    // 11. Second back-translation with positions (using lou_backTranslate)
    int inlen2 = inlen;
    int outlen2 = outlen;
    int result2 = lou_backTranslate(table_to_use,
                                   inbuf.data(), &inlen2,
                                   outbuf.data(), &outlen2,
                                   typeform_array.data(),
                                   spacing.data(),
                                   outputPos.data(),
                                   inputPos.data(),
                                   cursorPos.data(), 0);

    // 12. Create a FILE* from input data for hypothetical file operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Could read from this file if needed
        assert_file_closed(&in_file);;
    }

    // 13. Create output file (not strictly needed but following instructions)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        assert_file_closed(&out_file);;
    }

    // 14. Cleanup all allocated resources
    lou_freeTableInfo(table_locale);
    free(table_name_copy);
    lou_freeTableFile(found_table);
    lou_freeTableFiles(tables);
    
    // Optional: Free internal liblouis memory
    lou_free();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}