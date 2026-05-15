// This is the entry of 17 fuzz drivers:
// 1, 2, 5, 13, 15, 16, 23, 24, 28, 29, 34, 35, 39, 41, 43, 51, 55
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 17) {
        case 0:
            return LLVMFuzzerTestOneInput_1(remainData, remainSize);
        case 1:
            return LLVMFuzzerTestOneInput_2(remainData, remainSize);
        case 2:
            return LLVMFuzzerTestOneInput_5(remainData, remainSize);
        case 3:
            return LLVMFuzzerTestOneInput_13(remainData, remainSize);
        case 4:
            return LLVMFuzzerTestOneInput_15(remainData, remainSize);
        case 5:
            return LLVMFuzzerTestOneInput_16(remainData, remainSize);
        case 6:
            return LLVMFuzzerTestOneInput_23(remainData, remainSize);
        case 7:
            return LLVMFuzzerTestOneInput_24(remainData, remainSize);
        case 8:
            return LLVMFuzzerTestOneInput_28(remainData, remainSize);
        case 9:
            return LLVMFuzzerTestOneInput_29(remainData, remainSize);
        case 10:
            return LLVMFuzzerTestOneInput_34(remainData, remainSize);
        case 11:
            return LLVMFuzzerTestOneInput_35(remainData, remainSize);
        case 12:
            return LLVMFuzzerTestOneInput_39(remainData, remainSize);
        case 13:
            return LLVMFuzzerTestOneInput_41(remainData, remainSize);
        case 14:
            return LLVMFuzzerTestOneInput_43(remainData, remainSize);
        case 15:
            return LLVMFuzzerTestOneInput_51(remainData, remainSize);
        case 16:
            return LLVMFuzzerTestOneInput_55(remainData, remainSize);
        default:
            return 0;
    }
    return 0;
}
