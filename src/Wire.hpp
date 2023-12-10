class Wire : public ElectricComponent {
public:
    component_data update(ChunkIndexer& world) {
        int voltageSum = world.boundGetVoxelAt(x, y).voltage;
        voltageSum += world.boundGetVoxelAt(x + 1, y).voltage;
        voltageSum += world.boundGetVoxelAt(x - 1, y).voltage;

        voltage = voltageSum /= 2;

        world.boundSetImagePixelAt(x, y, sf::Color(elm::Copper.r, elm::Copper.g, elm::Copper.b + voltage));

        component_data data{};
        return data;
    }

    int getVoltage() {
        return voltage;
    }

    void setVoltage(int newVoltage) {
        voltage = newVoltage;
    }

    bool clear() {
        return false;
    }

private:
    int voltage = 0;
    int resistance = 3;
};
