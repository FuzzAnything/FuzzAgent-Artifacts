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
//<ID> 655
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["lou_registerLogCallback","lou_logPrint","lou_version","lou_findTables","lou_getTypeformForEmphClass","lou_freeTableFiles","lou_free","lou_charToDots","lou_freeTableFiles","lou_free"],"critical_calls":["lou_registerLogCallback","lou_logPrint","lou_version","lou_findTables","lou_getTypeformForEmphClass","lou_charToDots","lou_freeTableFiles","lou_free"],"visited":0}
/**/


extern "C" {
}

// Log callback function
static void  id_000034_log_callback(logLevels level, const char *message) {
    // Do nothing in fuzzing
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for basic operations
    if (size < 2) {
        return 0;
    }

    // Register log callback first
    lou_registerLogCallback( id_000034_log_callback);

    // Use lou_logPrint with a fixed format
    lou_logPrint("Fuzzing liblouis version: %s", lou_version());

    // 1. Find tables using a query from input data
    // Extract a null-terminated query string from input
    size_t query_len = size < 100 ? size : 100;
    std::vector<char> query_vec(data, data + query_len);
    query_vec.push_back('\0'); // Ensure null termination
    const char *query = query_vec.data();

    char **tables = lou_findTables(query);
    
    // If tables were found, use the first one
    const char *table_to_use = NULL;
    if (tables && tables[0]) {
        table_to_use = tables[0];
    } else {
        // Fallback to a common table
        table_to_use = "en-us-comp8.ctb";
    }

    // 2. Get typeform for emphasis class
    // Extract emphasis class from input (different part than query)
    size_t emph_start = query_len < size ? query_len : 0;
    size_t emph_len = size - emph_start < 50 ? size - emph_start : 50;
    std::vector<char> emph_vec(data + emph_start, data + emph_start + emph_len);
    emph_vec.push_back('\0');
    const char *emph_class = emph_vec.data();

    formtype typeform = lou_getTypeformForEmphClass(table_to_use, emph_class);
    (void)typeform; // Use typeform to avoid unused variable warning

    // 3. Convert characters to dots using lou_charToDots
    // Use remaining input as widechar buffer
    size_t widechar_start = emph_start + emph_len;
    if (widechar_start >= size) {
        widechar_start = size / 2; // Fallback if we've consumed all input
    }
    
    size_t remaining = size - widechar_start;
    if (remaining < sizeof(widechar) * 2) {
        // Not enough data for conversion, clean up and return
        if (tables) {
            lou_freeTableFiles(tables);
        }
        lou_free();
        return 0;
    }

    // Convert input to widechars (2 bytes per widechar)
    size_t widechar_len = remaining / sizeof(widechar);
    const widechar *inbuf = reinterpret_cast<const widechar*>(data + widechar_start);
    
    // Prepare output buffer
    std::vector<widechar> outbuf(widechar_len * 2); // Allocate extra space
    
    // Call lou_charToDots
    int result = lou_charToDots(
        table_to_use,
        inbuf,
        outbuf.data(),
        static_cast<int>(widechar_len),
        0  // mode
    );
    
    (void)result; // Use result to avoid unused variable warning

    // 4. Clean up resources
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    lou_free();
    
    return 0;
}