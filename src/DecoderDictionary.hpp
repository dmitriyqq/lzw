#pragma once

#include <string_view>
#include <memory>
#include <unordered_map>
#include <vector>
#include <climits>
#include <iostream>
#include "BaseDictionary.hpp"

class DecoderDictionary : public BaseDictionary {
    std::vector<std::string> m_data;


    void insert(char c, uint32_t i) override {
        insert(std::string(1, c));
    }
public:
    DecoderDictionary() {
        initializeDictionary();
    }
    
    void insert(const std::string &str) {
        m_data.emplace_back(str);
    }

    void update(uint32_t code, char c) {
        m_data[code] += c;
    }

    const std::string get(uint32_t code) {
        return m_data[code];
    }

    size_t size() const {
        return m_data.size();
    }
};