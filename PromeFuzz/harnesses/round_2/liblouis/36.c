// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static const char *tableList = "en-us-g2.ctb";
static const int MAX_BUF_SIZE = 1024;

static void initialize_widechar_buffer(widechar *buf, int len, const uint8_t *data, size_t size, size_t *offset) {
    for (int i = 0; i < len && *offset + 1 < size; i++) {
        buf[i] = (widechar)((data[*offset] << 8) | data[*offset + 1]);
        *offset += 2;
    }
}

static void initialize_formtype_buffer(formtype *buf, int len, const uint8_t *data, size_t size, size_t *offset) {
    for (int i = 0; i < len && *offset + 1 < size; i++) {
        buf[i] = (formtype)((data[*offset] << 8) | data[*offset + 1]);
        *offset += 2;
    }
}

static void initialize_char_buffer(char *buf, int len, const uint8_t *data, size_t size, size_t *offset) {
    for (int i = 0; i < len && *offset < size; i++) {
        buf[i] = (char)(data[*offset] % 256);
        (*offset)++;
    }
}

static void initialize_int_buffer(int *buf, int len, const uint8_t *data, size_t size, size_t *offset) {
    for (int i = 0; i < len && *offset + 3 < size; i++) {
        buf[i] = (int)((data[*offset] << 24) | (data[*offset + 1] << 16) | (data[*offset + 2] << 8) | data[*offset + 3]);
        *offset += 4;
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    size_t offset = 0;
    int func_selector = Data[offset] % 6;
    offset++;

    int inlen = (Data[offset] % (MAX_BUF_SIZE / 2)) + 1;
    offset = (offset + 1) % Size;
    int outlen = inlen * 2;

    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!inbuf || !outbuf) {
        free(inbuf);
        free(outbuf);
        return 0;
    }

    size_t data_offset = offset;
    initialize_widechar_buffer(inbuf, inlen, Data, Size, &data_offset);

    int mode = Data[data_offset % Size] % 4;
    data_offset = (data_offset + 1) % Size;

    switch (func_selector) {
        case 0: { // lou_backTranslate
            formtype *typeform = NULL;
            char *spacing = NULL;
            int *outputPos = (int *)malloc(inlen * sizeof(int));
            int *inputPos = (int *)malloc(outlen * sizeof(int));
            int cursorPos = 0;

            if (Data[data_offset % Size] % 2) {
                typeform = (formtype *)malloc(inlen * sizeof(formtype));
                if (typeform) {
                    initialize_formtype_buffer(typeform, inlen, Data, Size, &data_offset);
                }
            }
            if (Data[data_offset % Size] % 2) {
                spacing = (char *)malloc(outlen * sizeof(char));
                if (spacing) {
                    initialize_char_buffer(spacing, outlen, Data, Size, &data_offset);
                }
            }
            if (outputPos) {
                initialize_int_buffer(outputPos, inlen, Data, Size, &data_offset);
            }
            if (inputPos) {
                initialize_int_buffer(inputPos, outlen, Data, Size, &data_offset);
            }
            cursorPos = Data[data_offset % Size] % (inlen + 1);
            data_offset = (data_offset + 1) % Size;

            int inlen_copy = inlen;
            int outlen_copy = outlen;
            lou_backTranslate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                             typeform, spacing, outputPos, inputPos, &cursorPos, mode);

            free(typeform);
            free(spacing);
            free(outputPos);
            free(inputPos);
            break;
        }
        case 1: { // lou_translateString
            formtype *typeform = NULL;
            char *spacing = NULL;

            if (Data[data_offset % Size] % 2) {
                typeform = (formtype *)malloc(inlen * sizeof(formtype));
                if (typeform) {
                    initialize_formtype_buffer(typeform, inlen, Data, Size, &data_offset);
                }
            }
            if (Data[data_offset % Size] % 2) {
                spacing = (char *)malloc((inlen + 1) * sizeof(char));
                if (spacing) {
                    initialize_char_buffer(spacing, inlen + 1, Data, Size, &data_offset);
                }
            }

            int inlen_copy = inlen;
            int outlen_copy = outlen;
            lou_translateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                               typeform, spacing, mode);

            free(typeform);
            free(spacing);
            break;
        }
        case 2: { // lou_charToDots
            lou_charToDots(tableList, inbuf, outbuf, inlen, mode);
            break;
        }
        case 3: { // lou_translate
            formtype *typeform = NULL;
            char *spacing = NULL;
            int *outputPos = (int *)malloc(inlen * sizeof(int));
            int *inputPos = (int *)malloc(outlen * sizeof(int));
            int cursorPos = 0;

            if (Data[data_offset % Size] % 2) {
                typeform = (formtype *)malloc(outlen * sizeof(formtype));
                if (typeform) {
                    memset(typeform, 0, outlen * sizeof(formtype));
                }
            }
            if (Data[data_offset % Size] % 2) {
                spacing = (char *)malloc(inlen * sizeof(char));
                if (spacing) {
                    initialize_char_buffer(spacing, inlen, Data, Size, &data_offset);
                }
            }
            if (outputPos) {
                initialize_int_buffer(outputPos, inlen, Data, Size, &data_offset);
            }
            if (inputPos) {
                initialize_int_buffer(inputPos, outlen, Data, Size, &data_offset);
            }
            cursorPos = Data[data_offset % Size] % (inlen + 1);
            data_offset = (data_offset + 1) % Size;

            int inlen_copy = inlen;
            int outlen_copy = outlen;
            lou_translate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                         typeform, spacing, outputPos, inputPos, &cursorPos, mode);

            free(typeform);
            free(spacing);
            free(outputPos);
            free(inputPos);
            break;
        }
        case 4: { // lou_getTable
            lou_getTable(tableList);
            break;
        }
        case 5: { // lou_backTranslateString
            formtype *typeform = NULL;
            char *spacing = NULL;

            if (Data[data_offset % Size] % 2) {
                typeform = (formtype *)malloc(outlen * sizeof(formtype));
                if (typeform) {
                    memset(typeform, 0, outlen * sizeof(formtype));
                }
            }
            if (Data[data_offset % Size] % 2) {
                spacing = (char *)malloc(outlen * sizeof(char));
                if (spacing) {
                    initialize_char_buffer(spacing, outlen, Data, Size, &data_offset);
                }
            }

            int inlen_copy = inlen;
            int outlen_copy = outlen;
            lou_backTranslateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                                   typeform, spacing, mode);

            free(typeform);
            free(spacing);
            break;
        }
    }

    free(inbuf);
    free(outbuf);
    return 0;
}