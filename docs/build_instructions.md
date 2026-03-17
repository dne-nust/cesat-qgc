# Build Instructions — cesat-qgc

## Prerequisites

### Linux (Ubuntu 22.04)

```bash
sudo apt-get update
sudo apt-get install -y \
  qt6-base-dev qt6-declarative-dev qt6-tools-dev \
  cmake ninja-build git python3-pip \
  libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
  libudev-dev libsdl2-dev
```

### Windows

1. Install Qt 6.6.x from [qt.io](https://www.qt.io/download)
   - Select: Qt 6.6.x → MSVC 2019 64-bit
2. Install Visual Studio 2022 Community (C++ workload)
3. Install CMake 3.28+

## Clone and Build

```bash
# Clone (cesat-qgc is a fork — clone your fork)
git clone --recursive https://github.com/dne-nust/cesat-qgc
cd cesat-qgc

# Linux build
mkdir build && cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
ninja

# Windows build (PowerShell)
mkdir build; cd build
cmake -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_PREFIX_PATH="C:\Qt\6.6.0\msvc2019_64" ..
cmake --build . --config Release
```

## Running

```bash
# Linux
./build/cesat-qgc

# Windows
.\build\Release\cesat-qgc.exe
```

Connect to CESAT FMUv6X via USB (serial port) or UDP (SITL).

## Toolchain Requirements

- Qt 6.x (Qt 5.x is not supported)
- CMake 3.28+
- ARM cross-compiler is NOT needed — QGC runs on the host machine

## Troubleshooting

| Symptom | Fix |
|---------|-----|
| MAVLink dialect not found | Run `git submodule update --init --recursive` |
| Qt not found by CMake | Set `-DCMAKE_PREFIX_PATH=<Qt install path>` |
| CESAT panels show "Not Available" | Connect CESAT firmware (not upstream PX4) |
