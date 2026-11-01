// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_nametoport at nametoaddr.c:304:1 in namedb.h
// pcap_nametollc at nametoaddr.c:609:1 in namedb.h
// pcap_nametoproto at nametoaddr.c:475:1 in namedb.h
// pcap_nametoeproto at nametoaddr.c:585:1 in namedb.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_nametoportrange at nametoaddr.c:437:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <namedb.h>

static int fuzzer_test_pcap_nametoport(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';

    int port = 0;
    int proto = 0;
    int ret = pcap_nametoport(str, &port, &proto);
    (void)ret;
    free(str);
    return 0;
}

static int fuzzer_test_pcap_nametollc(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';

    int ret = pcap_nametollc(str);
    (void)ret;
    free(str);
    return 0;
}

static int fuzzer_test_pcap_nametoproto(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';

    int ret = pcap_nametoproto(str);
    (void)ret;
    free(str);
    return 0;
}

static int fuzzer_test_pcap_nametoeproto(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';

    int ret = pcap_nametoeproto(str);
    (void)ret;
    free(str);
    return 0;
}

static int fuzzer_test_pcap_tstamp_type_name_to_val(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';

    int ret = pcap_tstamp_type_name_to_val(str);
    (void)ret;
    free(str);
    return 0;
}

static int fuzzer_test_pcap_nametoportrange(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';

    int port1 = 0;
    int port2 = 0;
    int proto = 0;
    int ret = pcap_nametoportrange(str, &port1, &port2, &proto);
    (void)ret;
    free(str);
    return 0;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzzer_test_pcap_nametoport(Data, Size);
    fuzzer_test_pcap_nametollc(Data, Size);
    fuzzer_test_pcap_nametoproto(Data, Size);
    fuzzer_test_pcap_nametoeproto(Data, Size);
    fuzzer_test_pcap_tstamp_type_name_to_val(Data, Size);
    fuzzer_test_pcap_nametoportrange(Data, Size);

    return 0;
}