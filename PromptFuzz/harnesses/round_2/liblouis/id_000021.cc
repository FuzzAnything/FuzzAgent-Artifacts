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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 35
//<Quality> {"density":6,"unique_branches":{"analyzeTable":[[734,25,734,44,0,0,4,0],[735,13,735,29,0,0,4,0],[735,33,735,52,0,0,4,1],[761,11,761,18,0,0,4,0],[769,12,769,19,0,0,4,0],[786,12,786,19,0,0,4,0],[791,16,791,20,0,0,4,0],[791,16,791,20,0,0,4,1],[792,14,792,27,0,0,4,0],[792,14,792,27,0,0,4,1],[793,15,793,21,0,0,4,0],[803,13,803,18,0,0,4,0],[803,22,803,37,0,0,4,1]],"matchLanguageTags":[[381,11,381,50,0,0,4,1],[385,9,385,14,0,0,4,1],[386,7,386,11,0,0,4,1],[387,7,387,46,0,0,4,1],[391,14,391,36,0,0,4,0],[391,14,391,36,0,0,4,1],[397,9,397,12,0,0,4,1]],"matchFeatureLists":[[476,11,476,16,0,0,4,0],[476,20,476,28,0,0,4,0],[483,10,483,18,0,0,4,0],[494,17,494,52,0,0,4,1]],"lou_logFile":[[204,6,204,21,0,0,4,0],[204,25,204,51,0,0,4,0],[206,6,206,21,0,0,4,0]],"lou_getTableInfo":[[1152,21,1152,22,0,0,4,0],[1155,7,1155,15,0,0,4,0],[1155,7,1155,15,0,0,4,1],[1156,8,1156,22,0,0,4,0],[1157,9,1157,38,0,0,4,0],[1157,9,1157,38,0,0,4,1],[1163,14,1163,21,0,0,4,0],[1163,14,1163,21,0,0,4,1]]},"library_calls":["lou_findTables","lou_findTables","lou_getTable","lou_getTableInfo","lou_logFile","lou_dotsToChar","lou_freeTableInfo","lou_freeTableFiles","lou_logEnd"],"critical_calls":["lou_findTables","lou_findTables","lou_getTable","lou_getTableInfo","lou_logFile","lou_dotsToChar","lou_freeTableInfo","lou_freeTableFiles","lou_logEnd"],"visited":1}
/**/


extern "C" {
}

// Helper function to safely extract strings from the input data
std::string  id_000021_extract_string(const uint8_t* data, size_t size, size_t& offset, size_t max_len) {
    if (offset >= size) return "";
    size_t len = std::min(max_len, size - offset);
    // Find null terminator within the extracted segment
    const uint8_t* start = data + offset;
    const uint8_t* end = start + len;
    const uint8_t* null_pos = static_cast<const uint8_t*>(memchr(start, '\0', len));
    if (null_pos && null_pos < end) {
        len = null_pos - start;
    }
    std::string result(reinterpret_cast<const char*>(start), len);
    offset += len;
    if (offset < size && data[offset] == '\0') offset++; // Skip null terminator
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Too small for meaningful fuzzing

    size_t offset = 0;
    
    // Extract components from the input data
    // First part: table query string
    std::string query =  id_000021_extract_string(data, size, offset, 50);
    if (query.empty()) query = "en-us-comp8";
    
    // Second part: table key for getTableInfo
    std::string table_key =  id_000021_extract_string(data, size, offset, 20);
    if (table_key.empty()) table_key = "locale";
    
    // Third part: filename for logging
    std::string log_filename =  id_000021_extract_string(data, size, offset, 30);
    if (log_filename.empty()) log_filename = "fuzz_log.txt";
    
    // Remaining data will be used for dots conversion
    size_t remaining_size = size - offset;
    if (remaining_size == 0) {
	return 0;
	}
	
    
    // 1. Find tables using the query
    char** tables = lou_findTables(query.c_str());
    if (!tables) {
        // Try with a default query if the first one failed
        tables = lou_findTables("en");
    }
    
    if (!tables) {
        // No tables found, can't proceed further
        return 0;
    }
    
    // 2. Get the first found table and retrieve its handle
    const char* table_name = tables[0];
    const void* table_handle = lou_getTable(table_name);
    
    // 3. Get table information
    char* table_info = lou_getTableInfo(table_name, table_key.c_str());
    
    // 4. Set up log file
    lou_logFile(log_filename.c_str());
    
    // 5. Prepare input buffer for dotsToChar conversion
    // Convert remaining data to widechar array
    size_t dots_length = remaining_size / sizeof(widechar);
    if (dots_length == 0) {
        dots_length = 1;
    }
    
    std::vector<widechar> inbuf(dots_length);
    std::vector<widechar> outbuf(dots_length);
    
    // Fill input buffer with data
    size_t copy_size = std::min(remaining_size, dots_length * sizeof(widechar));
    memcpy(inbuf.data(), data + offset, copy_size);
    
    // 6. Perform dots to char conversion
    int mode = 0; // Default mode
    int result = lou_dotsToChar(table_name, inbuf.data(), outbuf.data(), 
                               static_cast<int>(dots_length), mode);
    
    // Clean up
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    lou_logEnd(); // Close log file
    
    return 0;
}