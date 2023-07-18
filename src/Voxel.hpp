#pragma once

class Voxel {
    public:
    
    unsigned char value = 0;
    unsigned char strenght = 1;
    bool used = false;
    bool isFalling = false;
    unsigned short maxTemp = 1000.f;
    short temp = .0f;
    char ambientDissipation = 1;
    
    private:
};