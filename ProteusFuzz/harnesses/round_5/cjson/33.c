// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void cleanup_json(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a root object to work with
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Use input data to generate keys and values
    const char *key_start = (const char *)Data;
    size_t key_len = Size;
    if (key_len > 100) key_len = 100; // Limit key length to avoid huge allocations

    // Ensure null termination for key
    char *key = (char *)malloc(key_len + 1);
    if (!key) {
        cleanup_json(root);
        return 0;
    }
    memcpy(key, key_start, key_len);
    key[key_len] = '\0';

    // Add some initial data to the object to make replacement/deletion meaningful
    cJSON_AddBoolToObject(root, "initial_bool", 1);
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    if (nested) {
        cJSON_AddBoolToObject(nested, "nested_bool", 0);
    }

    // Create a new item to use for replacement
    cJSON *new_item = cJSON_CreateNumber((double)(Data[0] % 100));
    if (!new_item) {
        new_item = cJSON_CreateString("fuzz_string");
    }

    // Fuzz cJSON_ReplaceItemInObjectCaseSensitive
    // We try to replace an existing key and a non-existing key
    cJSON_ReplaceItemInObjectCaseSensitive(root, "initial_bool", new_item);
    cJSON_ReplaceItemInObjectCaseSensitive(root, key, new_item); // Might fail if key doesn't exist, which is fine

    // Fuzz cJSON_DeleteItemFromObject
    // Try deleting existing and non-existing keys
    cJSON_DeleteItemFromObject(root, "initial_bool");
    cJSON_DeleteItemFromObject(root, key);
    cJSON_DeleteItemFromObject(root, "nested");

    // Fuzz cJSON_AddBoolToObject
    // Add a boolean with the fuzzed key
    cJSON *added_bool = cJSON_AddBoolToObject(root, key, Data[0] % 2);
    if (added_bool) {
        // Try adding again to test overwrite behavior
        cJSON_AddBoolToObject(root, key, 1);
    }

    // Fuzz cJSON_AddObjectToObject
    // Add a nested object with the fuzzed key
    cJSON *added_obj = cJSON_AddObjectToObject(root, key);
    if (added_obj) {
        // Add something inside the new object
        cJSON_AddBoolToObject(added_obj, "inner", 1);
    }

    // Fuzz cJSON_ReplaceItemInObject
    // Create another item for replacement
    cJSON *replace_item = cJSON_CreateString("replacement_value");
    if (replace_item) {
        cJSON_ReplaceItemInObject(root, key, replace_item);
        cJSON_ReplaceItemInObject(root, "non_existent_key", replace_item);
    }

    // Fuzz cJSON_HasObjectItem
    // Check for existence of various keys
    cJSON_HasObjectItem(root, key);
    cJSON_HasObjectItem(root, "initial_bool");
    cJSON_HasObjectItem(root, "nested");
    cJSON_HasObjectItem(root, "non_existent");
    cJSON_HasObjectItem(NULL, key); // Test NULL object handling
    cJSON_HasObjectItem(root, NULL); // Test NULL key handling

    // Cleanup
    free(key);
    cleanup_json(root);

    return 0;
}