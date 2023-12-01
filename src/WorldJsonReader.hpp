#pragma once
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <fstream>
#include <string>

class WorldJsonReader {
public:

    ~WorldJsonReader() {
        close();
    }

    void close() {
        json_stream_read.close();
    }

    bool open(const std::string &path) {
        json_stream_read.open(path);

        return json_stream_read.good();
    }

    bool init() {
        return reader.parse(json_stream_read, actualJson);
    }

    // std::pair ... first arg hexadecimal color
    // 
    //const std::pair<long long, int> getColorLayer() {
        
    //}
    
private:
    std::ifstream json_stream_read;

    Json::Value actualJson;
    Json::Reader reader;
};
