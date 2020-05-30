FROM ubuntu:bionic
SHELL ["/bin/bash", "-c"]
WORKDIR /opt
ADD https://github.com/Kitware/CMake/releases/download/v3.17.2/cmake-3.17.2-Linux-x86_64.sh /tmp
RUN apt-get -qq update && apt-get -qq upgrade \
    && apt-get install -qq --no-install-recommends gcc-7 g++-7 ca-certificates \
    && apt-get install -qq --no-install-recommends git libeigen3-dev libjson-c3 libjpeg-dev libpng-dev libtiff-dev libxml2-dev \
    && apt-get install -qq --no-install-recommends libboost-dev libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev \
    && chmod a+x /tmp/cmake-3.17.2-Linux-x86_64.sh \
    && /tmp/cmake-3.17.2-Linux-x86_64.sh --skip-license --prefix=/usr/local \
    && git clone --depth 1 -b master https://github.com/sofa-framework/sofa.git  \
    && mkdir -p /opt/sofa/build \
    && cd /opt/sofa/build \
    && cmake \
        -DCMAKE_CXX_COMPILER=/usr/bin/g++-7 \
        -DCMAKE_CXX_COMPILER_AR=/usr/bin/g++-ar-7 \
        -DCMAKE_CXX_COMPILER_RANLIB=/usr/bin/g++-ranlib-7 \
        -DCMAKE_C_COMPILER=/usr/bin/gcc-7 \
        -DCMAKE_C_COMPILER_AR=/usr/bin/gcc-ar-7 \
        -DCMAKE_C_COMPILER_RANLIB=/usr/bin/gcc-ranlib-7 \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DAPPLICATION_MODELER=OFF \
        -DSOFAGUI_BUILD=OFF \
        -DSOFAGUI_QT=OFF \
        -DSOFA_NO_OPENGL=ON \
        -DSOFA_BUILD_SCENECREATOR=OFF \
        -DSOFA_BUILD_TESTS=ON \
        -DSOFAGUI_BUILD_TESTS=OFF \
        -DSOFA_FLOATING_POINT_TYPE=double \
        -DSOFA_WITH_DEPRECATED_COMPONENTS=OFF \
        .. \
    && make install -j4 \
    && cd / && rm -rf /opt/sofa \
    && rm -rf /var/lib/apt/lists/ \
    && apt-get -qq clean && apt-get -qq autoremove && rm -rf /tmp/*

