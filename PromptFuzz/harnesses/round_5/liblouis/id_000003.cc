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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 1, nr_unique_branch: 0
//<Quality> {"density":2,"unique_branches":{},"library_calls":["lou_setLogLevel","lou_getDataPath","lou_readCharFromFile","lou_checkTable","lou_findTable","lou_freeTableFile","lou_listTables","lou_freeTableFiles"],"critical_calls":["lou_setLogLevel","lou_getDataPath","lou_readCharFromFile","lou_checkTable","lou_findTable","lou_freeTableFile","lou_listTables","lou_freeTableFiles"],"visited":1}
/**/


// Include liblouis headers (adjust paths as needed)

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if no data
    if (size == 0) {
	return 0;
	}
	

    // 1. Set log level - use first byte modulo valid levels
    // Assuming logLevels enum has at least 4 levels (0-3)
    int log_level = data[0] % 4;
    lou_setLogLevel(static_cast<logLevels>(log_level));

    // 2. Get and free data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        // Could optionally use the path, but we just free it per API requirements
        free(data_path); // lou_getDataPath returns malloc'd string
    }

    // 3. Create input file from data for lou_readCharFromFile
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Write data to a temporary file for lou_readCharFromFile
    const char *temp_filename = "temp_input_file";
    FILE *temp_file = fopen(temp_filename, "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
    } else {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Read character from the temporary file
    int mode = 0;
    int result = lou_readCharFromFile(temp_filename, &mode);
    // Result is ignored as we're just testing the API

    // 5. Check table using a substring from data as table name
    // Use first 100 bytes or less for table name
    size_t table_name_len = size < 100 ? size : 100;
    std::vector<char> table_name(table_name_len + 1);
    memcpy(table_name.data(), data, table_name_len);
    table_name[table_name_len] = '\0';
    
    int check_result = lou_checkTable(table_name.data());
    // Result ignored for fuzzing

    // 6. Find table (if any) and free it
    // Use a different part of data for query
    size_t query_offset = size / 2;
    size_t query_len = size - query_offset;
    if (query_len > 100) query_len = 100;
    
    std::vector<char> query(query_len + 1);
    memcpy(query.data(), data + query_offset, query_len);
    query[query_len] = '\0';
    
    char *found_table = lou_findTable(query.data());
    if (found_table) {
        lou_freeTableFile(found_table);
    }

    // 7. List tables and free them
    char **table_list = lou_listTables();
    if (table_list) {
        lou_freeTableFiles(table_list);
    }

    // 8. Cleanup
    assert_file_closed(&in_file);;
    remove(temp_filename); // Delete temporary file
    
    // Note: lou_free() is not called as it would cleanup global state
    // which could affect subsequent fuzzing iterations
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
}