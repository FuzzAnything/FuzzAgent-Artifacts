// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_strings(const char*** SubpropertyNames, cmsUInt32Number count) {
    if (SubpropertyNames != NULL && *SubpropertyNames != NULL) {
        for (cmsUInt32Number i = 0; i < count; i++) {
            if ((*SubpropertyNames)[i] != NULL) {
                free((*SubpropertyNames)[i]);
            }
        }
        free(*SubpropertyNames);
        *SubpropertyNames = NULL;
    }
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for IT8 operations
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Prepare string data from fuzz input
    // Allocate Size + 1 to ensure we have space for the null terminator
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) {
        remove(dummy_file);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Try to find a delimiter to split strings safely
    char *str1 = input_str;
    char *str2 = input_str;

    // Find a safe delimiter within the buffer bounds
    char *delim = NULL;
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] == 0 || Data[i] == ',' || Data[i] == ';' || Data[i] == ':') {
            delim = &input_str[i];
            break;
        }
    }

    if (delim != NULL) {
        *delim = '\0';
        str1 = input_str;
        str2 = delim + 1;
        // Ensure str2 is not pointing past the end of the buffer
        if (str2 >= &input_str[Size]) {
            str2 = "SAMPLE_ID";
        }
    } else {
        // No delimiter found, use the whole string as str1 and a default for str2
        str2 = "SAMPLE_ID";
    }

    // 6. cmsstrcasecmp - This is a public API and should compile
    // Ensure str1 is null-terminated (it is by construction)
    // Ensure str2 is valid
    if (str1 != NULL && str2 != NULL) {
        int res6 = cmsstrcasecmp(str1, str2);
        (void)res6;
    }

    // Cleanup
    free(input_str);
    remove(dummy_file);

    return 0;
}