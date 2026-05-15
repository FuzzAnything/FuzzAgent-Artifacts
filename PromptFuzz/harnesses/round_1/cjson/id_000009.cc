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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 114, nr_unique_branch: 21
//<Quality> {"density":57,"unique_branches":{"cJSON_Duplicate_rec":[[2797,9,2797,14,0,0,4,1],[2803,9,2803,17,0,0,4,1],[2811,9,2811,26,0,0,4,0],[2811,9,2811,26,0,0,4,1],[2814,13,2814,34,0,0,4,1],[2819,9,2819,21,0,0,4,0],[2819,9,2819,21,0,0,4,1],[2821,27,2821,59,0,0,4,1],[2822,13,2822,29,0,0,4,1],[2828,9,2828,17,0,0,4,1],[2834,12,2834,25,0,0,4,0],[2834,12,2834,25,0,0,4,1],[2836,12,2836,41,0,0,4,1],[2840,13,2840,22,0,0,4,1],[2844,13,2844,25,0,0,4,0],[2844,13,2844,25,0,0,4,1],[2859,9,2859,16,0,0,4,0],[2859,20,2859,34,0,0,4,0],[2859,20,2859,34,0,0,4,1]],"cJSON_IsObject":[[3054,9,3054,21,0,0,4,0]],"cJSON_ReplaceItemViaPointer":[[2404,13,2404,38,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateTrue","cJSON_CreateFalse","cJSON_CreateNull","cJSON_IsTrue","cJSON_IsFalse","cJSON_IsNull","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateBool","cJSON_CreateString","cJSON_InsertItemInArray","cJSON_CreateNull","cJSON_ReplaceItemInArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_AddItemReferenceToArray","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddStringToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateArray","cJSON_GetObjectItemCaseSensitive","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_GetArrayItem","cJSON_AddItemReferenceToArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsFalse","cJSON_IsNull","cJSON_IsTrue","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_CreateFalse","cJSON_AddItemToObject","cJSON_CreateNull","cJSON_AddItemToObject","cJSON_IsTrue","cJSON_IsFalse","cJSON_IsNull","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateTrue","cJSON_CreateFalse","cJSON_CreateNull","cJSON_IsTrue","cJSON_IsFalse","cJSON_IsNull","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateBool","cJSON_CreateString","cJSON_InsertItemInArray","cJSON_CreateNull","cJSON_ReplaceItemInArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_AddItemReferenceToArray","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddStringToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_GetObjectItemCaseSensitive","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_GetArrayItem","cJSON_AddItemReferenceToArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsFalse","cJSON_IsNull","cJSON_IsTrue","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_CreateFalse","cJSON_AddItemToObject","cJSON_CreateNull","cJSON_AddItemToObject","cJSON_IsTrue","cJSON_IsFalse","cJSON_IsNull","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":1}
/**/


