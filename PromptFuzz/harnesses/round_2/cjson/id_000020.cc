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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"cJSON_GetNumberValue":[[111,9,111,30,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_CreateArray","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddFalseToObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetNumberValue","cJSON_IsFalse","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetNumberValue","cJSON_CreateObject","cJSON_AddFalseToObject","cJSON_CreateArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsFalse","cJSON_GetArrayItem","cJSON_GetNumberValue","cJSON_PrintUnformatted","cJSON_free","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_CreateArray","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddFalseToObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetNumberValue","cJSON_IsFalse","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetNumberValue","cJSON_CreateObject","cJSON_AddFalseToObject","cJSON_CreateArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsFalse","cJSON_GetArrayItem","cJSON_GetNumberValue","cJSON_PrintUnformatted","cJSON_free","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Looking at the instructions and the provided APIs, I need to create a fuzz driver that:
1. Uses the specified APIs at least once
2. Processes the input JSON data
3. Performs operations that demonstrate how these APIs can work together

Here's my implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // If size is too small to contain valid JSON, return early
    if (size < 2) {
        return 0;
    }
    
    // Create a null-terminated string from the input data
    char *input_string = new char[size + 1];
    memcpy(input_string, data, size);
    input_string[size] = '\0';
    
    // Parse the input JSON
    cJSON *root = cJSON_Parse(input_string);
    if (root == nullptr) {
        delete[] input_string;
        return 0;
    }
    
    // Get the "widget" object
    cJSON *widget = cJSON_GetObjectItemCaseSensitive(root, "widget");
    if (widget == nullptr) {
        cJSON_Delete(root);
        delete[] input_string;
        return 0;
    }
    
    // Get the "text" object from within widget
    cJSON *text = cJSON_GetObjectItemCaseSensitive(widget, "text");
    if (text == nullptr) {
        cJSON_Delete(root);
        delete[] input_string;
        return 0;
    }
    
    // Create a temporary array for demonstration
    cJSON *temp_array = cJSON_CreateArray();
    if (temp_array != nullptr) {
        // Add some items to the array
        cJSON *num1 = cJSON_CreateNumber(10.5);
        cJSON *num2 = cJSON_CreateNumber(20.3);
        cJSON *num3 = cJSON_CreateNumber(30.7);
        
        cJSON_AddItemToArray(temp_array, num1);
        cJSON_AddItemToArray(temp_array, num2);
        cJSON_AddItemToArray(temp_array, num3);
        
        // EVENT: Demonstrate how the specified APIs can work together
        // This event shows a workflow where we manipulate an array and object
        
        // 1. First, add a false value to the text object
        cJSON *added_false = cJSON_AddFalseToObject(text, "is_visible");
        
        // 2. Get a number value from the text object (size field)
        cJSON *size_field = cJSON_GetObjectItemCaseSensitive(text, "size");
        double size_value = 0.0;
        if (size_field != nullptr) {
            size_value = cJSON_GetNumberValue(size_field);
        }
        
        // 3. Check if our added false value is indeed false
        if (added_false != nullptr) {
            cJSON_bool is_false = cJSON_IsFalse(added_false);
            // We could use this for something, but just calling it is enough for fuzzing
        }
        
        // 4. Detach an item from our temp array by index
        cJSON *detached_item = cJSON_DetachItemFromArray(temp_array, 1); // Detach the second item
        
        // 5. Insert the detached item back at a different position
        if (detached_item != nullptr) {
            cJSON_InsertItemInArray(temp_array, 0, detached_item);
        }
        
        // 6. Get a pointer to an item in the array
        cJSON *array_item = cJSON_GetArrayItem(temp_array, 1);
        if (array_item != nullptr) {
            // 7. Detach the item via pointer
            cJSON *detached_via_pointer = cJSON_DetachItemViaPointer(temp_array, array_item);
            if (detached_via_pointer != nullptr) {
                cJSON_Delete(detached_via_pointer);
            }
        }
        
        // 8. Print the modified JSON structure without formatting
        char *unformatted_json = cJSON_PrintUnformatted(root);
        if (unformatted_json != nullptr) {
            // We could write this to a file or process it further
            cJSON_free(unformatted_json);
        }
        
        // Clean up the temporary array
        cJSON_Delete(temp_array);
    }
    
    // Let's also work with the window object
    cJSON *window = cJSON_GetObjectItemCaseSensitive(widget, "window");
    if (window != nullptr) {
        // Get the width value
        cJSON *width = cJSON_GetObjectItemCaseSensitive(window, "width");
        if (width != nullptr) {
            double width_value = cJSON_GetNumberValue(width);
            // Could use this value for something
        }
    }
    
    // Another demonstration: create a simple object and array
    cJSON *demo_object = cJSON_CreateObject();
    if (demo_object != nullptr) {
        cJSON_AddFalseToObject(demo_object, "demo_false");
        
        // Create an array inside the demo object
        cJSON *demo_array = cJSON_CreateArray();
        if (demo_array != nullptr) {
            for (int i = 0; i < 3; i++) {
                cJSON *item = cJSON_CreateNumber(i * 10.0);
                cJSON_AddItemToArray(demo_array, item);
            }
            
            cJSON_AddItemToObject(demo_object, "demo_array", demo_array);
            
            // Detach and insert items in the array
            if (cJSON_GetArraySize(demo_array) > 1) {
                cJSON *detached = cJSON_DetachItemFromArray(demo_array, 1);
                if (detached != nullptr) {
                    cJSON_InsertItemInArray(demo_array, 0, detached);
                }
                
                // Check if an item is false
                cJSON *false_item = cJSON_GetObjectItemCaseSensitive(demo_object, "demo_false");
                if (false_item != nullptr) {
                    cJSON_bool check_false = cJSON_IsFalse(false_item);
                }
                
                // Get number value from array
                cJSON *array_num = cJSON_GetArrayItem(demo_array, 0);
                if (array_num != nullptr) {
                    double num_val = cJSON_GetNumberValue(array_num);
                }
                
                // Print unformatted JSON
                char *demo_json = cJSON_PrintUnformatted(demo_object);
                if (demo_json != nullptr) {
                    cJSON_free(demo_json);
                }
            }
        }
        
        // Detach via pointer demonstration
        cJSON *child = demo_object->child;
        while (child != nullptr) {
            cJSON *next = child->next;
            if (strcmp(child->string, "demo_false") == 0) {
                cJSON *detached = cJSON_DetachItemViaPointer(demo_object, child);
                if (detached != nullptr) {
                    cJSON_Delete(detached);
                }
                break;
            }
            child = next;
        }
        
        cJSON_Delete(demo_object);
    }
    
    // Clean up
    cJSON_Delete(root);
    delete[] input_string;
    
    return 0;
}