#include "ElectricitySimulation.hpp"

void ElectricitySimulation::load() {
    
}

void ElectricitySimulation::addElement(const std::shared_ptr<ElectricComponent> component) {
    components.push_back(component);
}

unsigned int i = 0;

std::list<sf::Vector2i> pos;

void floodFill(ChunkIndexer& wr, int x, int y, int currColor, int newColor, int voltage) 
{ 
    i++;
    // Base cases 
    if (wr.boundGetVoxelAt(x,y).value != currColor) 
        return; 
    if (wr.boundGetVoxelAt(x,y).value == newColor) 
        return; 

    sf::Vector2i bound_pos = wr.getBoundedVector(sf::Vector2i(x, y));
    Voxel &voxel = wr.getVoxelAt(bound_pos.x, bound_pos.y);
    
  
    // Replace the color at cell (x, y) 
    voxel.value = newColor; 

    if(voltage > 0) 
        voxel.voltage = voltage; 

    sf::Color col = wr.getImagePixelAt(bound_pos.x, bound_pos.y);

    wr.setImagePixelAt(bound_pos.x, bound_pos.y, sf::Color(col.r, col.g, voltage));

    pos.emplace_back(sf::Vector2f(x, y));
  
    // Recursively call for north, east, south and west 
    floodFill(wr, x + 1, y, currColor, newColor, voltage); 
    floodFill(wr, x - 1, y, currColor, newColor, voltage); 
    floodFill(wr, x, y+1, currColor, newColor, voltage); 
    floodFill(wr, x, y-1, currColor, newColor, voltage); 
}

void ElectricitySimulation::update(ChunkIndexer& world) {
    // Update debug info

    i = 0;
    debug_globals::electricity_source_count = ElectricityGlobal::source_positions.size();
    debug_globals::electricity_component_count = components.size(); 

    for(auto &component : components) {
        component->update(world);
    }

    ElectricityGlobal::check_sources(world);

    for(const auto& source : ElectricityGlobal::source_positions) {
        if(source.second != 0) {

            for(auto &p : pos) {
                Voxel &voxel = world.boundGetVoxelAt(p.x, p.y);
                voxel.value = VoxelValues::COPPER;
                voxel.voltage = 0;
                world.setImagePixelAt(p.x, p.y, elm::getInfoFromType(VoxelValues::COPPER).color);
            }

            int current_val = (int)VoxelValues::COPPER;
            int new_val = (int)VoxelValues::URANIUM235;

            pos.clear();

            if(world.boundGetVoxelAt(source.first.x + 1, source.first.y).value == current_val)
                floodFill(world, source.first.x + 1, source.first.y, current_val, new_val, source.second);

            else if(world.boundGetVoxelAt(source.first.x - 1, source.first.y).value == current_val)
                floodFill(world, source.first.x - 1, source.first.y, current_val, new_val, source.second);

            else if(world.boundGetVoxelAt(source.first.x, source.first.y + 1).value == current_val)
                floodFill(world, source.first.x, source.first.y + 1, current_val, new_val, source.second);

            else if(world.boundGetVoxelAt(source.first.x, source.first.y - 1).value == current_val)
                floodFill(world, source.first.x, source.first.y - 1, current_val, new_val, source.second);
        }
    }
}
