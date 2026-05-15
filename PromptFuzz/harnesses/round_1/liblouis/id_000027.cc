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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 3.5, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["lou_logEnd","lou_registerLogCallback","lou_setLogLevel","lou_setDataPath","lou_listTables","lou_free","lou_findTable","lou_freeTableFiles","lou_free","lou_getTable","lou_freeTableFile","lou_freeTableFiles","lou_free","lou_getTypeformForEmphClass","lou_freeTableFile","lou_freeTableFiles","lou_free","lou_backTranslate","lou_translatePrehyphenated","lou_freeTableFile","lou_freeTableFiles","lou_free"],"critical_calls":["lou_logEnd","lou_registerLogCallback","lou_setLogLevel","lou_setDataPath","lou_listTables","lou_findTable","lou_getTable","lou_getTypeformForEmphClass","lou_backTranslate","lou_translatePrehyphenated","lou_freeTableFile","lou_freeTableFiles","lou_free"],"visited":1}
/**/


// Include necessary headers for liblouis

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < sizeof(uint32_t)) {
	return 0;
	}
	
    
    // Setup logging first
    static bool logInitialized = false;
    if (!logInitialized) {
        lou_logEnd(); // Clean any previous logs
        // Use a dummy log callback that ignores messages
        lou_registerLogCallback([](logLevels level, const char* msg) {
            // Do nothing with the log message
        });
        lou_setLogLevel(static_cast<logLevels>(0)); // Minimal logging
        logInitialized = true;
    }
    
    // Set data path to current directory
    char *oldPath = lou_setDataPath(fuzz_str_1);
    if (oldPath) {
        // Old path returned, but we don't need to use it
    }
    
    // List available tables
    char **tables = lou_listTables();
    if (!tables) {
        lou_free();
        return 0;
    }
    
    // Find a table to use - try different queries based on input data
    const char *tableQuery = "en-us-g2.ctb"; // Default query
    char *foundTable = nullptr;
    
    // Use first byte to select query strategy
    uint8_t querySelector = data[0] % 4;
    
    switch (querySelector) {
        case 0:
            tableQuery = "en-*";
            break;
        case 1:
            tableQuery = "*-g2.ctb";
            break;
        case 2:
            tableQuery = "braille*";
            break;
        default:
            tableQuery = "en-us-g2.ctb";
    }
    
    foundTable = lou_findTable(fuzz_str_2);
    if (!foundTable) {
        // Try first available table
        if (tables[0]) {
            foundTable = strdup(tables[0]);
        } else {
            lou_freeTableFiles(tables);
            lou_free();
            return 0;
        }
    }
    
    // Get the table for use
    const void *tablePtr = lou_getTable(foundTable);
    if (!tablePtr) {
        if (foundTable) lou_freeTableFile(foundTable);
        lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }
    
    // Get emphasis class typeform
    formtype emphasisTypeform = lou_getTypeformForEmphClass(foundTable, fuzz_str_3);
    
    // Prepare input data for processing
    size_t processedSize = size - 1; // Skip first byte used for query selector
    const uint8_t *processData = data + 1;
    
    if (processedSize < sizeof(widechar)) {
        if (foundTable) lou_freeTableFile(foundTable);
        lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }
    
    // Convert input data to widechars (assuming little-endian)
    size_t widecharCount = processedSize / sizeof(widechar);
    std::vector<widechar> inbuf(widecharCount);
    
    for (size_t i = 0; i < widecharCount; i++) {
        inbuf[i] = (processData[i * sizeof(widechar)] |
                   (processData[i * sizeof(widechar) + 1] << 8));
    }
    
    // BACK-TRANSLATION (convert braille dots to text)
    int inlen = static_cast<int>(widecharCount);
    int outlen = inlen * 4; // Output could be larger
    std::vector<widechar> outbuf(outlen);
    std::vector<formtype> typeform(outlen);
    std::vector<char> spacing(outlen);
    
    // Initialize arrays
    std::fill(typeform.begin(), typeform.end(), emphasisTypeform);
    std::fill(spacing.begin(), spacing.end(), 0);
    
    int outputPos[outlen];
    int inputPos[outlen];
    int cursorPos = 0;
    int mode = 0;
    
    // Perform back-translation
    int backTranslateResult = lou_backTranslate(
        foundTable,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform.data(),
        spacing.data(),
        outputPos,
        inputPos,
        &cursorPos,
        mode
    );
    
    // If back-translation succeeded, try translatePrehyphenated
    if (backTranslateResult != 0 && outlen > 0) {
        // Prepare for forward translation with hyphenation
        int transInLen = outlen;
        int transOutLen = outlen * 4;
        std::vector<widechar> transOutBuf(transOutLen);
        std::vector<char> inputHyphens(transInLen);
        std::vector<char> outputHyphens(transOutLen);
        
        // Initialize hyphen arrays
        std::fill(inputHyphens.begin(), inputHyphens.end(), 0);
        std::fill(outputHyphens.begin(), outputHyphens.end(), 0);
        
        // Set some hyphens based on input data pattern
        for (int i = 0; i < transInLen && i < static_cast<int>(processedSize); i++) {
            inputHyphens[i] = (processData[i % processedSize] % 2) ? '-' : 0;
        }
        
        // Try translatePrehyphenated
        int transResult = lou_translatePrehyphenated(
            foundTable,
            outbuf.data(), // Use back-translation output as input
            &transInLen,
            transOutBuf.data(),
            &transOutLen,
            typeform.data(),
            spacing.data(),
            outputPos,
            inputPos,
            &cursorPos,
            inputHyphens.data(),
            outputHyphens.data(),
            mode | 1 // Include noContractions mode
        );
        
        // The result is not critical for fuzzing
        (void)transResult;
    }
    
    // Cleanup
    if (foundTable) {
        lou_freeTableFile(foundTable);
    }
    
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // Note: lou_free() is called automatically on program exit,
    // but we call it here to ensure clean state for next iteration
    lou_free();
    
    return 0;
}