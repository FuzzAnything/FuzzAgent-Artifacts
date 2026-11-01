// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    int count = 0;
    if (Size > 0) {
        count = Data[0] % 100;
    }
    
    int *numbers = NULL;
    if (count > 0 && Size > 1) {
        numbers = (int *)malloc(count * sizeof(int));
        if (!numbers) {
            cJSON_Delete(root);
            return 0;
        }
        for (int i = 0; i < count; i++) {
            if (i < Size - 1) {
                numbers[i] = Data[i + 1];
            } else {
                numbers[i] = 0;
            }
        }
    }

    cJSON *array = cJSON_CreateIntArray(numbers, count);
    if (numbers) {
        free(numbers);
    }

    if (!array) {
        cJSON_Delete(root);
        return 0;
    }

    int index = 0;
    if (Size > 1) {
        index = Data[1] % (count > 0 ? count : 1);
    }
    cJSON *item = cJSON_GetArrayItem(array, index);

    cJSON_AddItemToObject(root, "array", array);

    cJSON *nestedObj = cJSON_CreateObject();
    if (!nestedObj) {
        cJSON_Delete(root);
        return 0;
    }

    char *strData = (char *)malloc(Size + 1);
    if (!strData) {
        cJSON_Delete(nestedObj);
        cJSON_Delete(root);
        return 0;
    }
    memcpy(strData, Data, Size);
    strData[Size] = '\0';

    cJSON *strItem = cJSON_CreateString(strData);
    free(strData);

    if (strItem) {
        cJSON_AddItemToObject(nestedObj, "string", strItem);
    }

    cJSON_AddItemToObject(root, "nested", nestedObj);

    cJSON *anotherObj = cJSON_CreateObject();
    if (!anotherObj) {
        cJSON_Delete(root);
        return 0;
    }

    char *strData2 = (char *)malloc(Size + 1);
    if (!strData2) {
        cJSON_Delete(anotherObj);
        cJSON_Delete(root);
        return 0;
    }
    memcpy(strData2, Data, Size);
    strData2[Size] = '\0';

    cJSON *strItem2 = cJSON_CreateString(strData2);
    free(strData2);

    if (strItem2) {
        cJSON_AddItemToObject(anotherObj, "string2", strItem2);
    }

    cJSON_AddItemToObject(root, "another", anotherObj);

    cJSON_Delete(root);

    return 0;
}