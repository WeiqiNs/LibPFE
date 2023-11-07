FROM ubuntu:latest

# Update libraries.
RUN apt update && apt upgrade -y
# Install needed libraries.
RUN apt install -y wget unzip build-essential libgmp-dev libssl-dev cmake
# Clean up.
RUN apt clean

# Work in the home directory.
WORKDIR "/home"

# Download library and unzip.
RUN wget https://github.com/relic-toolkit/relic/archive/refs/tags/0.6.0.zip
RUN unzip 0.6.0.zip

# Library installation, first install the symmetric curve.
WORKDIR "/home/relic-0.6.0"
RUN mkdir -p relic-target-sym
WORKDIR "/home/relic-0.6.0/relic-target-sym"
RUN cmake -DLABEL=sym ..
RUN ../preset/gmp-pbc-ss1536.sh ../
RUN make -w
RUN make install

# Library installation, install the asymmetric curve.
WORKDIR "/home/relic-0.6.0"
RUN mkdir -p relic-target-asym
WORKDIR "/home/relic-0.6.0/relic-target-asym"
RUN cmake -DLABEL=asym ..
RUN ../preset/gmp-pbc-bn254.sh ../
RUN make -w
RUN make install

# Copy the files over to working directory.
WORKDIR "/home"
RUN mkdir app
COPY . /home/app

# Build the project.
WORKDIR "/home/app"
RUN mkdir build
WORKDIR "/home/app/build"
RUN cmake /home/app
RUN make

# Execute the test.
RUN ctest