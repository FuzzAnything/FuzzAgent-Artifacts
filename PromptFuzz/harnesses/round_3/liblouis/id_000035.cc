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
//<ID> 510
//<Prompt> []
/*<Combination>: [
*/
//<score> 49.333332, nr_unique_branch: 270
//<Quality> {"density":4,"unique_branches":{"passGetAttributes":[[1711,3,1711,17,0,0,4,0],[1714,3,1714,24,0,0,4,0],[1717,3,1717,17,0,0,4,0],[1720,3,1720,18,0,0,4,0],[1726,3,1726,22,0,0,4,0],[1729,3,1729,19,0,0,4,0],[1732,3,1732,19,0,0,4,0],[1735,3,1735,19,0,0,4,0],[1738,3,1738,19,0,0,4,0]],"addForwardPassRule":[[948,2,948,16,0,0,4,0],[951,2,951,16,0,0,4,0]],"parseCharsInternal":[[1301,5,1301,12,0,0,4,0]],"pattern_compile_2":[[1053,46,1053,80,0,0,4,0],[1054,5,1054,39,0,0,4,0]],"pattern_reverse_branch":[[1229,2,1229,19,0,0,4,0]],"compilePassOpcode":[[1914,3,1914,18,0,0,4,0],[1915,8,1915,75,0,0,4,1],[2058,8,2058,49,0,0,4,0],[2060,9,2060,67,0,0,4,1],[2061,9,2061,72,0,0,4,1],[2073,8,2073,50,0,0,4,0],[2074,9,2075,56,0,0,4,1],[2116,8,2116,61,0,0,4,1],[2121,8,2121,18,0,0,4,0],[2123,8,2123,12,0,0,4,0],[2124,7,2124,33,0,0,4,0],[2124,7,2124,33,0,0,4,1],[2124,37,2124,63,0,0,4,0],[2124,37,2124,63,0,0,4,1],[2125,8,2125,34,0,0,4,0],[2126,9,2126,75,0,0,4,1],[2128,9,2129,58,0,0,4,1],[2131,9,2132,61,0,0,4,1],[2243,3,2243,17,0,0,4,0],[2244,8,2244,74,0,0,4,1],[2271,3,2271,17,0,0,4,0],[2275,8,2275,18,0,0,4,0],[2277,8,2277,12,0,0,4,0],[2278,7,2278,33,0,0,4,0],[2278,7,2278,33,0,0,4,1],[2278,37,2278,63,0,0,4,0],[2278,37,2278,63,0,0,4,1],[2279,8,2279,34,0,0,4,0],[2280,9,2280,75,0,0,4,1],[2282,9,2283,58,0,0,4,1],[2285,9,2286,61,0,0,4,1]],"pattern_compile_expression":[[619,8,619,32,0,0,4,0],[619,36,619,40,0,0,4,0],[624,8,624,31,0,0,4,0],[624,35,624,39,0,0,4,0],[624,35,624,39,0,0,4,1],[626,40,626,44,0,0,4,1],[627,9,627,13,0,0,4,0],[697,2,697,10,0,0,4,0],[699,7,699,32,0,0,4,1],[713,2,713,10,0,0,4,0],[715,7,715,32,0,0,4,1],[904,2,904,10,0,0,4,0],[907,7,907,32,0,0,4,1]],"getNextAttribute":[[1096,6,1096,10,0,0,4,1]],"putDots":[[615,10,615,23,0,0,4,0]],"hyphenHashFree":[[2461,14,2461,32,0,0,4,0],[2461,14,2461,32,0,0,4,1],[2462,33,2462,34,0,0,4,0],[2462,33,2462,34,0,0,4,1]],"addRule":[[1035,30,1035,50,0,0,4,0],[1035,54,1035,74,0,0,4,0]],"hyphenGetNewState":[[2506,6,2506,48,0,0,4,0],[2506,6,2506,48,0,0,4,1],[2509,6,2509,19,0,0,4,1]],"allocateSpaceInTranslationTable":[[451,6,451,20,0,0,4,1]],"pattern_compile_3":[[1102,5,1102,39,0,0,4,0],[1104,5,1104,39,0,0,4,0]],"passFindCharacters":[[732,8,732,24,0,0,4,1],[745,8,745,52,0,0,4,1],[752,3,752,17,0,0,4,0],[779,3,779,18,0,0,4,0],[788,3,788,20,0,0,4,0]],"parseDots":[[1470,4,1470,12,0,0,4,0],[1474,4,1474,12,0,0,4,0]],"findRuleName":[[1581,9,1581,17,0,0,4,0],[1582,7,1582,41,0,0,4,0],[1582,7,1582,41,0,0,4,1],[1583,5,1583,76,0,0,4,0]],"compileHyphenation":[[2554,6,2554,18,0,0,4,1],[2560,7,2560,32,0,0,4,1],[2564,8,2564,36,0,0,4,0],[2564,8,2564,36,0,0,4,1],[2567,7,2567,23,0,0,4,1],[2567,27,2567,47,0,0,4,1],[2567,51,2567,71,0,0,4,0],[2567,51,2567,71,0,0,4,1],[2568,5,2568,25,0,0,4,1],[2572,15,2572,30,0,0,4,0],[2572,15,2572,30,0,0,4,1],[2573,8,2573,28,0,0,4,0],[2573,8,2573,28,0,0,4,1],[2573,32,2573,52,0,0,4,0],[2573,32,2573,52,0,0,4,1],[2583,15,2583,32,0,0,4,0],[2583,15,2583,32,0,0,4,1],[2585,7,2585,28,0,0,4,0],[2585,7,2585,28,0,0,4,1],[2590,7,2590,12,0,0,4,0],[2597,10,2597,31,0,0,4,0],[2597,10,2597,31,0,0,4,1],[2601,8,2601,29,0,0,4,0],[2601,8,2601,29,0,0,4,1],[2607,11,2607,30,0,0,4,0],[2607,11,2607,30,0,0,4,1],[2609,14,2609,32,0,0,4,0],[2609,14,2609,32,0,0,4,1],[2610,33,2610,34,0,0,4,0],[2610,33,2610,34,0,0,4,1],[2611,16,2611,35,0,0,4,0],[2613,17,2613,35,0,0,4,0],[2613,17,2613,35,0,0,4,1],[2616,9,2616,33,0,0,4,0],[2616,9,2616,33,0,0,4,1],[2618,8,2618,14,0,0,4,0],[2623,14,2623,32,0,0,4,0],[2623,14,2623,32,0,0,4,1],[2624,7,2624,35,0,0,4,0],[2624,7,2624,35,0,0,4,1]],"hyphenAddTrans":[[2523,6,2523,19,0,0,4,0],[2523,6,2523,19,0,0,4,1],[2525,11,2525,39,0,0,4,0],[2525,11,2525,39,0,0,4,1]],"passGetNumber":[[1796,9,1796,42,0,0,4,1]],"hyphenStringHash":[[2439,14,2439,27,0,0,4,0],[2439,14,2439,27,0,0,4,1],[2441,7,2441,27,0,0,4,0],[2441,7,2441,27,0,0,4,1]],"compileRule":[[2883,6,2883,57,0,0,4,0],[2884,7,2884,12,0,0,4,0],[3077,8,3078,56,0,0,4,1],[3188,3,3188,19,0,0,4,0],[3189,3,3189,19,0,0,4,0],[3205,3,3205,23,0,0,4,0],[3218,4,3218,20,0,0,4,0],[3219,4,3219,20,0,0,4,0],[3233,9,3233,24,0,0,4,1],[3251,35,3251,59,0,0,4,0],[3252,11,3252,42,0,0,4,0],[3283,5,3283,11,0,0,4,0],[3284,10,3284,73,0,0,4,1],[3291,10,3293,43,0,0,4,1],[3292,9,3292,15,0,0,4,0],[3292,9,3292,15,0,0,4,1],[3324,9,3326,21,0,0,4,1],[3325,8,3325,14,0,0,4,0],[3331,4,3331,20,0,0,4,0],[3335,9,3337,21,0,0,4,1],[3336,8,3336,14,0,0,4,0],[3358,8,3358,14,0,0,4,1],[3405,9,3405,52,0,0,4,0],[3485,3,3485,25,0,0,4,0],[3486,3,3486,23,0,0,4,0],[3632,4,3632,26,0,0,4,0],[3633,9,3633,44,0,0,4,1],[3636,19,3636,42,0,0,4,0],[3636,46,3636,64,0,0,4,1],[3637,9,3637,51,0,0,4,1],[3642,21,3642,41,0,0,4,0],[3642,21,3642,41,0,0,4,1],[3643,10,3643,52,0,0,4,1],[3652,4,3652,24,0,0,4,0],[3653,9,3653,44,0,0,4,1],[3656,19,3656,40,0,0,4,0],[3656,44,3656,60,0,0,4,1],[3657,9,3657,49,0,0,4,1],[3662,21,3662,41,0,0,4,0],[3662,21,3662,41,0,0,4,1],[3663,10,3663,52,0,0,4,1],[3825,3,3825,27,0,0,4,0],[3826,8,3826,43,0,0,4,1],[3827,8,3827,76,0,0,4,1],[3831,20,3831,40,0,0,4,0],[3831,20,3831,40,0,0,4,1],[3837,8,3837,43,0,0,4,1],[3838,8,3838,47,0,0,4,1],[3842,20,3842,40,0,0,4,0],[3842,20,3842,40,0,0,4,1],[3848,3,3848,25,0,0,4,0],[3849,8,3849,43,0,0,4,1],[3850,20,3850,40,0,0,4,0],[3850,20,3850,40,0,0,4,1],[3852,9,3852,11,0,0,4,1],[3881,3,3881,19,0,0,4,0],[3882,8,3882,15,0,0,4,1],[3911,3,3911,19,0,0,4,0],[3930,8,3930,15,0,0,4,0],[3938,3,3938,22,0,0,4,0],[3939,8,3939,43,0,0,4,1],[3941,8,3941,51,0,0,4,1],[3943,20,3943,31,0,0,4,0],[3944,9,3944,29,0,0,4,0],[3944,9,3944,29,0,0,4,1],[3946,10,3946,44,0,0,4,1],[3949,10,3949,22,0,0,4,1],[3964,14,3964,21,0,0,4,0],[3964,14,3964,21,0,0,4,1],[3965,11,3965,34,0,0,4,0],[3966,24,3966,36,0,0,4,0],[3966,24,3966,36,0,0,4,1],[4003,3,4003,22,0,0,4,0],[4007,8,4008,28,0,0,4,1],[4039,8,4039,31,0,0,4,1],[4042,8,4042,31,0,0,4,1],[4054,3,4054,19,0,0,4,0],[4067,8,4067,29,0,0,4,0],[4151,10,4151,51,0,0,4,1],[4158,10,4158,20,0,0,4,1],[4184,9,4184,34,0,0,4,1],[4205,4,4205,19,0,0,4,0],[4208,9,4208,36,0,0,4,0],[4209,10,4209,43,0,0,4,1],[4263,9,4263,40,0,0,4,1],[4287,3,4287,26,0,0,4,0],[4290,3,4290,25,0,0,4,0],[4295,3,4295,18,0,0,4,0]],"finalizeCharacter":[[4415,7,4415,32,0,0,4,0],[4418,8,4418,52,0,0,4,0],[4418,8,4418,52,0,0,4,1],[4419,6,4421,36,0,0,4,0],[4419,6,4421,36,0,0,4,1],[4423,9,4423,46,0,0,4,0],[4423,9,4423,46,0,0,4,1],[4474,7,4474,23,0,0,4,0]],"includeFile":[[4986,6,4986,9,0,0,4,0]],"finalizeTable":[[4529,8,4529,35,0,0,4,0],[4533,9,4533,18,0,0,4,1]],"addBackwardPassRule":[[975,2,975,18,0,0,4,0],[981,2,981,16,0,0,4,0],[984,2,984,16,0,0,4,0]],"hyphenHashLookup":[[2491,6,2491,22,0,0,4,0],[2491,6,2491,22,0,0,4,1],[2493,32,2493,33,0,0,4,0],[2493,32,2493,33,0,0,4,1],[2494,7,2494,36,0,0,4,0],[2494,7,2494,36,0,0,4,1],[2495,15,2495,30,0,0,4,0],[2495,15,2495,30,0,0,4,1],[2496,8,2496,41,0,0,4,0],[2496,8,2496,41,0,0,4,1],[2497,7,2497,23,0,0,4,0],[2497,7,2497,23,0,0,4,1]],"lou_getTypeformForEmphClass":[[5263,18,5263,38,0,0,4,1],[5264,7,5264,55,0,0,4,1]],"passGetName":[[1820,46,1820,54,0,0,4,0],[1826,11,1826,42,0,0,4,1]],"hyphenHashNew":[[2452,6,2452,48,0,0,4,1]],"passGetString":[[1781,7,1781,48,0,0,4,0]],"isMatchPatternEscape":[[1259,28,1259,37,0,0,4,0],[1259,41,1259,50,0,0,4,0],[1259,54,1259,63,0,0,4,1]],"compileSwap":[[1663,30,1663,50,0,0,4,0],[1666,7,1666,51,0,0,4,1],[1668,6,1668,26,0,0,4,1],[1671,7,1671,55,0,0,4,1]],"hyphenHashInsert":[[2476,6,2476,44,0,0,4,1],[2479,6,2479,13,0,0,4,1],[2481,14,2481,29,0,0,4,0],[2481,14,2481,29,0,0,4,1]],"compileSwapDots":[[1636,9,1636,28,0,0,4,1],[1637,7,1637,30,0,0,4,1],[1640,8,1640,48,0,0,4,1],[1642,17,1642,37,0,0,4,0],[1642,17,1642,37,0,0,4,1]],"compileTable":[[5065,6,5065,18,0,0,4,0]]},"library_calls":["lou_readCharFromFile","lou_setDataPath","lou_listTables","lou_getTypeformForEmphClass","lou_getTypeformForEmphClass","lou_freeTableFiles","lou_logFile","lou_compileString","lou_free","lou_logFile"],"critical_calls":["lou_readCharFromFile","lou_setDataPath","lou_listTables","lou_getTypeformForEmphClass","lou_getTypeformForEmphClass","lou_freeTableFiles","lou_logFile","lou_compileString","lou_free","lou_logFile"],"visited":2}
/**/


