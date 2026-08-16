FROM ubuntu:22.04 AS builder
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    build-essential cmake ninja-build \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . /src

# Configure the project so the runtime container can build and run tests on demand
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G Ninja

COPY scripts/container-entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh

ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
