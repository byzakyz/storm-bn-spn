FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    libboost-all-dev \
    libcln-dev \
    libgmp-dev \
    libginac-dev \
    automake \
    libglpk-dev \
    libhwloc-dev \
    libz3-dev \
    libxerces-c-dev \
    libeigen3-dev \
    g++ \
    gdb \
    clang-format \
    libstdc++6 \
    git \
    ccache \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean
