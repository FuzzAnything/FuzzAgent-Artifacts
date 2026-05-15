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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 25
//<Quality> {"density":11,"unique_branches":{"cJSON_DetachItemViaPointer":[[2270,9,2270,27,0,0,4,1],[2281,14,2281,32,0,0,4,0]],"case_insensitive_strcmp":[[135,9,135,26,0,0,4,1],[135,30,135,47,0,0,4,1],[140,9,140,27,0,0,4,1],[145,11,145,49,0,0,4,0],[145,11,145,49,0,0,4,1],[147,13,147,29,0,0,4,0],[147,13,147,29,0,0,4,1]],"cJSON_IsString":[[3034,9,3034,21,0,0,4,1]],"cJSON_CreateTrue":[[2475,8,2475,12,0,0,4,0]],"cJSON_CreateRaw":[[2584,8,2584,12,0,0,4,0],[2588,12,2588,30,0,0,4,1]],"print":[[1252,9,1252,31,0,0,4,1],[1258,9,1258,35,0,0,4,1],[1265,9,1265,34,0,0,4,0],[1268,13,1268,28,0,0,4,1]],"cJSON_IsObject":[[3054,9,3054,21,0,0,4,1]],"get_object_item":[[1961,9,1961,23,0,0,4,1],[1970,16,1970,41,0,0,4,0],[1970,45,1970,152,0,0,4,0],[1970,45,1970,152,0,0,4,1]],"cJSON_IsFalse":[[2984,9,2984,21,0,0,4,1]],"cJSON_AddRawToObject":[[2225,9,2225,73,0,0,4,0]],"cJSON_IsNumber":[[3024,9,3024,21,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsObject","cJSON_CreateTrue","cJSON_AddRawToObject","cJSON_IsString","cJSON_IsNumber","cJSON_IsFalse","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsObject","cJSON_CreateTrue","cJSON_AddRawToObject","cJSON_IsString","cJSON_IsNumber","cJSON_IsFalse","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // 1. Parse input data as JSON
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
        return 0; // Invalid JSON input
    }
    
    // 2. Check if parsed item is an object
    if (cJSON_IsObject(parsed)) {
        // 3. Create a true value
        cJSON *true_item = cJSON_CreateTrue();
        
        // 4. Add raw JSON string to object
        const char *raw_json = "{\"fuzz\":\"test\"}";
        cJSON *raw_item = cJSON_AddRawToObject(parsed, fuzz_str_1, fuzz_str_2);
        
        // 5. Check various types in the object
        cJSON *child = parsed->child;
        while (child) {
            if (cJSON_IsString(child)) {
                // Found a string field
            }
            if (cJSON_IsNumber(child)) {
                // Found a number field
            }
            if (cJSON_IsFalse(child)) {
                // Found a false field
            }
            child = child->next;
        }
        
        // 6. Detach an item from object (if it exists)
        cJSON *detached = cJSON_DetachItemFromObject(parsed, fuzz_str_3);
        if (detached) {
            cJSON_Delete(detached);
        }
        
        // 7. Print the modified JSON
        char *printed = cJSON_Print(parsed);
        if (printed) {
            // Could write to file if needed
            // FILE *out_file = fopen("output_file", "wb");
            // if (out_file) {
            //     fwrite(printed, 1, strlen(printed), out_file);
            //     assert_file_closed(&out_file);;
            // }
            cJSON_free(printed);
        }
        
        cJSON_Delete(true_item);
    }
    
    // 8. Clean up
    cJSON_Delete(parsed);
    
    return 0;
}