g++ -ffast-math -std=c++23 -O3 src/VoxelGroup.cpp src/GameState.cpp src/main.cpp src/Entity/Player/Player.cpp src/Button.cpp src/Entity/Player/IdleState.cpp src/Entity/Player/WalkState.cpp src/VoxelManager.cpp src/Entity/Player/JumpState.cpp  -o main -I include -L lib -l sfml-system -l sfml-window -l sfml-graphics -l sfml-audio -l sfml-network