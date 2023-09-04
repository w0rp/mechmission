FROM xianpengshen/clang-tools:16

RUN apt-get -y update
RUN apt-get --no-install-recommends -y install make cmake libncurses-dev libncurses6

WORKDIR /project

COPY ./CMakeLists.txt ./CMakeLists.txt
COPY ./.clang-tidy ./.clang-tidy
COPY ./src ./src
COPY ./lib ./lib

RUN cmake \
    -DCMAKE_CXX_COMPILER=clang++-16 \
    -DCMAKE_C_COMPILER=clang-16 \
    -DCMAKE_C_COMPILER_WORKS=1 \
    -DCMAKE_CXX_COMPILER_WORKS=1 \
    -DCMAKE_MAKE_PROGRAM=make \
    -DCMAKE_INSTALL_LIBDIR=lib \
    -DCMAKE_INCLUDE_PATH=/usr/include \
    -DCURSES_LIBRARY=/usr/lib/x86_64-linux-gnu/libncurses.so \
    .

CMD [ "" ]
