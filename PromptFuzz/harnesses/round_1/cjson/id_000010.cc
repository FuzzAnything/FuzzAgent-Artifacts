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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 13
//<Quality> {"density":14,"unique_branches":{"cJSON_SetValuestring":[[441,9,441,25,0,0,4,1],[441,29,441,59,0,0,4,1],[441,63,441,97,0,0,4,1],[446,9,446,36,0,0,4,1],[446,40,446,59,0,0,4,1],[454,9,454,25,0,0,4,0],[454,9,454,25,0,0,4,1],[457,16,457,58,0,0,4,0],[465,9,465,21,0,0,4,1],[469,9,469,36,0,0,4,0]],"cJSON_PrintBuffered":[[1321,9,1321,22,0,0,4,1],[1327,9,1327,18,0,0,4,1],[1338,9,1338,31,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_CreateObjectReference","cJSON_Delete","cJSON_AddNumberToObject","cJSON_GetNumberValue","cJSON_IsBool","cJSON_IsString","cJSON_SetValuestring","cJSON_PrintBuffered","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_CreateObjectReference","cJSON_AddNumberToObject","cJSON_GetNumberValue","cJSON_IsBool","cJSON_IsString","cJSON_SetValuestring","cJSON_PrintBuffered","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse the input JSON data
    char *json_string = (char *)malloc(size + 1);
    if (!json_string) {
        return 0;
    }
    memcpy(json_string, data, size);
    json_string[size] = '\0';
    
    // Parse JSON with length
    cJSON *root = cJSON_ParseWithLength(json_string, size);
    free(json_string);
    
    if (!root) {
        return 0;
    }
    
    // 2. Check if the root is invalid (should return false for valid JSON)
    cJSON_bool is_invalid = cJSON_IsInvalid(root);
    
    // 3. Create an object reference
    cJSON *object_ref = cJSON_CreateObjectReference(root);
    if (!object_ref) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 4. Add a number to the root object
    const char *number_key = "fuzz_added_number";
    cJSON *added_number = cJSON_AddNumberToObject(root, number_key, 42.5);
    
    // 5. Get the number value (if it exists)
    double number_value = 0.0;
    if (added_number) {
        number_value = cJSON_GetNumberValue(added_number);
    }
    
    // 6. Check if the added number is a boolean (should return false)
    cJSON_bool is_bool = cJSON_IsBool(added_number);
    
    // 7. Find a string item to modify using cJSON_SetValuestring
    // Let's find the first string item in the JSON
    cJSON *current = root->child;
    cJSON *string_item = NULL;
    while (current) {
        if (cJSON_IsString(current)) {
            string_item = current;
            break;
        }
        current = current->next;
    }
    
    // If found a string item, change its value
    if (string_item) {
        char *old_valuestring = string_item->valuestring;
        char *new_valuestring = cJSON_SetValuestring(string_item, "modified_by_fuzzer");
        (void)new_valuestring; // Prevent unused variable warning
    }
    
    // 8. Use cJSON_PrintBuffered to print formatted JSON
    char *buffered_print = NULL;
    int prebuffer_size = 1024;
    buffered_print = cJSON_PrintBuffered(root, prebuffer_size, 1); // fmt = 1 for formatted
    
    if (buffered_print) {
        // Do something with the buffered print output
        size_t buffered_len = strlen(buffered_print);
        (void)buffered_len; // Prevent unused variable warning
        cJSON_free(buffered_print);
    }
    
    // 9. Use cJSON_PrintPreallocated with a pre-allocated buffer
    char preallocated_buffer[2048];
    cJSON_bool print_success = cJSON_PrintPreallocated(root, preallocated_buffer, 
                                                      sizeof(preallocated_buffer), 0); // format = 0
    
    // 10. Create output file and write results
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (print_success) {
            fwrite(preallocated_buffer, strlen(preallocated_buffer), 1, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // 11. Cleanup
    cJSON_Delete(object_ref);
    cJSON_Delete(root);
    
    assert_file_closed(&out_file);
	return 0;
}