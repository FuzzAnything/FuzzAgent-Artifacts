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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["lou_registerLogCallback","lou_charSize","lou_listTables","lou_free","lou_freeTableFiles","lou_free","lou_checkTable","lou_freeTableFiles","lou_free","lou_getTableInfo","lou_freeTableInfo","lou_findTable","lou_freeTableFile","lou_freeTableFiles","lou_free","lou_freeTableFiles","lou_free","lou_freeTableFiles","lou_free","lou_translateString","lou_dotsToChar","lou_freeTableFiles","lou_free"],"critical_calls":["lou_registerLogCallback","lou_charSize","lou_listTables","lou_checkTable","lou_getTableInfo","lou_freeTableInfo","lou_findTable","lou_freeTableFile","lou_translateString","lou_dotsToChar","lou_freeTableFiles","lou_free"],"visited":0}
/**/


// Log callback function
static void  id_000007_logCallback(logLevels level, const char* message) {
    // Empty callback - just consume the log message
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Register log callback
    lou_registerLogCallback( id_000007_logCallback);
    
    // Get character size
    int char_size = lou_charSize();
    if (char_size <= 0) {
        return 0;
    }
    
    // List available tables
    char** table_list = lou_listTables();
    if (!table_list) {
        lou_free();
        return 0;
    }
    
    // Find a table to use (first non-empty table from the list)
    const char* table_name = nullptr;
    for (int i = 0; table_list[i] != nullptr; ++i) {
        if (strlen(table_list[i]) > 0) {
            table_name = table_list[i];
            break;
        }
    }
    
    // If no valid table found, clean up and return
    if (!table_name) {
        lou_freeTableFiles(table_list);
        lou_free();
        return 0;
    }
    
    // Check the table
    int table_check_result = lou_checkTable(table_name);
    if (table_check_result != 0) {
        lou_freeTableFiles(table_list);
        lou_free();
        return 0;
    }
    
    // Get table information
    char* table_info = lou_getTableInfo(table_name, fuzz_str_1);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    // Try to find the table using lou_findTable
    char* found_table = lou_findTable(table_name);
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    // Calculate maximum number of wide characters we can process
    size_t max_widechars = size / sizeof(widechar);
    if (max_widechars == 0) {
        lou_freeTableFiles(table_list);
        lou_free();
        return 0;
    }
    
    // Allocate buffers
    widechar* inbuf = static_cast<widechar*>(malloc(size));
    if (!inbuf) {
        lou_freeTableFiles(table_list);
        lou_free();
        return 0;
    }
    
    // Copy data to input buffer
    memcpy(inbuf, data, size);
    
    // Prepare variables for translation
    int inlen = static_cast<int>(max_widechars);
    int outlen = inlen * 2; // Allow for expansion
    widechar* outbuf = static_cast<widechar*>(malloc(outlen * sizeof(widechar)));
    formtype* typeform = static_cast<formtype*>(calloc(inlen, sizeof(formtype)));
    char* spacing = static_cast<char*>(calloc(inlen, sizeof(char)));
    
    if (!outbuf || !typeform || !spacing) {
        free(inbuf);
        free(outbuf);
        free(typeform);
        free(spacing);
        lou_freeTableFiles(table_list);
        lou_free();
        return 0;
    }
    
    // Perform translation
    int translate_mode = 0; // Use default mode
    int translate_result = lou_translateString(
        table_name, inbuf, &inlen, outbuf, &outlen, typeform, spacing, fuzz_int32_t_2
    );
    
    // If translation succeeded, try dotsToChar conversion
    if (translate_result != 0 && outlen > 0) {
        // Allocate buffer for dotsToChar output
        widechar* dots_outbuf = static_cast<widechar*>(malloc(outlen * sizeof(widechar)));
        if (dots_outbuf) {
            // Convert dots to characters
            int dots_result = lou_dotsToChar(table_name, outbuf, dots_outbuf, outlen, fuzz_int32_t_3);
            (void)dots_result; // Result intentionally unused
            
            free(dots_outbuf);
        }
    }
    
    // Clean up all allocated resources
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    lou_freeTableFiles(table_list);
    lou_free();
    
    return 0;
}