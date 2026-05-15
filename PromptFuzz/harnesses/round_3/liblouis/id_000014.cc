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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["lou_getDataPath","lou_registerTableResolver","lou_indexTables","lou_findTable","lou_freeTableFile","lou_translate","lou_charToDots","lou_dotsToChar","lou_translatePrehyphenated","lou_free"],"critical_calls":["lou_getDataPath","lou_registerTableResolver","lou_indexTables","lou_findTable","lou_freeTableFile","lou_translate","lou_charToDots","lou_dotsToChar","lou_translatePrehyphenated","lou_free"],"visited":0}
/**/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 4) {
	return 0;
	}
	

    // 1. Initialize liblouis and use lou_getDataPath()
    char *data_path = lou_getDataPath();
    if (data_path) {
        free(data_path); // liblouis allocates with malloc
    }

    // 2. Register a table resolver (minimal implementation)
    lou_registerTableResolver([](const char *table, const char *base) -> char** {
        return nullptr; // Simple null resolver for fuzzing
    });

    // 3. Index tables - create a table list from input data
    // Use first 2 bytes to determine table name length (max 64)
    size_t table_name_len = (data[0] % 64) + 1;
    if (table_name_len > size - 1) table_name_len = size - 1;
    
    std::vector<char> table_list(table_name_len + 1);
    for (size_t i = 0; i < table_name_len; ++i) {
        table_list[i] = (data[i + 1] % 26) + 'a'; // Convert to lowercase letters
    }
    table_list[table_name_len] = '\0';
    
    const char *tables[] = {table_list.data(), nullptr};
    lou_indexTables(tables);

    // 4. Find table using the generated name
    char *found_table = lou_findTable(table_list.data());
    if (found_table) {
        lou_freeTableFile(found_table);
    }

    // 5. Prepare input buffer for translation
    // Use remaining data for widechar input
    size_t data_offset = table_name_len + 1;
    if (data_offset >= size) {
	return 0;
	}
	
    
    size_t remaining = size - data_offset;
    size_t widechar_count = remaining / sizeof(widechar);
    if (widechar_count == 0) {
	return 0;
	}
	
    
    std::vector<widechar> inbuf(widechar_count);
    std::vector<widechar> outbuf(widechar_count * 2); // Output buffer
    std::vector<widechar> dotsbuf(widechar_count * 2); // Dots buffer
    
    // Copy data to input buffer
    memcpy(inbuf.data(), data + data_offset, std::min(remaining, widechar_count * sizeof(widechar)));
    
    int inlen = static_cast<int>(widechar_count);
    int outlen = static_cast<int>(outbuf.size());
    int dotslen = static_cast<int>(dotsbuf.size());
    
    // 6. Perform translation using lou_translate()
    int outputPos[100], inputPos[100], cursorPos[100];
    formtype typeform[100] = {0};
    char spacing[100] = {0};
    
    lou_translate(table_list.data(), inbuf.data(), &inlen, 
                  outbuf.data(), &outlen, typeform, spacing,
                  outputPos, inputPos, cursorPos, fuzz_int32_t_1);
    
    // 7. Convert characters to dots using lou_charToDots()
    lou_charToDots(table_list.data(), inbuf.data(), dotsbuf.data(), 
                   static_cast<int>(widechar_count), fuzz_int32_t_2);
    
    // 8. Convert dots back to characters using lou_dotsToChar()
    std::vector<widechar> backbuf(widechar_count);
    lou_dotsToChar(table_list.data(), dotsbuf.data(), backbuf.data(),
                   static_cast<int>(widechar_count), fuzz_int32_t_3);
    
    // 9. Perform pre-hyphenated translation
    // Prepare hyphenation arrays
    std::vector<char> inputHyphens(widechar_count);
    std::vector<char> outputHyphens(widechar_count * 2);
    
    // Initialize hyphens from input data
    for (size_t i = 0; i < widechar_count && i < 100; ++i) {
        inputHyphens[i] = (data[(data_offset + i) % size] % 2) ? '1' : '0';
    }
    
    int inlen2 = static_cast<int>(widechar_count);
    int outlen2 = static_cast<int>(outbuf.size());
    
    lou_translatePrehyphenated(table_list.data(), inbuf.data(), &inlen2,
                               outbuf.data(), &outlen2, typeform, spacing,
                               outputPos, inputPos, cursorPos,
                               inputHyphens.data(), outputHyphens.data(), fuzz_int32_t_4);
    
    // 10. Clean up
    lou_free();
    
    return 0;
}