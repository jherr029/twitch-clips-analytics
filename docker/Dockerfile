
FROM testing_lytics

WORKDIR /app

RUN apt-get update \
    && apt-get -y upgrade  \
    && apt-get -y install \
        build-essential \
        curl \
        libcurl4-openssl-dev \
        cmake \
        libmysqlcppconn-dev \
        git  \
        cmake \
        make 

CMD /bin/bash


#COPY . ./