// Include cJSON headers
extern "C" {
    #include "cJSON.h"
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input data as JSON
    const char *data_str = reinterpret_cast<const char*>(data);
    cJSON *root = cJSON_ParseWithLength(data_str, size);
    if (root == nullptr) {
        return 0; // Invalid JSON input
    }

    // 2. Ensure we have the widget object from the example structure
    cJSON *widget = cJSON_GetObjectItemCaseSensitive(root, "widget");
    if (!cJSON_IsObject(widget)) {
        cJSON_Delete(root);
        return 0;
    }

    // 3. Get the debug field and check boolean conditions
    cJSON *debug = cJSON_GetObjectItemCaseSensitive(widget, "debug");
    if (cJSON_IsString(debug)) {
        // Create temporary boolean items for testing
        cJSON *tmp_true = cJSON_CreateTrue();
        cJSON *tmp_false = cJSON_CreateFalse();
        cJSON *tmp_null = cJSON_CreateNull();
        
        // Test boolean check functions
        if (cJSON_IsTrue(tmp_true)) {
            // Intentionally empty - just calling the function
        }
        
        if (cJSON_IsFalse(tmp_false)) {
            // Intentionally empty - just calling the function
        }
        
        if (cJSON_IsNull(tmp_null)) {
            // Intentionally empty - just calling the function
        }
        
        // Clean up temporary items
        cJSON_Delete(tmp_true);
        cJSON_Delete(tmp_false);
        cJSON_Delete(tmp_null);
    }

    // 4. Get the window object and work with its array-like properties
    cJSON *window = cJSON_GetObjectItemCaseSensitive(widget, "window");
    if (cJSON_IsObject(window)) {
        // Create an array to demonstrate array operations
        cJSON *test_array = cJSON_CreateArray();
        
        // Add items to array
        cJSON_AddItemToArray(test_array, cJSON_CreateNumber(100));
        cJSON_AddItemToArray(test_array, cJSON_CreateString("test"));
        cJSON_AddItemToArray(test_array, cJSON_CreateBool(1));
        
        // Insert item at position 1
        cJSON *insert_item = cJSON_CreateString("inserted");
        cJSON_InsertItemInArray(test_array, 1, insert_item);
        
        // Replace item at position 2
        cJSON *replace_item = cJSON_CreateNull();
        cJSON_ReplaceItemInArray(test_array, 2, replace_item);
        
        // Add reference to existing item (first item in array)
        if (cJSON_GetArraySize(test_array) > 0) {
            cJSON *first_item = cJSON_GetArrayItem(test_array, 0);
            cJSON_AddItemReferenceToArray(test_array, first_item);
        }
        
        // Add the test array to window object
        cJSON_AddItemToObject(window, "test_array", test_array);
    }

    // 5. Get the text object and add a string field
    cJSON *text = cJSON_GetObjectItemCaseSensitive(widget, "text");
    if (cJSON_IsObject(text)) {
        // Add a new string field to the text object
        cJSON_AddStringToObject(text, "fuzzed_string", "added_by_fuzzer");
    }

    // 6. Try to parse the JSON as array operations on image properties
    cJSON *image = cJSON_GetObjectItemCaseSensitive(widget, "image");
    if (cJSON_IsObject(image)) {
        // Create an array of image properties
        cJSON *prop_array = cJSON_CreateArray();
        
        // Add image properties to array
        const char* props[] = {"src", "name", "hOffset", "vOffset", "alignment"};
        for (const char* prop : props) {
            cJSON *prop_item = cJSON_GetObjectItemCaseSensitive(image, prop);
            if (prop_item) {
                cJSON_AddItemToArray(prop_array, cJSON_Duplicate(prop_item, 1));
            }
        }
        
        // Add array to image
        cJSON_AddItemToObject(image, "properties_array", prop_array);
        
        // Test array operations on properties array
        int array_size = cJSON_GetArraySize(prop_array);
        if (array_size >= 2) {
            // Insert a boolean item at position 1
            cJSON *bool_item = cJSON_CreateBool(0);
            cJSON_InsertItemInArray(prop_array, 1, bool_item);
            
            // Replace item at position 2
            cJSON *new_string_item = cJSON_CreateString("replaced");
            cJSON_ReplaceItemInArray(prop_array, 2, new_string_item);
            
            // Add reference to first item
            cJSON *first_prop = cJSON_GetArrayItem(prop_array, 0);
            if (first_prop) {
                cJSON_AddItemReferenceToArray(prop_array, first_prop);
            }
            
            // Test boolean checks on array items
            for (int i = 0; i < cJSON_GetArraySize(prop_array); i++) {
                cJSON *item = cJSON_GetArrayItem(prop_array, i);
                if (cJSON_IsFalse(item)) {
                    // Do nothing - just checking
                }
                if (cJSON_IsNull(item)) {
                    // Do nothing - just checking
                }
                if (cJSON_IsTrue(item)) {
                    // Do nothing - just checking
                }
            }
        }
    }

    // 7. Create a new object to test more operations
    cJSON *new_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(new_obj, "test_field", "test_value");
    
    // Add some boolean fields
    cJSON *true_field = cJSON_CreateTrue();
    cJSON_AddItemToObject(new_obj, "true_field", true_field);
    
    cJSON *false_field = cJSON_CreateFalse();
    cJSON_AddItemToObject(new_obj, "false_field", false_field);
    
    cJSON *null_field = cJSON_CreateNull();
    cJSON_AddItemToObject(new_obj, "null_field", null_field);
    
    // Test boolean checks
    cJSON_IsTrue(true_field);
    cJSON_IsFalse(false_field);
    cJSON_IsNull(null_field);
    
    // Add the new object to root
    cJSON_AddItemToObject(root, "fuzzed_object", new_obj);

    // 8. Create output file and write the modified JSON
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *printed = cJSON_Print(root);
        if (printed) {
            fwrite(printed, strlen(printed), 1, out_file);
            cJSON_free(printed);
        }
        assert_file_closed(&out_file);;
    }

    // 9. Clean up all allocated resources
    cJSON_Delete(root);
    
    assert_file_closed(&out_file);
	return 0;
}