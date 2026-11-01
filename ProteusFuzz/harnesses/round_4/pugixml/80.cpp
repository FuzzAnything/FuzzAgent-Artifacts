// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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
#include <functional>

// Helper function to parse input data into an XML document
static pugi::xml_document parse_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    // load_buffer returns xml_parse_result, we ignore it here as we just need the doc state
    // even if parsing fails, the doc object is valid (empty)
    doc.load_buffer(data, size);
    return doc;
}

// Predicate for find_attribute that checks attribute name
struct NamePredicate {
    std::string target_name;
    bool operator()(const pugi::xml_attribute& attr) const {
        if (!attr) return false;
        return std::string(attr.name()) == target_name;
    }
};

// Predicate for find_attribute that checks attribute value
struct ValuePredicate {
    std::string target_value;
    bool operator()(const pugi::xml_attribute& attr) const {
        if (!attr) return false;
        return std::string(attr.value()) == target_value;
    }
};

// Predicate for find_attribute that always returns true (to get first)
struct AlwaysTruePredicate {
    bool operator()(const pugi::xml_attribute& attr) const {
        return attr; // Returns true if attribute is valid
    }
};

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc = parse_input(Data, Size);
    
    // Get the root node
    pugi::xml_node root = doc.root();
    
    // If no root, create a dummy one to test node methods on a valid node
    if (!root) {
        root = doc.append_child("root");
    }

    // Ensure we have some attributes to work with for insert_before/after
    // Add a few dummy attributes if none exist
    if (!root.first_attribute()) {
        root.append_attribute("attr1");
        root.append_attribute("attr2");
    }

    // 1. Test pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    (void)first_attr; // Use to avoid unused variable warning

    // 2. Test pugi::xml_node::last_attribute
    pugi::xml_attribute last_attr = root.last_attribute();
    (void)last_attr;

    // 3. Test pugi::xml_node::attribute
    // Use a name derived from input data to vary the search
    std::string name_str(reinterpret_cast<const char*>(Data), Size);
    // Sanitize name to be a valid XML attribute name (simple check)
    if (!name_str.empty() && name_str[0] >= 'a' && name_str[0] <= 'z') {
        pugi::xml_attribute found_attr = root.attribute(name_str.c_str());
        (void)found_attr;
    } else {
        // Fallback to a known attribute name if input is not a valid name
        pugi::xml_attribute found_attr = root.attribute("attr1");
        (void)found_attr;
    }

    // 4. Test pugi::xml_node::insert_attribute_before
    // We need a reference attribute. Use first_attr if valid, else create one.
    pugi::xml_attribute ref_attr = root.first_attribute();
    if (!ref_attr) {
        ref_attr = root.append_attribute("temp_ref");
    }
    
    // Create a name for the new attribute
    std::string new_name = "new_attr_before";
    if (Size > 0) {
        new_name[0] = static_cast<char>(Data[0] % 26 + 'a');
    }
    
    pugi::xml_attribute new_attr_before = root.insert_attribute_before(new_name.c_str(), ref_attr);
    (void)new_attr_before;

    // 5. Test pugi::xml_node::insert_attribute_after
    pugi::xml_attribute new_attr_after = root.insert_attribute_after(new_name.c_str(), ref_attr);
    (void)new_attr_after;

    // 6. Test pugi::xml_node::find_attribute
    // Test with a name predicate
    NamePredicate name_pred;
    name_pred.target_name = "attr1";
    pugi::xml_attribute found_by_name = root.find_attribute(name_pred);
    (void)found_by_name;

    // Test with a value predicate
    ValuePredicate val_pred;
    val_pred.target_value = ""; // Empty value is common
    pugi::xml_attribute found_by_val = root.find_attribute(val_pred);
    (void)found_by_val;

    // Test with always true predicate
    AlwaysTruePredicate always_pred;
    pugi::xml_attribute found_any = root.find_attribute(always_pred);
    (void)found_any;

    // Additional edge cases:
    // Try inserting before/after an invalid attribute
    pugi::xml_attribute invalid_attr; // Default constructed, empty
    pugi::xml_attribute result_invalid_before = root.insert_attribute_before("invalid_test", invalid_attr);
    (void)result_invalid_before;

    pugi::xml_attribute result_invalid_after = root.insert_attribute_after("invalid_test", invalid_attr);
    (void)result_invalid_after;

    // Try accessing attributes on a null node
    pugi::xml_node null_node;
    pugi::xml_attribute null_first = null_node.first_attribute();
    (void)null_first;
    
    pugi::xml_attribute null_last = null_node.last_attribute();
    (void)null_last;

    pugi::xml_attribute null_attr = null_node.attribute("test");
    (void)null_attr;

    return 0;
}