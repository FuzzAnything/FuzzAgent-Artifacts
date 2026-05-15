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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["lou_registerLogCallback","lou_registerTableResolver","lou_listTables","lou_freeTableFiles","lou_findTable","lou_checkTable","lou_getTypeformForEmphClass","lou_translateString","lou_hyphenate","lou_charToDots","lou_freeTableFile","lou_free"],"critical_calls":["lou_registerLogCallback","lou_registerTableResolver","lou_listTables","lou_freeTableFiles","lou_findTable","lou_checkTable","lou_getTypeformForEmphClass","lou_translateString","lou_hyphenate","lou_charToDots","lou_freeTableFile","lou_free"],"visited":0}
/**/


// Include liblouis headers (adjust paths as needed)

// Dummy table resolver function
static char ** id_000013_dummy_table_resolver(const char *table_name, const char *base_name) {
    // Always return NULL to indicate no resolution
    return nullptr;
}

// Dummy log callback function
static void  id_000013_dummy_log_callback(logLevels level, const char *message) {
    // Do nothing with the log message
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
	return 0;
	}
	

    // 1. Register log callback
    lou_registerLogCallback( id_000013_dummy_log_callback);
    
    // 2. Register table resolver
    lou_registerTableResolver( id_000013_dummy_table_resolver);
    
    // 3. List available tables
    char **tables = lou_listTables();
    if (tables) {
        // Process table list (just iterate through them)
        for (int i = 0; tables[i] != nullptr; ++i) {
            // Can check or use table names if needed
        }
        // Free table list
        lou_freeTableFiles(tables);
    }
    
    // 4. Find a table using part of input as query
    std::string query;
    size_t query_len = std::min(size, (size_t)100);
    query.assign(reinterpret_cast<const char*>(data), query_len);
    char *found_table = lou_findTable(query.c_str());
    
    // Use default table if none found
    const char *table_list = found_table ? found_table : "en-us-g2.ctb";
    
    // 5. Check the table
    int table_check_result = lou_checkTable(table_list);
    (void)table_check_result; // Use result to avoid unused variable warning
    
    // 6. Prepare input buffer from remaining data
    size_t remaining_data = size - query_len;
    const uint8_t *input_data = data + query_len;
    
    // Convert input to widechars (assuming UTF-16/unsigned short)
    // We'll interpret the input as raw widechars
    std::vector<widechar> inbuf;
    if (remaining_data >= sizeof(widechar)) {
        size_t widechar_count = remaining_data / sizeof(widechar);
        inbuf.resize(widechar_count);
        memcpy(inbuf.data(), input_data, widechar_count * sizeof(widechar));
    } else {
        // If not enough data for widechars, create minimal buffer
        inbuf = {0x0041, 0x0042, 0x0043}; // "ABC"
    }
    
    // 7. Get typeform for emphasis class
    formtype typeform_val = lou_getTypeformForEmphClass(table_list, fuzz_str_1);
    
    // 8. Translate string
    int inlen = static_cast<int>(inbuf.size());
    int outlen = inlen * 4; // Allocate extra space for output
    std::vector<widechar> outbuf(outlen);
    std::vector<char> spacing(inlen);
    std::vector<formtype> typeforms(inlen, typeform_val);
    
    int translate_result = lou_translateString(
        table_list,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeforms.data(),
        spacing.data(),
        fuzz_int32_t_2
    );
    (void)translate_result;
    
    // 9. Hyphenate the input
    std::vector<char> hyphens(inbuf.size());
    int hyphenate_result = lou_hyphenate(
        table_list,
        inbuf.data(),
        static_cast<int>(inbuf.size()),
        hyphens.data(),
        fuzz_int32_t_3
    );
    (void)hyphenate_result;
    
    // 10. Convert characters to dots
    std::vector<widechar> dotsbuf(inbuf.size());
    int dots_result = lou_charToDots(
        table_list,
        inbuf.data(),
        dotsbuf.data(),
        static_cast<int>(inbuf.size()),
        0
    );
    (void)dots_result;
    
    // 11. Clean up
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    // Free any internal liblouis resources
    lou_free();
    
    return 0;
}