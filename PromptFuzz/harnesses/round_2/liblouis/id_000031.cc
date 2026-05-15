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
//<ID> 682
//<Prompt> []
/*<Combination>: [
*/
//<score> 100, nr_unique_branch: 156
//<Quality> {"density":8,"unique_branches":{"resolveEmphasisBeginEnd":[[2698,11,2698,25,0,0,4,0],[2698,29,2698,52,0,0,4,1]],"back_selectRule":[[743,8,743,49,0,0,4,1],[744,9,744,51,0,0,4,1],[747,11,748,63,0,0,4,0],[784,6,784,29,0,0,4,0],[788,11,788,54,0,0,4,0],[832,11,832,29,0,0,4,0],[835,6,835,29,0,0,4,0],[836,11,836,29,0,0,4,0]],"translateString":[[3765,7,3765,30,0,0,4,1],[3772,7,3772,29,0,0,4,0],[3777,6,3777,20,0,0,4,1],[3778,7,3781,21,0,0,4,0],[3797,9,3797,25,0,0,4,0],[3816,3,3816,18,0,0,4,0],[3817,8,3817,25,0,0,4,0],[3817,29,3817,80,0,0,4,0]],"resolveEmphasisWords":[[2742,5,2742,35,0,0,4,0],[2742,39,2742,58,0,0,4,0],[2828,6,2828,28,0,0,4,0]],"resolveEmphasisAllSymbols":[[3058,6,3058,22,0,0,4,1],[3076,13,3076,29,0,0,4,0],[3078,19,3078,36,0,0,4,0],[3078,19,3078,36,0,0,4,1],[3079,8,3079,15,0,0,4,1],[3082,8,3082,16,0,0,4,0],[3083,9,3083,39,0,0,4,1],[3086,10,3086,18,0,0,4,0],[3087,11,3087,40,0,0,4,1],[3091,10,3091,17,0,0,4,1]],"isBegWord":[[460,31,460,37,0,0,4,1]],"_lou_translate":[[1352,9,1352,19,0,0,4,0]],"backTranslateString":[[1224,35,1224,70,0,0,4,0],[1225,5,1225,39,0,0,4,0],[1280,3,1280,26,0,0,4,0],[1373,3,1373,19,0,0,4,0],[1374,8,1376,46,0,0,4,1],[1436,8,1436,36,0,0,4,0],[1437,9,1437,29,0,0,4,1],[1474,8,1474,35,0,0,4,1],[1475,6,1475,32,0,0,4,0],[1475,36,1475,65,0,0,4,1]],"findCharOrDots":[[50,6,50,12,0,0,4,0],[57,9,57,15,0,0,4,0],[59,7,59,28,0,0,4,0]],"copyCharacters":[[932,7,932,16,0,0,4,0],[933,8,933,56,0,0,4,1],[934,11,934,20,0,0,4,0],[934,11,934,20,0,0,4,1]],"passDoTest":[[774,3,774,18,0,0,4,0],[775,8,775,16,0,0,4,0],[775,8,775,16,0,0,4,1],[790,3,790,16,0,0,4,0],[825,12,825,25,0,0,4,0],[829,10,829,22,0,0,4,1],[833,16,833,27,0,0,4,0],[841,7,841,43,0,0,4,0],[841,47,841,66,0,0,4,0],[841,47,841,66,0,0,4,1],[843,10,843,45,0,0,4,1],[847,10,850,20,0,0,4,0],[847,10,850,20,0,0,4,1],[847,13,847,26,0,0,4,0],[847,13,847,26,0,0,4,1],[851,11,851,23,0,0,4,0],[852,17,852,28,0,0,4,1],[910,3,910,10,0,0,4,0],[911,8,911,72,0,0,4,0],[914,8,914,20,0,0,4,1],[914,38,914,49,0,0,4,0],[914,53,914,60,0,0,4,0],[914,53,914,60,0,0,4,1]],"inSequence":[[1900,20,1900,26,0,0,4,0],[1901,7,1901,59,0,0,4,1],[1902,7,1902,77,0,0,4,0]],"translatePass":[[1082,3,1082,17,0,0,4,0],[1083,3,1083,17,0,0,4,0]],"checkAttr":[[69,10,69,85,0,0,4,0],[69,30,69,51,0,0,4,0]],"checkNumericMode":[[3578,6,3578,68,0,0,4,1],[3581,6,3581,19,0,0,4,0],[3582,7,3582,72,0,0,4,1],[3590,14,3590,70,0,0,4,0],[3590,14,3590,70,0,0,4,1],[3591,22,3591,39,0,0,4,0],[3592,9,3592,72,0,0,4,1],[3598,16,3598,71,0,0,4,0]],"pattern_check_expression":[[1325,11,1325,41,0,0,4,0],[1325,11,1325,41,0,0,4,1],[1325,45,1325,75,0,0,4,0],[1327,7,1327,46,0,0,4,1],[1346,7,1346,57,0,0,4,1],[1377,3,1377,17,0,0,4,0],[1377,3,1377,17,0,0,4,1],[1383,3,1383,17,0,0,4,1],[1389,3,1389,15,0,0,4,1],[1396,3,1396,20,0,0,4,1],[1400,3,1400,21,0,0,4,1],[1447,3,1447,20,0,0,4,1],[1468,3,1468,21,0,0,4,1],[1489,3,1489,15,0,0,4,1],[1496,3,1496,22,0,0,4,0],[1496,3,1496,22,0,0,4,1],[1500,8,1500,11,0,0,4,0],[1500,15,1500,26,0,0,4,1],[1504,8,1504,12,0,0,4,1],[1513,3,1513,17,0,0,4,1],[1529,3,1529,16,0,0,4,1],[1555,3,1555,15,0,0,4,1],[1558,3,1558,10,0,0,4,1],[1565,10,1565,40,0,0,4,0],[1565,10,1565,40,0,0,4,1],[1569,8,1569,37,0,0,4,0]],"resolveEmphasisPassages":[[2935,31,2935,61,0,0,4,0],[2935,65,2935,84,0,0,4,0],[2953,6,2953,37,0,0,4,0],[2956,8,2956,39,0,0,4,0],[2961,8,2961,15,0,0,4,1],[2985,36,2985,57,0,0,4,0],[2987,8,2987,20,0,0,4,1],[2987,24,2987,52,0,0,4,1]],"passDoAction":[[1007,3,1007,17,0,0,4,0],[1008,8,1008,20,0,0,4,1],[1031,3,1031,10,0,0,4,0],[1032,8,1032,64,0,0,4,0]],"_lou_showDots":[[264,24,264,40,0,0,4,0],[264,24,264,40,0,0,4,1],[264,44,264,68,0,0,4,0],[265,28,265,54,0,0,4,0],[265,28,265,54,0,0,4,1],[266,8,266,52,0,0,4,0],[266,8,266,52,0,0,4,1],[267,6,267,32,0,0,4,0],[270,7,270,34,0,0,4,1],[272,7,272,30,0,0,4,1]],"back_passDoAction":[[1747,3,1747,19,0,0,4,0],[1748,3,1748,17,0,0,4,0],[1749,8,1749,75,0,0,4,1],[1762,3,1762,17,0,0,4,0]],"back_passDoTest":[[1695,53,1695,60,0,0,4,1]],"for_selectRule":[[2110,12,2110,32,0,0,4,0],[2111,13,2111,54,0,0,4,1],[2121,10,2121,40,0,0,4,1],[2161,12,2163,31,0,0,4,1],[2223,12,2223,40,0,0,4,0],[2224,10,2224,42,0,0,4,0],[2229,12,2229,40,0,0,4,0],[2262,7,2262,21,0,0,4,0],[2265,12,2265,24,0,0,4,1],[2265,28,2265,51,0,0,4,1],[2266,12,2266,68,0,0,4,1],[2274,12,2275,57,0,0,4,1],[2280,12,2282,26,0,0,4,1]],"pattern_check_attrs":[[1305,6,1305,42,0,0,4,1]],"markEmphases":[[3338,8,3338,32,0,0,4,0]]},"library_calls":["lou_registerLogCallback","lou_logFile","lou_getDataPath","lou_listTables","lou_freeTableFiles","lou_findTable","lou_compileString","lou_freeTableFile","lou_charToDots","lou_dotsToChar","lou_translateString","lou_backTranslateString","lou_freeTableFile","lou_free"],"critical_calls":["lou_registerLogCallback","lou_logFile","lou_getDataPath","lou_listTables","lou_freeTableFiles","lou_findTable","lou_compileString","lou_charToDots","lou_dotsToChar","lou_translateString","lou_backTranslateString","lou_freeTableFile","lou_free"],"visited":1}
/**/


