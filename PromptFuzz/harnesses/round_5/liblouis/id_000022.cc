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
//<ID> 538
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"lou_registerTableResolver":[[4880,6,4880,15,0,0,4,0]]},"library_calls":["lou_findTables","lou_registerTableResolver","lou_checkTable","lou_logPrint","lou_dotsToChar","lou_logPrint","lou_backTranslate","lou_logPrint","lou_freeTableFiles","lou_logPrint","lou_readCharFromFile","lou_logPrint","lou_free"],"critical_calls":["lou_findTables","lou_registerTableResolver","lou_checkTable","lou_logPrint","lou_dotsToChar","lou_logPrint","lou_backTranslate","lou_logPrint","lou_freeTableFiles","lou_readCharFromFile","lou_logPrint","lou_free"],"visited":1}
/**/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 2) {
        return 0;
    }

    // 1. First, try to find available tables using lou_findTables
    // Use part of the input as query for table search
    size_t query_len = size > 100 ? 100 : size;
    char *query = static_cast<char*>(malloc(query_len + 1));
    if (!query) {
	return 0;
	}
	
    
    memcpy(query, data, query_len);
    query[query_len] = '\0';
    
    char **found_tables = lou_findTables(query);
    free(query);
    
    // 2. Register a table resolver (even if we don't actually use it)
    lou_registerTableResolver(nullptr);
    
    // 3. If we found tables, use the first one for processing
    const char *table_to_use = nullptr;
    if (found_tables && found_tables[0]) {
        table_to_use = found_tables[0];
        
        // 4. Check the table
        int check_result = lou_checkTable(table_to_use);
        
        // Log the check result
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Table check result: %d", check_result);
        lou_logPrint(log_msg);
        
        // 5. Prepare input for dotsToChar conversion
        // We'll use part of the input data as braille dots
        size_t dots_len = (size - query_len) / 2;
        if (dots_len > 0) {
            widechar *inbuf = static_cast<widechar*>(malloc(dots_len * sizeof(widechar)));
            widechar *outbuf = static_cast<widechar*>(malloc(dots_len * sizeof(widechar)));
            
            if (inbuf && outbuf) {
                // Convert byte data to widechar for dotsToChar
                for (size_t i = 0; i < dots_len; i++) {
                    inbuf[i] = static_cast<widechar>(data[query_len + i]);
                }
                
                // Perform dots to char conversion
                int mode = 0; // Default mode
                int dots_result = lou_dotsToChar(table_to_use, inbuf, outbuf, dots_len, mode);
                
                // Log the conversion result
                snprintf(log_msg, sizeof(log_msg), "DotsToChar result: %d", dots_result);
                lou_logPrint(log_msg);
                
                // 6. Now try backtranslate with the converted output
                if (dots_result > 0) {
                    int inlen = dots_result; // Use actual conversion result
                    int outlen = dots_len;
                    
                    // Allocate buffers for backtranslate
                    widechar *back_inbuf = static_cast<widechar*>(malloc(dots_result * sizeof(widechar)));
                    widechar *back_outbuf = static_cast<widechar*>(malloc(dots_len * sizeof(widechar)));
                    
                    if (back_inbuf && back_outbuf) {
                        // Copy the converted characters for backtranslation
                        memcpy(back_inbuf, outbuf, dots_result * sizeof(widechar));
                        
                        // Additional parameters for backtranslate
                        formtype *typeform = nullptr;
                        char *spacing = nullptr;
                        int *outputPos = nullptr;
                        int *inputPos = nullptr;
                        int *cursorPos = nullptr;
                        
                        // Perform back translation
                        int back_result = lou_backTranslate(
                            table_to_use, 
                            back_inbuf, 
                            &inlen, 
                            back_outbuf, 
                            &outlen, 
                            typeform, 
                            spacing, 
                            outputPos, 
                            inputPos, 
                            cursorPos, 
                            mode
                        );
                        
                        // Log back translation result
                        snprintf(log_msg, sizeof(log_msg), "BackTranslate result: %d", back_result);
                        lou_logPrint(log_msg);
                        
                        free(back_inbuf);
                        free(back_outbuf);
                    }
                }
                
                free(inbuf);
                free(outbuf);
            }
        }
        
        // 7. Clean up found tables
        if (found_tables) {
            lou_freeTableFiles(found_tables);
        }
    } else {
        // If no tables found, just log a message
        lou_logPrint("No tables found with the given query");
    }
    
    // 8. Additional test: Try to use the input as a file for lou_readCharFromFile
    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Create a temporary filename
        const char *temp_filename = "fuzz_input.tmp";
        FILE *temp_file = fopen(temp_filename, "wb");
        if (temp_file) {
            fwrite(data, 1, size, temp_file);
            assert_file_closed(&temp_file);;
            
            // Try to read character from the file
            int mode = 0;
            int read_result = lou_readCharFromFile(temp_filename, &mode);
            
            // Log the result
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "ReadCharFromFile result: %d, mode: %d", read_result, mode);
            lou_logPrint(log_msg);
            
            // Remove temporary file
            remove(temp_filename);
        }
        assert_file_closed(&in_file);;
    }
    
    // 9. Final cleanup
    lou_free();
    
    assert_file_closed(&in_file);
	return 0;
}