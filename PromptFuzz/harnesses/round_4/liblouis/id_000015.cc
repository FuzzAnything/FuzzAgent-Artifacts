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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 140, nr_unique_branch: 141
//<Quality> {"density":14,"unique_branches":{"parseCharsInternal":[[1272,6,1273,41,0,0,4,1]],"pattern_reverse_branch":[[1220,2,1220,20,0,0,4,0],[1227,2,1227,16,0,0,4,0],[1228,2,1228,14,0,0,4,0],[1230,2,1230,20,0,0,4,0]],"compileRule":[[2960,7,2960,43,0,0,4,1],[2961,7,2961,28,0,0,4,1],[2961,32,2961,52,0,0,4,1],[3103,8,3103,34,0,0,4,0],[3103,38,3103,60,0,0,4,0],[3112,8,3112,33,0,0,4,0],[3112,37,3112,58,0,0,4,0],[3298,5,3298,11,0,0,4,0],[3299,10,3300,37,0,0,4,1],[3307,10,3309,43,0,0,4,1],[3308,9,3308,15,0,0,4,0],[3314,5,3314,12,0,0,4,1],[3503,4,3503,23,0,0,4,0],[3510,9,3511,42,0,0,4,1],[3731,8,3732,41,0,0,4,1],[3782,8,3783,41,0,0,4,1],[4045,10,4045,15,0,0,4,0],[4126,5,4126,13,0,0,4,0],[4128,5,4128,13,0,0,4,0]],"passFindCharacters":[[746,6,746,38,0,0,4,1],[763,3,763,15,0,0,4,0]],"compilePassOpcode":[[1905,8,1905,27,0,0,4,1],[1995,3,1995,21,0,0,4,0],[1997,8,1997,78,0,0,4,1],[2000,4,2000,16,0,0,4,0],[2001,9,2001,73,0,0,4,1],[2004,4,2004,16,0,0,4,1],[2014,4,2014,16,0,0,4,1],[2024,9,2025,56,0,0,4,1],[2029,9,2030,56,0,0,4,1],[2033,4,2033,11,0,0,4,1],[2066,8,2066,27,0,0,4,1],[2071,8,2071,79,0,0,4,1],[2073,8,2073,50,0,0,4,1],[2081,8,2081,27,0,0,4,1],[2085,8,2085,79,0,0,4,1],[2207,3,2207,21,0,0,4,0],[2209,8,2209,78,0,0,4,1],[2212,4,2212,16,0,0,4,0],[2213,9,2213,73,0,0,4,1],[2215,9,2216,56,0,0,4,1],[2220,9,2221,56,0,0,4,1],[2224,4,2224,18,0,0,4,1],[2225,4,2225,20,0,0,4,1],[2233,4,2233,11,0,0,4,1],[2312,7,2312,17,0,0,4,1]],"passGetNumber":[[1797,4,1797,42,0,0,4,0]],"freeTranslationTable":[[4932,18,4932,38,0,0,4,1]],"pattern_compile_3":[[1101,7,1101,38,0,0,4,0],[1101,42,1101,71,0,0,4,0],[1103,5,1103,40,0,0,4,0],[1109,7,1109,42,0,0,4,0],[1110,8,1110,34,0,0,4,1],[1115,8,1115,40,0,0,4,1],[1117,11,1117,43,0,0,4,0],[1117,11,1117,43,0,0,4,1],[1145,8,1145,38,0,0,4,1],[1145,42,1145,78,0,0,4,1],[1148,11,1148,41,0,0,4,0],[1148,11,1148,41,0,0,4,1],[1148,45,1148,81,0,0,4,0],[1148,45,1148,81,0,0,4,1],[1177,8,1177,79,0,0,4,1]],"passGetDots":[[1763,8,1763,44,0,0,4,1],[1764,7,1764,50,0,0,4,0],[1765,8,1765,51,0,0,4,0],[1765,8,1765,51,0,0,4,1]],"addBackwardPassRule":[[972,2,972,18,0,0,4,0],[972,2,972,18,0,0,4,1],[975,2,975,18,0,0,4,1],[978,2,978,16,0,0,4,0],[978,2,978,16,0,0,4,1],[981,2,981,16,0,0,4,1],[984,2,984,16,0,0,4,1],[987,2,987,9,0,0,4,1],[990,9,990,22,0,0,4,0],[990,9,990,22,0,0,4,1],[992,7,992,35,0,0,4,0],[992,7,992,35,0,0,4,1]],"passGetAttributes":[[1705,3,1705,21,0,0,4,0],[1723,3,1723,22,0,0,4,0]],"allocateSpaceInTranslationTable":[[445,9,445,31,0,0,4,1]],"putCharDotsMapping":[[683,11,683,25,0,0,4,0]],"passGetVariableNumber":[[1805,6,1805,51,0,0,4,1],[1809,6,1809,20,0,0,4,0],[1809,24,1809,42,0,0,4,0]],"finalizeCharacter":[[4464,7,4464,36,0,0,4,1]],"pattern_compile_expression":[[626,13,626,36,0,0,4,0],[626,40,626,44,0,0,4,0],[627,9,627,13,0,0,4,1],[635,7,635,29,0,0,4,1],[647,7,648,31,0,0,4,1],[658,2,658,10,0,0,4,0],[660,7,660,33,0,0,4,1],[681,7,682,41,0,0,4,1],[685,7,685,32,0,0,4,1],[720,2,720,10,0,0,4,0],[722,7,722,32,0,0,4,1],[742,7,742,31,0,0,4,0],[745,33,745,54,0,0,4,0],[746,9,746,32,0,0,4,0],[746,9,746,32,0,0,4,1],[747,8,747,30,0,0,4,1],[758,4,758,12,0,0,4,0],[761,4,761,12,0,0,4,0],[770,4,770,12,0,0,4,0],[773,4,773,12,0,0,4,0],[776,4,776,12,0,0,4,0],[782,4,782,12,0,0,4,0],[785,4,785,12,0,0,4,0],[788,4,788,12,0,0,4,0],[793,4,793,12,0,0,4,0],[794,4,794,12,0,0,4,0],[795,4,795,12,0,0,4,0],[802,9,802,11,0,0,4,1],[817,4,817,12,0,0,4,0],[828,7,828,10,0,0,4,0],[839,8,839,32,0,0,4,0],[839,36,839,40,0,0,4,0],[844,8,844,31,0,0,4,0],[844,35,844,39,0,0,4,0],[844,35,844,39,0,0,4,1],[847,7,847,29,0,0,4,1],[849,7,849,59,0,0,4,1],[855,15,855,37,0,0,4,0],[855,15,855,37,0,0,4,1],[856,8,856,33,0,0,4,0],[856,8,856,33,0,0,4,1],[856,37,856,41,0,0,4,0]],"isMatchPatternEscape":[[1259,54,1259,63,0,0,4,0]],"pattern_compile_2":[[1048,7,1048,38,0,0,4,0],[1048,42,1048,71,0,0,4,0],[1049,8,1049,79,0,0,4,1]],"addRule":[[1038,8,1038,14,0,0,4,1],[1040,8,1040,15,0,0,4,0],[1041,9,1041,48,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_listTables","lou_free","lou_checkTable","lou_freeTableFiles","lou_free","lou_getTableInfo","lou_freeTableInfo","lou_freeTableFiles","lou_free","lou_freeTableInfo","lou_freeTableFiles","lou_free","lou_freeTableInfo","lou_freeTableFiles","lou_free","lou_dotsToChar","lou_translateString","lou_freeTableInfo","lou_freeTableFiles","lou_free"],"critical_calls":["lou_registerLogCallback","lou_listTables","lou_checkTable","lou_getTableInfo","lou_dotsToChar","lou_translateString","lou_freeTableInfo","lou_freeTableFiles","lou_free"],"visited":1}
/**/


