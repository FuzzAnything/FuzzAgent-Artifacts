// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void cleanup_emph_classes(char const **classes) {
    if (classes) {
        for (int i = 0; classes[i] != NULL; i++) {
            free((void *)classes[i]);
        }
        free((void *)classes);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a null-terminated string from input for table list and text
    char *input_str = malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Prepare buffers for translation
    const int max_len = Size + 100;
    widechar *inbuf = malloc(max_len * sizeof(widechar));
    widechar *outbuf = malloc(max_len * sizeof(widechar));
    formtype *typeform = malloc(max_len * sizeof(formtype));
    char *spacing = malloc(max_len * sizeof(char));
    int *outputPos = malloc(max_len * sizeof(int));
    int *inputPos = malloc(max_len * sizeof(int));

    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(input_str);
        free(inbuf);
        free(outbuf);
        free(typeform);
        free(spacing);
        free(outputPos);
        free(inputPos);
        return 0;
    }

    // Initialize buffers
    memset(inbuf, 0, max_len * sizeof(widechar));
    memset(outbuf, 0, max_len * sizeof(widechar));
    memset(typeform, 0, max_len * sizeof(formtype));
    memset(spacing, 0, max_len * sizeof(char));
    memset(outputPos, 0, max_len * sizeof(int));
    memset(inputPos, 0, max_len * sizeof(int));

    int inlen = Size;
    int outlen = max_len;
    int cursorPos = 0;
    int mode = 0;

    // 1. lou_getEmphClasses
    char const **emph_classes = lou_getEmphClasses(input_str);
    cleanup_emph_classes(emph_classes);

    // 2. lou_readCharFromFile (use dummy file)
    int mode_file = 0;
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
    int ch = lou_readCharFromFile("./dummy_file", &mode_file);
    (void)ch; // suppress unused warning
    if (mode_file == 1) {
        // Try reading again to trigger EOF
        ch = lou_readCharFromFile("./dummy_file", &mode_file);
        (void)ch;
    }

    // 3. lou_translate
    // Convert input_str to widechar for inbuf
    for (int i = 0; i < Size && i < max_len; i++) {
        inbuf[i] = (widechar)input_str[i];
    }
    int ret_translate = lou_translate(input_str, inbuf, &inlen, outbuf, &outlen,
                                      typeform, spacing, outputPos, inputPos,
                                      &cursorPos, mode);
    (void)ret_translate;

    // 4. lou_translateString
    int ret_translateString = lou_translateString(input_str, inbuf, &inlen, outbuf, &outlen,
                                                  typeform, spacing, mode);
    (void)ret_translateString;

    // 5. lou_compileString
    int ret_compile = lou_compileString(input_str, input_str);
    (void)ret_compile;

    // 6. lou_getTable
    const void *table = lou_getTable(input_str);
    (void)table;

    // Cleanup
    free(input_str);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);

    return 0;
}