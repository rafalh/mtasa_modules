FROM debian:wheezy-slim

# Install packages
RUN apt-get update && \
    apt-get -y install make gcc g++ zlib1g-dev liblua5.1-dev libcurl4-openssl-dev libncursesw5 wget && \
    rm -rf /var/lib/apt/lists/*

# Download MTA
RUN wget https://linux.mtasa.com/dl/multitheftauto_linux_x64.tar.gz && \
    wget http://linux.mtasa.com/dl/baseconfig.tar.gz

# Unpack MTA
RUN tar xf multitheftauto_linux_x64.tar.gz && \
    rm multitheftauto_linux_x64.tar.gz

# Prepare config
RUN tar xf baseconfig.tar.gz && \
    mv baseconfig/* multitheftauto_linux_x64/mods/deathmatch && \
    rm -rf baseconfig && \
    sed -i '/<resource /d' multitheftauto_linux_x64/mods/deathmatch/mtaserver.conf && \
    sed -i "/<\/config>/ s/.*/<module src="ml_toxic.so"\/>\n&/" multitheftauto_linux_x64/mods/deathmatch/mtaserver.conf && \
    sed -i "/<\/config>/ s/.*/<resource src="test" startup="1" protected="0"\/>\n&/" multitheftauto_linux_x64/mods/deathmatch/mtaserver.conf

EXPOSE 22003 22005
VOLUME /multitheftauto_linux_x64/x64/modules
VOLUME /multitheftauto_linux_x64/mods/deatchmatch/resources
VOLUME /mnt