// Include liblouis headers
extern "C" {
}

// Simple log callback function
static void  id_000015_logCallback(logLevels level, const char *message) {
    // Minimal logging - just to satisfy the API requirement
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // 1. Register a log callback
    lou_registerLogCallback( id_000015_logCallback);
    
    // 2. List available tables
    char **tables = lou_listTables();
    if (!tables) {
        lou_free();
        return 0;
    }
    
    // Find the first valid table
    const char *table_to_use = nullptr;
    for (int i = 0; tables[i] != nullptr; i++) {
        if (lou_checkTable(tables[i]) == 0) {
            table_to_use = tables[i];
            break;
        }
    }
    
    if (!table_to_use) {
        lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }
    
    // 3. Get table info
    char *table_info = lou_getTableInfo(table_to_use, fuzz_str_1);
    // Note: table_info might be nullptr, but that's OK for fuzzing
    
    // 4. Prepare input data for dotsToChar
    // We need at least 2 bytes for one widechar
    if (size < 2) {
        if (table_info) lou_freeTableInfo(table_info);
        lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }
    
    // Convert byte stream to widechar array for dots
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count == 0) {
        if (table_info) lou_freeTableInfo(table_info);
        lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }
    
    // Allocate buffers
    widechar *dots_input = static_cast<widechar*>(malloc(widechar_count * sizeof(widechar)));
    widechar *char_output = static_cast<widechar*>(malloc(widechar_count * sizeof(widechar)));
    
    if (!dots_input || !char_output) {
        free(dots_input);
        free(char_output);
        if (table_info) lou_freeTableInfo(table_info);
        lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }
    
    // Copy data to dots_input buffer
    memcpy(dots_input, data, widechar_count * sizeof(widechar));
    
    // 5. Convert dots to characters
    int dots_to_char_result = lou_dotsToChar(
        table_to_use,
        dots_input,
        char_output,
        static_cast<int>(widechar_count),
        fuzz_int32_t_2  // mode
    );
    
    // 6. Prepare for translateString
    if (dots_to_char_result > 0) {
        int inlen = dots_to_char_result;
        int outlen = inlen * 2;  // Allow for expansion
        
        widechar *translate_in = char_output;
        widechar *translate_out = static_cast<widechar*>(malloc(outlen * sizeof(widechar)));
        formtype *typeform = static_cast<formtype*>(calloc(inlen, sizeof(formtype)));
        char *spacing = static_cast<char*>(calloc(inlen, sizeof(char)));
        
        if (translate_out && typeform && spacing) {
            // 7. Translate the characters back (or forward)
            int translate_result = lou_translateString(
                table_to_use,
                translate_in,
                &inlen,
                translate_out,
                &outlen,
                typeform,
                spacing,
                fuzz_int32_t_3  // mode
            );
            
            // Result is not used, but the call was made
            (void)translate_result;
        }
        
        free(translate_out);
        free(typeform);
        free(spacing);
    }
    
    // Cleanup
    free(dots_input);
    free(char_output);
    
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    lou_freeTableFiles(tables);
    lou_free();
    
    return 0;
}