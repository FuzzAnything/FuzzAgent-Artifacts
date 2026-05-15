// This is the entry of 13 fuzz drivers:
// 1, 6, 10, 14, 17, 29, 31, 35, 36, 42, 44, 49, 51
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 13) {
        case 0:
            return LLVMFuzzerTestOneInput_1(remainData, remainSize);
        case 1:
            return LLVMFuzzerTestOneInput_6(remainData, remainSize);
        case 2:
            return LLVMFuzzerTestOneInput_10(remainData, remainSize);
        case 3:
            return LLVMFuzzerTestOneInput_14(remainData, remainSize);
        case 4:
            return LLVMFuzzerTestOneInput_17(remainData, remainSize);
        case 5:
            return LLVMFuzzerTestOneInput_29(remainData, remainSize);
        case 6:
            return LLVMFuzzerTestOneInput_31(remainData, remainSize);
        case 7:
            return LLVMFuzzerTestOneInput_35(remainData, remainSize);
        case 8:
            return LLVMFuzzerTestOneInput_36(remainData, remainSize);
        case 9:
            return LLVMFuzzerTestOneInput_42(remainData, remainSize);
        case 10:
            return LLVMFuzzerTestOneInput_44(remainData, remainSize);
        case 11:
            return LLVMFuzzerTestOneInput_49(remainData, remainSize);
        case 12:
            return LLVMFuzzerTestOneInput_51(remainData, remainSize);
        default:
            return 0;
    }
    return 0;
}
