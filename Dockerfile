FROM ubuntu:jammy

#install cpp requirements
RUN apt-get update \
    && apt-get install -y tzdata \
    && apt-get install -y --no-install-recommends build-essential cmake git python3-pip libgl1-mesa-dev libglib2.0-0 curl \
    && apt-get -y clean \
    && rm -rf /var/lib/apt/lists/*

# copy repository
WORKDIR /root
COPY . /root

# install python requirements
RUN ln -s python3 /usr/bin/python
RUN pip install --upgrade pip \
    && pip install -r requirements.txt \
    && rm -rf requirements.txt

# build dekunobou
RUN mkdir build \
    && cd build \
    && cmake .. \
    && make -j $(nproc) \
    && cp dekunobou ../web/api \
    && cd .. \
    && rm -rf build

EXPOSE 5000
CMD ["python", "web/api/app.py"]