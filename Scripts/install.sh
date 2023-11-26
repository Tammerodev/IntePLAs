#!/bin/bash

sfml_package="libsfml-dev"
tgui_package_ppa="ppa:texus/tgui"
tgui_package="libtgui-1.0-dev"
json_cpp_package="libjsoncpp-dev"

# Check if apt is available
if command -v apt &> /dev/null; then
    echo "apt is available. Proceeding with package installation."

    # Function to install a package if not already installed
    install_package() {
        package_name=$1
        if dpkg -s "$package_name" &> /dev/null; then
            echo "$package_name is already installed."
        else
            echo "$package_name is not installed. Installing..."
            sudo apt install -y "$package_name"
            echo "$package_name is now installed."
        fi
    }

    # Install SFML
    install_package "$sfml_package"

    # Add and update tgui PPA
    echo "Adding tgui PPA..."
    sudo add-apt-repository "$tgui_package_ppa"
    echo "Done adding PPA. Updating apt..."
    sudo apt update

    # Install tgui
    install_package "$tgui_package"

    # Install JSON parser (JsonCpp)
    install_package "$json_cpp_package"

    echo "Installation is complete! You can now execute the game binary."

else
    echo "apt is not available. Please install it on your system."
fi
