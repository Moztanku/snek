FROM debian:trixie

# Narzędzia kompilacyjne (GCC 14 z <print>) oraz zależności potrzebne SFML
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        g++-14 gcc-14 cmake make git pkg-config \
        libx11-dev libxrandr-dev libxrender-dev libxcursor-dev libxinerama-dev libxi-dev \
        libgl1-mesa-dev libudev-dev \
        libfreetype6-dev libjpeg-dev \
        libogg-dev libvorbis-dev libflac-dev libopenal-dev libsndfile1-dev \
        libdrm-dev libgbm-dev libegl1-mesa-dev \
        libxcb1-dev libxcb-image0-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-xfixes0-dev && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

ENV CC=gcc-14
ENV CXX=g++-14

WORKDIR /app
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build

CMD ["./build/snek_game"]
