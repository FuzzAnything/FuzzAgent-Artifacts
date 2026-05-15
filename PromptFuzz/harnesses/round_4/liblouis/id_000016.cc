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
//<ID> 450
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":4,"unique_branches":{"compileRule":[[3066,7,3066,13,0,0,4,0]]},"library_calls":["lou_registerTableResolver","lou_findTable","lou_freeTableFile","lou_freeTableFile","lou_charToDots","lou_hyphenate","lou_translatePrehyphenated","lou_freeTableFile","lou_free"],"critical_calls":["lou_registerTableResolver","lou_findTable","lou_charToDots","lou_hyphenate","lou_translatePrehyphenated","lou_freeTableFile","lou_free"],"visited":0}
/**/


// Include liblouis headers

// Custom table resolver function
static char ** id_000016_table_resolver(const char *table, const char *base) {
    // Simple resolver that returns the table name as-is
    static char *result[2];
    static char table_copy[256];
    
    if (table) {
        strncpy(table_copy, table, sizeof(table_copy) - 1);
        table_copy[sizeof(table_copy) - 1] = '\0';
        result[0] = table_copy;
        result[1] = nullptr;
        return result;
    }
    return nullptr;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 4) {
        return 0;
    }

    // 1. Register table resolver
    lou_registerTableResolver( id_000016_table_resolver);

    // 2. Find a table using part of the input as query
    // Use first 100 bytes or less as query string
    size_t query_len = size > 100 ? 100 : size;
    std::string query_str(reinterpret_cast<const char*>(data), query_len);
    char *table_name = lou_findTable(query_str.c_str());
    
    // If no table found, use a default one
    const char *table_to_use = table_name ? table_name : "en-us-g2.ctb";

    // 3. Prepare input buffer for processing
    // Skip the query part for the actual data processing
    size_t data_offset = query_len;
    if (data_offset >= size) {
        data_offset = 0;
    }
    
    size_t remaining_size = size - data_offset;
    if (remaining_size < 2) {
        if (table_name) lou_freeTableFile(table_name);
        return 0;
    }

    // Convert input data to widechar format (assuming UTF-16)
    // Each widechar is 2 bytes, so we need at least 2 bytes
    size_t max_widechars = remaining_size / 2;
    if (max_widechars < 1) {
        if (table_name) lou_freeTableFile(table_name);
        return 0;
    }

    std::vector<widechar> inbuf(max_widechars);
    const uint8_t* data_ptr = data + data_offset;
    
    // Convert bytes to widechars (simple byte pairing)
    for (size_t i = 0; i < max_widechars; i++) {
        inbuf[i] = (data_ptr[i * 2] << 8) | data_ptr[i * 2 + 1];
    }

    int inlen = static_cast<int>(max_widechars);
    int outlen = inlen * 2; // Allow for expansion

    // 4. Call lou_charToDots
    std::vector<widechar> dots_out(outlen);
    int dots_result = lou_charToDots(table_to_use, inbuf.data(), dots_out.data(), inlen, fuzz_int32_t_1);
    
    // 5. Call lou_hyphenate
    std::vector<char> hyphens(inlen);
    int hyphen_result = lou_hyphenate(table_to_use, inbuf.data(), inlen, hyphens.data(), fuzz_int32_t_2);

    // 6. Call lou_translatePrehyphenated
    std::vector<widechar> translate_out(outlen);
    std::vector<char> output_hyphens(outlen);
    std::vector<int> outputPos(outlen);
    std::vector<int> inputPos(outlen);
    std::vector<int> cursorPos(outlen);
    
    formtype typeform = 0;
    char spacing = 0;
    
    int translate_result = lou_translatePrehyphenated(
        table_to_use,
        inbuf.data(),
        &inlen,
        translate_out.data(),
        &outlen,
        &typeform,
        &spacing,
        outputPos.data(),
        inputPos.data(),
        cursorPos.data(),
        hyphens.data(),
        output_hyphens.data(),
        fuzz_int32_t_3
    );

    // Cleanup
    if (table_name) {
        lou_freeTableFile(table_name);
    }

    // Optional: Free any global resources
    lou_free();

    return 0;
}