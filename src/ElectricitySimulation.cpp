#include "ElectricitySimulation.hpp"

void ElectricitySimulation::load() {
    
}

void ElectricitySimulation::addElement(const std::shared_ptr<ElectricComponent> component) {
    components.push_back(component);
}

unsigned int i = 0;

void floodFill(ChunkIndexer& wr, int x, int y, int currColor, int newColor) 
{ 
    i++;
    // Base cases 
    if (wr.boundGetVoxelAt(x,y).value != currColor) 
        return; 
    if (wr.boundGetVoxelAt(x,y).value == newColor) 
        return; 
  
    // Replace the color at cell (x, y) 
    wr.boundGetVoxelAt(x,y).value = newColor; 
    wr.boundSetImagePixelAt(x,y, sf::Color(i / 4, i / 4, i / 4));
  
    // Recursively call for north, east, south and west 
    floodFill(wr, x+1, y, currColor, newColor); 
    floodFill(wr, x-1, y, currColor, newColor); 
    floodFill(wr, x, y+1, currColor, newColor); 
    floodFill(wr, x, y-1, currColor, newColor); 
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
            int current_val = (int)elm::ValCopper;
            int new_val = (int)elm::ValUranium235;

            if(world.boundGetVoxelAt(source.first.x + 1, source.first.y).value == current_val)
                floodFill(world, source.first.x + 1, source.first.y, current_val, new_val);
            
            else if(world.boundGetVoxelAt(source.first.x - 1, source.first.y).value == current_val)
                floodFill(world, source.first.x - 1, source.first.y, current_val, new_val);

            else if(world.boundGetVoxelAt(source.first.x, source.first.y + 1).value == current_val)
                floodFill(world, source.first.x, source.first.y + 1, current_val, new_val);

            else if(world.boundGetVoxelAt(source.first.x, source.first.y - 1).value == current_val)
                floodFill(world, source.first.x, source.first.y - 1, current_val, new_val);
        }
    }
}
