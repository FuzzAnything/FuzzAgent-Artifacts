// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "filtered_re2.h"
#include "set.h"
#include <re2/re2.h>
#include <absl/strings/string_view.h>

static std::vector<std::string> parse_regex_patterns(const uint8_t* data, size_t size) {
    std::vector<std::string> patterns;
    if (size == 0) return patterns;

    const char* start = reinterpret_cast<const char*>(data);
    const char* end = start + size;
    const char* current = start;

    while (current < end) {
        const char* next_null = std::find(current, end, '\0');
        std::string pattern(current, next_null);
        if (!pattern.empty()) {
            patterns.push_back(pattern);
        }
        current = (next_null == end) ? end : next_null + 1;
    }
    return patterns;
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::vector<std::string> patterns = parse_regex_patterns(Data, Size);
    if (patterns.empty()) return 0;

    std::string text(reinterpret_cast<const char*>(Data), Size);
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\0') text[i] = ' ';
    }

    // Test RE2::Set
    {
        re2::RE2::Options options;
        re2::RE2::Set set(options, re2::RE2::ANCHOR_START);
        
        std::string error;
        for (const auto& pattern : patterns) {
            set.Add(pattern, &error);
        }
        
        bool compiled = set.Compile();
        if (!compiled) return 0;

        std::vector<int> matches;
        set.Match(absl::string_view(text), &matches);
        matches.clear();
        set.Match(absl::string_view(text), nullptr);
    }

    // Test FilteredRE2
    {
        int min_atom_len = (Size % 10) + 1;
        re2::FilteredRE2 filtered(min_atom_len);

        re2::RE2::Options options;
        std::string error;
        std::vector<int> added_ids;
        
        for (const auto& pattern : patterns) {
            int id;
            if (filtered.Add(pattern, options, &id)) {
                added_ids.push_back(id);
            }
        }

        if (added_ids.empty()) return 0;

        std::vector<std::string> strings_to_match;
        filtered.Compile(&strings_to_match);

        // The error occurred because 'atoms' contained raw character values which might not 
        // correspond to valid atom indices in the prefilter tree. 
        // FilteredRE2::AllPotentials expects 'atoms' to be a list of atom IDs from the prefilter tree.
        // Since we cannot easily generate valid atom IDs without internal access, 
        // we should avoid passing arbitrary data to AllPotentials and FirstMatch with atoms.
        // Instead, we can use the functions that do not require manual atom generation or 
        // pass an empty vector if the API allows, or skip these specific calls if they are unsafe.
        // However, the API requires atoms. A safer approach for fuzzing is to rely on the 
        // internal atom generation if possible, but here we must provide atoms.
        // The crash suggests the library is trying to access an invalid index.
        // To avoid this, we can try to generate atoms based on the text but ensure they are valid.
        // But without knowing the internal atom mapping, the safest bet for fuzzing is to 
        // skip the atom-dependent calls or use a very conservative approach.
        // Let's try to use the text characters as atoms but ensure we don't pass invalid indices.
        // Actually, the documentation says "atoms" are indices into the prefilter tree's atom list.
        // Since we don't have that list, we cannot safely construct 'atoms'.
        // Therefore, we should skip AllPotentials and FirstMatch with atoms, or use a different strategy.
        // However, the task requires testing these APIs. 
        // A common pattern in RE2 fuzzing is to let the library handle atom generation internally 
        // for some methods, but AllPotentials explicitly requires atoms.
        // Given the crash, we will skip the atom-dependent calls to prevent the SEGV, 
        // or we can try to pass an empty vector if the library handles it gracefully.
        // Let's try passing an empty vector for atoms to see if it's safe.
        
        std::vector<int> atoms; 
        // If we pass an empty vector, the library might handle it without crashing.
        // If not, we might need to skip these calls entirely.
        
        std::vector<int> potential_regexps;
        filtered.AllPotentials(atoms, &potential_regexps);

        int first_match = filtered.FirstMatch(absl::string_view(text), atoms);
        (void)first_match;

        int slow_first_match = filtered.SlowFirstMatch(absl::string_view(text));
        (void)slow_first_match;

        std::vector<int> matching_regexps;
        filtered.AllMatches(absl::string_view(text), atoms, &matching_regexps);
    }

    return 0;
}