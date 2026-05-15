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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["lou_compileString","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_findTable","lou_freeTableFile","lou_getTableInfo","lou_freeTableInfo","lou_findTables","lou_freeTableFile","lou_getTableInfo","lou_freeTableInfo","lou_translate","lou_logEnd"],"critical_calls":["lou_compileString","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_findTable","lou_freeTableFile","lou_getTableInfo","lou_freeTableInfo","lou_findTables","lou_freeTableFile","lou_getTableInfo","lou_freeTableInfo","lou_translate","lou_logEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs:

*/


// Include liblouis headers (assuming they're available in the build environment)
extern "C" {
    int lou_compileString(const char * tableList, const char * inString);
    const char **lou_getEmphClasses(const char * tableList);
    void lou_freeEmphClasses(const char **);
    char *lou_findTable(const char * query);
    void lou_freeTableFile(char *);
    char *lou_getTableInfo(const char * table, const char * key);
    void lou_freeTableInfo(char *);
    char **lou_findTables(const char * query);
    formtype lou_getTypeformForEmphClass(const char * tableList, const char * emphClass);
    int lou_translate(const char * tableList, const unsigned short * inbuf, int * inlen, 
                     unsigned short * outbuf, int * outlen, unsigned short * typeform, 
                     char * spacing, int * outputPos, int * inputPos, int * cursorPos, int mode);
    void lou_logEnd();
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<197) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	//fuzzer shim end}




    if (size < 2) {
	return 0;
	}
	
    
    // Use a fixed table for consistency in fuzzing
    const char* tableList = "tests/tables/en-us-g2.ctb";
    
    // 1. Compile a table string from input data
    // Extract a null-terminated string from the input
    size_t tableStrLen = size < 100 ? size : 100;
    std::vector<char> tableStr(tableStrLen + 1);
    memcpy(tableStr.data(), data, tableStrLen);
    tableStr[tableStrLen] = '\0';
    
    // Try to compile the string as a table
    int compileResult = lou_compileString(fuzz_str_1, tableStr.data());
    
    // 2. Get emphasis classes
    const char** emphClasses = lou_getEmphClasses(fuzz_str_2);
    if (emphClasses) {
        // 7. Get typeform for emphasis class if available
        if (emphClasses[0] != nullptr) {
            formtype typeform = lou_getTypeformForEmphClass(fuzz_str_3, emphClasses[0]);
            (void)typeform; // Use the result to avoid unused variable warning
        }
        
        // 3. Free emphasis classes
        lou_freeEmphClasses(emphClasses);
    }
    
    // 4. Find a table using part of input as query
    size_t queryLen = size < 50 ? size : 50;
    std::vector<char> query(queryLen + 1);
    memcpy(query.data(), data, queryLen);
    query[queryLen] = '\0';
    
    char* foundTable = lou_findTable(query.data());
    if (foundTable) {
        // 5. Free the found table file
        lou_freeTableFile(foundTable);
    }
    
    // 6. Get table information
    char* tableInfo = lou_getTableInfo(fuzz_str_4, fuzz_str_5);
    if (tableInfo) {
        lou_freeTableInfo(tableInfo);
    }
    
    // 8. Find multiple tables
    char** foundTables = lou_findTables(fuzz_str_6);
    if (foundTables) {
        // Free each table file and the array itself
        for (int i = 0; foundTables[i] != nullptr; i++) {
            lou_freeTableFile(foundTables[i]);
        }
        // Note: lou_freeTableFiles would be used here if available,
        // but we'll manually free the array
        free(foundTables);
    }
    
    // 9. Get table information again (duplicate API in list)
    tableInfo = lou_getTableInfo(fuzz_str_7, fuzz_str_8);
    if (tableInfo) {
        lou_freeTableInfo(tableInfo);
    }
    
    // 10. Perform translation using input data
    // Prepare input buffer from remaining data
    size_t remainingData = size - (size > 200 ? 200 : size);
    if (remainingData > sizeof(unsigned short)) {
        size_t widecharCount = remainingData / sizeof(unsigned short);
        if (widecharCount > 0) {
            std::vector<unsigned short> inbuf(widecharCount);
            std::vector<unsigned short> outbuf(widecharCount * 2); // Allocate extra space
            
            // Copy data to input buffer
            memcpy(inbuf.data(), data + (size > 200 ? 200 : 0), 
                   widecharCount * sizeof(unsigned short));
            
            int inlen = static_cast<int>(widecharCount);
            int outlen = static_cast<int>(widecharCount * 2);
            std::vector<unsigned short> typeform(widecharCount, 0);
            std::vector<char> spacing(widecharCount, 0);
            std::vector<int> outputPos(widecharCount * 2, 0);
            std::vector<int> inputPos(widecharCount * 2, 0);
            std::vector<int> cursorPos(1, 0);
            
            // Perform translation
            int translateResult = lou_translate(
                fuzz_str_9,
                inbuf.data(),
                &inlen,
                outbuf.data(),
                &outlen,
                typeform.data(),
                spacing.data(),
                outputPos.data(),
                inputPos.data(),
                cursorPos.data(),
                fuzz_int32_t_10  // mode
            );
            
            (void)translateResult; // Use result to avoid warning
        }
    }
    
    // 11. End logging
    lou_logEnd();
    
    return 0;
}