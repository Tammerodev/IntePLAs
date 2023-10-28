#pragma once

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <string>

class JsonReader {
public:
    bool open(const std::string &path) {
        json_stream_read.open(path);

        return json_stream_read.good();
    }

    bool init() {
        reader.parse(json_stream_read, actualJson);
    }

    const std::string readParameterAsString(const std::string& param) {
        return actualJson[param].asString();
    }
    
private:
    std::ifstream json_stream_read;

    Json::Value actualJson;
    Json::Reader reader;
};