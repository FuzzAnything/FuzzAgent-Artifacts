// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

#define MAX_INPUT_LEN 1024
#define MAX_OUTPUT_LEN 2048

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write dummy table file
    const char *table_content = 
        "# Dummy table for fuzzing\n"
        "noback context \\s 0\n"
        "always \\s 0\n"
        "a 1\n"
        "b 12\n"
        "c 14\n"
        "d 145\n"
        "e 15\n"
        "f 124\n"
        "g 1245\n"
        "h 125\n"
        "i 24\n"
        "j 245\n";
    
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(table_content, 1, strlen(table_content), fp);
    fclose(fp);
    
    const char *tableList = "./dummy_file";
    
    // Determine parameters from input data
    uint8_t mode_selector = Data[0];
    size_t data_offset = 1;
    
    // Extract input length (capped)
    int inlen = (Size - data_offset) / sizeof(widechar);
    if (inlen > MAX_INPUT_LEN) inlen = MAX_INPUT_LEN;
    if (inlen < 1) inlen = 1;
    
    // Prepare input buffer
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    if (!inbuf) return 0;
    
    size_t copy_size = (inlen * sizeof(widechar) < Size - data_offset) ? 
                       inlen * sizeof(widechar) : Size - data_offset;
    memcpy(inbuf, Data + data_offset, copy_size);
    
    // Prepare output buffer
    int outlen = MAX_OUTPUT_LEN;
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }
    
    // Prepare auxiliary arrays
    formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
    char *spacing = (char *)malloc(outlen * sizeof(char));
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int cursorPos = 0;
    char *hyphens = (char *)malloc((inlen + 1) * sizeof(char));
    char *inputHyphens = (char *)malloc(inlen * sizeof(char));
    char *outputHyphens = (char *)malloc(outlen * sizeof(char));
    
    // Initialize arrays
    if (typeform) memset(typeform, 0, outlen * sizeof(formtype));
    if (spacing) memset(spacing, 0, outlen * sizeof(char));
    if (outputPos) memset(outputPos, 0, inlen * sizeof(int));
    if (inputPos) memset(inputPos, 0, outlen * sizeof(int));
    if (hyphens) memset(hyphens, 0, (inlen + 1) * sizeof(char));
    if (inputHyphens) memset(inputHyphens, 0, inlen * sizeof(char));
    if (outputHyphens) memset(outputHyphens, 0, outlen * sizeof(char));
    
    // Test different functions based on mode selector
    int mode = mode_selector & 0x0F;
    int use_typeform = (mode_selector & 0x10) ? 1 : 0;
    int use_spacing = (mode_selector & 0x20) ? 1 : 0;
    int use_positions = (mode_selector & 0x40) ? 1 : 0;
    
    // 1. lou_translateString
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_translateString(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                           use_typeform ? typeform : NULL,
                           use_spacing ? spacing : NULL,
                           mode);
    }
    
    // 2. lou_translate
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_translate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                     use_typeform ? typeform : NULL,
                     use_spacing ? spacing : NULL,
                     use_positions ? outputPos : NULL,
                     use_positions ? inputPos : NULL,
                     use_positions ? &cursorPos : NULL,
                     mode);
    }
    
    // 3. lou_backTranslateString
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_backTranslateString(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                               use_typeform ? typeform : NULL,
                               use_spacing ? spacing : NULL,
                               mode);
    }
    
    // 4. lou_backTranslate
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_backTranslate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                         use_typeform ? typeform : NULL,
                         use_spacing ? spacing : NULL,
                         use_positions ? outputPos : NULL,
                         use_positions ? inputPos : NULL,
                         use_positions ? &cursorPos : NULL,
                         mode);
    }
    
    // 5. lou_translatePrehyphenated
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_translatePrehyphenated(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                                  use_typeform ? typeform : NULL,
                                  use_spacing ? spacing : NULL,
                                  use_positions ? outputPos : NULL,
                                  use_positions ? inputPos : NULL,
                                  use_positions ? &cursorPos : NULL,
                                  inputHyphens, outputHyphens, mode);
    }
    
    // 6. lou_hyphenate
    {
        lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
    }
    
    // Cleanup
    free(inbuf);
    free(outbuf);
    if (typeform) free(typeform);
    if (spacing) free(spacing);
    if (outputPos) free(outputPos);
    if (inputPos) free(inputPos);
    if (hyphens) free(hyphens);
    if (inputHyphens) free(inputHyphens);
    if (outputHyphens) free(outputHyphens);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}