#pragma once

class Voxel {
    public:
    
    unsigned char value = 0;
    long strenght = 1;
    bool used = false;
    bool isFalling = false;
    float maxTemp = 1000.f;
    float temp = .0f;
    float ambientDissipation = 0.1;
    
    private:
};