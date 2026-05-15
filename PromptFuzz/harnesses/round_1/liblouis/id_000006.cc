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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 196
//<Quality> {"density":6,"unique_branches":{"passGetName":[[1826,11,1826,42,0,0,4,1]],"hyphenHashFree":[[2461,14,2461,32,0,0,4,0],[2461,14,2461,32,0,0,4,1],[2462,33,2462,34,0,0,4,0],[2462,33,2462,34,0,0,4,1]],"hyphenHashInsert":[[2476,6,2476,44,0,0,4,1],[2479,6,2479,13,0,0,4,1],[2481,14,2481,29,0,0,4,0],[2481,14,2481,29,0,0,4,1]],"freeTranslationTable":[[4936,6,4936,18,0,0,4,0]],"getTable":[[5177,9,5177,26,0,0,4,0],[5200,9,5200,26,0,0,4,0]],"hyphenGetNewState":[[2506,6,2506,48,0,0,4,0],[2506,6,2506,48,0,0,4,1],[2509,6,2509,19,0,0,4,1]],"addRule":[[1035,6,1035,26,0,0,4,0],[1035,30,1035,50,0,0,4,0],[1035,54,1035,74,0,0,4,0]],"compileHyphenation":[[2554,6,2554,18,0,0,4,1],[2560,7,2560,32,0,0,4,1],[2564,8,2564,36,0,0,4,0],[2564,8,2564,36,0,0,4,1],[2567,7,2567,23,0,0,4,1],[2567,27,2567,47,0,0,4,1],[2567,51,2567,71,0,0,4,0],[2567,51,2567,71,0,0,4,1],[2568,5,2568,25,0,0,4,1],[2572,15,2572,30,0,0,4,0],[2572,15,2572,30,0,0,4,1],[2573,8,2573,28,0,0,4,0],[2573,8,2573,28,0,0,4,1],[2573,32,2573,52,0,0,4,0],[2573,32,2573,52,0,0,4,1],[2583,15,2583,32,0,0,4,0],[2583,15,2583,32,0,0,4,1],[2585,7,2585,28,0,0,4,0],[2585,7,2585,28,0,0,4,1],[2590,7,2590,12,0,0,4,0],[2597,10,2597,31,0,0,4,0],[2597,10,2597,31,0,0,4,1],[2601,8,2601,29,0,0,4,0],[2601,8,2601,29,0,0,4,1],[2607,11,2607,30,0,0,4,0],[2607,11,2607,30,0,0,4,1],[2609,14,2609,32,0,0,4,0],[2609,14,2609,32,0,0,4,1],[2610,33,2610,34,0,0,4,0],[2610,33,2610,34,0,0,4,1],[2611,16,2611,35,0,0,4,0],[2613,17,2613,35,0,0,4,0],[2613,17,2613,35,0,0,4,1],[2616,9,2616,33,0,0,4,0],[2616,9,2616,33,0,0,4,1],[2618,8,2618,14,0,0,4,0],[2623,14,2623,32,0,0,4,0],[2623,14,2623,32,0,0,4,1],[2624,7,2624,35,0,0,4,0],[2624,7,2624,35,0,0,4,1]],"finalizeCharacter":[[4419,6,4421,36,0,0,4,1],[4423,9,4423,46,0,0,4,0],[4474,7,4474,23,0,0,4,0]],"hyphenHashLookup":[[2491,6,2491,22,0,0,4,0],[2491,6,2491,22,0,0,4,1],[2493,32,2493,33,0,0,4,0],[2493,32,2493,33,0,0,4,1],[2494,7,2494,36,0,0,4,0],[2494,7,2494,36,0,0,4,1],[2495,15,2495,30,0,0,4,0],[2495,15,2495,30,0,0,4,1],[2496,8,2496,41,0,0,4,0],[2496,8,2496,41,0,0,4,1],[2497,7,2497,23,0,0,4,0],[2497,7,2497,23,0,0,4,1]],"getNextAttribute":[[1096,6,1096,10,0,0,4,1]],"parseCharsInternal":[[1301,5,1301,12,0,0,4,0]],"addRuleName":[[1595,6,1595,77,0,0,4,1],[1601,14,1601,30,0,0,4,0],[1601,14,1601,30,0,0,4,1],[1603,8,1603,16,0,0,4,0],[1603,8,1603,16,0,0,4,1],[1603,20,1603,28,0,0,4,0],[1603,34,1603,42,0,0,4,0],[1603,46,1603,54,0,0,4,0]],"compileRule":[[2884,7,2884,12,0,0,4,0],[3233,9,3233,24,0,0,4,1],[3251,35,3251,59,0,0,4,0],[3252,11,3252,42,0,0,4,0],[3283,5,3283,11,0,0,4,0],[3284,10,3284,73,0,0,4,1],[3291,10,3293,43,0,0,4,1],[3292,9,3292,15,0,0,4,0],[3292,9,3292,15,0,0,4,1],[3405,9,3405,52,0,0,4,0],[3632,4,3632,26,0,0,4,0],[3633,9,3633,44,0,0,4,1],[3636,19,3636,42,0,0,4,0],[3636,46,3636,64,0,0,4,1],[3637,9,3637,51,0,0,4,1],[3642,21,3642,41,0,0,4,0],[3642,21,3642,41,0,0,4,1],[3643,10,3643,52,0,0,4,1],[3652,4,3652,24,0,0,4,0],[3653,9,3653,44,0,0,4,1],[3656,19,3656,40,0,0,4,0],[3656,44,3656,60,0,0,4,1],[3657,9,3657,49,0,0,4,1],[3662,21,3662,41,0,0,4,0],[3662,21,3662,41,0,0,4,1],[3663,10,3663,52,0,0,4,1],[3911,3,3911,19,0,0,4,0],[3930,8,3930,15,0,0,4,0],[3938,3,3938,22,0,0,4,0],[3939,8,3939,43,0,0,4,1],[3941,8,3941,51,0,0,4,1],[3943,20,3943,31,0,0,4,0],[3944,9,3944,29,0,0,4,0],[3944,9,3944,29,0,0,4,1],[3946,10,3946,44,0,0,4,1],[3949,10,3949,22,0,0,4,1],[3964,14,3964,21,0,0,4,0],[3964,14,3964,21,0,0,4,1],[3965,11,3965,34,0,0,4,0],[3966,24,3966,36,0,0,4,0],[3966,24,3966,36,0,0,4,1],[4042,8,4042,31,0,0,4,1],[4151,10,4151,51,0,0,4,1],[4263,9,4263,40,0,0,4,1],[4294,3,4294,18,0,0,4,0]],"addBackwardPassRule":[[981,2,981,16,0,0,4,0],[984,2,984,16,0,0,4,0]],"deallocateRuleNames":[[1621,9,1621,18,0,0,4,0],[1621,9,1621,18,0,0,4,1]],"parseDots":[[1470,4,1470,12,0,0,4,0]],"compileSwap":[[1663,6,1663,26,0,0,4,0],[1668,6,1668,26,0,0,4,0],[1669,7,1669,50,0,0,4,1],[1671,7,1671,55,0,0,4,1],[1673,6,1674,18,0,0,4,1],[1676,6,1676,51,0,0,4,1]],"passGetAttributes":[[1717,3,1717,17,0,0,4,0],[1726,3,1726,22,0,0,4,0],[1729,3,1729,19,0,0,4,0],[1732,3,1732,19,0,0,4,0],[1735,3,1735,19,0,0,4,0],[1738,3,1738,19,0,0,4,0]],"hyphenAddTrans":[[2523,6,2523,19,0,0,4,0],[2523,6,2523,19,0,0,4,1],[2525,11,2525,39,0,0,4,0],[2525,11,2525,39,0,0,4,1]],"findRuleName":[[1581,9,1581,17,0,0,4,0],[1582,7,1582,41,0,0,4,0],[1582,7,1582,41,0,0,4,1],[1583,5,1583,76,0,0,4,0]],"hyphenHashNew":[[2452,6,2452,48,0,0,4,1]],"compilePassOpcode":[[2073,8,2073,50,0,0,4,0],[2074,9,2075,56,0,0,4,1],[2116,8,2116,61,0,0,4,1],[2121,8,2121,18,0,0,4,0],[2123,8,2123,12,0,0,4,0],[2124,7,2124,33,0,0,4,0],[2124,7,2124,33,0,0,4,1],[2124,37,2124,63,0,0,4,0],[2124,37,2124,63,0,0,4,1],[2125,8,2125,34,0,0,4,0],[2126,9,2126,75,0,0,4,1],[2128,9,2129,58,0,0,4,1],[2131,9,2132,61,0,0,4,1],[2271,3,2271,17,0,0,4,0],[2275,8,2275,18,0,0,4,0],[2277,8,2277,12,0,0,4,0],[2278,7,2278,33,0,0,4,0],[2278,7,2278,33,0,0,4,1],[2278,37,2278,63,0,0,4,0],[2278,37,2278,63,0,0,4,1],[2279,8,2279,34,0,0,4,0],[2280,9,2280,75,0,0,4,1],[2282,9,2283,58,0,0,4,1],[2285,9,2286,61,0,0,4,1]],"passFindCharacters":[[752,3,752,17,0,0,4,0],[788,3,788,20,0,0,4,0]],"isMatchPatternEscape":[[1259,28,1259,37,0,0,4,0],[1259,41,1259,50,0,0,4,0],[1259,54,1259,63,0,0,4,1]],"passGetString":[[1781,7,1781,48,0,0,4,0]],"pattern_compile_expression":[[619,8,619,32,0,0,4,0],[619,36,619,40,0,0,4,0],[624,35,624,39,0,0,4,1],[626,40,626,44,0,0,4,1],[904,2,904,10,0,0,4,0]],"addForwardPassRule":[[951,2,951,16,0,0,4,0]],"allocateSpaceInTranslationTable":[[451,6,451,20,0,0,4,1]],"putDots":[[615,10,615,23,0,0,4,0]],"hyphenStringHash":[[2439,14,2439,27,0,0,4,0],[2439,14,2439,27,0,0,4,1],[2441,7,2441,27,0,0,4,0],[2441,7,2441,27,0,0,4,1]]},"library_calls":["lou_getDataPath","lou_findTables","lou_indexTables","lou_checkTable","lou_getTypeformForEmphClass","lou_getTypeformForEmphClass","lou_freeTableFiles","lou_findTables","lou_indexTables","lou_checkTable","lou_getTypeformForEmphClass","lou_freeTableFiles","lou_free"],"critical_calls":["lou_getDataPath","lou_findTables","lou_indexTables","lou_checkTable","lou_getTypeformForEmphClass","lou_getTypeformForEmphClass","lou_freeTableFiles","lou_freeTableFiles","lou_getTypeformForEmphClass","lou_checkTable","lou_indexTables","lou_findTables","lou_free"],"visited":7}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs to process braille table data:

