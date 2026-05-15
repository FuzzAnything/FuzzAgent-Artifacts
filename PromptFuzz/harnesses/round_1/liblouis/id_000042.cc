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
//<ID> 1104
//<Prompt> []
/*<Combination>: [
*/
//<score> 17.5, nr_unique_branch: 81
//<Quality> {"density":5,"unique_branches":{"back_selectRule":[[729,8,729,37,0,0,4,0],[746,6,746,22,0,0,4,0],[747,11,748,63,0,0,4,1]],"findBackPassRule":[[646,9,646,19,0,0,4,0],[651,3,651,19,0,0,4,0],[651,3,651,19,0,0,4,1],[652,8,652,24,0,0,4,1],[654,3,654,19,0,0,4,0],[654,3,654,19,0,0,4,1],[655,8,655,24,0,0,4,1],[657,3,657,17,0,0,4,0],[657,3,657,17,0,0,4,1],[658,8,658,24,0,0,4,1],[660,3,660,17,0,0,4,1],[663,3,663,17,0,0,4,1],[666,3,666,10,0,0,4,1],[670,7,671,38,0,0,4,0],[670,7,671,38,0,0,4,1]],"back_passDoAction":[[1739,6,1740,66,0,0,4,1],[1744,31,1744,51,0,0,4,0],[1744,31,1744,51,0,0,4,1],[1746,9,1746,38,0,0,4,0],[1746,9,1746,38,0,0,4,1],[1747,3,1747,19,0,0,4,1],[1748,3,1748,17,0,0,4,1],[1756,3,1756,17,0,0,4,1],[1762,3,1762,17,0,0,4,1],[1765,3,1765,17,0,0,4,0],[1765,3,1765,17,0,0,4,1],[1767,8,1767,17,0,0,4,1],[1775,8,1777,21,0,0,4,1],[1782,3,1782,10,0,0,4,0],[1782,3,1782,10,0,0,4,1],[1783,8,1783,64,0,0,4,0]],"lou_listTables":[[1181,6,1181,17,0,0,4,1]],"back_passDoTest":[[1599,6,1599,34,0,0,4,0],[1599,6,1599,34,0,0,4,1],[1603,9,1603,39,0,0,4,0],[1605,7,1605,26,0,0,4,1],[1607,3,1607,18,0,0,4,1],[1611,3,1611,17,0,0,4,1],[1615,3,1615,21,0,0,4,0],[1615,3,1615,21,0,0,4,1],[1617,8,1617,15,0,0,4,0],[1617,8,1617,15,0,0,4,1],[1623,3,1623,16,0,0,4,0],[1623,3,1623,16,0,0,4,1],[1627,3,1627,19,0,0,4,0],[1627,3,1627,19,0,0,4,1],[1628,3,1628,17,0,0,4,0],[1628,3,1628,17,0,0,4,1],[1633,3,1633,25,0,0,4,0],[1633,3,1633,25,0,0,4,1],[1637,3,1637,23,0,0,4,1],[1641,3,1641,23,0,0,4,0],[1641,3,1641,23,0,0,4,1],[1649,16,1649,52,0,0,4,0],[1649,16,1649,52,0,0,4,1],[1650,9,1650,29,0,0,4,1],[1654,9,1657,19,0,0,4,0],[1654,9,1657,19,0,0,4,1],[1654,12,1654,13,0,0,4,0],[1654,12,1654,13,0,0,4,1],[1663,8,1663,15,0,0,4,0],[1663,8,1663,15,0,0,4,1],[1665,7,1665,43,0,0,4,1],[1677,3,1677,17,0,0,4,1],[1681,3,1681,20,0,0,4,0],[1681,3,1681,20,0,0,4,1],[1684,8,1684,33,0,0,4,0],[1691,3,1691,10,0,0,4,0],[1691,3,1691,10,0,0,4,1],[1692,8,1692,72,0,0,4,0],[1695,8,1695,20,0,0,4,0],[1695,8,1695,20,0,0,4,1],[1695,24,1695,32,0,0,4,0],[1695,24,1695,32,0,0,4,1],[1695,38,1695,49,0,0,4,0],[1695,38,1695,49,0,0,4,1],[1695,53,1695,60,0,0,4,0]],"analyzeTable":[[735,33,735,52,0,0,4,0]]},"library_calls":["lou_registerLogCallback","lou_findTables","lou_listTables","lou_checkTable","lou_checkTable","lou_freeTableFiles","lou_freeTableFiles","lou_free","lou_getTableInfo","lou_freeTableInfo","lou_freeTableFiles","lou_freeTableFiles","lou_free","lou_backTranslate","lou_freeTableFiles","lou_freeTableFiles","lou_free"],"critical_calls":["lou_registerLogCallback","lou_findTables","lou_listTables","lou_checkTable","lou_getTableInfo","lou_freeTableInfo","lou_backTranslate","lou_freeTableFiles","lou_freeTableFiles","lou_free"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs to achieve back-translation of braille output:

