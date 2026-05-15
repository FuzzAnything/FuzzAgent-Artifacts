#include <fuzzer/FuzzedDataProvider.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <liblouis.h>

namespace harness_000 {
/*
 * Fuzzing harness for liblouis translation APIs
 * Targets: lou_translateString, lou_translate, lou_backTranslateString, lou_backTranslate
 */



// Mode constants (from liblouis.h)
#define ucBrl 0
#define compbrlAtCursor 1
#define dotsIO 2
#define noContractions 8

// Global initialization flag
static int initialized = 0;

// Log callback to avoid excessive logging during fuzzing
void avoid_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Cleanup resources on exit
static void __attribute__((destructor)) free_resources(void) {
    lou_free();
}

// Helper function to check if table exists
static int check_table_exists(const char* table_name) {
    // Simple check - in real implementation would use lou_checkTable
    // For now, return 1 for known tables
    if (strstr(table_name, ".ctb") || strstr(table_name, ".utb")) {
        return 1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
    }

    // Consume operation type to decide which API to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume mode parameter (translation mode)
    int mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Choose a translation table from a small set
    const char* tables[] = {
        "en-us-g2.ctb",
        "en-us-g1.ctb", 
        "en-ueb-g2.ctb",
        "en-ueb-g1.ctb",
        "de-de-g2.ctb",
        "fr-bfu-g2.ctb"
    };
    uint8_t table_idx = fdp.ConsumeIntegral<uint8_t>() % 6;
    const char* table = tables[table_idx];
    
    // Check if table exists before proceeding
    if (!check_table_exists(table)) {
        return 0;
    }
    
    // Consume input string/text (limited to reasonable size)
    size_t max_input_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string input_text = fdp.ConsumeRandomLengthString(max_input_len);
    
    if (input_text.empty()) {
        return 0;
    }
    
    // Convert input string to widechar format
    // Simple approach: copy characters directly
    size_t widechar_len = input_text.length();
    widechar* inbuf = (widechar*)malloc((widechar_len + 1) * sizeof(widechar));
    if (!inbuf) {
        return 0;
    }
    
    for (size_t i = 0; i < widechar_len; i++) {
        inbuf[i] = (widechar)(unsigned char)input_text[i];
    }
    inbuf[widechar_len] = 0;
    
    // Prepare output buffer (allocate larger for expansion)
    int outlen = widechar_len * 16;  // Reasonable maximum expansion
    widechar* outbuf = (widechar*)malloc((outlen + 1) * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }
    
    // Prepare other parameters
    int inlen = (int)widechar_len;
    int actual_outlen = outlen;
    
    formtype* typeform = NULL;
    char* spacing = NULL;
    int* outputPos = NULL;
    int* inputPos = NULL;
    int* cursorPos = NULL;
    
    // For some operations, allocate position arrays
    if (operation == 1 || operation == 3) { // lou_translate or lou_backTranslate
        outputPos = (int*)malloc(inlen * sizeof(int));
        inputPos = (int*)malloc(outlen * sizeof(int));
        cursorPos = (int*)malloc(sizeof(int));
        if (!outputPos || !inputPos || !cursorPos) {
            free(inbuf);
            free(outbuf);
            if (outputPos) free(outputPos);
            if (inputPos) free(inputPos);
            if (cursorPos) free(cursorPos);
            return 0;
        }
        // Initialize arrays
        for (int i = 0; i < inlen; i++) {
            outputPos[i] = 0;
        }
        for (int i = 0; i < outlen; i++) {
            inputPos[i] = 0;
        }
        *cursorPos = 0;
    }
    
    int result = 0;
    
    // Execute the selected translation operation
    switch (operation) {
        case 0: // lou_translateString
            result = lou_translateString(table, inbuf, &inlen, outbuf, &actual_outlen, 
                                        typeform, spacing, mode);
            break;
            
        case 1: // lou_translate
            result = lou_translate(table, inbuf, &inlen, outbuf, &actual_outlen,
                                  typeform, spacing, outputPos, inputPos, cursorPos, mode);
            break;
            
        case 2: // lou_backTranslateString
            result = lou_backTranslateString(table, inbuf, &inlen, outbuf, &actual_outlen,
                                           typeform, spacing, mode);
            break;
            
        case 3: // lou_backTranslate
            result = lou_backTranslate(table, inbuf, &inlen, outbuf, &actual_outlen,
                                     typeform, spacing, outputPos, inputPos, cursorPos, mode);
            break;
    }
    
    // Clean up
    free(inbuf);
    free(outbuf);
    if (outputPos) free(outputPos);
    if (inputPos) free(inputPos);
    if (cursorPos) free(cursorPos);
    
    // Return 0 regardless of result to continue fuzzing
    return 0;
}

} // namespace harness_000
