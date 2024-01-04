class Wire : public ElectricComponent {
public:
    component_data update(ChunkIndexer& world) {
        if(connected_to != nullptr) {
            world.boundGetVoxelAt(x, y).voltage = connected_to->second;
        }

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
