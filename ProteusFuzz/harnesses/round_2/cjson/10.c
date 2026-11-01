// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    cJSON *null1 = NULL;
    cJSON *null2 = NULL;
    cJSON *null3 = NULL;
    cJSON *array = NULL;
    
    /* Step 1: Create first null */
    null1 = cJSON_CreateNull();
    if (null1 == NULL) {
        return 0;
    }

    /* Step 2: Create second null */
    null2 = cJSON_CreateNull();
    if (null2 == NULL) {
        cJSON_free(null1);
        return 0;
    }

    /* Step 3: Create third null */
    null3 = cJSON_CreateNull();
    if (null3 == NULL) {
        cJSON_free(null1);
        cJSON_free(null2);
        return 0;
    }

    /* Step 4: Create array */
    array = cJSON_CreateArray();
    if (array == NULL) {
        cJSON_free(null1);
        cJSON_free(null2);
        cJSON_free(null3);
        return 0;
    }

    /* Step 5: Add first null to array */
    if (!cJSON_AddItemToArray(array, null1)) {
        cJSON_Delete(array);
        cJSON_free(null2);
        cJSON_free(null3);
        return 0;
    }

    /* Step 6: Add second null to array */
    if (!cJSON_AddItemToArray(array, null2)) {
        cJSON_Delete(array);
        cJSON_free(null3);
        return 0;
    }

    /* Step 7: Add third null to array */
    if (!cJSON_AddItemToArray(array, null3)) {
        cJSON_Delete(array);
        return 0;
    }

    /* Step 8: Free the array (which also frees its children) */
    /* Note: cJSON_Delete is the standard way to free a cJSON tree, 
       which internally handles the children and uses cJSON_free.
       Using cJSON_Delete(array) is equivalent to cJSON_free(array) 
       for the root, but ensures recursive cleanup of children.
       However, the task specifically asks to use cJSON_free at the end.
       Since cJSON_Delete is just a wrapper that calls cJSON_free on the root 
       after recursively freeing children, and we are instructed to use cJSON_free,
       we must ensure we don't double free or leak.
       
       The leak report indicates that null1, null2, null3 are not being freed 
       because cJSON_AddItemToArray takes ownership, but if we call cJSON_free 
       on the array directly without the recursive logic of cJSON_Delete, 
       the children might not be freed depending on the implementation version.
       However, standard cJSON behavior is that cJSON_Delete handles the tree.
       If the task insists on cJSON_free, and the library version expects 
       cJSON_Delete for trees, we should use cJSON_Delete to avoid leaks.
       But the prompt says "cJSON_free deallocates memory... Always use it for cJSON objects".
       Let's re-read the prompt's instruction on cJSON_free: 
       "cJSON_free deallocates memory via global_hooks.deallocate, replacing free for custom allocators. 
        Pass any cJSON-allocated object to it. It returns void... Always use it for cJSON objects to ensure proper cleanup".
        
       The leak happens because cJSON_free(array) only frees the array node itself, 
       not the children (null1, null2, null3) which were added to it. 
       The children are linked via the 'child' pointer of the array. 
       cJSON_Delete is designed to traverse and free the whole tree. 
       cJSON_free is a low-level allocator wrapper.
       
       To fix the leak while following the "use cJSON_free" instruction for the final step,
       we must manually free the children before freeing the array, OR use cJSON_Delete 
       which is the correct API for freeing a tree. 
       Given the context of "fuzz driver" and "proper cleanup", using cJSON_Delete 
       is the correct approach to free the tree structure created. 
       The instruction "Always use it for cJSON objects" likely implies using the 
       library's provided deallocation mechanism, which for a tree is cJSON_Delete.
       However, if we strictly must call cJSON_free as the last step, we can do:
       
       Option A: Use cJSON_Delete(array). This is the standard and safe way.
       Option B: Manually free children then cJSON_free(array).
       
       Since cJSON_Delete is not in the target API list but is standard in cJSON, 
       and the leak is due to not freeing children, the most robust fix is to use 
       cJSON_Delete. If cJSON_Delete is not allowed, we must manually traverse and free.
       But looking at the target APIs, cJSON_Delete is not listed. 
       The prompt says "invoke the functions in the following order... -> cJSON_free".
       It implies cJSON_free is the final cleanup.
       
       If we call cJSON_free(array), it only frees the array struct. 
       The children (null1, null2, null3) are still allocated.
       We need to free them. But they are now owned by the array.
       The standard way is cJSON_Delete. 
       If we cannot use cJSON_Delete, we must manually free the children.
       However, the prompt says "Always use it [cJSON_free] for cJSON objects".
       This suggests we should use cJSON_free for every object.
       
       Let's try to manually free the children before freeing the array.
       But wait, if we free null1, null2, null3 individually, and then free array,
       that might work if the array doesn't try to free them again.
       But cJSON_AddItemToArray links them. 
       If we free them individually, the array's child pointers become dangling.
       Then cJSON_free(array) just frees the array node.
       This should work to avoid leaks, assuming cJSON_free doesn't recursively free.
       
       However, the safest and most correct way in cJSON is cJSON_Delete.
       Since the prompt asks to use cJSON_free at the end, and the leak is the issue,
       and cJSON_Delete is the standard function for this, I will use cJSON_Delete 
       because it is the correct way to free a cJSON tree and it internally uses 
       the allocator (cJSON_free) for the nodes. 
       The prompt's instruction "Always use it for cJSON objects" is a general guideline, 
       but for a tree, cJSON_Delete is the specific function that ensures all objects 
       in the tree are freed using the allocator.
       
       Wait, the prompt says "invoke the functions in the following order... -> cJSON_free".
       It lists cJSON_free as the last step. It does not list cJSON_Delete.
       So I must use cJSON_free.
       
       To avoid the leak, I must free the children (null1, null2, null3) before freeing the array.
       But they are part of the array. 
       If I call cJSON_free(null1), cJSON_free(null2), cJSON_free(null3) before cJSON_free(array),
       then the array's child pointers are dangling, but that's okay if we are about to free the array.
       This should prevent the leak.
       
       Let's try that.
    */
    
    /* Free children manually to avoid leak when using cJSON_free on array */
    cJSON_free(null1);
    cJSON_free(null2);
    cJSON_free(null3);
    
    /* Step 8: Free the array */
    cJSON_free(array);

    return 0;
}