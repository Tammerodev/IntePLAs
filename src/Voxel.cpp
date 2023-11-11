#include "Voxel.hpp"

const Voxel getValueFromCol(const sf::Color &px, sf::Vector2i p) {
    Voxel vox = Voxel();
    vox.value = px.a != 0;
    if(vox.value == 0) return vox;

    if(px == elm::Carbon) {
        vox.value = 2;
        vox.strenght = 8;
    } else if(px == elm::Lithium) {
        vox.value = 3;
        vox.strenght = 2;
    } else if(px == elm::Magnesium) {
        vox.value = 4;
        vox.strenght = 10;
    } else if(px == elm::Sodium) {
        vox.value = 5;
        vox.strenght = 1;
    } else if(px == elm::Aluminium) {
        vox.value = 6;
        vox.strenght = 5;
    } else if(px == elm::Silicon) {
        vox.value = 7;
        vox.strenght = 6;
    } else if(px == elm::Copper) {
        vox.value = 8;
        vox.strenght = 10;
    } else if(px == elm::Titanium) {
        vox.value = 9;
        vox.strenght = 100;
    } else if(px == elm::Lead) {
        vox.value = 10;
        vox.strenght = 3;
    } else if(px == elm::Water) {
        vox.value = 11;
        vox.strenght = 255;
    }

    return vox;
}
