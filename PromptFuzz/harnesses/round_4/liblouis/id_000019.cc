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
//<ID> 463
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"lou_logFile":[[201,6,201,22,0,0,4,0]]},"library_calls":["lou_version","lou_listTables","lou_freeTableFiles","lou_logFile","lou_freeTableFiles","lou_hyphenate","lou_getEmphClasses","lou_freeEmphClasses","lou_freeTableFiles","lou_free","lou_logFile"],"critical_calls":["lou_version","lou_listTables","lou_logFile","lou_hyphenate","lou_getEmphClasses","lou_freeEmphClasses","lou_freeTableFiles","lou_free","lou_logFile"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs:

*/


// Include liblouis headers
extern "C" {
    const char *lou_version();
    int lou_hyphenate(const char * tableList, const widechar * inbuf, int inlen, char * hyphens, int mode);
    const char **lou_getEmphClasses(const char * tableList);
    void lou_logFile(const char * filename);
    char **lou_listTables();
    void lou_freeEmphClasses(const char **);
    void lou_free();
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 2) {
	return 0;
	}
	
    
    // 1. Get liblouis version
    const char *version = lou_version();
    if (version) {
        // Version string is available, but we don't use it further
    }
    
    // 2. List available tables
    char **tables = lou_listTables();
    if (!tables || !tables[0]) {
        // No tables available, can't proceed with other operations
        if (tables) {
            // Free the table list if it was allocated
            void *temp = tables;
            lou_freeTableFiles((char **)temp);
        }
        return 0;
    }
    
    // Use the first available table
    const char *table = tables[0];
    
    // 3. Set up log file (using a fixed filename as per instructions)
    lou_logFile(fuzz_str_1);
    
    // 4. Prepare input data for hyphenation
    // We'll interpret the data as widechar array
    int inlen = size / sizeof(widechar);
    if (inlen < 1) {
        // Not enough data for even one widechar
        void *temp = tables;
        lou_freeTableFiles((char **)temp);
        return 0;
    }
    
    // Convert data to widechar array
    std::vector<widechar> inbuf(inlen);
    memcpy(inbuf.data(), data, inlen * sizeof(widechar));
    
    // Ensure null termination for safety
    if (inlen > 0) {
        // Make sure we don't have embedded nulls that might be interpreted as string end
        // We'll use the full buffer as is
    }
    
    // 5. Allocate hyphens array (needs to be at least inlen+2 according to liblouis docs)
    std::vector<char> hyphens(inlen + 2);
    
    // 6. Call hyphenate with the input data
    // Mode 0 is the default
    int result = lou_hyphenate(table, inbuf.data(), inlen, hyphens.data(), fuzz_int32_t_2);
    
    // 7. Get emphasis classes for the table
    const char **emphClasses = lou_getEmphClasses(table);
    
    // 8. Free emphasis classes if they were allocated
    if (emphClasses) {
        lou_freeEmphClasses(emphClasses);
    }
    
    // 9. Free the table list
    void *temp = tables;
    lou_freeTableFiles((char **)temp);
    
    // 10. Free liblouis resources
    lou_free();
    
    // 11. Clean up any file resources
    // The log file was opened by liblouis, we should close it
    // lou_logFile with NULL filename closes the log file
    lou_logFile(NULL);
    
    // Remove the output file if it was created
    remove("output_file");
    
    assert_file_name_closed("output_file");
	return 0;
}