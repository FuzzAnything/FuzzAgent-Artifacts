// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void setup_dummy_table(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "table en-gb-8bit\n");
        fprintf(f, "format: 8bit\n");
        fprintf(f, "input: 8bit\n");
        fprintf(f, "output: 8bit\n");
        fprintf(f, "translation: 1:1\n");
        fclose(f);
    }
}

static void cleanup_dummy_table(const char *filename) {
    remove(filename);
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    const char *dummy_file = "./dummy_file";
    setup_dummy_table(dummy_file);

    if (Size < 1) {
        cleanup_dummy_table(dummy_file);
        return 0;
    }

    // Create a null-terminated string from input for table list or content
    char *table_list = (char *)malloc(Size + 1);
    if (!table_list) {
        cleanup_dummy_table(dummy_file);
        return 0;
    }
    memcpy(table_list, Data, Size);
    table_list[Size] = '\0';

    // Prepare input buffer for translation (widechar)
    size_t max_inlen = Size;
    widechar *inbuf = (widechar *)malloc(max_inlen * sizeof(widechar));
    int *inlen = (int *)malloc(sizeof(int));
    if (!inbuf || !inlen) {
        free(table_list);
        free(inbuf);
        free(inlen);
        cleanup_dummy_table(dummy_file);
        return 0;
    }
    *inlen = 0;
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] < 256) {
            inbuf[*inlen] = (widechar)Data[i];
            (*inlen)++;
        }
    }

    // Prepare output buffer
    widechar *outbuf = (widechar *)malloc(max_inlen * sizeof(widechar));
    int *outlen = (int *)malloc(sizeof(int));
    if (!outbuf || !outlen) {
        free(table_list);
        free(inbuf);
        free(inlen);
        cleanup_dummy_table(dummy_file);
        return 0;
    }
    *outlen = max_inlen;

    // Optional parameters for lou_translate
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = (int *)malloc(max_inlen * sizeof(int));
    int *inputPos = (int *)malloc(max_inlen * sizeof(int));
    int cursorPos = 0;

    if (!outputPos || !inputPos) {
        free(table_list);
        free(inbuf);
        free(inlen);
        free(outbuf);
        free(outlen);
        free(outputPos);
        free(inputPos);
        cleanup_dummy_table(dummy_file);
        return 0;
    }

    // Fuzz lou_getTable
    const void *table = lou_getTable(table_list);
    if (table) {
        // If table loaded, try translation
        int mode = (Data[0] % 4) - 2; // Generate mode: -2, -1, 0, 1
        int ret = lou_translate(table_list, inbuf, inlen, outbuf, outlen,
                                typeform, spacing, outputPos, inputPos,
                                &cursorPos, mode);
        (void)ret; // Ignore return for fuzzing stability
    }

    // Fuzz lou_translateString
    // Ensure we have at least 2 bytes before accessing Data[1]
    int mode2 = 0;
    if (Size >= 2) {
        mode2 = (Data[1] % 4) - 2;
    }
    int ret2 = lou_translateString(table_list, inbuf, inlen, outbuf, outlen,
                                   typeform, spacing, mode2);
    (void)ret2;

    // Fuzz lou_compileString
    char *compile_input = (char *)malloc(Size + 1);
    if (compile_input) {
        memcpy(compile_input, Data, Size);
        compile_input[Size] = '\0';
        int ret3 = lou_compileString(table_list, compile_input);
        (void)ret3;
        free(compile_input);
    }

    // Fuzz lou_getEmphClasses
    char const **emph_classes = lou_getEmphClasses(table_list);
    if (emph_classes) {
        // Note: liblouis documentation says caller must free result.
        // However, the structure is an array of strings.
        // We assume the array itself is allocated and needs freeing.
        // In some versions, it might be static. We try to free safely.
        // Since we don't know the exact allocation strategy, we skip freeing
        // to avoid double-free if it's static, or we rely on the library.
        // For fuzzing, we just check it doesn't crash.
        (void)emph_classes;
    }

    // Fuzz lou_indexTables
    const char **tables = (const char **)malloc(2 * sizeof(const char *));
    if (tables) {
        tables[0] = table_list;
        tables[1] = NULL;
        lou_indexTables(tables);
        free((void *)tables);
    }

    // Cleanup
    free(table_list);
    free(inbuf);
    free(inlen);
    free(outbuf);
    free(outlen);
    free(outputPos);
    free(inputPos);
    cleanup_dummy_table(dummy_file);

    return 0;
}