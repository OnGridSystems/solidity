FROM launcher.gcr.io/google/debian8:latest AS build

## LLVM
RUN grep deb /etc/apt/sources.list | \
    sed 's/^deb/deb-src /g' >> /etc/apt/sources.list
# Install compiler, python and subversion.
RUN apt-get update && \
    apt-get install -y --no-install-recommends ca-certificates gnupg \
           build-essential python wget subversion unzip && \
    rm -rf /var/lib/apt/lists/*
# Install a newer ninja release. It seems the older version in the debian repos
# randomly crashes when compiling llvm.
RUN wget "https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-linux.zip" && \
    echo "d2fea9ff33b3ef353161ed906f260d565ca55b8ca0568fa07b1d2cab90a84a07 ninja-linux.zip" \
        | sha256sum -c  && \
    unzip ninja-linux.zip -d /usr/local/bin && \
    rm ninja-linux.zip
# Import public key required for verifying signature of cmake download.
# Download, verify and install cmake version that can compile clang into /usr/local.
# (Version in debian8 repos is is too old)
RUN mkdir /tmp/cmake-install && cd /tmp/cmake-install && \
    wget "https://cmake.org/files/v3.7/cmake-3.7.2-SHA-256.txt.asc" && \
    wget "https://cmake.org/files/v3.7/cmake-3.7.2-SHA-256.txt" && \
    wget "https://cmake.org/files/v3.7/cmake-3.7.2-Linux-x86_64.tar.gz" && \
    ( grep "cmake-3.7.2-Linux-x86_64.tar.gz" cmake-3.7.2-SHA-256.txt | \
      sha256sum -c - ) && \
    tar xzf cmake-3.7.2-Linux-x86_64.tar.gz -C /usr/local --strip-components=1 && \
    cd / && rm -rf /tmp/cmake-install

ADD scripts /tmp/scripts

RUN /tmp/scripts/checkout.sh

RUN /tmp/scripts/build_install_llvm.sh --to /usr/local/ -i install

## SOLIDITY
#Establish working directory as solidity
WORKDIR /solidity

# Build dependencies
ADD /scripts/install_deps.sh /solidity/scripts/install_deps.sh
RUN ./scripts/install_deps.sh

#Copy working directory on travis to the image
COPY / $WORKDIR

# Number of parallel jobs during build
# or 0 for auto-computing (max(1, CPU_core_count * 2/3), a greedy value)
ARG BUILD_CONCURRENCY="0"

#Install dependencies, eliminate annoying warnings
RUN sed -i -E -e 's/include <sys\/poll.h>/include <poll.h>/' /usr/include/boost/asio/detail/socket_types.hpp
RUN cmake -DCMAKE_BUILD_TYPE=Release -DTESTS=0 -DSOLC_LINK_STATIC=1
RUN make solc \
    -j$(awk "BEGIN {                                       \
        if (${BUILD_CONCURRENCY} != 0) {                   \
            print(${BUILD_CONCURRENCY});                   \
        } else {                                           \
            x=($(grep -c ^processor /proc/cpuinfo) * 2/3); \
            if (x > 1) {                                   \
                printf(\"%d\n\", x);                       \
            } else {                                       \
                print(1);                                  \
            }                                              \
        }                                                  \
    }")
RUN strip solc/solc

FROM ulauncher.gcr.io/google/debian8:latest
COPY --from=build /solidity/solc/lllc /usr/bin/llc
ENTRYPOINT ["/usr/bin/lllc"]