*/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<3) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Early exit if input is too small for meaningful processing
    if (size < 2) {
        return 0;
    }

    // Step 1: Get current data path
    char *dataPath = lou_getDataPath();
    // We could optionally use dataPath, but for fuzzing we'll just note it exists
    
    // Step 2: Extract query string from input data
    // Use first half of input as query, second half as emphasis class
    size_t query_len = size / 2;
    size_t emph_len = size - query_len;
    
    if (query_len == 0 || emph_len == 0) {
        return 0;
    }
    
    // Create null-terminated strings from input data
    std::vector<char> query_buf(query_len + 1);
    std::vector<char> emph_buf(emph_len + 1);
    
    memcpy(query_buf.data(), data, query_len);
    memcpy(emph_buf.data(), data + query_len, emph_len);
    
    query_buf[query_len] = '\0';
    emph_buf[emph_len] = '\0';
    
    const char *query = query_buf.data();
    const char *emphClass = emph_buf.data();
    
    // Step 3: Find tables using the query
    char **tables = lou_findTables(query);
    
    if (tables) {
        // Step 4: Index the found tables
        lou_indexTables(const_cast<const char **>(tables));
        
        // Step 5: For each table found, check it and get typeform for emphasis class
        for (int i = 0; tables[i] != nullptr; i++) {
            // Check the table
            int check_result = lou_checkTable(tables[i]);
            (void)check_result; // Use result to avoid unused variable warning
            
            // Get typeform for emphasis class
            formtype typeform = lou_getTypeformForEmphClass(tables[i], emphClass);
            (void)typeform; // Use result to avoid unused variable warning
            
            // Also test with empty string
            formtype typeform_empty = lou_getTypeformForEmphClass(tables[i], fuzz_str_1);
            (void)typeform_empty;
        }
        
        // Step 6: Free the tables array
        lou_freeTableFiles(tables);
    }
    
    // Step 7: Test with specific table patterns
    const char *test_queries[] = {
        "en-",
        "de-",
        "*",
        ""
    };
    
    for (const auto &test_query : test_queries) {
        char **test_tables = lou_findTables(test_query);
        if (test_tables) {
            lou_indexTables(const_cast<const char **>(test_tables));
            
            // Check first few tables if available
            for (int i = 0; i < 3 && test_tables[i] != nullptr; i++) {
                lou_checkTable(test_tables[i]);
                lou_getTypeformForEmphClass(test_tables[i], emphClass);
            }
            
            lou_freeTableFiles(test_tables);
        }
    }
    
    // Step 8: Clean up - free all liblouis resources
    lou_free();
    
    return 0;
}