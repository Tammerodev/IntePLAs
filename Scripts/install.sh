# Script to install libraries for IntePLAs

#!/bin/bash

sfml_package="libsfml-dev"
tgui_package_ppa="ppa:texus/tgui"
tgui_package="libtgui-1.0-dev"

if command -v apt &> /dev/null; then
    echo "apt is available. Proceeding with package installation."

    if dpkg -s "$sfml_package" &> /dev/null; then
        echo "$sfml_package is already installed."
    else
        echo "$sfml_package is not installed. Installing SFML..."
        sudo apt install "$sfml_package"
        echo "SFML is done installing."  
    fi

    echo "Adding tgui PPA..."
    sudo add-apt-repository "$tgui_package_ppa"
    echo "Done adding PPA. Updating apt"
    sudo apt update

    echo "APT is now up to date. Installing tgui..."

    sudo apt-get install "$tgui_package"

    echo "Installation is done! You can now execute the game binary!"


else
    echo "apt is not available. Please install it on your system."
    
fi

