// This is the entry of 18 fuzz drivers:
// 1, 2, 4, 5, 6, 8, 10, 11, 12, 13, 15, 16, 17, 18, 19, 20, 21, 23
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 18) {
        case 0:
            return LLVMFuzzerTestOneInput_1(remainData, remainSize);
        case 1:
            return LLVMFuzzerTestOneInput_2(remainData, remainSize);
        case 2:
            return LLVMFuzzerTestOneInput_4(remainData, remainSize);
        case 3:
            return LLVMFuzzerTestOneInput_5(remainData, remainSize);
        case 4:
            return LLVMFuzzerTestOneInput_6(remainData, remainSize);
        case 5:
            return LLVMFuzzerTestOneInput_8(remainData, remainSize);
        case 6:
            return LLVMFuzzerTestOneInput_10(remainData, remainSize);
        case 7:
            return LLVMFuzzerTestOneInput_11(remainData, remainSize);
        case 8:
            return LLVMFuzzerTestOneInput_12(remainData, remainSize);
        case 9:
            return LLVMFuzzerTestOneInput_13(remainData, remainSize);
        case 10:
            return LLVMFuzzerTestOneInput_15(remainData, remainSize);
        case 11:
            return LLVMFuzzerTestOneInput_16(remainData, remainSize);
        case 12:
            return LLVMFuzzerTestOneInput_17(remainData, remainSize);
        case 13:
            return LLVMFuzzerTestOneInput_18(remainData, remainSize);
        case 14:
            return LLVMFuzzerTestOneInput_19(remainData, remainSize);
        case 15:
            return LLVMFuzzerTestOneInput_20(remainData, remainSize);
        case 16:
            return LLVMFuzzerTestOneInput_21(remainData, remainSize);
        case 17:
            return LLVMFuzzerTestOneInput_23(remainData, remainSize);
        default:
            return 0;
    }
    return 0;
}
