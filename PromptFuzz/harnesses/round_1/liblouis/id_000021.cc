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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 8.5, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["lou_getDataPath","lou_logPrint","lou_logPrint","lou_listTables","lou_logPrint","lou_checkTable","lou_freeTableFiles","lou_logPrint","lou_getTable","lou_freeTableFiles","lou_logPrint","lou_getEmphClasses","lou_logPrint","lou_freeEmphClasses","lou_freeTableFiles","lou_logPrint","lou_freeTableFiles","lou_logPrint","lou_backTranslateString","lou_logPrint","lou_backTranslate","lou_logPrint","lou_dotsToChar","lou_logPrint","lou_freeTableFiles","lou_free"],"critical_calls":["lou_getDataPath","lou_logPrint","lou_logPrint","lou_listTables","lou_checkTable","lou_getTable","lou_getEmphClasses","lou_logPrint","lou_freeEmphClasses","lou_backTranslateString","lou_logPrint","lou_backTranslate","lou_logPrint","lou_dotsToChar","lou_logPrint","lou_freeTableFiles","lou_free"],"visited":1}
/**/


// Include the liblouis headers

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Validate input constraints
    if (!data || size == 0) {
        return 0;
    }

    // 1. Use lou_getDataPath()
    const char *dataPath = lou_getDataPath();
    if (dataPath) {
        lou_logPrint("Data path: %s", dataPath);
    }

    // 2. Use lou_logPrint()
    lou_logPrint("Fuzzing input size: %zu", size);

    // 3. Use lou_listTables() to get available tables
    char **tables = lou_listTables();
    if (!tables) {
        lou_logPrint("No tables available");
        return 0;
    }

    // Find the first valid table
    const char *tableList = nullptr;
    for (int i = 0; tables[i] != nullptr; ++i) {
        if (lou_checkTable(tables[i]) == 0) {
            tableList = tables[i];
            break;
        }
    }

    if (!tableList) {
        lou_freeTableFiles(tables);
        lou_logPrint("No valid tables found");
        return 0;
    }

    // 4. Use lou_getTable()
    const void *table = lou_getTable(tableList);
    if (!table) {
        lou_freeTableFiles(tables);
        lou_logPrint("Failed to get table");
        return 0;
    }

    // 5. Use lou_getEmphClasses()
    const char **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        for (int i = 0; emphClasses[i] != nullptr; ++i) {
            lou_logPrint("Emph class: %s", emphClasses[i]);
        }
        lou_freeEmphClasses(emphClasses);
    }

    // Create FILE* for input using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        lou_freeTableFiles(tables);
        lou_logPrint("Failed to create input file");
        assert_file_closed(&in_file);
	return 0;
    }

    // Read input as widechars (assuming UTF-16 like encoding)
    std::vector<widechar> inbuf;
    widechar wc;
    while (fread(&wc, sizeof(widechar), 1, in_file) == 1) {
        inbuf.push_back(wc);
    }
    assert_file_closed(&in_file);;

    if (inbuf.empty()) {
        lou_freeTableFiles(tables);
        lou_logPrint("No widechars read from input");
        assert_file_closed(&in_file);
	return 0;
    }

    int inlen = static_cast<int>(inbuf.size());
    int outlen = inlen * 2; // Conservative estimate
    std::vector<widechar> outbuf(outlen);
    std::vector<char> spacing(inlen, 0);
    formtype typeform = 0;

    // 6. Use lou_backTranslateString()
    int result1 = lou_backTranslateString(
        tableList,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        &typeform,
        spacing.data(),
        0
    );

    lou_logPrint("backTranslateString result: %d, inlen: %d, outlen: %d", 
                 result1, inlen, outlen);

    // Reset variables for lou_backTranslate
    inlen = static_cast<int>(inbuf.size());
    outlen = inlen * 2;
    outbuf.assign(outlen, 0);
    std::vector<int> outputPos(outlen);
    std::vector<int> inputPos(inlen);
    int cursorPos = 0;

    // 7. Use lou_backTranslate()
    int result2 = lou_backTranslate(
        tableList,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        &typeform,
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        &cursorPos,
        0
    );

    lou_logPrint("backTranslate result: %d, cursorPos: %d", result2, cursorPos);

    // 8. Use lou_dotsToChar() - convert dots back to characters
    // First need to prepare dots data (simulate dots input)
    std::vector<widechar> dotsBuf;
    for (size_t i = 0; i < std::min<size_t>(size, 100); i++) {
        dotsBuf.push_back(static_cast<widechar>(data[i] % 256));
    }
    
    if (!dotsBuf.empty()) {
        std::vector<widechar> charBuf(dotsBuf.size());
        int dotsResult = lou_dotsToChar(
            tableList,
            dotsBuf.data(),
            charBuf.data(),
            static_cast<int>(dotsBuf.size()),
            fuzz_int32_t_1
        );
        lou_logPrint("dotsToChar result: %d", dotsResult);
    }

    // Create output file (not strictly necessary for fuzzing but follows instructions)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some output if available
        if (outlen > 0) {
            fwrite(outbuf.data(), sizeof(widechar), 
                   std::min<size_t>(outlen, outbuf.size()), out_file);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up
    lou_freeTableFiles(tables);
    
    // Free internal liblouis resources
    lou_free();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}