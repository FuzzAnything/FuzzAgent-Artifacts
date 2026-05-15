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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 1
//<Quality> {"density":27,"unique_branches":{"cJSON_GetStringValue":[[101,9,101,30,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateArray","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddItemReferenceToArray","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsTrue","cJSON_Delete","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_AddItemReferenceToArray","cJSON_CreateArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_AddItemReferenceToArray","cJSON_Delete","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_AddTrueToObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsTrue","cJSON_IsTrue","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddItemReferenceToArray","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsTrue","cJSON_Delete","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_AddItemReferenceToArray","cJSON_CreateArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_AddItemReferenceToArray","cJSON_Delete","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_AddTrueToObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsTrue","cJSON_IsTrue","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse input JSON data
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (parsed_json == nullptr) {
        return 0;
    }

    // Create a new array
    cJSON *new_array = cJSON_CreateArray();
    if (new_array == nullptr) {
        cJSON_Delete(parsed_json);
        return 0;
    }

    // Get the glossary object from parsed JSON
    cJSON *glossary = cJSON_GetObjectItemCaseSensitive(parsed_json, "glossary");
    if (glossary != nullptr && cJSON_IsObject(glossary)) {
        // Get GlossDiv object
        cJSON *gloss_div = cJSON_GetObjectItemCaseSensitive(glossary, "GlossDiv");
        if (gloss_div != nullptr && cJSON_IsObject(gloss_div)) {
            // Get GlossList object
            cJSON *gloss_list = cJSON_GetObjectItemCaseSensitive(gloss_div, "GlossList");
            if (gloss_list != nullptr && cJSON_IsObject(gloss_list)) {
                // Get GlossEntry object
                cJSON *gloss_entry = cJSON_GetObjectItemCaseSensitive(gloss_list, "GlossEntry");
                if (gloss_entry != nullptr && cJSON_IsObject(gloss_entry)) {
                    // Add reference to GlossEntry in our new array
                    cJSON_AddItemReferenceToArray(new_array, gloss_entry);
                    
                    // Get Acronym field from GlossEntry
                    cJSON *acronym = cJSON_GetObjectItemCaseSensitive(gloss_entry, "Acronym");
                    if (acronym != nullptr) {
                        // Get string value of Acronym
                        char *acronym_str = cJSON_GetStringValue(acronym);
                        if (acronym_str != nullptr) {
                            // Create a new object to add true value
                            cJSON *new_obj = cJSON_CreateObject();
                            if (new_obj != nullptr) {
                                // Add true field to new object
                                cJSON_AddTrueToObject(new_obj, "isValidAcronym");
                                
                                // Check if the true field is actually true
                                cJSON *true_field = cJSON_GetObjectItemCaseSensitive(new_obj, "isValidAcronym");
                                if (true_field != nullptr) {
                                    cJSON_bool is_true = cJSON_IsTrue(true_field);
                                    // Use is_true to avoid unused variable warning
                                    (void)is_true;
                                }
                                
                                // Clean up the temporary object
                                cJSON_Delete(new_obj);
                            }
                        }
                    }
                    
                    // Delete the "SortAs" field from GlossEntry (case sensitive)
                    cJSON_DeleteItemFromObjectCaseSensitive(gloss_entry, "SortAs");
                    
                    // Check if GlossDef exists and add reference to it
                    cJSON *gloss_def = cJSON_GetObjectItemCaseSensitive(gloss_entry, "GlossDef");
                    if (gloss_def != nullptr) {
                        cJSON_AddItemReferenceToArray(new_array, gloss_def);
                    }
                }
            }
        }
    }
    
    // Create another array and add references to items from first array
    cJSON *second_array = cJSON_CreateArray();
    if (second_array != nullptr) {
        // Get size of first array
        int array_size = cJSON_GetArraySize(new_array);
        for (int i = 0; i < array_size; i++) {
            cJSON *item = cJSON_GetArrayItem(new_array, i);
            if (item != nullptr) {
                cJSON_AddItemReferenceToArray(second_array, item);
            }
        }
        
        // Clean up second array
        cJSON_Delete(second_array);
    }
    
    // Create a test object with multiple true values
    cJSON *test_object = cJSON_CreateObject();
    if (test_object != nullptr) {
        cJSON_AddTrueToObject(test_object, "test1");
        cJSON_AddTrueToObject(test_object, "test2");
        
        // Check if the true values are actually true
        cJSON *test1 = cJSON_GetObjectItemCaseSensitive(test_object, "test1");
        cJSON *test2 = cJSON_GetObjectItemCaseSensitive(test_object, "test2");
        
        if (test1 != nullptr) {
            cJSON_bool is_true1 = cJSON_IsTrue(test1);
            (void)is_true1;
        }
        
        if (test2 != nullptr) {
            cJSON_bool is_true2 = cJSON_IsTrue(test2);
            (void)is_true2;
        }
        
        // Delete one of the fields case sensitively
        cJSON_DeleteItemFromObjectCaseSensitive(test_object, "test1");
        
        // Clean up test object
        cJSON_Delete(test_object);
    }
    
    // Try to get string values from various fields
    cJSON *title = cJSON_GetObjectItemCaseSensitive(parsed_json, "glossary");
    if (title != nullptr) {
        title = cJSON_GetObjectItemCaseSensitive(title, "title");
        if (title != nullptr) {
            char *title_str = cJSON_GetStringValue(title);
            (void)title_str; // Use title_str to avoid unused variable warning
        }
    }
    
    // Clean up all allocated resources
    cJSON_Delete(new_array);
    cJSON_Delete(parsed_json);
    
    return 0;
}