// Include necessary headers for liblouis

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<53) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;
    }

    // 1. Create a temporary file with the fuzzer input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Write the data to a file for lou_readCharFromFile
    FILE *temp_file = fopen("input_file", "wb");
    if (!temp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy data from memory to file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, temp_file);
    }
    
    assert_file_closed(&temp_file);;
    assert_file_closed(&in_file);;
    
    // 2. Call lou_readCharFromFile to read the first character from the file
    int mode = 0;
    int read_char = lou_readCharFromFile("input_file", &mode);
    (void)read_char; // Suppress unused variable warning
    
    // 3. Set data path - use current directory
    char *old_path = lou_setDataPath(fuzz_str_1);
    (void)old_path; // Suppress unused variable warning
    
    // 4. List available tables
    char **tables = lou_listTables();
    if (tables) {
        // 5. For each table, try to get typeform for an emphasis class
        for (int i = 0; tables[i] != NULL; i++) {
            // Try to get typeform for a sample emphasis class
            formtype typeform = lou_getTypeformForEmphClass(tables[i], fuzz_str_2);
            (void)typeform; // Suppress unused variable warning
            
            // Try another sample emphasis class
            formtype typeform2 = lou_getTypeformForEmphClass(tables[i], fuzz_str_3);
            (void)typeform2; // Suppress unused variable warning
        }
        
        // Free the table list
        lou_freeTableFiles(tables);
    }
    
    // 6. Set up logging to a file
    lou_logFile(fuzz_str_4);
    
    // 7. Perform some additional operations using the input data
    
    // Try to compile the input data as a table string
    // First, ensure the data is null-terminated for string operations
    std::vector<char> table_data(data, data + size);
    if (size > 0 && table_data[size - 1] != '\0') {
        table_data.push_back('\0');
    }
    
    // Try to compile the table string if it looks reasonable
    if (size > 10) { // Arbitrary threshold
        int compile_result = lou_compileString(fuzz_str_5, table_data.data());
        (void)compile_result; // Suppress unused variable warning
    }
    
    // 8. Clean up resources
    
    // Close and remove temporary files
    remove("input_file");
    remove("output_file");
    
    // Free liblouis resources
    lou_free();
    
    // Reset log file to default (stderr)
    lou_logFile(NULL);
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}