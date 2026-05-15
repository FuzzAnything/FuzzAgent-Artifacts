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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 17
//<Quality> {"density":8,"unique_branches":{"lou_getTableInfo":[[1152,21,1152,22,0,0,4,0],[1155,7,1155,15,0,0,4,1],[1163,14,1163,21,0,0,4,0],[1163,14,1163,21,0,0,4,1]],"analyzeTable":[[734,25,734,44,0,0,4,0],[735,13,735,29,0,0,4,0],[735,33,735,52,0,0,4,1],[761,11,761,18,0,0,4,0],[769,12,769,19,0,0,4,0],[786,12,786,19,0,0,4,0],[791,16,791,20,0,0,4,0],[791,16,791,20,0,0,4,1],[792,14,792,27,0,0,4,0],[792,14,792,27,0,0,4,1],[793,15,793,21,0,0,4,0],[803,13,803,18,0,0,4,0],[803,22,803,37,0,0,4,1]]},"library_calls":["lou_compileString","lou_findTable","lou_getTableInfo","lou_freeTableInfo","lou_freeTableFile","lou_findTables","lou_freeTableFiles","lou_getTypeformForEmphClass","lou_dotsToChar","lou_charToDots","lou_free"],"critical_calls":["lou_compileString","lou_findTable","lou_getTableInfo","lou_freeTableInfo","lou_freeTableFile","lou_findTables","lou_freeTableFiles","lou_getTypeformForEmphClass","lou_dotsToChar","lou_charToDots","lou_free"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs to achieve an event of processing braille data:

*/


// Include liblouis headers (using forward declarations for types)
typedef unsigned short formtype;
typedef unsigned short widechar;

// External liblouis API declarations
extern "C" {
    int lou_compileString(const char * tableList, const char * inString);
    char *lou_getTableInfo(const char * table, const char * key);
    char *lou_findTable(const char * query);
    char **lou_findTables(const char * query);
    int lou_charToDots(const char * tableList, const widechar * inbuf, 
                      widechar * outbuf, int length, int mode);
    formtype lou_getTypeformForEmphClass(const char * tableList, const char * emphClass);
    int lou_dotsToChar(const char * tableList, widechar * inbuf, 
                      widechar * outbuf, int length, int mode);
    void lou_freeTableInfo(char *);
    void lou_freeTableFile(char *);
    void lou_freeTableFiles(char **);
    void lou_free();
}

// Helper function to check if pointer is within data bounds
static bool  id_000017_is_in_bounds(const uint8_t* data, size_t size, const void* ptr, size_t len = 1) {
    const uint8_t* start = data;
    const uint8_t* end = data + size;
    const uint8_t* p = static_cast<const uint8_t*>(ptr);
    return (p >= start && (p + len) <= end);
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<61) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Early return if data is too small
    if (size < 4) {
	return 0;
	}
	
    
    // We'll interpret the input as liblouis output data (braille dots)
    // The fuzzer will test round-trip conversion: dots -> chars -> dots
    
    // Use first part of data as table list name
    // Find null terminator or use first 64 bytes max
    size_t tableNameLen = 0;
    while (tableNameLen < size && tableNameLen < 64 && data[tableNameLen] != 0) {
        tableNameLen++;
    }
    
    if (tableNameLen == 0 || tableNameLen >= size) {
	return 0;
	}
	
    
    // Create table list string (ensure null termination)
    char* tableList = static_cast<char*>(malloc(tableNameLen + 1));
    if (!tableList) {
	return 0;
	}
	
    
    memcpy(tableList, data, tableNameLen);
    tableList[tableNameLen] = 0;
    
    // Remaining data for processing
    const uint8_t* brailleData = data + tableNameLen + 1;
    size_t brailleSize = size - tableNameLen - 1;
    
    if (brailleSize < sizeof(widechar) * 2) {
        free(tableList);
        return 0;
    }
    
    // 1. Try to compile the table
    // Use a fixed table string for compilation since we can't guarantee valid table syntax
    const char* testTableString = "table mytable \nchars a 1\n";
    lou_compileString(tableList, fuzz_str_1);
    
    // 2. Find tables and get table info
    char* foundTable = lou_findTable(tableList);
    if (foundTable) {
        char* tableInfo = lou_getTableInfo(foundTable, fuzz_str_2);
        if (tableInfo) {
            lou_freeTableInfo(tableInfo);
        }
        lou_freeTableFile(foundTable);
    }
    
    // 3. Find multiple tables
    char** allTables = lou_findTables(fuzz_str_3);
    if (allTables) {
        lou_freeTableFiles(allTables);
    }
    
    // 4. Get typeform for emphasis class
    formtype typeform = lou_getTypeformForEmphClass(tableList, fuzz_str_4);
    
    // 5. Process braille data through round-trip conversion
    // Calculate number of widechar elements we can process
    size_t maxElements = brailleSize / sizeof(widechar);
    if (maxElements > 256) maxElements = 256; // Limit for safety
    
    if (maxElements >= 2) {
        // Allocate buffers
        widechar* inbuf = static_cast<widechar*>(malloc(maxElements * sizeof(widechar)));
        widechar* outbuf1 = static_cast<widechar*>(malloc(maxElements * sizeof(widechar)));
        widechar* outbuf2 = static_cast<widechar*>(malloc(maxElements * sizeof(widechar)));
        
        if (inbuf && outbuf1 && outbuf2) {
            // Copy braille data to input buffer
            memcpy(inbuf, brailleData, maxElements * sizeof(widechar));
            
            // Ensure buffers are within bounds
            if ( id_000017_is_in_bounds(data, size, inbuf, maxElements * sizeof(widechar))) {
                // 6. Convert dots to characters
                int result1 = lou_dotsToChar(tableList, inbuf, outbuf1, 
                                           static_cast<int>(maxElements), fuzz_int32_t_5);
                
                if (result1 > 0) {
                    // 7. Convert characters back to dots
                    int result2 = lou_charToDots(tableList, outbuf1, outbuf2, 
                                               static_cast<int>(maxElements), fuzz_int32_t_6);
                    
                    // Optional: compare inbuf and outbuf2 to check round-trip consistency
                }
            }
        }
        
        // Clean up buffers
        free(inbuf);
        free(outbuf1);
        free(outbuf2);
    }
    
    // 8. Clean up resources
    free(tableList);
    
    // Final cleanup of liblouis
    lou_free();
    
    return 0;
}