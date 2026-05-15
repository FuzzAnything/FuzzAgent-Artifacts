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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 61
//<Quality> {"density":2,"unique_branches":{"isSpace":[[531,21,531,30,0,0,4,0]],"compileString":[[4577,6,4577,11,0,0,4,1]],"parseQuery":[[552,7,552,18,0,0,4,0],[552,22,552,34,0,0,4,0],[553,8,553,11,0,0,4,1],[555,9,555,12,0,0,4,0],[560,9,560,11,0,0,4,1],[564,9,564,34,0,0,4,0],[564,9,564,34,0,0,4,1],[566,10,566,14,0,0,4,0],[566,10,566,14,0,0,4,1],[573,10,573,38,0,0,4,0],[573,10,573,38,0,0,4,1],[613,10,613,45,0,0,4,1],[620,8,620,18,0,0,4,0],[620,8,620,18,0,0,4,1],[621,14,621,23,0,0,4,0],[622,8,622,12,0,0,4,0],[622,8,622,12,0,0,4,1],[622,16,622,19,0,0,4,0],[622,16,622,19,0,0,4,1],[626,9,626,24,0,0,4,0],[626,9,626,24,0,0,4,1],[632,14,632,29,0,0,4,1],[633,8,633,11,0,0,4,0],[635,13,635,16,0,0,4,0],[645,6,645,19,0,0,4,0],[648,7,648,14,0,0,4,0],[664,22,664,23,0,0,4,0],[664,22,664,23,0,0,4,1]],"compileTable":[[5001,6,5001,22,0,0,4,1],[5003,6,5003,23,0,0,4,0],[5003,27,5003,40,0,0,4,1],[5004,6,5004,22,0,0,4,1],[5012,6,5012,22,0,0,4,1],[5015,6,5015,22,0,0,4,1],[5029,22,5029,38,0,0,4,1],[5042,7,5042,19,0,0,4,0],[5043,8,5043,65,0,0,4,0],[5072,7,5072,23,0,0,4,1]],"isValidChar":[[523,50,523,58,0,0,4,0]],"parseLanguageTag":[[298,10,298,18,0,0,4,1],[299,56,299,74,0,0,4,0],[301,7,301,14,0,0,4,0],[301,18,301,25,0,0,4,0],[301,42,301,57,0,0,4,0]],"_lou_resolveTable":[[4867,6,4867,49,0,0,4,1]],"getTable":[[5213,21,5213,42,0,0,4,0],[5217,7,5217,23,0,0,4,1]],"lou_indexTables":[[960,6,960,17,0,0,4,0]],"lou_registerTableResolver":[[4880,6,4880,15,0,0,4,1]],"isAlpha":[[268,48,268,56,0,0,4,1]],"lou_dotsToChar":[[4157,6,4157,23,0,0,4,1],[4157,27,4157,40,0,0,4,1],[4157,44,4157,58,0,0,4,1],[4160,6,4160,19,0,0,4,0]],"lou_findTable":[[1069,6,1069,17,0,0,4,0],[1074,23,1074,24,0,0,4,1],[1084,6,1084,15,0,0,4,1]],"compileCharDef":[[2661,6,2661,11,0,0,4,1],[2677,6,2677,11,0,0,4,1]]},"library_calls":["lou_setLogLevel","lou_registerTableResolver","lou_getDataPath","lou_findTable","lou_getTable","lou_indexTables","lou_readCharFromFile","lou_dotsToChar","lou_freeTableFile","lou_free"],"critical_calls":["lou_setLogLevel","lou_registerTableResolver","lou_getDataPath","lou_findTable","lou_getTable","lou_indexTables","lou_readCharFromFile","lou_dotsToChar","lou_freeTableFile","lou_free"],"visited":1}
/**/


extern "C" {
}

// Custom table resolver function
char ** id_000004_table_resolver(const char *table_name, const char *base) {
    // This resolver simply returns NULL for simplicity
    // In a real scenario, you might resolve table files differently
    return NULL;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;  // Need some data to work with
    }

    // 1. Set log level first (assuming logLevels enum exists)
    lou_setLogLevel(static_cast<logLevels>(0));  // Using 0 as default log level

    // 2. Register a table resolver
    lou_registerTableResolver( id_000004_table_resolver);

    // 3. Get current data path
    char *data_path = lou_getDataPath();

    // 4. Find a table using the query - use part of input as query
    // Use first 100 bytes or less for query
    size_t query_len = size > 100 ? 100 : size;
    char *query = static_cast<char*>(malloc(query_len + 1));
    if (!query) {
        if (data_path) free(data_path);
        return 0;
    }
    memcpy(query, data, query_len);
    query[query_len] = '\0';
    
    char *found_table = lou_findTable(query);
    free(query);
    query = NULL;

    // 5. Get table if found
    const void *table = NULL;
    if (found_table) {
        table = lou_getTable(found_table);
    }

    // 6. Index tables if we found one
    if (found_table) {
        const char *tables[] = {found_table, NULL};
        lou_indexTables(tables);
    }

    // 7. Read a character from the input file
    // First, write input to a file
    FILE *in_file = fopen("input_file", "wb");
    if (in_file) {
        fwrite(data, 1, size, in_file);
        assert_file_closed(&in_file);;
        
        int mode = 0;
        int char_read = lou_readCharFromFile("input_file", &mode);
        // Remove the file after reading
        remove("input_file");
    }

    // 8. Use lou_dotsToChar - convert part of input to characters
    // Allocate buffers for conversion
    size_t dots_len = size / sizeof(widechar);
    if (dots_len > 0) {
        std::vector<widechar> inbuf(dots_len);
        std::vector<widechar> outbuf(dots_len);
        
        // Copy data to input buffer
        memcpy(inbuf.data(), data, dots_len * sizeof(widechar));
        
        // Use a simple table for conversion if available
        const char *table_list = found_table ? found_table : "en-us-g2.ctb";
        int result = lou_dotsToChar(
            table_list,
            inbuf.data(),
            outbuf.data(),
            dots_len,
            0  // mode
        );
    }

    // 9. Clean up resources
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    if (data_path) {
        free(data_path);
    }
    
    // 10. Final cleanup
    lou_free();

    assert_file_closed(&in_file);
	return 0;
}