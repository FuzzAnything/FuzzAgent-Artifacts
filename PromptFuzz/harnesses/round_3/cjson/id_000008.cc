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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 20
//<Quality> {"density":12,"unique_branches":{"cJSON_InsertItemInArray":[[2338,9,2338,18,0,0,4,1],[2338,22,2338,37,0,0,4,1],[2344,9,2344,31,0,0,4,0]],"cJSON_AddFalseToObject":[[2177,9,2177,75,0,0,4,0]],"cJSON_CreateStringArray":[[2748,9,2748,20,0,0,4,1],[2748,24,2748,41,0,0,4,1],[2755,17,2755,18,0,0,4,0],[2755,22,2755,41,0,0,4,0],[2755,22,2755,41,0,0,4,1],[2758,12,2758,14,0,0,4,1],[2763,12,2763,14,0,0,4,0],[2763,12,2763,14,0,0,4,1],[2774,9,2774,10,0,0,4,0],[2774,14,2774,22,0,0,4,0]],"cJSON_IsInvalid":[[2974,9,2974,21,0,0,4,1]],"cJSON_HasObjectItem":[[1995,12,1995,47,0,0,4,0],[1995,12,1995,47,0,0,4,1]],"cJSON_SetNumberHelper":[[413,9,413,23,0,0,4,1],[418,9,418,26,0,0,4,1],[422,14,422,39,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateArray","cJSON_Delete","cJSON_CreateStringArray","cJSON_Delete","cJSON_Delete","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_AddFalseToObject","cJSON_InsertItemInArray","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_InsertItemInArray","cJSON_IsInvalid","cJSON_IsInvalid","cJSON_CreateString","cJSON_ReplaceItemViaPointer","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateArray","cJSON_CreateStringArray","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_AddFalseToObject","cJSON_InsertItemInArray","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_InsertItemInArray","cJSON_IsInvalid","cJSON_IsInvalid","cJSON_CreateString","cJSON_ReplaceItemViaPointer","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	
		const char * fuzzer_var_cJSON_CreateStringArray_3_0[3] = {fuzz_str_1,fuzz_str_2,fuzz_str_3};//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Parse input JSON data
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
        return 0;
    }

    // 2. Create a new array
    cJSON *new_array = cJSON_CreateArray();
    if (!new_array) {
        cJSON_Delete(parsed);
        return 0;
    }

    // 3. Create string array for insertion
    const char *strings[] = {"fuzz1", "fuzz2", "fuzz3"};
    cJSON *string_array = cJSON_CreateStringArray(fuzzer_var_cJSON_CreateStringArray_3_0, 3);
    if (!string_array) {
        cJSON_Delete(new_array);
        cJSON_Delete(parsed);
        return 0;
    }

    // 4. Check if "menu" exists in parsed JSON
    cJSON_bool has_menu = cJSON_HasObjectItem(parsed, fuzz_str_4);
    
    // 5. Add false value to object if menu exists
    if (has_menu) {
        cJSON *menu = cJSON_GetObjectItem(parsed, fuzz_str_5);
        if (menu) {
            cJSON_AddFalseToObject(menu, fuzz_str_6);
        }
    }

    // 6. Insert string array into new array at position 0
    cJSON_InsertItemInArray(new_array, 0, string_array);

    // 7. Create a number item and set its value
    cJSON *number_item = cJSON_CreateNumber(0.0);
    if (number_item) {
        // Use cJSON_SetNumberHelper to change the value
        cJSON_SetNumberHelper(number_item, 42.5);
        
        // Insert number item into array
        cJSON_InsertItemInArray(new_array, 1, number_item);
    }

    // 8. Check if items are invalid (should return false for valid items)
    cJSON_bool is_invalid1 = cJSON_IsInvalid(string_array);
    cJSON_bool is_invalid2 = cJSON_IsInvalid(number_item);

    // 9. Replace an item in the array via pointer
    cJSON *replacement = cJSON_CreateString(fuzz_str_7);
    if (replacement && new_array->child) {
        // Replace the first child (string_array) with replacement
        cJSON_ReplaceItemViaPointer(new_array, new_array->child, replacement);
        // Don't delete string_array as it's now owned by new_array
    }

    // 10. Print the modified JSON using buffered printing
    char *printed = cJSON_PrintBuffered(parsed, fuzz_int32_t_8, fuzz_int32_t_9);
    if (printed) {
        // Do something with the printed JSON (e.g., write to file)
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed, strlen(printed), 1, out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed);
    }

    // 11. Clean up
    cJSON_Delete(new_array);  // This will delete replacement and number_item
    cJSON_Delete(parsed);
    
    return 0;
}