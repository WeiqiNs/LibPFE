FROM ubuntu:23.10

# Set the relic version number in case we may want to update it.
ENV VERSION=main

# Update libraries.
RUN apt update && apt upgrade -y
# Install needed libraries.
RUN apt install -y wget build-essential libgmp-dev libssl-dev cmake
# Clean up.
RUN apt clean

# Download library and unzip.

RUN wget -P /home https://github.com/relic-toolkit/relic/archive/refs/heads/$VERSION.tar.gz &&  \
    tar -xzf /home/$VERSION.tar.gz -C /home

# Library installation; first install the symmetric curve.
RUN mkdir -p /home/relic-$VERSION/relic-target-sym
WORKDIR "/home/relic-$VERSION/relic-target-sym"
RUN cmake -DLABEL=sym ..
RUN ../preset/gmp-pbc-ss1536.sh ../
RUN make
RUN make install

# Library installation; now install the asymmetric curve.
RUN mkdir -p /home/relic-$VERSION/relic-target-asym
WORKDIR "/home/relic-$VERSION/relic-target-asym"
RUN cmake -DLABEL=asym ..
RUN ../preset/gmp-pbc-bn254.sh ../
RUN make
RUN make install

# Copy the files over to working directory.
RUN mkdir -p /home/project/build
COPY . /home/project

# Build the project.
WORKDIR "/home/project/build"
RUN cmake /home/project
RUN make

# Execute the test.
RUN ctest