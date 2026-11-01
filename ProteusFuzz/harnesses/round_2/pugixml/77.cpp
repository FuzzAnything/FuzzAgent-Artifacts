// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <algorithm>

static bool predicate_test(pugi::xml_attribute attr) {
    if (!attr) return false;
    const char* name = attr.name();
    if (!name) return false;
    // Simple predicate: return true if name starts with 'a' or 'b'
    return name[0] == 'a' || name[0] == 'b';
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a document
    pugi::xml_document doc;
    
    // Load XML from fuzz input
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    if (!result) {
        // Even if parsing fails partially, we can still test on the root or empty nodes
        // but we should ensure we don't crash on invalid structures.
    }

    // Get root node
    pugi::xml_node root = doc.root();

    // Test 1: first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    (void)first_attr; // Suppress unused warning

    // Test 2: last_attribute
    pugi::xml_attribute last_attr = root.last_attribute();
    (void)last_attr;

    // Test 4: attribute (by name)
    // Try to find an attribute named "id" or based on fuzz data if possible
    // Using a fixed name for safety, or constructing from data if valid
    std::string name_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure null termination for C-string safety if we were to use it directly, 
    // but attribute() takes const char_t*. Let's just use a safe static name or part of data.
    // To avoid buffer issues, let's just use a static name for the API call test.
    pugi::xml_attribute attr_by_name = root.attribute("test");
    (void)attr_by_name;

    // Test 3: find_attribute with predicate
    pugi::xml_attribute found_attr = root.find_attribute(predicate_test);
    (void)found_attr;

    // Test 2 & 5: insert_attribute_before and insert_attribute_after
    // We need a reference attribute to insert before/after.
    // If root has attributes, pick one. Otherwise, these will return empty (safe).
    pugi::xml_attribute ref_attr = root.first_attribute();
    
    // Prepare a name for the new attribute
    // We need a valid string_view_t. 
    // Let's create a temporary string from the fuzz data if it's printable, else default.
    std::string new_name = "fuzz_attr";
    if (Size > 0 && Data[0] != 0) {
        // Try to make a simple name from data, ensuring it's not too long
        size_t len = (Size < 10) ? Size : 10;
        new_name.assign(reinterpret_cast<const char*>(Data), len);
        // Ensure no nulls in the middle for safety if library expects C-string internally
        // pugixml string_view_t usually handles this, but let's be safe.
        new_name.erase(std::remove(new_name.begin(), new_name.end(), 0), new_name.end());
        if (new_name.empty()) new_name = "fuzz_attr";
    }

    std::string_view name_view = new_name;

    // Insert before
    pugi::xml_attribute new_attr_before = root.insert_attribute_before(name_view, ref_attr);
    (void)new_attr_before;

    // Insert after
    pugi::xml_attribute new_attr_after = root.insert_attribute_after(name_view, ref_attr);
    (void)new_attr_after;

    // Additional exploration: try on child nodes if any
    for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
        pugi::xml_attribute n_first = node.first_attribute();
        pugi::xml_attribute n_last = node.last_attribute();
        pugi::xml_attribute n_find = node.find_attribute(predicate_test);
        pugi::xml_attribute n_attr = node.attribute("test");
        
        pugi::xml_attribute n_ref = node.first_attribute();
        pugi::xml_attribute n_ins_before = node.insert_attribute_before(name_view, n_ref);
        pugi::xml_attribute n_ins_after = node.insert_attribute_after(name_view, n_ref);
        
        (void)n_first; (void)n_last; (void)n_find; (void)n_attr;
        (void)n_ins_before; (void)n_ins_after;
    }

    return 0;
}