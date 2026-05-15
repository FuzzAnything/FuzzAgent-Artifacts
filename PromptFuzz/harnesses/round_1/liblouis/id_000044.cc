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
//<ID> 1197
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 73
//<Quality> {"density":2,"unique_branches":{"resolveEmphasisAllSymbols":[[3058,6,3058,22,0,0,4,1],[3076,13,3076,29,0,0,4,0],[3078,19,3078,36,0,0,4,0],[3078,19,3078,36,0,0,4,1],[3079,8,3079,15,0,0,4,1],[3082,8,3082,16,0,0,4,0],[3083,9,3083,39,0,0,4,1],[3086,10,3086,18,0,0,4,0],[3087,11,3087,40,0,0,4,1],[3091,10,3091,17,0,0,4,1]],"markEmphases":[[3338,8,3338,32,0,0,4,0]],"checkNumericMode":[[3578,6,3578,68,0,0,4,1],[3581,6,3581,19,0,0,4,0],[3582,7,3582,72,0,0,4,1],[3590,14,3590,70,0,0,4,1]],"copyCharacters":[[932,7,932,16,0,0,4,0],[933,8,933,56,0,0,4,1],[934,11,934,20,0,0,4,0],[934,11,934,20,0,0,4,1]],"passDoAction":[[1007,3,1007,17,0,0,4,0],[1008,8,1008,20,0,0,4,1],[1031,3,1031,10,0,0,4,0],[1032,8,1032,64,0,0,4,0]],"undefinedCharacter":[[2304,6,2304,22,0,0,4,0],[2320,10,2320,16,0,0,4,1],[2323,59,2323,74,0,0,4,1],[2329,7,2329,15,0,0,4,0]],"passDoTest":[[774,3,774,18,0,0,4,0],[775,8,775,16,0,0,4,0],[775,8,775,16,0,0,4,1],[790,3,790,16,0,0,4,0],[825,12,825,25,0,0,4,0],[841,7,841,43,0,0,4,0],[841,47,841,66,0,0,4,0],[841,47,841,66,0,0,4,1],[843,10,843,45,0,0,4,0],[843,10,843,45,0,0,4,1],[847,10,850,20,0,0,4,0],[847,10,850,20,0,0,4,1],[847,13,847,26,0,0,4,1],[851,11,851,23,0,0,4,0],[852,17,852,28,0,0,4,1],[910,3,910,10,0,0,4,0],[911,8,911,72,0,0,4,0],[914,8,914,20,0,0,4,1],[914,38,914,49,0,0,4,0],[914,53,914,60,0,0,4,0],[914,53,914,60,0,0,4,1]],"translateString":[[3777,6,3777,20,0,0,4,1],[3778,7,3781,21,0,0,4,0],[3797,9,3797,25,0,0,4,0]],"translatePass":[[1082,3,1082,17,0,0,4,0]],"isNoLetsignBefore":[[1710,18,1710,49,0,0,4,0],[1711,7,1711,37,0,0,4,1]],"resolveEmphasisWords":[[2742,5,2742,35,0,0,4,0],[2828,6,2828,28,0,0,4,0]],"resolveEmphasisPassages":[[2935,31,2935,61,0,0,4,0],[2953,6,2953,37,0,0,4,0],[2956,8,2956,39,0,0,4,0],[2961,8,2961,15,0,0,4,1],[2977,9,2977,16,0,0,4,0],[2985,7,2985,15,0,0,4,1],[2985,36,2985,57,0,0,4,0],[2987,8,2987,20,0,0,4,1],[2987,24,2987,52,0,0,4,0],[2995,8,2995,15,0,0,4,0],[2996,9,2996,16,0,0,4,1],[3012,16,3012,38,0,0,4,0],[3013,10,3013,52,0,0,4,1]],"_lou_translate":[[1250,30,1250,48,0,0,4,0]],"_lou_charToFallbackDots":[[434,6,434,15,0,0,4,1],[439,10,439,16,0,0,4,0],[439,10,439,16,0,0,4,1]]},"library_calls":["lou_getDataPath","lou_findTables","lou_indexTables","lou_registerLogCallback","lou_logFile","lou_readCharFromFile","lou_translate","lou_freeTableFiles","lou_free"],"critical_calls":["lou_getDataPath","lou_findTables","lou_indexTables","lou_registerLogCallback","lou_logFile","lou_readCharFromFile","lou_translate","lou_freeTableFiles","lou_free"],"visited":0}
/**/


extern "C" {
}

// Custom log callback function
static void  id_000044_log_callback(logLevels level, const char *msg) {
    // Minimal logging to avoid overhead during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Ensure minimum input size for basic operations
    if (size < 10) {
        return 0;
    }

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Write the data to a file for lou_readCharFromFile
    const char *input_filename = "input_file";
    FILE *tmp_file = fopen(input_filename, "wb");
    if (!tmp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    fwrite(data, 1, size, tmp_file);
    assert_file_closed(&tmp_file);;

    // 1. Get data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        free(data_path);  // Free the returned string
    }

    // 2. Find tables using part of input as query
    // Use first 10 bytes as query (or less if size is small)
    size_t query_len = size > 10 ? 10 : size;
    std::vector<char> query(query_len + 1);
    memcpy(query.data(), data, query_len);
    query[query_len] = '\0';
    
    char **tables = lou_findTables(query.data());
    
    // 3. Index the found tables
    if (tables) {
        lou_indexTables((const char **)tables);
    }

    // 4. Register log callback
    lou_registerLogCallback( id_000044_log_callback);

    // 5. Set log file
    lou_logFile(fuzz_str_1);

    // 6. Read character from file
    int mode = 0;
    int char_result = lou_readCharFromFile(input_filename, &mode);

    // 7. Translate using found tables or default table
    const char *table_list = "en-us-g2.ctb";  // Default table
    if (tables && tables[0]) {
        table_list = tables[0];
    }

    // Prepare input buffer for translation
    // Use remaining input data (after query) for translation
    size_t translation_size = size - query_len;
    if (translation_size > 0 && translation_size <= size) {
        // Convert bytes to widechars (2 bytes per widechar)
        size_t widechar_count = translation_size / 2;
        if (widechar_count > 0) {
            std::vector<widechar> inbuf(widechar_count);
            std::vector<widechar> outbuf(widechar_count * 2);  // Allow expansion
            memcpy(inbuf.data(), data + query_len, widechar_count * sizeof(widechar));
            
            int inlen = static_cast<int>(widechar_count);
            int outlen = static_cast<int>(outbuf.size());
            formtype *typeform = nullptr;
            char *spacing = nullptr;
            int *outputPos = nullptr;
            int *inputPos = nullptr;
            int *cursorPos = nullptr;
            
            lou_translate(fuzz_str_2, inbuf.data(), &inlen, outbuf.data(), &outlen,
                         typeform, spacing, outputPos, inputPos, cursorPos, fuzz_int32_t_3);
        }
    }

    // 8. Clean up resources
    
    // Free tables array
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // Close input file
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove(input_filename);
    remove("log.txt");
    remove("output_file");  // In case it was created elsewhere
    
    // Call lou_free to release internal resources
    lou_free();
    
    assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}