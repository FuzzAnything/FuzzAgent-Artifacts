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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 2
//<Quality> {"density":19,"unique_branches":{"cJSON_Compare":[[3135,43,3135,62,0,0,4,1],[3147,17,3147,39,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_AddBoolToObject","cJSON_IsBool","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_SetValuestring","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_Delete","cJSON_IsArray","cJSON_GetArrayItem","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Compare","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_AddBoolToObject","cJSON_IsBool","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_SetValuestring","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_Delete","cJSON_IsArray","cJSON_GetArrayItem","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Compare","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<78) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	//fuzzer shim end}




    // Parse the input JSON data
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
        // If parsing fails, return early
        return 0;
    }
    
    // 1. Use cJSON_GetObjectItem to get nested objects
    cJSON *menu = cJSON_GetObjectItem(parsed, fuzz_str_1);
    if (menu && cJSON_IsObject(menu)) {
        cJSON *popup = cJSON_GetObjectItem(menu, fuzz_str_2);
        if (popup && cJSON_IsObject(popup)) {
            cJSON *menuitem = cJSON_GetObjectItem(popup, fuzz_str_3);
            
            // 2. Use cJSON_IsBool to check boolean values
            // Create a boolean item and add it to the object
            cJSON *boolItem = cJSON_AddBoolToObject(popup, fuzz_str_4, fuzz_int32_t_5);
            if (boolItem) {
                // Check if it's boolean
                if (cJSON_IsBool(boolItem)) {
                    // Boolean verification successful
                }
            }
            
            // 3. Use cJSON_IsRaw to check raw items
            // First create a raw item
            cJSON *rawItem = cJSON_CreateRaw(fuzz_str_6);
            if (rawItem) {
                // Check if it's raw
                cJSON_IsRaw(rawItem);
                cJSON_Delete(rawItem);
            }
            
            // 4. Use cJSON_SetValuestring to modify string values
            cJSON *id = cJSON_GetObjectItem(menu, fuzz_str_7);
            if (id && cJSON_IsString(id)) {
                char *oldValue = cJSON_GetStringValue(id);
                if (oldValue) {
                    // Set a new value
                    char *newValue = cJSON_SetValuestring(id, fuzz_str_8);
                    // newValue now contains the modified string
                }
            }
            
            // 5. Use cJSON_SetNumberHelper to modify number values
            // Create a number item first
            cJSON *numItem = cJSON_CreateNumber(42.5);
            if (numItem) {
                double newNum = cJSON_SetNumberHelper(numItem, 99.9);
                cJSON_Delete(numItem);
            }
            
            // 6. Use cJSON_AddItemReferenceToArray with menuitem array
            if (menuitem && cJSON_IsArray(menuitem)) {
                // Get first item from array
                cJSON *firstItem = cJSON_GetArrayItem(menuitem, fuzz_int32_t_9);
                if (firstItem) {
                    // Create a new array and add reference
                    cJSON *newArray = cJSON_CreateArray();
                    if (newArray) {
                        cJSON_AddItemReferenceToArray(newArray, firstItem);
                        
                        // 7. Use cJSON_Compare to compare arrays
                        cJSON_Compare(menuitem, newArray, fuzz_int32_t_10);
                        
                        cJSON_Delete(newArray);
                    }
                }
            }
        }
    }
    
    // Clean up
    cJSON_Delete(parsed);
    
    return 0;
}