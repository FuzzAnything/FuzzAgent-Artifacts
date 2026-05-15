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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 8
//<Quality> {"density":8,"unique_branches":{"indexTablePath":[[1055,6,1055,24,0,0,4,0]],"compileString":[[4577,25,4577,44,0,0,4,0]],"for_selectRule":[[2042,7,2042,21,0,0,4,0]],"_lou_getTablePath":[[4725,22,4725,37,0,0,4,1],[4765,22,4765,42,0,0,4,0]],"_lou_translate":[[1250,30,1250,48,0,0,4,1],[1283,20,1283,38,0,0,4,1]],"back_selectRule":[[754,6,754,24,0,0,4,0]]},"library_calls":["lou_setDataPath","lou_getDataPath","lou_logPrint","lou_free","lou_findTables","lou_checkTable","lou_logPrint","lou_getTable","lou_logPrint","lou_compileString","lou_logPrint","lou_freeTableFiles","lou_free","lou_freeTableFiles","lou_free","lou_translateString","lou_logPrint","lou_backTranslateString","lou_logPrint","lou_freeTableFiles","lou_free"],"critical_calls":["lou_setDataPath","lou_getDataPath","lou_logPrint","lou_findTables","lou_checkTable","lou_logPrint","lou_getTable","lou_logPrint","lou_compileString","lou_logPrint","lou_translateString","lou_logPrint","lou_backTranslateString","lou_logPrint","lou_freeTableFiles","lou_free"],"visited":1}
/**/


extern "C" {
}

// Custom type declarations from liblouis
typedef unsigned short widechar;
typedef unsigned short formtype;

// Function declarations for liblouis APIs (already declared in liblouis.h)
// We'll use the actual header includes

// Log callback function if needed
static void  id_000026_logCallback(int level, const char *message) {
    // Optional: Can be implemented if needed
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if not enough data
    if (size < 4) {
        return 0;
    }

    // Set up data path - use first part of input as path string
    const char *data_path = reinterpret_cast<const char*>(data);
    size_t path_len = strnlen(data_path, size);
    if (path_len == 0) path_len = 1; // Ensure at least null terminator
    
    char *path_copy = static_cast<char*>(malloc(path_len + 1));
    if (!path_copy) {
	return 0;
	}
	
    memcpy(path_copy, data_path, path_len);
    path_copy[path_len] = '\0';
    
    char *old_path = lou_setDataPath(path_copy);
    free(path_copy);
    
    // Get current data path and log it
    char *current_path = lou_getDataPath();
    if (current_path) {
        lou_logPrint("Current data path: %s", current_path);
    }
    
    // Find tables using query from input data
    size_t query_offset = (path_len + 1 < size) ? path_len + 1 : size - 1;
    const char *query = reinterpret_cast<const char*>(data + query_offset);
    size_t query_len = strnlen(query, size - query_offset);
    if (query_len == 0) query_len = 1;
    
    char *query_copy = static_cast<char*>(malloc(query_len + 1));
    if (!query_copy) {
        lou_free();
        return 0;
    }
    memcpy(query_copy, query, query_len);
    query_copy[query_len] = '\0';
    
    char **tables = lou_findTables(query_copy);
    free(query_copy);
    
    // If tables found, use the first one for operations
    const char *table_to_use = nullptr;
    if (tables && tables[0]) {
        table_to_use = tables[0];
        
        // Check the table
        int check_result = lou_checkTable(table_to_use);
        if (check_result == 0) {
            lou_logPrint("Table %s is valid", table_to_use);
        }
        
        // Get the table (internal representation)
        const void *table_ptr = lou_getTable(table_to_use);
        if (table_ptr) {
            lou_logPrint("Got table pointer for %s", table_to_use);
        }
        
        // Compile a test string if we have enough input data
        size_t compile_offset = query_offset + query_len + 1;
        if (compile_offset < size) {
            const char *compile_str = reinterpret_cast<const char*>(data + compile_offset);
            size_t compile_len = strnlen(compile_str, size - compile_offset);
            if (compile_len > 0) {
                char *compile_copy = static_cast<char*>(malloc(compile_len + 1));
                if (compile_copy) {
                    memcpy(compile_copy, compile_str, compile_len);
                    compile_copy[compile_len] = '\0';
                    
                    int compile_result = lou_compileString(table_to_use, compile_copy);
                    if (compile_result == 0) {
                        lou_logPrint("Successfully compiled string for table %s", table_to_use);
                    }
                    
                    free(compile_copy);
                }
            }
        }
    }
    
    // Create a test widechar buffer from input data for translation
    // Use remaining data for translation buffer
    size_t widechar_offset = query_offset + query_len + 1;
    if (widechar_offset >= size) {
        if (tables) lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }
    
    size_t widechar_data_size = size - widechar_offset;
    if (widechar_data_size < sizeof(widechar) * 2) {
        if (tables) lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }
    
    // Convert to widechar array
    const widechar *widechar_data = reinterpret_cast<const widechar*>(data + widechar_offset);
    int inlen = static_cast<int>(widechar_data_size / sizeof(widechar));
    
    // Create output buffers
    std::vector<widechar> outbuf(inlen * 2); // Allocate extra space
    int outlen = static_cast<int>(outbuf.size());
    
    std::vector<widechar> back_outbuf(inlen * 2);
    int back_outlen = static_cast<int>(back_outbuf.size());
    
    // Perform translation if we have a valid table
    if (table_to_use) {
        int mode = 0;
        formtype *typeform = nullptr;
        char *spacing = nullptr;
        
        // Make copies of lengths since they will be modified
        int trans_inlen = inlen;
        int trans_outlen = outlen;
        
        int translate_result = lou_translateString(
            table_to_use,
            widechar_data,
            &trans_inlen,
            outbuf.data(),
            &trans_outlen,
            typeform,
            spacing,
            mode
        );
        
        if (translate_result != 0) {
            lou_logPrint("Translation successful, output length: %d", trans_outlen);
            
            // Try back-translation
            int back_inlen = trans_outlen;
            int back_trans_outlen = back_outlen;
            
            int back_translate_result = lou_backTranslateString(
                table_to_use,
                outbuf.data(),
                &back_inlen,
                back_outbuf.data(),
                &back_trans_outlen,
                typeform,
                spacing,
                mode
            );
            
            if (back_translate_result != 0) {
                lou_logPrint("Back-translation successful, output length: %d", back_trans_outlen);
            }
        }
    }
    
    // Clean up
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    lou_free();
    
    return 0;
}