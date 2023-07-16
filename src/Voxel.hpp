#pragma once

class Voxel {
    public:
    
    unsigned char value = 0;
    long strenght = 1;
    bool used = false;
    bool isFalling = false;
    float maxTemp = 1000.f;
    float temp = .0f;
    int64_t ambientDissipation = 1;
    
    private:
};