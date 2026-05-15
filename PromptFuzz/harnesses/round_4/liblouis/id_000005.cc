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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 113
//<Quality> {"density":1,"unique_branches":{"isMatchPatternEscape":[[1259,54,1259,63,0,0,4,1]],"_lou_showString":[[177,4,177,10,0,0,4,0]],"compileNumber":[[2342,6,2342,39,0,0,4,0]],"addCharacterClass":[[1124,17,1124,27,0,0,4,1],[1126,9,1126,20,0,0,4,0]],"compileSwap":[[1664,7,1664,46,0,0,4,0]],"_lou_pattern_compile":[[1196,6,1197,49,0,0,4,0],[1203,6,1203,63,0,0,4,0]],"pattern_compile_1":[[1015,7,1016,41,0,0,4,0]],"compileCharDef":[[2653,6,2653,27,0,0,4,0]],"pattern_compile_2":[[1053,46,1053,80,0,0,4,0],[1060,8,1060,40,0,0,4,0]],"addRuleName":[[1603,34,1603,42,0,0,4,1]],"compileBeforeAfter":[[2690,6,2690,37,0,0,4,0]],"compileGrouping":[[2360,6,2360,42,0,0,4,0],[2361,6,2361,49,0,0,4,1],[2362,14,2362,34,0,0,4,0],[2362,14,2362,34,0,0,4,1],[2362,38,2362,63,0,0,4,0],[2363,6,2363,27,0,0,4,0]],"compileRule":[[2960,7,2960,43,0,0,4,0],[3073,3,3073,21,0,0,4,0],[3077,8,3078,56,0,0,4,0],[3077,8,3078,56,0,0,4,1],[3109,8,3109,12,0,0,4,0],[3118,8,3118,12,0,0,4,0],[3134,3,3134,21,0,0,4,0],[3144,8,3144,17,0,0,4,1],[3151,8,3152,28,0,0,4,1],[3154,8,3154,34,0,0,4,1],[3160,8,3160,12,0,0,4,1],[3163,8,3163,33,0,0,4,1],[3169,8,3169,12,0,0,4,1],[3171,8,3172,59,0,0,4,1],[3188,3,3188,19,0,0,4,0],[3204,3,3204,22,0,0,4,0],[3205,3,3205,23,0,0,4,0],[3218,4,3218,20,0,0,4,0],[3236,10,3236,25,0,0,4,0],[3240,7,3240,27,0,0,4,0],[3324,9,3326,21,0,0,4,1],[3325,8,3325,14,0,0,4,0],[3346,9,3348,21,0,0,4,0],[3347,8,3347,14,0,0,4,1],[3357,9,3359,29,0,0,4,0],[3358,8,3358,14,0,0,4,1],[3682,8,3683,28,0,0,4,0],[3689,8,3689,43,0,0,4,0],[3690,8,3690,79,0,0,4,0],[3698,3,3698,21,0,0,4,0],[3699,8,3699,43,0,0,4,1],[3700,8,3700,67,0,0,4,1],[3704,20,3704,40,0,0,4,0],[3704,20,3704,40,0,0,4,1],[3708,8,3708,43,0,0,4,0],[3721,8,3722,28,0,0,4,0],[3731,8,3732,41,0,0,4,0],[3752,3,3752,34,0,0,4,0],[3753,8,3753,43,0,0,4,1],[3754,20,3754,40,0,0,4,0],[3754,20,3754,40,0,0,4,1],[3756,9,3756,11,0,0,4,0],[3756,9,3756,11,0,0,4,1],[3766,8,3766,43,0,0,4,0],[3792,9,3792,11,0,0,4,0],[3802,8,3802,43,0,0,4,0],[3836,3,3836,30,0,0,4,0],[3837,8,3837,43,0,0,4,0],[3837,8,3837,43,0,0,4,1],[3838,8,3838,47,0,0,4,1],[3842,20,3842,40,0,0,4,0],[3842,20,3842,40,0,0,4,1],[3882,8,3882,15,0,0,4,0],[3911,3,3911,19,0,0,4,0],[3939,8,3939,43,0,0,4,0],[3977,8,3977,43,0,0,4,0],[3992,8,3992,43,0,0,4,0],[4003,3,4003,22,0,0,4,0],[4007,8,4008,28,0,0,4,1],[4014,8,4014,42,0,0,4,1],[4055,8,4055,43,0,0,4,0],[4131,5,4131,13,0,0,4,0],[4145,10,4145,26,0,0,4,0],[4211,9,4211,50,0,0,4,0],[4212,9,4212,54,0,0,4,1],[4223,8,4223,49,0,0,4,0],[4290,3,4290,25,0,0,4,0],[4304,8,4304,29,0,0,4,0],[4319,3,4319,10,0,0,4,0]],"pattern_compile_expression":[[618,32,618,53,0,0,4,1],[635,7,635,29,0,0,4,0],[697,2,697,10,0,0,4,0],[699,7,699,32,0,0,4,1],[821,4,821,11,0,0,4,0],[838,32,838,53,0,0,4,1],[847,7,847,29,0,0,4,0],[868,2,868,10,0,0,4,0],[871,7,871,30,0,0,4,0],[871,7,871,30,0,0,4,1],[874,7,874,31,0,0,4,0],[874,7,874,31,0,0,4,1],[877,33,877,54,0,0,4,0],[877,33,877,54,0,0,4,1],[878,9,878,32,0,0,4,1],[879,8,879,30,0,0,4,0],[885,7,885,59,0,0,4,1],[891,15,891,37,0,0,4,0],[891,15,891,37,0,0,4,1],[892,8,892,33,0,0,4,1],[901,7,901,10,0,0,4,1],[912,2,912,11,0,0,4,0],[915,7,915,30,0,0,4,0],[915,7,915,30,0,0,4,1]]},"library_calls":["lou_setLogLevel","lou_registerLogCallback","lou_indexTables","lou_compileString","lou_getEmphClasses","lou_charToDots","lou_free"],"critical_calls":["lou_setLogLevel","lou_registerLogCallback","lou_indexTables","lou_compileString","lou_getEmphClasses","lou_charToDots","lou_free"],"visited":1}
/**/


