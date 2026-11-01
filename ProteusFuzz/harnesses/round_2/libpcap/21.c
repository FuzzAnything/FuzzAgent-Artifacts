// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_nametollc at nametoaddr.c:609:1 in namedb.h
// pcap_nametoportrange at nametoaddr.c:437:1 in namedb.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_nametoproto at nametoaddr.c:475:1 in namedb.h
// pcap_nametoeproto at nametoaddr.c:585:1 in namedb.h
// pcap_nametoport at nametoaddr.c:304:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <namedb.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static int fuzz_pcap_nametollc(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    // Ensure null termination
    char *str = malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';
    
    int result = pcap_nametollc(str);
    (void)result; // Use result to prevent unused variable warning
    
    free(str);
    return 0;
}

static int fuzz_pcap_nametoportrange(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';
    
    int start_port, end_port, proto;
    int result = pcap_nametoportrange(str, &start_port, &end_port, &proto);
    (void)result;
    
    free(str);
    return 0;
}

static int fuzz_pcap_tstamp_type_name_to_val(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';
    
    int result = pcap_tstamp_type_name_to_val(str);
    (void)result;
    
    free(str);
    return 0;
}

static int fuzz_pcap_nametoproto(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';
    
    int result = pcap_nametoproto(str);
    (void)result;
    
    free(str);
    return 0;
}

static int fuzz_pcap_nametoeproto(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';
    
    int result = pcap_nametoeproto(str);
    (void)result;
    
    free(str);
    return 0;
}

static int fuzz_pcap_nametoport(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = malloc(size + 1);
    if (!str) return 0;
    memcpy(str, data, size);
    str[size] = '\0';
    
    int port, proto;
    int result = pcap_nametoport(str, &port, &proto);
    (void)result;
    
    free(str);
    return 0;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Split input into chunks for different functions if needed, 
    // or just pass the whole buffer to each function.
    // Here we pass the whole buffer to each function to maximize coverage.
    
    fuzz_pcap_nametollc(Data, Size);
    fuzz_pcap_nametoportrange(Data, Size);
    fuzz_pcap_tstamp_type_name_to_val(Data, Size);
    fuzz_pcap_nametoproto(Data, Size);
    fuzz_pcap_nametoeproto(Data, Size);
    fuzz_pcap_nametoport(Data, Size);
    
    return 0;
}