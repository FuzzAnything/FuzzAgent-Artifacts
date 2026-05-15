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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 14
//<Quality> {"density":21,"unique_branches":{"cJSON_AddObjectToObject":[[2237,9,2237,76,0,0,4,0]],"replace_item_in_object":[[2429,9,2429,30,0,0,4,1],[2429,34,2429,50,0,0,4,1],[2435,9,2435,51,0,0,4,0],[2435,55,2435,84,0,0,4,1],[2440,9,2440,36,0,0,4,1]],"cJSON_CreateString":[[2534,8,2534,12,0,0,4,0],[2538,12,2538,30,0,0,4,1]],"cJSON_ReplaceItemViaPointer":[[2387,9,2387,30,0,0,4,1],[2400,13,2400,38,0,0,4,0],[2404,13,2404,38,0,0,4,0]],"case_insensitive_strcmp":[[135,30,135,47,0,0,4,0]],"cJSON_CreateBool":[[2499,22,2499,29,0,0,4,1]],"cJSON_AddStringToObject":[[2213,9,2213,76,0,0,4,0]]},"library_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_CreateObject","cJSON_IsObject","cJSON_AddObjectToObject","cJSON_AddStringToObject","cJSON_CreateBool","cJSON_CreateFalse","cJSON_CreateNull","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_IsFalse","cJSON_AddStringToObject","cJSON_CreateBool","cJSON_ReplaceItemInObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_CreateObject","cJSON_IsObject","cJSON_AddObjectToObject","cJSON_AddStringToObject","cJSON_CreateBool","cJSON_CreateFalse","cJSON_CreateNull","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_IsFalse","cJSON_AddStringToObject","cJSON_CreateBool","cJSON_ReplaceItemInObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<120) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}




    // 1. Check cJSON version
    const char *version = cJSON_Version();
    (void)version; // Use version to avoid unused variable warning
    
    // 2. Parse input data as JSON
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (root == NULL) {
        // If parsing fails, create a new object
        root = cJSON_CreateObject();
    }
    
    // 3. Check if root is an object
    if (cJSON_IsObject(root)) {
        // 4. Add a new object to the root
        cJSON *new_object = cJSON_AddObjectToObject(root, fuzz_str_1);
        
        // 5. Add a string to the new object
        cJSON_AddStringToObject(new_object, fuzz_str_2, fuzz_str_3);
        
        // 6. Create boolean values
        cJSON *true_bool = cJSON_CreateBool(fuzz_int32_t_4);
        cJSON *false_bool = cJSON_CreateFalse();
        cJSON *null_item = cJSON_CreateNull();
        
        // 7. Check if certain items are numbers or false
        cJSON *number_check = cJSON_GetObjectItem(root, fuzz_str_5);
        if (number_check != NULL) {
            cJSON *title_item = cJSON_GetObjectItem(number_check, fuzz_str_6);
            if (title_item != NULL) {
                // Check if it's a number (it's not, but we call the API)
                cJSON_bool is_num = cJSON_IsNumber(title_item);
                (void)is_num;
            }
        }
        
        // 8. Check if false_bool is actually false
        cJSON_bool is_false = cJSON_IsFalse(false_bool);
        (void)is_false;
        
        // 9. Replace an item in the object
        // First add an item to replace
        cJSON *to_replace = cJSON_AddStringToObject(new_object, "replace_me", fuzz_str_7);
        if (to_replace != NULL) {
            // Create replacement item
            cJSON *replacement = cJSON_CreateBool(fuzz_int32_t_8);
            cJSON_ReplaceItemInObject(new_object, "replace_me", replacement);
            // replacement is now owned by new_object, don't delete separately
        }
        
        // 10. Add the boolean items to the new object
        cJSON_AddItemToObject(new_object, fuzz_str_9, true_bool);
        cJSON_AddItemToObject(new_object, fuzz_str_10, false_bool);
        cJSON_AddItemToObject(new_object, fuzz_str_11, null_item);
        
        // 11. Clean up - print and delete
        char *printed = cJSON_Print(root);
        if (printed != NULL) {
            cJSON_free(printed);
        }
    }
    
    // 12. Always clean up root
    cJSON_Delete(root);
    
    return 0;
}