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
//<ID> 642
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 4
//<Quality> {"density":5,"unique_branches":{"_lou_translate":[[1340,9,1340,52,0,0,4,0],[1392,6,1392,25,0,0,4,0]],"translateString":[[4005,29,4005,48,0,0,4,0],[4005,52,4005,74,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_indexTables","lou_findTables","lou_freeTableFiles","lou_listTables","lou_freeTableFiles","lou_free","lou_free","lou_charToDots","lou_getTypeformForEmphClass","lou_translate","lou_free"],"critical_calls":["lou_registerLogCallback","lou_indexTables","lou_findTables","lou_freeTableFiles","lou_listTables","lou_freeTableFiles","lou_charToDots","lou_getTypeformForEmphClass","lou_translate","lou_free"],"visited":1}
/**/


extern "C" {
}

// Custom log callback to handle logging
static void  id_000028_logCallback(logLevels level, const char* message) {
    // Simple callback that does nothing with the log message
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 2) {
        return 0;  // Not enough data to process
    }

    // Step 1: Register log callback
    lou_registerLogCallback( id_000028_logCallback);

    // Step 2: Create a temporary file with the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Read table list from the first part of input
    // We'll use the first byte as table list identifier
    char tableList[256] = {0};
    size_t tableListLen = (data[0] % 128) + 1;  // Ensure reasonable length
    if (tableListLen > size - 1) {
        tableListLen = size - 1;
    }
    
    // Copy table list from data (skip first byte which stores length)
    for (size_t i = 0; i < tableListLen && i + 1 < size; i++) {
        tableList[i] = data[i + 1];
    }
    tableList[tableListLen] = '\0';

    // Step 4: Index tables
    const char* tables[] = {tableList, nullptr};
    lou_indexTables(tables);

    // Step 5: Find tables using a query
    char** foundTables = lou_findTables(tableList);
    if (foundTables) {
        lou_freeTableFiles(foundTables);
    }

    // Step 6: List all available tables
    char** allTables = lou_listTables();
    if (allTables) {
        lou_freeTableFiles(allTables);
    }

    // Step 7: Prepare input buffer for character conversion
    // Calculate available data for widechar conversion
    size_t dataOffset = 1 + tableListLen;  // Skip length byte and table list
    if (dataOffset >= size) {
        assert_file_closed(&in_file);;
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }

    size_t remainingSize = size - dataOffset;
    size_t widecharCount = remainingSize / sizeof(widechar);
    
    if (widecharCount == 0) {
        assert_file_closed(&in_file);;
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }

    // Allocate buffers
    std::vector<widechar> inbuf(widecharCount);
    std::vector<widechar> outbuf(widecharCount * 2);  // Output might be larger
    
    // Convert bytes to widechars
    for (size_t i = 0; i < widecharCount && dataOffset + i * sizeof(widechar) + 1 < size; i++) {
        uint16_t val = 0;
        if (dataOffset + i * sizeof(widechar) + 1 < size) {
            val = (data[dataOffset + i * sizeof(widechar)] << 8) | 
                  data[dataOffset + i * sizeof(widechar) + 1];
        } else if (dataOffset + i * sizeof(widechar) < size) {
            val = data[dataOffset + i * sizeof(widechar)] << 8;
        }
        inbuf[i] = static_cast<widechar>(val);
    }

    // Step 8: Convert characters to dots
    int mode = 0;
    int length = static_cast<int>(widecharCount);
    lou_charToDots(tableList, inbuf.data(), outbuf.data(), length, mode);

    // Step 9: Get typeform for emphasis class
    // Use a simple emphasis class name
    const char* emphClass = "italic";
    formtype typeform = lou_getTypeformForEmphClass(tableList, emphClass);

    // Step 10: Prepare for translation
    int inlen = static_cast<int>(widecharCount);
    int outlen = static_cast<int>(outbuf.size());
    
    std::vector<formtype> typeformBuf(widecharCount, typeform);
    std::vector<char> spacingBuf(widecharCount, 0);
    std::vector<int> outputPos(widecharCount, 0);
    std::vector<int> inputPos(widecharCount, 0);
    std::vector<int> cursorPos(1, 0);

    // Step 11: Perform translation
    lou_translate(tableList, inbuf.data(), &inlen, outbuf.data(), &outlen,
                  typeformBuf.data(), spacingBuf.data(), 
                  outputPos.data(), inputPos.data(), cursorPos.data(), mode);

    // Step 12: Cleanup
    assert_file_closed(&in_file);;
    lou_free();

    assert_file_closed(&in_file);
	return 0;
}