// Custom log callback function
static void  id_000005_custom_log_callback(logLevels level, const char* message) {
    // Empty callback for fuzzing - just ignore logs
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Early exit if we don't have enough data
    if (size < 2) {
        return 0;
    }

    // Use FILE* to read input data as suggested
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Set log level (use a random level based on first byte)
    lou_setLogLevel(static_cast<logLevels>(data[0] % 5));  // 0-4 for log levels

    // 2. Register log callback
    lou_registerLogCallback( id_000005_custom_log_callback);

    // 3. Index tables - create a table list from input data
    // We'll use a simple hardcoded table for indexing
    const char* tables_to_index[] = {"en-us-g2.ctb", nullptr};
    lou_indexTables(tables_to_index);

    // 4. Compile a table string from input data
    // Use part of input as table string (limit to reasonable size)
    size_t compile_len = size > 100 ? 100 : size;
    char* table_string = static_cast<char*>(malloc(compile_len + 1));
    if (!table_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read from our memory file
    fseek(in_file, 0, SEEK_SET);
    size_t read_count = fread(table_string, 1, compile_len, in_file);
    table_string[read_count] = '\0';
    
    // Try to compile the string as a table
    int compile_result = lou_compileString("en-us-g2.ctb", table_string);
    (void)compile_result; // Result intentionally ignored for fuzzing
    
    free(table_string);

    // 5. Get emphasis classes
    const char** emph_classes = lou_getEmphClasses(fuzz_str_1);
    if (emph_classes) {
        // Free the emphasis classes array
        void (*free_func)(const char**) = lou_freeEmphClasses;
        free_func(emph_classes);
    }

    // 6. Use charToDots with remaining input data
    // Reset file position
    fseek(in_file, 0, SEEK_SET);
    
    // Determine how many widechars we can read
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count > 100) {
        widechar_count = 100; // Limit input size
    }
    
    if (widechar_count > 0) {
        std::vector<widechar> inbuf(widechar_count);
        std::vector<widechar> outbuf(widechar_count);
        
        // Read widechars from input
        for (size_t i = 0; i < widechar_count; i++) {
            if (fread(&inbuf[i], sizeof(widechar), 1, in_file) != 1) {
                inbuf[i] = 0;
            }
        }
        
        // Determine mode from input data
        int mode = 0;
        if (size > 0) {
            mode = data[size - 1] % 4; // 0-3
        }
        
        // Call charToDots
        int result = lou_charToDots(
            fuzz_str_2,        // table list
            inbuf.data(),          // input buffer
            outbuf.data(),         // output buffer
            static_cast<int>(widechar_count), // length
            fuzz_int32_t_3                   // mode
        );
        (void)result; // Result intentionally ignored for fuzzing
    }

    // Clean up
    assert_file_closed(&in_file);;
    
    // Free any internal resources
    lou_free();

    assert_file_closed(&in_file);
	return 0;
}