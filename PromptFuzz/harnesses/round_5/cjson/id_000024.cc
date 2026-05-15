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
#include <cJSON.h>
}
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 2
//<Quality> {"density":5,"unique_branches":{"cJSON_SetValuestring":[[457,16,457,58,0,0,4,1],[457,62,457,104,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_CreateString","cJSON_CreateBool","cJSON_CreateFalse","cJSON_IsFalse","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_Compare","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_CreateString","cJSON_CreateBool","cJSON_CreateFalse","cJSON_IsFalse","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_Compare","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

// Helper function to traverse and modify JSON structure
static void  id_000024_modify_json_structure(cJSON *item) {
    if (!item) return;
    
    if (cJSON_IsString(item)) {
        // Change string value to "modified"
        char *new_val = strdup("modified");
        if (new_val) {
            cJSON_SetValuestring(item, new_val);
            free(new_val);
        }
    } else if (cJSON_IsNumber(item)) {
        // Change number value to 42.0
        cJSON_SetNumberHelper(item, 42.0);
    }
    
    // Recursively process children
    if (item->child) {
         id_000024_modify_json_structure(item->child);
    }
    // Process next sibling
    if (item->next) {
         id_000024_modify_json_structure(item->next);
    }
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have valid input
    if (size == 0 || data == nullptr) {
        return 0;
    }
    
    // Create a null-terminated string from input data
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse JSON using cJSON_ParseWithLength
    cJSON *parsed = cJSON_ParseWithLength(json_str, size);
    if (!parsed) {
        free(json_str);
        return 0;
    }
    
    // Create a duplicate of the parsed JSON
    cJSON *duplicate = cJSON_Duplicate(parsed, 1); // recurse = true
    
    if (duplicate) {
        // Modify the duplicate structure
         id_000024_modify_json_structure(duplicate);
        
        // Create additional JSON items for testing
        cJSON *test_string = cJSON_CreateString("test");
        cJSON *test_bool = cJSON_CreateBool(1);
        cJSON *test_false = cJSON_CreateFalse();
        
        if (test_string && test_bool && test_false) {
            // Check if the false item is indeed false
            if (cJSON_IsFalse(test_false)) {
                // Create a raw JSON item (though we can't easily test IsRaw without actual raw JSON)
                // We'll create a raw item and add it to the duplicate
                cJSON *raw_item = cJSON_CreateRaw("\"raw_value\"");
                if (raw_item) {
                    // Check if it's raw (should return true)
                    cJSON_IsRaw(raw_item);
                    cJSON_Delete(raw_item);
                }
            }
            
            // Compare original and modified duplicate
            cJSON_Compare(parsed, duplicate, 1); // case_sensitive = true
            
            // Clean up test items
            cJSON_Delete(test_string);
            cJSON_Delete(test_bool);
            cJSON_Delete(test_false);
        }
        
        // Clean up duplicate
        cJSON_Delete(duplicate);
    }
    
    // Clean up
    cJSON_Delete(parsed);
    free(json_str);
    
    return 0;
}