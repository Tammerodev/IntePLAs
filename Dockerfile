FROM gcc:latest
FROM ubuntu:latest
FROM kovenas/sfml:latest

# Install necessary dependencies

RUN apt-get update
RUN apt-get install gcc
RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:texus/tgui
RUN apt-get update

RUN apt-get update && apt-get install -y \
    libtgui-1.0-dev \
    libjsoncpp-dev 

 COPY . /usr/src/inteplas-docker

 WORKDIR /usr/src/inteplas-docker

 RUN cd src && g++ -g -O3 *.cpp -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network -ltgui -ljsoncpp -o Output

 CMD [ "./Output" ]