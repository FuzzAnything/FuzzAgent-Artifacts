// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "liblouis.h"

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

static void init_buffers(const uint8_t *data, size_t size, 
                         widechar **inbuf, int *inlen,
                         widechar **outbuf, int *outlen,
                         formtype **typeform, char **spacing,
                         int **outputPos, int **inputPos, int *cursorPos) {
    // Limit input size
    size_t actual_size = size > MAX_INPUT_SIZE ? MAX_INPUT_SIZE : size;
    
    // Allocate input buffer (widechar)
    *inlen = (int)actual_size;
    *inbuf = (widechar *)malloc((*inlen + 1) * sizeof(widechar));
    if (*inbuf) {
        for (size_t i = 0; i < actual_size; i++) {
            (*inbuf)[i] = (widechar)data[i];
        }
        (*inbuf)[actual_size] = 0; // Null terminator
    }
    
    // Allocate output buffer
    *outlen = MAX_OUTPUT_SIZE;
    *outbuf = (widechar *)malloc(*outlen * sizeof(widechar));
    
    // Allocate typeform array (size based on output buffer)
    *typeform = (formtype *)malloc(*outlen * sizeof(formtype));
    if (*typeform) {
        memset(*typeform, 0, *outlen * sizeof(formtype));
    }
    
    // Allocate spacing array (size based on output buffer)
    *spacing = (char *)malloc(*outlen * sizeof(char));
    if (*spacing) {
        memset(*spacing, 0, *outlen * sizeof(char));
    }
    
    // Allocate position arrays
    *outputPos = (int *)malloc(*inlen * sizeof(int));
    *inputPos = (int *)malloc(*outlen * sizeof(int));
    
    // Initialize cursor position
    *cursorPos = 0;
}

static void free_buffers(widechar *inbuf, widechar *outbuf,
                         formtype *typeform, char *spacing,
                         int *outputPos, int *inputPos) {
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
}

int LLVMFuzzerTestOneInput_5(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Initialize library
    lou_setDataPath(".");
    
    // Common variables
    const char *tableList = "en-us-g2.ctb";
    widechar *inbuf = NULL, *outbuf = NULL;
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL, *inputPos = NULL;
    int cursorPos = 0;
    int inlen = 0, outlen = 0;
    int mode = data[0] % 256;
    
    // Initialize buffers
    init_buffers(data, size, &inbuf, &inlen, &outbuf, &outlen,
                 &typeform, &spacing, &outputPos, &inputPos, &cursorPos);
    
    if (!inbuf || !outbuf) {
        free_buffers(inbuf, outbuf, typeform, spacing, outputPos, inputPos);
        return 0;
    }
    
    // Test lou_translateString
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_translateString(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                           typeform, spacing, mode);
    }
    
    // Test lou_backTranslateString
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_backTranslateString(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                               typeform, spacing, mode);
    }
    
    // Test lou_backTranslate
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_backTranslate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                         typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    }
    
    // Test lou_translate
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_translate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                     typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    }
    
    // Test lou_getTypeformForEmphClass
    {
        const char *emphClass = "italic";
        lou_getTypeformForEmphClass(tableList, emphClass);
    }
    
    // Test lou_compileString
    {
        // Create a dummy table file first
        int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            const char *table_content = "include en-us-g2.ctb\n";
            write(fd, table_content, strlen(table_content));
            close(fd);
            
            const char *compile_string = "dots 123456";
            lou_compileString("./dummy_file", compile_string);
            
            // Clean up
            unlink("./dummy_file");
        }
    }
    
    // Clean up
    free_buffers(inbuf, outbuf, typeform, spacing, outputPos, inputPos);
    
    return 0;
}