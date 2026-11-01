#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

// Helper to create a simple cJSON object
static cJSON *create_test_object(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;

    // Add some data to make it non-trivial
    cJSON_AddItemToObject(root, "data", cJSON_CreateNumber((double)data[0]));
    cJSON_AddItemToObject(root, "str", cJSON_CreateString("test"));
    
    return root;
}

// Custom hooks that fail allocation
static void *fail_allocate(size_t size) { return NULL; }
static void *fail_reallocate(void *ptr, size_t size) { return NULL; }
static void fail_deallocate(void *ptr) { (void)ptr; }

// Custom hooks that fail reallocate but succeed allocate
static void *success_allocate(size_t size) { return malloc(size); }
static void *fail_reallocate_only(void *ptr, size_t size) { return NULL; }
static void success_deallocate(void *ptr) { free(ptr); }

// Standard hooks
static void *std_allocate(size_t size) { return malloc(size); }
static void *std_reallocate(void *ptr, size_t size) { return realloc(ptr, size); }
static void std_deallocate(void *ptr) { free(ptr); }

// Fixed: cJSON_Hooks in this environment only has 2 members: malloc_fn, free_fn
// The previous code tried to pass 3 members, causing type mismatch and excess element warnings.
static cJSON_Hooks standard_hooks = { std_allocate, std_deallocate };
static cJSON_Hooks fail_alloc_hooks = { fail_allocate, fail_deallocate };
// Note: realloc hook is not supported in this cJSON version, so we use standard deallocate.
static cJSON_Hooks fail_realloc_hooks = { success_allocate, success_deallocate };
static cJSON_Hooks no_realloc_fail_alloc_hooks = { fail_allocate, fail_deallocate };

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int mode = Data[0] % 5;
    cJSON *item = NULL;
    char *result = NULL;

    // Create a test object
    item = create_test_object(Data, Size);
    if (!item) return 0;

    // Scenario 1: Invalid structure to trigger print_value failure (Line 1258)
    if (mode == 1) {
        // Corrupt the type to trigger print_value failure
        item->type = -1; 
        // Use standard hooks to ensure we hit the print_value path
        cJSON_InitHooks(&standard_hooks);
        result = cJSON_PrintUnformatted(item);
        if (result) free(result);
    }
    // Scenario 2: Allocate failure (Line 1252)
    else if (mode == 2) {
        cJSON_InitHooks(&fail_alloc_hooks);
        result = cJSON_PrintUnformatted(item);
        if (result) free(result);
    }
    // Scenario 3: Reallocate failure (Line 1268)
    // Note: This scenario relies on realloc hooks which are not supported in this cJSON version.
    // It will fall back to standard realloc behavior.
    else if (mode == 3) {
        cJSON_InitHooks(&fail_realloc_hooks);
        result = cJSON_PrintUnformatted(item);
        if (result) free(result);
    }
    // Scenario 4: No reallocate, allocate failure (Line 1276)
    else if (mode == 4) {
        cJSON_InitHooks(&no_realloc_fail_alloc_hooks);
        result = cJSON_PrintUnformatted(item);
        if (result) free(result);
    }
    // Scenario 0: Normal (Line 1265)
    else {
        cJSON_InitHooks(&standard_hooks);
        result = cJSON_PrintUnformatted(item);
        if (result) free(result);
        // Also try formatted
        result = cJSON_Print(item);
        if (result) free(result);
    }

    // Cleanup
    cJSON_Delete(item);
    
    // Restore standard hooks to avoid side effects
    cJSON_InitHooks(&standard_hooks);

    return 0;
}