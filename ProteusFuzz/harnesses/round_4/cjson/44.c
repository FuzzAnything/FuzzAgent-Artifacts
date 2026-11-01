// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static cJSON *create_test_array_from_fuzz(const uint8_t *Data, size_t Size) {
    cJSON *root = cJSON_CreateArray();
    if (!root) return NULL;

    size_t idx = 0;
    while (idx < Size) {
        if (idx >= Size) break;
        int type = Data[idx] % 5;
        cJSON *item = NULL;
        idx++;

        if (idx >= Size) break;

        if (type == 0) {
            // Number
            int val = Data[idx];
            item = cJSON_CreateNumber(val);
            idx++;
        } else if (type == 1) {
            // String (short, safe)
            if (idx >= Size) break;
            int len = Data[idx] % 10;
            idx++;
            if (idx + len > Size) break;
            char *str = malloc(len + 1);
            if (str) {
                memcpy(str, Data + idx, len);
                str[len] = '\0';
                item = cJSON_CreateString(str);
                free(str);
                idx += len;
            }
        } else if (type == 2) {
            item = cJSON_CreateNull();
        } else if (type == 3) {
            item = cJSON_CreateTrue();
        } else {
            item = cJSON_CreateFalse();
        }

        if (item) {
            cJSON_AddItemToArray(root, item);
        }
    }
    return root;
}

static cJSON *create_test_object_from_fuzz(const uint8_t *Data, size_t Size) {
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;

    size_t idx = 0;
    int key_count = 0;
    while (idx < Size && key_count < 10) {
        if (idx >= Size) break;
        int type = Data[idx] % 5;
        cJSON *item = NULL;
        idx++;

        // Create a simple key
        char key[16];
        if (idx >= Size) break;
        int key_len = (Data[idx] % 5) + 1;
        idx++;
        
        if (idx + key_len > Size) break;

        for (int i = 0; i < key_len; i++) {
            key[i] = (char)('a' + (Data[idx] % 26));
            idx++;
        }
        key[key_len] = '\0';

        if (type == 0) {
            if (idx >= Size) break;
            item = cJSON_CreateNumber(Data[idx]);
            idx++;
        } else if (type == 1) {
            if (idx >= Size) break;
            int len = Data[idx] % 10;
            idx++;
            if (idx + len > Size) break;
            char *str = malloc(len + 1);
            if (str) {
                memcpy(str, Data + idx, len);
                str[len] = '\0';
                item = cJSON_CreateString(str);
                free(str);
                idx += len;
            }
        } else if (type == 2) {
            item = cJSON_CreateNull();
        } else if (type == 3) {
            item = cJSON_CreateTrue();
        } else {
            item = cJSON_CreateFalse();
        }

        if (item) {
            cJSON_AddItemToObject(root, key, item);
            key_count++;
        }
    }
    return root;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Strategy 1: Test array operations
    cJSON *arr = create_test_array_from_fuzz(Data, Size);
    if (arr) {
        // Test cJSON_GetArraySize
        int size = cJSON_GetArraySize(arr);
        (void)size; // Suppress unused warning

        // Test cJSON_DeleteItemFromArray
        // Try deleting at various indices (including out of bounds)
        int delete_idx = Data[0] % (size + 2); // +2 to test out of bounds
        cJSON_DeleteItemFromArray(arr, delete_idx);

        // Test cJSON_DetachItemViaPointer
        if (size > 0) {
            cJSON *first = cJSON_GetArrayItem(arr, 0);
            if (first) {
                cJSON *detached = cJSON_DetachItemViaPointer(arr, first);
                if (detached) {
                    // Re-attach or delete
                    cJSON_Delete(detached);
                }
            }
        }

        // Test cJSON_AddItemReferenceToArray
        if (size > 0) {
            cJSON *ref_item = cJSON_GetArrayItem(arr, 0);
            if (ref_item) {
                cJSON_AddItemReferenceToArray(arr, ref_item);
            }
        }

        // Test cJSON_CreateObjectReference
        if (size > 0) {
            cJSON *child = cJSON_GetArrayItem(arr, 0);
            if (child) {
                cJSON *ref = cJSON_CreateObjectReference(child);
                if (ref) {
                    cJSON_Delete(ref);
                }
            }
        }

        cJSON_Delete(arr);
    }

    // Strategy 2: Test object operations
    cJSON *obj = create_test_object_from_fuzz(Data, Size);
    if (obj) {
        // Test cJSON_GetArraySize (works on objects too)
        int obj_size = cJSON_GetArraySize(obj);
        (void)obj_size;

        // Test cJSON_AddItemToObject
        char new_key[16];
        int key_len = (Data[0] % 5) + 1;
        // Ensure we don't read out of bounds when generating key
        if (Size < (size_t)key_len) {
            key_len = Size;
        }
        
        for (int i = 0; i < key_len; i++) {
            new_key[i] = (char)('x' + (Data[i] % 26));
        }
        new_key[key_len] = '\0';
        
        cJSON *new_item = cJSON_CreateNumber(123);
        if (new_item) {
            cJSON_AddItemToObject(obj, new_key, new_item);
        }

        // Test cJSON_DetachItemViaPointer on object
        if (obj_size > 0) {
            cJSON *child = cJSON_GetObjectItem(obj, new_key);
            if (!child && obj_size > 0) {
                // Try first child if key not found
                child = obj->child;
            }
            if (child) {
                cJSON *detached = cJSON_DetachItemViaPointer(obj, child);
                if (detached) {
                    cJSON_Delete(detached);
                }
            }
        }

        // Test cJSON_AddItemReferenceToArray on object (invalid usage but safe)
        if (obj_size > 0) {
            cJSON *child = obj->child;
            if (child) {
                cJSON_AddItemReferenceToArray(obj, child);
            }
        }

        cJSON_Delete(obj);
    }

    return 0;
}