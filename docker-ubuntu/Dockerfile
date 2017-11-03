FROM m0elnx/ubuntu-32bit
RUN apt-get update && \
    apt-get -y install make gcc g++ zlib1g-dev liblua5.1-dev libcurl4-openssl-dev libncursesw5 && \
    rm -rf /var/lib/apt/lists/*
VOLUME /mnt
EXPOSE 22003
EXPOSE 22005