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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 2
//<Quality> {"density":16,"unique_branches":{"cJSON_Duplicate_rec":[[2821,27,2821,59,0,0,4,0]],"get_object_item":[[1955,29,1955,43,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_AddBoolToObject","cJSON_AddItemToObjectCS","cJSON_CreateString","cJSON_SetValuestring","cJSON_AddItemToObjectCS","cJSON_IsObject","cJSON_IsBool","cJSON_CreateRaw","cJSON_GetObjectItemCaseSensitive","cJSON_ReplaceItemViaPointer","cJSON_Duplicate","cJSON_Compare","cJSON_Print","cJSON_CreateStringReference","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_AddBoolToObject","cJSON_AddItemToObjectCS","cJSON_CreateString","cJSON_SetValuestring","cJSON_AddItemToObjectCS","cJSON_IsObject","cJSON_IsBool","cJSON_CreateRaw","cJSON_GetObjectItemCaseSensitive","cJSON_ReplaceItemViaPointer","cJSON_Duplicate","cJSON_Compare","cJSON_Print","cJSON_CreateStringReference","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Looking at the list of APIs and the input data, I'll create a fuzzer driver that processes JSON data, manipulates it, and tests various cjson library functions. The event I'll implement is: parsing JSON input, creating/modifying JSON objects, comparing them, and ensuring proper cleanup.

*/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<124) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}




    // Create null-terminated string from input data for parsing
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
	return 0;
	}
	
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // 1. Parse the input JSON using cJSON_Parse
    cJSON *parsed_json = cJSON_Parse(input_str);
    
    // 2. Create a new JSON object using cJSON_CreateObject
    cJSON *new_object = cJSON_CreateObject();
    
    if (parsed_json && new_object) {
        // 3. Add boolean to the new object using cJSON_AddBoolToObject
        cJSON *bool_item = cJSON_AddBoolToObject(new_object, "fuzz_bool", fuzz_int32_t_1);
        
        // 4. Add the parsed JSON to new object using cJSON_AddItemToObjectCS
        cJSON_AddItemToObjectCS(new_object, fuzz_str_2, parsed_json);
        
        // 5. Create a string using cJSON_CreateString
        cJSON *string_item = cJSON_CreateString(fuzz_str_3);
        
        // 6. Set value string using cJSON_SetValuestring
        if (string_item) {
            cJSON_SetValuestring(string_item, fuzz_str_4);
            
            // Add the string to new object
            cJSON_AddItemToObjectCS(new_object, fuzz_str_5, string_item);
        }
        
        // 7. Check if items are objects/bools using cJSON_IsObject and cJSON_IsBool
        cJSON_bool is_obj = cJSON_IsObject(new_object);
        cJSON_bool is_bool = cJSON_IsBool(bool_item);
        
        // 8. Create raw JSON using cJSON_CreateRaw
        cJSON *raw_item = cJSON_CreateRaw(fuzz_str_6);
        
        // 9. Get object item case sensitive using cJSON_GetObjectItemCaseSensitive
        cJSON *retrieved_item = cJSON_GetObjectItemCaseSensitive(new_object, "fuzz_bool");
        
        if (retrieved_item && raw_item) {
            // 10. Replace item via pointer using cJSON_ReplaceItemViaPointer
            cJSON_ReplaceItemViaPointer(new_object, retrieved_item, raw_item);
        }
        
        // 11. Compare JSON objects using cJSON_Compare
        cJSON *comparison_copy = cJSON_Duplicate(new_object, fuzz_int32_t_7);
        cJSON_bool compare_result = cJSON_Compare(new_object, comparison_copy, fuzz_int32_t_8);
        
        // 12. Print JSON using cJSON_Print
        char *printed_json = cJSON_Print(new_object);
        
        // 13. Create string reference using cJSON_CreateStringReference
        cJSON *string_ref = cJSON_CreateStringReference(fuzz_str_9);
        
        // Clean up comparison copy
        if (comparison_copy) {
            cJSON_Delete(comparison_copy);
        }
        
        // Clean up printed JSON string
        if (printed_json) {
            free(printed_json);
        }
        
        // Clean up string reference
        if (string_ref) {
            cJSON_Delete(string_ref);
        }
    }
    
    // 14. Clean up main objects using cJSON_Delete
    // Note: parsed_json is now owned by new_object, so we shouldn't delete it separately
    if (new_object) {
        cJSON_Delete(new_object);
    } else if (parsed_json) {
        // If new_object creation failed but parsed_json succeeded
        cJSON_Delete(parsed_json);
    }
    
    // Clean up input string
    free(input_str);
    
    return 0;
}