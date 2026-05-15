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
//<ID> 432
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":3,"unique_branches":{"_lou_translate":[[1186,62,1186,76,0,0,4,0]]},"library_calls":["lou_registerLogCallback","lou_getDataPath","lou_listTables","lou_checkTable","lou_freeTableFiles","lou_findTables","lou_freeTableFiles","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_translateString","lou_translate","lou_free"],"critical_calls":["lou_registerLogCallback","lou_getDataPath","lou_listTables","lou_checkTable","lou_freeTableFiles","lou_findTables","lou_freeTableFiles","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_translateString","lou_translate","lou_free"],"visited":0}
/**/



// Log callback function to handle liblouis logging
static void  id_000025_logCallback(logLevels level, const char *message) {
    // Minimal logging to avoid slowing down fuzzing
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small to be meaningful
    if (size < 2) {
	return 0;
	}
	
    
    // Step 1: Register log callback first
    lou_registerLogCallback( id_000025_logCallback);
    
    // Step 2: Get data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        // Could potentially use the data path, but for fuzzing we just record it
        free(data_path);
    }
    
    // Step 3: List available tables
    char **tables_list = lou_listTables();
    if (!tables_list) {
	return 0;
	}
	  // No tables available
    
    // Find a valid table to use
    std::string table_to_use;
    for (int i = 0; tables_list[i] != nullptr; i++) {
        // Check if table is valid
        if (lou_checkTable(tables_list[i]) == 0) {
            table_to_use = tables_list[i];
            break;
        }
    }
    lou_freeTableFiles(tables_list);
    
    if (table_to_use.empty()) {
	return 0;
	}
	  // No valid tables found
    
    // Step 4: Use input data to find tables (treat first part of input as query)
    size_t query_len = size > 100 ? 100 : size;
    std::vector<char> query(query_len + 1);
    memcpy(query.data(), data, query_len);
    query[query_len] = '\0';
    
    char **found_tables = lou_findTables(query.data());
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }
    
    // Step 5: Get typeform for emphasis class (use a known emphasis class)
    const char **emph_classes = lou_getEmphClasses(table_to_use.c_str());
    formtype typeform = 0;
    if (emph_classes && emph_classes[0]) {
        typeform = lou_getTypeformForEmphClass(table_to_use.c_str(), emph_classes[0]);
    }
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    // Step 6: Prepare input buffer for translation
    // Convert data to widechar format (liblouis expects UTF-16/widechar)
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count == 0) {
	return 0;
	}
	
    
    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), data, widechar_count * sizeof(widechar));
    
    // Ensure the buffer is properly terminated for string operations
    if (widechar_count > 0 && inbuf[widechar_count - 1] != 0) {
        inbuf.push_back(0);
        widechar_count++;
    }
    
    int inlen = static_cast<int>(widechar_count - 1);  // Exclude null terminator
    
    // Step 7: Allocate output buffers
    std::vector<widechar> outbuf(inlen * 2);  // Output might be larger
    std::vector<char> spacing(inlen);
    std::vector<formtype> typeform_array(inlen, typeform);
    
    int outlen = static_cast<int>(outbuf.size());
    
    // Step 8: Try translateString first
    int result1 = lou_translateString(
        table_to_use.c_str(),
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform_array.data(),
        spacing.data(),
        0
    );
    
    // Reset for next translation
    inlen = static_cast<int>(widechar_count - 1);
    outlen = static_cast<int>(outbuf.size());
    
    // Step 9: Prepare position arrays for full translate
    std::vector<int> outputPos(inlen);
    std::vector<int> inputPos(inlen);
    std::vector<int> cursorPos(inlen);
    
    // Step 10: Try full translate function
    int result2 = lou_translate(
        table_to_use.c_str(),
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform_array.data(),
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        cursorPos.data(),
        0
    );
    
    // Step 11: Cleanup
    lou_free();
    
    // Use results to avoid optimization
    (void)result1;
    (void)result2;
    
    return 0;
}