// Custom log callback function
static void  id_000031_log_callback(logLevels level, const char *message) {
    // Simple callback that does nothing
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 2) {
        return 0;  // Need at least some data
    }

    // 1. Register log callback
    lou_registerLogCallback( id_000031_log_callback);
    
    // 2. Set log file
    lou_logFile("output_file");
    
    // 3. Get data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        free(data_path);
    }
    
    // 4. List available tables
    char **tables = lou_listTables();
    if (tables) {
        // Free the table list
        lou_freeTableFiles(tables);
    }
    
    // 5. Find a table using query from input data
    // Use first 100 bytes or size-1 if smaller as query string
    size_t query_len = (size - 1 > 100) ? 100 : size - 1;
    std::string query((const char*)data, query_len);
    char *found_table = lou_findTable(query.c_str());
    
    // Default table to use if findTable fails
    const char *table_to_use = "en-us-g2.ctb";
    
    if (found_table) {
        table_to_use = found_table;
    }
    
    // 6. Compile a string - use a simple test pattern
    const char *compile_string = "table test\n";
    lou_compileString(table_to_use, compile_string);
    
    // Create a FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        if (found_table) {
            lou_freeTableFile(found_table);
        }
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // Use to avoid unused variable warning
    
    // 7. Read input and convert to widechars for processing
    // We'll use a portion of the input as text to process
    size_t text_size = (size / 2 > 1024) ? 1024 : size / 2;
    std::vector<widechar> inbuf(text_size);
    
    // Convert bytes to widechars (simple conversion)
    for (size_t i = 0; i < text_size && i < size; i++) {
        inbuf[i] = data[i] & 0xFF;
    }
    
    // Prepare output buffers
    std::vector<widechar> outbuf(text_size * 2);  // Allow for expansion
    std::vector<widechar> dotsbuf(text_size * 2);
    std::vector<widechar> backbuf(text_size * 2);
    
    int inlen = static_cast<int>(text_size);
    int outlen = static_cast<int>(outbuf.size());
    int dotslen = static_cast<int>(dotsbuf.size());
    int backlen = static_cast<int>(backbuf.size());
    
    formtype *typeform = nullptr;
    char *spacing = nullptr;
    int mode = 0;
    
    // 8. Perform charToDots conversion
    lou_charToDots(table_to_use, inbuf.data(), dotsbuf.data(), inlen, mode);
    
    // 9. Perform dotsToChar conversion
    lou_dotsToChar(table_to_use, dotsbuf.data(), outbuf.data(), dotslen, mode);
    
    // Reset lengths for string operations
    inlen = static_cast<int>(text_size);
    outlen = static_cast<int>(outbuf.size());
    
    // 10. Perform translateString
    lou_translateString(table_to_use, inbuf.data(), &inlen, outbuf.data(), 
                       &outlen, typeform, spacing, mode);
    
    // Reset lengths for back translation
    inlen = outlen;  // Use output from translate as input for back translate
    backlen = static_cast<int>(backbuf.size());
    
    // 11. Perform backTranslateString
    lou_backTranslateString(table_to_use, outbuf.data(), &inlen, 
                           backbuf.data(), &backlen, typeform, spacing, mode);
    
    // 12. Cleanup
    assert_file_closed(&in_file);;
    
    // Open output file for writing (though we won't write to it in this example)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int fd_out = fuzz_fileno(out_file);
        (void)fd_out; // Use to avoid unused variable warning
        assert_file_closed(&out_file);;
    }
    
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    // 13. Final cleanup
    lou_free();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
}