*/


extern "C" {
}

// Simple log callback function
static void  id_000042_fuzz_log_callback(logLevels level, const char *msg) {
    // Minimal logging to avoid overhead during fuzzing
    (void)level;
    (void)msg;
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<11) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < sizeof(widechar) * 2) {
        return 0;
    }

    // 1. Register log callback
    lou_registerLogCallback( id_000042_fuzz_log_callback);

    // 2. Find tables using the input data as query
    // Use first few bytes as query string (null-terminated)
    size_t query_len = size > 100 ? 100 : size;
    char *query = static_cast<char*>(malloc(query_len + 1));
    if (!query) {
	return 0;
	}
	
    
    memcpy(query, data, query_len);
    query[query_len] = '\0';
    
    char **found_tables = lou_findTables(query);
    free(query);
    
    // 3. List all available tables
    char **all_tables = lou_listTables();
    
    // 4. Check a table if we found one
    const char *table_to_use = NULL;
    if (found_tables && found_tables[0]) {
        table_to_use = found_tables[0];
        lou_checkTable(table_to_use);
    } else if (all_tables && all_tables[0]) {
        table_to_use = all_tables[0];
        lou_checkTable(table_to_use);
    }
    
    if (!table_to_use) {
        // Cleanup and return if no tables available
        if (found_tables) lou_freeTableFiles(found_tables);
        if (all_tables) lou_freeTableFiles(all_tables);
        lou_free();
        return 0;
    }
    
    // 5. Get table info
    char *table_info = lou_getTableInfo(table_to_use, fuzz_str_1);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    // 6. Prepare input buffer for back-translation
    // The input data represents braille output that we want to back-translate
    size_t max_chars = size / sizeof(widechar);
    if (max_chars < 1) {
        if (found_tables) lou_freeTableFiles(found_tables);
        if (all_tables) lou_freeTableFiles(all_tables);
        lou_free();
        return 0;
    }
    
    // Convert byte stream to widechar array (simulating braille output)
    std::vector<widechar> inbuf(max_chars);
    size_t chars_to_copy = max_chars;
    if (chars_to_copy * sizeof(widechar) > size) {
        chars_to_copy = size / sizeof(widechar);
    }
    memcpy(inbuf.data(), data, chars_to_copy * sizeof(widechar));
    
    // 7. Setup parameters for back-translation
    int inlen = static_cast<int>(chars_to_copy);
    std::vector<widechar> outbuf(inlen * 2); // Allocate enough space
    int outlen = static_cast<int>(outbuf.size());
    
    // Optional parameters (can be NULL if not needed)
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL;
    int *inputPos = NULL;
    int *cursorPos = NULL;
    
    // Mode for back-translation
    int mode = 0;
    
    // 8. Perform back-translation
    int result = lou_backTranslate(table_to_use, 
                                   inbuf.data(), 
                                   &inlen, 
                                   outbuf.data(), 
                                   &outlen, 
                                   typeform, 
                                   spacing, 
                                   outputPos, 
                                   inputPos, 
                                   cursorPos, 
                                   mode);
    
    // 9. Clean up resources
    if (found_tables) lou_freeTableFiles(found_tables);
    if (all_tables) lou_freeTableFiles(all_tables);
    
    // 10. Free liblouis resources
    lou_free();
    
    // Use the result to avoid dead code elimination
    if (result == 0) {
        // Back-translation failed, but that's OK for fuzzing
        return 0;
    }
    
    return 0;
}