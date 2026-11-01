// This is the entry of 5 fuzz drivers:
// 5, 46, 64
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);

// Entry function
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Driver selector
    if (Size < 1) {
        return 0;
    }
    const uint8_t *selector = Data;
    unsigned int driverIndex = 0;
    memcpy(&driverIndex, selector, 1);

    // Remain data and size
    const uint8_t *remainData = Data + 1;
    size_t remainSize = Size - 1;
    if (remainSize == 0) {
        return 0;
    }

    // The selector prefix can make remainData unaligned. Copy the payload to
    // malloc-aligned storage before passing it to generated C/C++ harnesses.
    uint8_t *alignedData = (uint8_t *)malloc(remainSize);
    if (alignedData == NULL) {
        return 0;
    }
    memcpy(alignedData, remainData, remainSize);

    // Select driver
    int result = 0;
    switch (driverIndex % 5) {
        case 0:
            result = LLVMFuzzerTestOneInput_5(alignedData, remainSize);
            break;
        case 1:
            result = LLVMFuzzerTestOneInput_46(alignedData, remainSize);
            break;
        case 2:
            result = LLVMFuzzerTestOneInput_64(alignedData, remainSize);
            break;
        case 3:
            result = LLVMFuzzerTestOneInput_65(alignedData, remainSize);
            break;
        case 4:
            result = LLVMFuzzerTestOneInput_66(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
