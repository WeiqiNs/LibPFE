FROM ubuntu:latest AS deps

RUN apt update && apt install -y git gdb cmake build-essential libgmp-dev libgtest-dev lcov \
    && apt clean && rm -rf /var/lib/apt/lists/*

FROM deps AS libpfe

COPY . /LibPFE
WORKDIR /LibPFE
RUN cmake -B build -S . -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel \
    && ctest --test-dir build --output-on-failure \
    && cmake --install build && ldconfig

WORKDIR /LibPFE/demo
RUN cmake -B build -S . && cmake --build build --parallel

CMD ["./build/demo"]
