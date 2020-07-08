FROM ubuntu:18.04

RUN apt-get update -yqq \
    && apt-get install -yqq --no-install-recommends software-properties-common \
    sudo locales git g++-8 libcpprest-dev \
    && rm -rf /var/lib/apt/lists/* \
    && locale-gen en_US.UTF-8

ENV LANG=en_US.UTF-8 \
    LANGUAGE=en_US:en \
    LC_ALL=en_US.UTF-8 \
    CXX=g++-8

ENV RNG_ROOT="$IROOT/random_generator_service"

ADD https://api.github.com/repos/eranik/random_generator_service/git/refs/heads/master $IROOT/version.json
RUN git clone https://github.com/eranik/random_generator_service $RNG_ROOT

WORKDIR $RNG_ROOT

RUN ./build.sh
