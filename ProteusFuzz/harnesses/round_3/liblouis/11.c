// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
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

static void
fill_widechar_buffer_from_fuzz(widechar *buf, const uint8_t *data, size_t size, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && i < size; i++) {
        buf[i] = (widechar)data[i];
    }
    for (; i < max_len; i++) {
        buf[i] = 0;
    }
}

static void
fill_formtype_buffer_from_fuzz(formtype *buf, const uint8_t *data, size_t size, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && i < size; i++) {
        buf[i] = (formtype)data[i];
    }
    for (; i < max_len; i++) {
        buf[i] = 0;
    }
}

static void
fill_char_buffer_from_fuzz(char *buf, const uint8_t *data, size_t size, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && i < size; i++) {
        buf[i] = (char)data[i];
    }
    for (; i < max_len; i++) {
        buf[i] = 0;
    }
}

int
LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    const int MAX_LEN = 500;
    const int CHAR_SIZE = lou_charSize();
    
    widechar inbuf[MAX_LEN];
    widechar outbuf[MAX_LEN];
    formtype typeform[MAX_LEN];
    char spacing[MAX_LEN];
    char hyphens[MAX_LEN];
    int outputPos[MAX_LEN];
    int inputPos[MAX_LEN];
    int cursorPos = 0;
    
    int inlen = Size < MAX_LEN ? (int)Size : MAX_LEN - 1;
    int outlen = MAX_LEN - 1;
    int mode;
    
    if (Size == 0) {
        return 0;
    }
    
    mode = Data[0] % 2;
    
    fill_widechar_buffer_from_fuzz(inbuf, Data, Size, MAX_LEN);
    fill_widechar_buffer_from_fuzz(outbuf, Data, Size, MAX_LEN);
    fill_formtype_buffer_from_fuzz(typeform, Data, Size, MAX_LEN);
    fill_char_buffer_from_fuzz(spacing, Data, Size, MAX_LEN);
    fill_char_buffer_from_fuzz(hyphens, Data, Size, MAX_LEN);
    
    memset(outputPos, 0, sizeof(outputPos));
    memset(inputPos, 0, sizeof(inputPos));
    
    const char *tableList = "en-gb";
    
    lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
    
    int temp_inlen = inlen;
    int temp_outlen = outlen;
    int temp_cursorPos = cursorPos;
    
    lou_backTranslate(tableList, inbuf, &temp_inlen, outbuf, &temp_outlen, typeform, spacing, outputPos, inputPos, &temp_cursorPos, mode);
    
    lou_dotsToChar(tableList, inbuf, outbuf, inlen, mode);
    
    temp_inlen = inlen;
    temp_outlen = outlen;
    
    lou_backTranslateString(tableList, inbuf, &temp_inlen, outbuf, &temp_outlen, typeform, spacing, mode);
    
    temp_inlen = inlen;
    temp_outlen = outlen;
    temp_cursorPos = cursorPos;
    
    lou_translate(tableList, inbuf, &temp_inlen, outbuf, &temp_outlen, typeform, spacing, outputPos, inputPos, &temp_cursorPos, mode);
    
    (void)CHAR_SIZE;
    
    return 0;
}