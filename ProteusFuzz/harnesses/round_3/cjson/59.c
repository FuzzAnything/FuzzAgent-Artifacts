#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

/* Helper to create a valid cJSON object */
static cJSON *create_test_object(void) {
    return cJSON_CreateObject();
}

/* Helper to cleanup */
static void cleanup_object(cJSON *obj) {
    if (obj) {
        cJSON_Delete(obj);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * TARGET: Line 2087 - Validation checks (NULL pointers, self-reference)
     * Strategy: Call APIs with NULL arguments to trigger the early return.
     */
    
    // Case 1: object is NULL
    cJSON_AddBoolToObject(NULL, "key", cJSON_True);
    
    // Case 2: string (key) is NULL
    cJSON *obj1 = create_test_object();
    if (obj1) {
        cJSON_AddBoolToObject(obj1, NULL, cJSON_True);
        cleanup_object(obj1);
    }

    /* 
     * TARGET: Line 2100 - Check if cJSON_strdup returned NULL
     * Strategy: Call a public API that uses constant_key = false (default).
     * This forces the code into the 'else' block (line 2097) and calls cJSON_strdup.
     * The check at 2100 is executed regardless of success/failure.
     */
    cJSON *obj2 = create_test_object();
    if (obj2) {
        // Use fuzz data to generate a key name
        char key[256];
        size_t key_len = (Size < 255) ? Size : 255;
        memcpy(key, Data, key_len);
        key[key_len] = '\0';
        
        // This call will:
        // 1. Create a bool item.
        // 2. Call add_item_to_object with constant_key = false.
        // 3. Execute cJSON_strdup (line 2099).
        // 4. Execute the check at line 2100.
        cJSON_AddBoolToObject(obj2, key, (Size % 2) ? cJSON_True : cJSON_False);
        
        cleanup_object(obj2);
    }

    /* 
     * TARGET: Line 2108 - Check if item->string is not NULL and needs deallocation
     * Strategy: Create an item, manually set its 'string' field to a non-NULL value,
     * then add it to an object using cJSON_AddItemToObject.
     */
    cJSON *obj3 = create_test_object();
    if (obj3) {
        // Create a boolean item
        cJSON *item = cJSON_CreateBool(cJSON_True);
        if (item) {
            // Manually allocate a string for the item's 'string' field
            // This simulates an item that already has a key name
            char *fake_key = (char *)malloc(10);
            if (fake_key) {
                memcpy(fake_key, "old_key", 8);
                fake_key[8] = '\0';
                
                // Set the item's string field
                item->string = fake_key;
                
                // Ensure type doesn't have cJSON_StringIsConst (default for CreateBool)
                // Now, when we add this item to the object, add_item_to_object will:
                // 1. Check constant_key (false).
                // 2. Allocate new_key.
                // 3. Check line 2108: !(item->type & ...) && (item->string != NULL)
                //    This will be TRUE, triggering the deallocation of fake_key.
                
                char new_key[256];
                size_t key_len = (Size < 255) ? Size : 255;
                memcpy(new_key, Data, key_len);
                new_key[key_len] = '\0';
                
                // Use cJSON_AddItemToObject to pass the pre-modified item
                cJSON_AddItemToObject(obj3, new_key, item);
                
                // Note: The item is now owned by the object. We should not free fake_key manually.
                // The object will free it when deleted.
            } else {
                cJSON_Delete(item);
            }
        }
        
        cleanup_object(obj3);
    }

    return 0;
}