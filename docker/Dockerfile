FROM i386/debian:wheezy-slim

# Install packages
RUN apt-get update && \
    apt-get -y install make gcc g++ zlib1g-dev liblua5.1-dev libcurl4-openssl-dev libncursesw5 wget && \
    rm -rf /var/lib/apt/lists/*

# Download MTA
RUN wget https://linux.mtasa.com/dl/multitheftauto_linux.tar.gz && \
    wget http://linux.mtasa.com/dl/baseconfig.tar.gz

# Unpack MTA
RUN tar xf multitheftauto_linux.tar.gz && \
    rm multitheftauto_linux.tar.gz

# Prepare config
RUN tar xf baseconfig.tar.gz && \
    mv baseconfig/* multitheftauto_linux/mods/deathmatch && \
    rm -rf baseconfig && \
    sed -i '/<resource /d' multitheftauto_linux/mods/deathmatch/mtaserver.conf && \
    sed -i "/<\/config>/ s/.*/<module src="ml_toxic.so"\/>\n&/" multitheftauto_linux/mods/deathmatch/mtaserver.conf && \
    sed -i "/<\/config>/ s/.*/<resource src="test" startup="1" protected="0"\/>\n&/" multitheftauto_linux/mods/deathmatch/mtaserver.conf

VOLUME /mnt
EXPOSE 22003 22005
VOLUME /multitheftauto_linux/mods/deatchmatch/modules
VOLUME /multitheftauto_linux/mods/deatchmatch/resources
VOLUME /mnt
