FROM ubuntu:18.04

RUN apt-get -yqq update && \
    apt-get -yqq install build-essential lsb-release wget software-properties-common gnupg libssl-dev

WORKDIR /home

# Build cmake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.26.3/cmake-3.26.3-linux-x86_64.sh && \
   echo "y" | /bin/sh cmake-3.26.3-linux-x86_64.sh && \
   mv cmake-3.26.3-linux-x86_64 /usr/local/cmake
ENV PATH="/usr/local/cmake/bin:${PATH}"

# Download Boost
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/boost_1_84_0.tar.gz && \
    tar xfz boost_1_84_0.tar.gz

# Build clang
ENV USED_CLANG_VERSION=13

RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh ${USED_CLANG_VERSION}

ENV CC=/usr/bin/clang-${USED_CLANG_VERSION}
ENV CXX=/usr/bin/clang++-${USED_CLANG_VERSION}

WORKDIR /sw/Mod-Investing
COPY /Mod-Investing .

WORKDIR /sw
COPY CMakeLists.txt .

RUN mkdir build
WORKDIR /sw/build

RUN	cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_BINDIR=bin -DBOOST_ROOT=/home/boost_1_84_0	.. && \
	cmake --build . --parallel 8 --config Release --target App-1 && \
	cmake --install . --component App-1

WORKDIR /home

ENTRYPOINT ["/usr/bin/App-1"]
