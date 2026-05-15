// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::word_boundary at re2.h:729:10 in re2.h
// re2::RE2::Options::perl_classes at re2.h:726:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::word_boundary at re2.h:729:10 in re2.h
// re2::RE2::Options::perl_classes at re2.h:726:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::word_boundary at re2.h:729:10 in re2.h
// re2::RE2::Options::perl_classes at re2.h:726:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::word_boundary at re2.h:729:10 in re2.h
// re2::RE2::Options::perl_classes at re2.h:726:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::word_boundary at re2.h:729:10 in re2.h
// re2::RE2::Options::perl_classes at re2.h:726:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::word_boundary at re2.h:729:10 in re2.h
// re2::RE2::Options::perl_classes at re2.h:726:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Create an Options object with default constructor
    re2::RE2::Options options;
    
    // Use fuzzing data to set boolean values for various options
    // We'll use the first 6 bytes to determine boolean values for setters
    // If we have at least 6 bytes, use them; otherwise use defaults or partial data
    
    // Byte 0: literal
    if (Size > 0) {
        options.set_literal(Data[0] & 1);
    }
    
    // Byte 1: case_sensitive
    if (Size > 1) {
        options.set_case_sensitive(Data[1] & 1);
    }
    
    // Byte 2: word_boundary
    if (Size > 2) {
        options.set_word_boundary(Data[2] & 1);
    }
    
    // Byte 3: perl_classes
    if (Size > 3) {
        options.set_perl_classes(Data[3] & 1);
    }
    
    // Call getters to check current settings
    // These calls exercise the getter functions
    bool word_boundary = options.word_boundary();
    bool perl_classes = options.perl_classes();
    
    // Use remaining bytes to create more diverse test scenarios
    // We'll create multiple Options objects with different settings
    if (Size > 4) {
        // Create a second Options object with different settings
        re2::RE2::Options options2;
        
        // Use different bytes for different settings
        if (Size > 4) {
            options2.set_literal(Data[4] & 1);
        }
        if (Size > 5) {
            options2.set_case_sensitive(Data[5] & 1);
        }
        if (Size > 6) {
            options2.set_word_boundary(Data[6] & 1);
        }
        if (Size > 7) {
            options2.set_perl_classes(Data[7] & 1);
        }
        
        // Exercise getters on the second object
        bool word_boundary2 = options2.word_boundary();
        bool perl_classes2 = options2.perl_classes();
        
        // Test with alternating settings using remaining bytes
        for (size_t i = 8; i + 3 < Size && i < 20; i += 4) {
            re2::RE2::Options temp_options;
            
            temp_options.set_literal(Data[i] & 1);
            temp_options.set_case_sensitive(Data[i+1] & 1);
            temp_options.set_word_boundary(Data[i+2] & 1);
            temp_options.set_perl_classes(Data[i+3] & 1);
            
            // Exercise getters
            bool temp_word_boundary = temp_options.word_boundary();
            bool temp_perl_classes = temp_options.perl_classes();
            
            // Do something with the values to prevent optimization
            if (temp_word_boundary && temp_perl_classes) {
                // This is just to use the values
            }
        }
    }
    
    // Test edge cases by explicitly setting specific combinations
    // Test all false
    re2::RE2::Options options_false;
    options_false.set_literal(false);
    options_false.set_case_sensitive(false);
    options_false.set_word_boundary(false);
    options_false.set_perl_classes(false);
    
    bool wb_false = options_false.word_boundary();
    bool pc_false = options_false.perl_classes();
    
    // Test all true
    re2::RE2::Options options_true;
    options_true.set_literal(true);
    options_true.set_case_sensitive(true);
    options_true.set_word_boundary(true);
    options_true.set_perl_classes(true);
    
    bool wb_true = options_true.word_boundary();
    bool pc_true = options_true.perl_classes();
    
    // Test mixed settings
    re2::RE2::Options options_mixed;
    options_mixed.set_literal(true);
    options_mixed.set_case_sensitive(false);
    options_mixed.set_word_boundary(true);
    options_mixed.set_perl_classes(false);
    
    bool wb_mixed = options_mixed.word_boundary();
    bool pc_mixed = options_mixed.perl_classes();
    
    // Use the values to prevent optimization
    if (word_boundary && perl_classes && wb_false && pc_false && wb_true && pc_true && wb_mixed && pc_mixed) {
        // This is just to use the values
    }
    
    return 0;
}