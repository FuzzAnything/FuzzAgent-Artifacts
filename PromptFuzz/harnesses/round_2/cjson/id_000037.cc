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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 62, nr_unique_branch: 1
//<Quality> {"density":31,"unique_branches":{"replace_item_in_object":[[2435,55,2435,84,0,0,4,0]]},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_free","cJSON_CreateObject","cJSON_Delete","cJSON_free","cJSON_AddTrueToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_IsObject","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_SetValuestring","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsString","cJSON_Duplicate","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_IsObject","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_SetValuestring","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsString","cJSON_Duplicate","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input JSON using cJSON_ParseWithLength
    char *json_str = (char *)cJSON_malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *parsed_root = cJSON_ParseWithLength(json_str, size);
    if (!parsed_root) {
        cJSON_free(json_str);
        return 0;
    }
    
    // 2. Create a new object to modify
    cJSON *new_obj = cJSON_CreateObject();
    if (!new_obj) {
        cJSON_Delete(parsed_root);
        cJSON_free(json_str);
        return 0;
    }
    
    // 3. Add some items to the new object using various APIs
    cJSON_AddTrueToObject(new_obj, "added_true");
    
    // Create a float array
    float float_numbers[] = {1.1f, 2.2f, 3.3f};
    cJSON *float_array = cJSON_CreateFloatArray(float_numbers, 3);
    if (float_array) {
        cJSON_AddItemToObject(new_obj, "float_array", float_array);
    }
    
    // Add a raw string
    cJSON_AddRawToObject(new_obj, "raw_data", "{\"temp\":123}");
    
    // Create a string
    cJSON *new_string = cJSON_CreateString("test_string");
    if (new_string) {
        cJSON_AddItemToObject(new_obj, "test_string", new_string);
    }
    
    // 4. Check and modify the parsed JSON structure
    if (cJSON_IsObject(parsed_root)) {
        // Check if "glossary" exists
        if (cJSON_HasObjectItem(parsed_root, "glossary")) {
            cJSON *glossary = cJSON_GetObjectItem(parsed_root, "glossary");
            
            if (cJSON_IsObject(glossary)) {
                // Replace "title" in glossary
                cJSON *title = cJSON_GetObjectItem(glossary, "title");
                if (title && cJSON_IsString(title)) {
                    cJSON_SetValuestring(title, "modified_title");
                }
                
                // Check for GlossDiv -> GlossList -> GlossEntry -> GlossDef -> GlossSeeAlso array
                cJSON *gloss_div = cJSON_GetObjectItem(glossary, "GlossDiv");
                if (gloss_div && cJSON_IsObject(gloss_div)) {
                    cJSON *gloss_list = cJSON_GetObjectItem(gloss_div, "GlossList");
                    if (gloss_list && cJSON_IsObject(gloss_list)) {
                        cJSON *gloss_entry = cJSON_GetObjectItem(gloss_list, "GlossEntry");
                        if (gloss_entry && cJSON_IsObject(gloss_entry)) {
                            cJSON *gloss_def = cJSON_GetObjectItem(gloss_entry, "GlossDef");
                            if (gloss_def && cJSON_IsObject(gloss_def)) {
                                cJSON *gloss_see_also = cJSON_GetObjectItem(gloss_def, "GlossSeeAlso");
                                if (gloss_see_also && cJSON_IsArray(gloss_see_also)) {
                                    int array_size = cJSON_GetArraySize(gloss_see_also);
                                    if (array_size > 0) {
                                        // Get first array item
                                        cJSON *first_item = cJSON_GetArrayItem(gloss_see_also, 0);
                                        if (first_item && cJSON_IsString(first_item)) {
                                            // Duplicate the array and replace an item
                                            cJSON *duplicated = cJSON_Duplicate(gloss_see_also, 1);
                                            if (duplicated) {
                                                // Create replacement string
                                                cJSON *replacement = cJSON_CreateString("REPLACED");
                                                if (replacement) {
                                                    // Replace first item in duplicated array
                                                    cJSON_ReplaceItemInObject(gloss_def, "GlossSeeAlso", duplicated);
                                                    // Note: cJSON_ReplaceItemInObject doesn't work on arrays directly
                                                    // We need to replace in the object containing the array
                                                    cJSON_Delete(replacement);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // 5. Check for true value in new object
    cJSON *true_item = cJSON_GetObjectItem(new_obj, "added_true");
    if (true_item && cJSON_IsTrue(true_item)) {
        // Valid true value found
    }
    
    // 6. Print the modified JSON
    char *printed_json = cJSON_Print(parsed_root);
    if (printed_json) {
        // Write to file using FILE*
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, 1, strlen(printed_json), out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed_json);
    }
    
    // 7. Clean up
    cJSON_Delete(parsed_root);
    cJSON_Delete(new_obj);
    cJSON_free(json_str);
    
    return 0;
}