// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
#include "config.h"
#include "liblouis.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void dummy_log_callback(logLevels level, const char *message) {
    (void)level;
    (void)message;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    lou_registerLogCallback(dummy_log_callback);

    size_t table_len = Size / 8;
    if (table_len > 64) table_len = 64;
    char *table_list = malloc(table_len + 1);
    if (!table_list) return 0;
    memcpy(table_list, Data, table_len);
    table_list[table_len] = '\0';

    size_t widechar_data_size = Size - table_len;
    if (widechar_data_size < 2) {
        free(table_list);
        return 0;
    }

    if (widechar_data_size % 2 != 0) widechar_data_size--;
    int widechar_len = widechar_data_size / 2;
    if (widechar_len > 256) widechar_len = 256;

    widechar *inbuf = malloc(widechar_len * sizeof(widechar));
    if (!inbuf) {
        free(table_list);
        return 0;
    }
    memcpy(inbuf, Data + table_len, widechar_len * sizeof(widechar));

    widechar *outbuf1 = malloc(widechar_len * sizeof(widechar));
    if (outbuf1) {
        lou_charToDots(table_list, inbuf, outbuf1, widechar_len, 0);
        lou_charToDots(table_list, inbuf, outbuf1, widechar_len, 1);
        free(outbuf1);
    }

    lou_checkTable(table_list);

    int inlen = widechar_len;
    int outlen = widechar_len * 4;
    widechar *outbuf2 = malloc(outlen * sizeof(widechar));
    if (outbuf2) {
        lou_translateString(table_list, inbuf, &inlen, outbuf2, &outlen, NULL, NULL, 0);
        
        formtype *typeform = malloc(inlen * sizeof(formtype));
        char *spacing = malloc((inlen + 1) * sizeof(char));
        if (typeform && spacing) {
            size_t typeform_data_size = inlen * sizeof(formtype);
            if (typeform_data_size > 0) {
                size_t offset = (table_len + widechar_len * sizeof(widechar)) % Size;
                for (int i = 0; i < inlen; i++) {
                    typeform[i] = Data[(offset + i) % Size] | (Data[(offset + i + 1) % Size] << 8);
                }
                memset(spacing, 0, (inlen + 1) * sizeof(char));
                lou_translateString(table_list, inbuf, &inlen, outbuf2, &outlen, typeform, spacing, 0);
            }
            free(typeform);
            free(spacing);
        }
        free(outbuf2);
    }

    int back_inlen = widechar_len;
    int back_outlen = widechar_len * 4;
    widechar *back_outbuf = malloc(back_outlen * sizeof(widechar));
    if (back_outbuf) {
        lou_backTranslateString(table_list, inbuf, &back_inlen, back_outbuf, &back_outlen, NULL, NULL, 0);
        
        formtype *back_typeform = malloc(back_outlen * sizeof(formtype));
        char *back_spacing = malloc(back_outlen * sizeof(char));
        if (back_typeform && back_spacing) {
            lou_backTranslateString(table_list, inbuf, &back_inlen, back_outbuf, &back_outlen, back_typeform, back_spacing, 0);
            free(back_typeform);
            free(back_spacing);
        }
        free(back_outbuf);
    }

    FILE *dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, dummy);
        fclose(dummy);
        
        int mode = 1;
        while (lou_readCharFromFile("./dummy_file", &mode) != EOF) {
        }
        remove("./dummy_file");
    }

    lou_charSize();

    free(inbuf);
    free(table_list);

    return 0;
}