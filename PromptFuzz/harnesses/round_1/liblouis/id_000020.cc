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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["lou_logFile","lou_setDataPath","lou_checkTable","lou_getTable","lou_translate","lou_free","lou_backTranslate","lou_translateString","lou_charToDots","lou_dotsToChar","lou_version","lou_charSize","lou_getTableInfo","lou_freeTableInfo","lou_listTables","lou_freeTableFiles","lou_setDataPath","lou_logEnd","lou_free"],"critical_calls":["lou_logFile","lou_setDataPath","lou_checkTable","lou_getTable","lou_translate","lou_backTranslate","lou_translateString","lou_charToDots","lou_dotsToChar","lou_version","lou_charSize","lou_getTableInfo","lou_freeTableInfo","lou_listTables","lou_freeTableFiles","lou_setDataPath","lou_logEnd","lou_free"],"visited":0}
/*Looking at the liblouis APIs and requirements, I'll create a fuzz driver that tests a round-trip translation process. Here's my approach:

1. **Event**: Perform translation then back-translation using the same table
2. **Required APIs**: 
   - `lou_setDataPath` to set table path
   - `lou_logFile` for logging
   - `lou_getTable` to load table
   - `lou_translate` to convert text to braille
   - `lou_backTranslate` to convert braille back to text

*/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Need minimum data
    }

    // Setup logging first
    lou_logFile(fuzz_str_1);
    
    // Set data path for tables (use current directory)
    char *original_path = lou_setDataPath(fuzz_str_2);
    
    // Parse input data
    // Format: [table_name][null][text_data]
    const char *table_name = reinterpret_cast<const char*>(data);
    size_t table_name_len = 0;
    
    // Find null terminator for table name
    for (size_t i = 0; i < size && i < 256; i++) {
        if (data[i] == 0) {
            table_name_len = i;
            break;
        }
    }
    
    if (table_name_len == 0 || table_name_len + 1 >= size) {
        return 0; // Invalid format
    }
    
    // Check if table exists
    if (lou_checkTable(table_name) != 0) {
        return 0; // Invalid table
    }
    
    // Get the table
    const void *table = lou_getTable(table_name);
    if (!table) {
        return 0; // Failed to load table
    }
    
    // Prepare text input (remaining data after table name)
    const uint8_t *text_data = data + table_name_len + 1;
    size_t text_data_size = size - (table_name_len + 1);
    
    // Convert to widechar (UTF-16)
    size_t widechar_len = text_data_size / 2;
    if (widechar_len == 0) {
        return 0;
    }
    
    // Allocate buffers
    widechar *inbuf = new widechar[widechar_len];
    widechar *translated_buf = new widechar[widechar_len * 2]; // May expand
    widechar *backtranslated_buf = new widechar[widechar_len * 2];
    
    // Copy text data to widechar buffer
    memcpy(inbuf, text_data, widechar_len * sizeof(widechar));
    
    // Variables for translation
    int inlen = static_cast<int>(widechar_len);
    int outlen = widechar_len * 2;
    int back_inlen, back_outlen;
    formtype *typeform = nullptr;
    char *spacing = nullptr;
    int *outputPos = nullptr;
    int *inputPos = nullptr;
    int *cursorPos = nullptr;
    
    // Perform forward translation (text to braille)
    int trans_result = lou_translate(
        table_name,
        inbuf,
        &inlen,
        translated_buf,
        &outlen,
        typeform,
        spacing,
        outputPos,
        inputPos,
        cursorPos,
        fuzz_int32_t_3  // mode
    );
    
    if (trans_result == 0) {
        // Translation failed, clean up and exit
        delete[] inbuf;
        delete[] translated_buf;
        delete[] backtranslated_buf;
        lou_free();
        return 0;
    }
    
    // Now back-translate (braille to text)
    back_inlen = outlen;
    back_outlen = widechar_len * 2;
    
    int back_result = lou_backTranslate(
        table_name,
        translated_buf,
        &back_inlen,
        backtranslated_buf,
        &back_outlen,
        typeform,
        spacing,
        outputPos,
        inputPos,
        cursorPos,
        0  // mode
    );
    
    // Additional test: Try translating the same input as a string
    int string_inlen = static_cast<int>(widechar_len);
    int string_outlen = widechar_len * 2;
    int string_result = lou_translateString(
        table_name,
        inbuf,
        &string_inlen,
        translated_buf,
        &string_outlen,
        typeform,
        spacing,
        fuzz_int32_t_4
    );
    
    // Test charToDots and dotsToChar
    if (widechar_len > 0) {
        widechar *dots_buf = new widechar[widechar_len];
        widechar *chars_buf = new widechar[widechar_len];
        
        int dots_result = lou_charToDots(
            table_name,
            inbuf,
            dots_buf,
            static_cast<int>(widechar_len),
            0
        );
        
        if (dots_result > 0) {
            int chars_result = lou_dotsToChar(
                table_name,
                dots_buf,
                chars_buf,
                static_cast<int>(widechar_len),
                fuzz_int32_t_5
            );
        }
        
        delete[] dots_buf;
        delete[] chars_buf;
    }
    
    // Clean up
    delete[] inbuf;
    delete[] translated_buf;
    delete[] backtranslated_buf;
    
    // Test other utility functions
    const char *version = lou_version();
    int char_size = lou_charSize();
    
    // Try to get table info
    char *table_info = lou_getTableInfo(table_name, fuzz_str_6);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    // List tables (just call, ignore result)
    char **tables = lou_listTables();
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // Reset data path
    if (original_path) {
        lou_setDataPath(original_path);
        // Note: lou_setDataPath returns allocated memory that should be freed
        free(original_path);
    }
    
    // End logging and cleanup
    lou_logEnd();
    lou_free();
    
    return 0;
}