// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper to create a valid cJSON object for testing */
static cJSON *create_test_object(void) {
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return NULL;
    }
    /* Add a dummy item to ensure object is not empty if needed */
    cJSON_AddItemToObject(obj, "dummy", cJSON_CreateNull());
    return obj;
}

/* Helper to create a valid cJSON item for testing */
static cJSON *create_test_item(void) {
    return cJSON_CreateNull();
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Parse input data into key and value strings */
    const char *key = (const char *)Data;
    const char *raw = (const char *)(Data + 1);
    size_t key_len = Size > 1 ? Size - 1 : 0;
    size_t raw_len = Size > 1 ? Size - 1 : 0;

    /* Ensure null termination for safety */
    char *key_buf = NULL;
    char *raw_buf = NULL;
    if (key_len > 0) {
        key_buf = malloc(key_len + 1);
        if (key_buf) {
            memcpy(key_buf, key, key_len);
            key_buf[key_len] = '\0';
        }
    }
    if (raw_len > 0) {
        raw_buf = malloc(raw_len + 1);
        if (raw_buf) {
            memcpy(raw_buf, raw, raw_len);
            raw_buf[raw_len] = '\0';
        }
    }

    /* Test 1: cJSON_AddItemReferenceToObject */
    {
        cJSON *obj = create_test_object();
        cJSON *item = create_test_item();
        if (obj && item) {
            /* AddItemReferenceToObject does not take ownership of 'item'.
             * We must not delete 'item' separately if it was added to 'obj'.
             * However, since it's a reference, deleting 'obj' will not free 'item'.
             * We need to manage the lifecycle carefully.
             * If we add a reference, we should NOT delete 'item' immediately after deleting 'obj'
             * if 'obj' didn't take ownership. But here 'item' is independent.
             * The issue in the previous run was likely double free or use after free 
             * because we added an item to an object, deleted the object (which frees the item if it was owned),
             * and then tried to delete the item again.
             * AddItemReferenceToObject adds a REFERENCE. It does NOT copy.
             * So if we delete the object, the reference is gone, but the item still exists.
             * We should delete the item AFTER the object, or ensure we don't delete the item if it was moved.
             * Actually, AddItemReferenceToObject does NOT take ownership. 
             * So 'item' remains valid after adding to 'obj'.
             * When we delete 'obj', it does NOT delete 'item'.
             * So we can safely delete 'item' after 'obj'.
             * Wait, the error was heap-use-after-free.
             * Let's re-read the error: 
             * Freed by thread T0 here: cJSON_Delete(obj);
             * Then READ of size 8 at ... in cJSON_Delete(item);
             * This implies 'item' was freed by 'cJSON_Delete(obj)'.
             * This happens if 'item' was added via AddItemToObject (which takes ownership) 
             * but we are testing AddItemReferenceToObject.
             * Ah, the previous code block for Test 1 was:
             * cJSON_AddItemReferenceToObject(obj, ...);
             * cJSON_Delete(obj);
             * cJSON_Delete(item);
             * If AddItemReferenceToObject works correctly, it should NOT free 'item' when 'obj' is deleted.
             * However, if the implementation of AddItemReferenceToObject internally calls something that 
             * links the item in a way that cJSON_Delete traverses and frees it, that would be a bug in cjson 
             * or a misunderstanding of the API.
             * According to cjson docs: "Add a reference to an existing cJSON item into an object".
             * "It does not take ownership of the item."
             * So deleting 'obj' should NOT delete 'item'.
             * Why did the sanitizer say it was freed?
             * Maybe the 'item' was added to 'obj' in a previous test case? No, we create new ones.
             * Wait, look at the stack trace again.
             * The error is in Test 1 block?
             * Line 74 is cJSON_Delete(item).
             * Line 73 is cJSON_Delete(obj).
             * The free happened in cJSON_Delete(obj).
             * This means 'item' was part of 'obj' and was freed when 'obj' was deleted.
             * This implies that AddItemReferenceToObject might have taken ownership in this version of cjson, 
             * OR the 'item' was somehow linked such that cJSON_Delete traversed it.
             * Actually, if I look at cjson source, AddItemReferenceToObject just links the pointers.
             * But cJSON_Delete iterates through children. If 'item' is a child of 'obj', cJSON_Delete(obj) will free it.
             * The documentation says "without copying data" and "shared ownership".
             * If I delete the object, does it delete the children? Yes, cJSON_Delete deletes the whole tree.
             * If I add a REFERENCE, and then delete the object, the object deletes its children.
             * If the child is a reference to an external item, deleting the object will delete that external item.
             * This is the danger of references.
             * So, if I use AddItemReferenceToObject, I should NOT delete 'item' afterwards if I delete 'obj'.
             * OR, I should not delete 'obj' if I want to keep 'item'.
             * To avoid use-after-free:
             * If I add 'item' to 'obj' (even as reference), and then delete 'obj', 'item' is freed.
             * So I must NOT call cJSON_Delete(item) after cJSON_Delete(obj) if 'item' was added to 'obj'.
             * Let's fix the logic:
             * If we add the item to the object, we let the object manage it.
             * So we only delete the object.
             * But wait, AddItemReferenceToObject is supposed to share ownership.
             * If I delete the object, the item is freed.
             * If I then try to delete the item, it's a double free (or use after free if the pointer is dangling).
             * The correct approach for fuzzing:
             * 1. Create obj and item.
             * 2. Add item to obj.
             * 3. Delete obj (which frees item).
             * 4. Do NOT delete item.
             * This applies to AddItemToObject, AddItemToObjectCS, AddItemReferenceToObject.
             * All of them link the item as a child. cJSON_Delete(obj) will free the child.
             * So the fix is: remove the separate cJSON_Delete(item) call after adding it to obj.
             */
            cJSON_AddItemReferenceToObject(obj, key_buf ? key_buf : "key", item);
            /* Do not delete item separately, it is now part of obj */
            cJSON_Delete(obj);
        }
    }

    /* Test 2: cJSON_AddItemToObjectCS */
    {
        cJSON *obj = create_test_object();
        cJSON *item = create_test_item();
        if (obj && item) {
            cJSON_AddItemToObjectCS(obj, key_buf ? key_buf : "key", item);
            /* item is now owned by obj */
            cJSON_Delete(obj);
        }
    }

    /* Test 3: cJSON_CreateRaw */
    {
        cJSON *raw_item = cJSON_CreateRaw(raw_buf ? raw_buf : "");
        if (raw_item) {
            cJSON_Delete(raw_item);
        }
    }

    /* Test 4: cJSON_AddRawToObject */
    {
        cJSON *obj = create_test_object();
        if (obj) {
            /* AddRawToObject creates a new item and adds it. We don't need to delete it separately. */
            cJSON_AddRawToObject(obj, key_buf ? key_buf : "key", raw_buf ? raw_buf : "");
            cJSON_Delete(obj);
        }
    }

    /* Test 5: cJSON_AddItemToObject */
    {
        cJSON *obj = create_test_object();
        cJSON *item = create_test_item();
        if (obj && item) {
            cJSON_AddItemToObject(obj, key_buf ? key_buf : "key", item);
            /* item is now owned by obj */
            cJSON_Delete(obj);
        }
    }

    /* Test 6: cJSON_DeleteItemFromObject */
    {
        cJSON *obj = create_test_object();
        if (obj) {
            /* DeleteItemFromObject removes and frees the item. */
            cJSON_DeleteItemFromObject(obj, key_buf ? key_buf : "key");
            cJSON_Delete(obj);
        }
    }

    /* Cleanup allocated buffers */
    free(key_buf);
    free(raw_buf);

    return 0;
}