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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 11.666667, nr_unique_branch: 34
//<Quality> {"density":7,"unique_branches":{"for_updatePositions":[[1536,6,1536,20,0,0,4,0],[1537,7,1537,29,0,0,4,0],[1537,33,1537,67,0,0,4,0]],"_lou_allocMem":[[5288,7,5288,28,0,0,4,1],[5299,7,5299,25,0,0,4,0],[5306,7,5306,29,0,0,4,0]],"for_selectRule":[[2042,7,2042,21,0,0,4,0]],"_lou_translate":[[1216,6,1216,23,0,0,4,0],[1217,15,1217,31,0,0,4,0],[1217,15,1217,31,0,0,4,1],[1218,6,1218,23,0,0,4,0],[1218,27,1218,42,0,0,4,0],[1218,27,1218,42,0,0,4,1],[1221,7,1221,53,0,0,4,1],[1273,7,1273,24,0,0,4,1],[1376,7,1376,24,0,0,4,0],[1379,16,1379,27,0,0,4,0],[1379,16,1379,27,0,0,4,1],[1380,9,1380,30,0,0,4,0],[1380,9,1380,30,0,0,4,1],[1381,13,1381,34,0,0,4,0],[1381,13,1381,34,0,0,4,1],[1382,11,1382,21,0,0,4,0],[1382,11,1382,21,0,0,4,1],[1382,25,1382,39,0,0,4,0],[1383,27,1383,37,0,0,4,1],[1388,8,1388,17,0,0,4,0],[1388,8,1388,17,0,0,4,1],[1389,11,1389,25,0,0,4,0],[1389,11,1389,25,0,0,4,1],[1396,6,1396,23,0,0,4,0],[1396,27,1396,43,0,0,4,0],[1396,27,1396,43,0,0,4,1],[1397,7,1397,24,0,0,4,0]]},"library_calls":["lou_listTables","lou_freeTableFiles","lou_freeTableFiles","lou_logFile","lou_translate","lou_charToDots","lou_translatePrehyphenated","lou_freeTableFiles","lou_logEnd"],"critical_calls":["lou_listTables","lou_logFile","lou_translate","lou_charToDots","lou_translatePrehyphenated","lou_freeTableFiles","lou_logEnd"],"visited":2}
/**/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 4) {
	return 0;
	}
	
    
    // 1. List available tables
    char **tables = lou_listTables();
    if (!tables) {
	return 0;
	}
	
    
    // Choose a table (use first available)
    const char *table = tables[0];
    if (!table) {
        lou_freeTableFiles(tables);
        return 0;
    }
    
    // 2. Setup input buffer from fuzzer data
    // Convert data to widechar (UTF-16) - using half the size for widechars
    size_t wc_size = size / 2;
    if (wc_size == 0) {
        lou_freeTableFiles(tables);
        return 0;
    }
    
    std::vector<widechar> inbuf(wc_size);
    memcpy(inbuf.data(), data, wc_size * sizeof(widechar));
    
    // 3. Setup output buffers
    std::vector<widechar> outbuf(wc_size * 2);  // Allow expansion
    std::vector<char> hyphens(wc_size);
    std::vector<char> outputHyphens(wc_size * 2);
    std::vector<int> outputPos(wc_size * 2);
    std::vector<int> inputPos(wc_size * 2);
    std::vector<int> cursorPos(1);
    
    // Initialize variables
    int inlen = wc_size;
    int outlen = outbuf.size();
    formtype *typeform = nullptr;  // No typeform
    char *spacing = nullptr;       // No spacing
    int mode = 0;                  // Default mode
    
    // 4. Setup logging to file
    lou_logFile(fuzz_str_1);
    
    // 5. First translation with standard translate
    int trans_result = lou_translate(
        table,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform,
        spacing,
        outputPos.data(),
        inputPos.data(),
        cursorPos.data(),
        fuzz_int32_t_2
    );
    
    // 6. Reset lengths for next operation
    inlen = wc_size;
    outlen = outbuf.size();
    
    // 7. charToDots conversion
    std::vector<widechar> dotsbuf(wc_size);
    int dots_result = lou_charToDots(
        table,
        inbuf.data(),
        dotsbuf.data(),
        inlen,
        fuzz_int32_t_3
    );
    
    // 8. Reset lengths again
    inlen = wc_size;
    outlen = outbuf.size();
    
    // 9. Translate with hyphenation support
    int prehyphen_result = lou_translatePrehyphenated(
        table,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform,
        spacing,
        outputPos.data(),
        inputPos.data(),
        cursorPos.data(),
        hyphens.data(),
        outputHyphens.data(),
        fuzz_int32_t_4
    );
    
    // 10. Cleanup
    lou_freeTableFiles(tables);
    lou_logEnd();  // Close log file
    
    // All operations complete
    return 0;
}