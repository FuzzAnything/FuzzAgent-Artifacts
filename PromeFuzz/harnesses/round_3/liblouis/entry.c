// This is the entry of 16 fuzz drivers:
// 2, 6, 8, 9, 10, 12, 13, 15, 16, 22, 24, 25, 27, 28, 29, 33
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);

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

    // Select driver
    switch (driverIndex % 16) {
        case 0:
            return LLVMFuzzerTestOneInput_2(remainData, remainSize);
        case 1:
            return LLVMFuzzerTestOneInput_6(remainData, remainSize);
        case 2:
            return LLVMFuzzerTestOneInput_8(remainData, remainSize);
        case 3:
            return LLVMFuzzerTestOneInput_9(remainData, remainSize);
        case 4:
            return LLVMFuzzerTestOneInput_10(remainData, remainSize);
        case 5:
            return LLVMFuzzerTestOneInput_12(remainData, remainSize);
        case 6:
            return LLVMFuzzerTestOneInput_13(remainData, remainSize);
        case 7:
            return LLVMFuzzerTestOneInput_15(remainData, remainSize);
        case 8:
            return LLVMFuzzerTestOneInput_16(remainData, remainSize);
        case 9:
            return LLVMFuzzerTestOneInput_22(remainData, remainSize);
        case 10:
            return LLVMFuzzerTestOneInput_24(remainData, remainSize);
        case 11:
            return LLVMFuzzerTestOneInput_25(remainData, remainSize);
        case 12:
            return LLVMFuzzerTestOneInput_27(remainData, remainSize);
        case 13:
            return LLVMFuzzerTestOneInput_28(remainData, remainSize);
        case 14:
            return LLVMFuzzerTestOneInput_29(remainData, remainSize);
        case 15:
            return LLVMFuzzerTestOneInput_33(remainData, remainSize);
        default:
            return 0;
    }
    return 0;
}
