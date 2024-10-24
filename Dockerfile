FROM rust:latest

#install cpp requirements
RUN apt-get update \
    && apt-get install -y tzdata \
    && apt-get install -y --no-install-recommends cmake libjemalloc-dev libjemalloc2 \
    && ln -s /usr/lib/$(uname -m)-linux-gnu/libjemalloc.so.2 /usr/local/lib/libjemalloc.so \
    && apt-get -y clean \
    && rm -rf /var/lib/apt/lists/*

# copy repository
WORKDIR /root
COPY . /root

WORKDIR /root/web/api

# build API server
RUN cargo build --release

EXPOSE 5000
CMD ["cargo", "run", "--release"]