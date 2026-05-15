// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Function selector */
    uint8_t selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    switch (selector) {
        case 0: { /* lou_readCharFromFile */
            write_dummy_file(fuzz_data, fuzz_size);
            int mode = 1;
            lou_readCharFromFile("./dummy_file", &mode);
            break;
        }
        case 1: { /* lou_hyphenate */
            if (fuzz_size < 1) break;
            size_t max_len = fuzz_size / sizeof(widechar);
            if (max_len < 1) max_len = 1;
            int inlen = (int)(fuzz_size % max_len);
            if (inlen < 0) inlen = 0;
            
            widechar *inbuf = (widechar *)malloc(max_len * sizeof(widechar));
            if (!inbuf) break;
            for (size_t i = 0; i < max_len && i * sizeof(widechar) < fuzz_size; i++) {
                inbuf[i] = (widechar)(fuzz_data[i * sizeof(widechar) % fuzz_size] | 0x40);
            }
            
            char *hyphens = (char *)malloc(inlen + 1);
            if (hyphens) {
                memset(hyphens, 0, inlen + 1);
                lou_hyphenate("en-us-g2.ctb", inbuf, inlen, hyphens, 0);
                free(hyphens);
            }
            free(inbuf);
            break;
        }
        case 2: { /* lou_translate */
            if (fuzz_size < 1) break;
            size_t max_inlen = fuzz_size / sizeof(widechar);
            if (max_inlen < 1) max_inlen = 1;
            int inlen = (int)(fuzz_size % max_inlen);
            if (inlen < 0) inlen = 0;
            int outlen = inlen * 2;
            if (outlen < 10) outlen = 10;
            
            widechar *inbuf = (widechar *)malloc(max_inlen * sizeof(widechar));
            if (!inbuf) break;
            for (size_t i = 0; i < max_inlen && i * sizeof(widechar) < fuzz_size; i++) {
                inbuf[i] = (widechar)(fuzz_data[i * sizeof(widechar) % fuzz_size]);
            }
            
            widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
            formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
            char *spacing = (char *)malloc(inlen * sizeof(char));
            int *outputPos = (int *)malloc(inlen * sizeof(int));
            int *inputPos = (int *)malloc(outlen * sizeof(int));
            int cursorPos = 0;
            
            if (outbuf && typeform && spacing && outputPos && inputPos) {
                memset(typeform, 0, outlen * sizeof(formtype));
                memset(spacing, 0, inlen * sizeof(char));
                lou_translate("en-us-g2.ctb", inbuf, &inlen, outbuf, &outlen,
                             typeform, spacing, outputPos, inputPos, &cursorPos, 0);
            }
            
            free(inbuf);
            free(outbuf);
            free(typeform);
            free(spacing);
            free(outputPos);
            free(inputPos);
            break;
        }
        case 3: { /* lou_charSize */
            lou_charSize();
            break;
        }
        case 4: { /* lou_translatePrehyphenated */
            if (fuzz_size < 1) break;
            size_t max_inlen = fuzz_size / sizeof(widechar);
            if (max_inlen < 1) max_inlen = 1;
            int inlen = (int)(fuzz_size % max_inlen);
            if (inlen < 0) inlen = 0;
            int outlen = inlen * 2;
            if (outlen < 10) outlen = 10;
            
            widechar *inbuf = (widechar *)malloc(max_inlen * sizeof(widechar));
            if (!inbuf) break;
            for (size_t i = 0; i < max_inlen && i * sizeof(widechar) < fuzz_size; i++) {
                inbuf[i] = (widechar)(fuzz_data[i * sizeof(widechar) % fuzz_size]);
            }
            
            widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
            formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
            char *spacing = (char *)malloc(inlen * sizeof(char));
            int *outputPos = (int *)malloc(inlen * sizeof(int));
            int *inputPos = (int *)malloc(outlen * sizeof(int));
            char *inputHyphens = (char *)malloc(inlen + 1);
            char *outputHyphens = (char *)malloc(outlen + 1);
            int cursorPos = 0;
            
            if (outbuf && typeform && spacing && outputPos && inputPos && 
                inputHyphens && outputHyphens) {
                memset(typeform, 0, outlen * sizeof(formtype));
                memset(spacing, 0, inlen * sizeof(char));
                memset(inputHyphens, 0, inlen + 1);
                memset(outputHyphens, 0, outlen + 1);
                lou_translatePrehyphenated("en-us-g2.ctb", inbuf, &inlen, outbuf, &outlen,
                                          typeform, spacing, outputPos, inputPos, &cursorPos,
                                          inputHyphens, outputHyphens, 0);
            }
            
            free(inbuf);
            free(outbuf);
            free(typeform);
            free(spacing);
            free(outputPos);
            free(inputPos);
            free(inputHyphens);
            free(outputHyphens);
            break;
        }
        case 5: { /* lou_translateString */
            if (fuzz_size < 1) break;
            size_t max_inlen = fuzz_size / sizeof(widechar);
            if (max_inlen < 1) max_inlen = 1;
            int inlen = (int)(fuzz_size % max_inlen);
            if (inlen < 0) inlen = 0;
            int outlen = inlen * 2;
            if (outlen < 10) outlen = 10;
            
            widechar *inbuf = (widechar *)malloc(max_inlen * sizeof(widechar));
            if (!inbuf) break;
            for (size_t i = 0; i < max_inlen && i * sizeof(widechar) < fuzz_size; i++) {
                inbuf[i] = (widechar)(fuzz_data[i * sizeof(widechar) % fuzz_size]);
            }
            
            widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
            formtype *typeform = (formtype *)malloc(inlen * sizeof(formtype));
            char *spacing = (char *)malloc((inlen + 1) * sizeof(char));
            
            if (outbuf && typeform && spacing) {
                memset(typeform, 0, inlen * sizeof(formtype));
                memset(spacing, 0, (inlen + 1) * sizeof(char));
                lou_translateString("en-us-g2.ctb", inbuf, &inlen, outbuf, &outlen,
                                   typeform, spacing, 0);
            }
            
            free(inbuf);
            free(outbuf);
            free(typeform);
            free(spacing);
            break;
        }
    }

    return 0;
}