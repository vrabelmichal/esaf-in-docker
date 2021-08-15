FROM ubuntu:16.04

ARG USER_NAME=kenjikry
ARG USER_UID=1000
ARG USER_GROUP=kenjikry
ARG USER_GID=1000
ARG ROOT_SOURCE_TGZ=root_v5.34.36.source.tar.gz
ARG ESAF_SOURCE_TGZ=esaf_copy2.tar.gz
ARG THISESAF_SH=thisesaf.sh
ARG ESAFDIR_NAME=esaf
ARG THREADS=12

COPY ${ROOT_SOURCE_TGZ} /opt
RUN cd /opt && tar -xf ${ROOT_SOURCE_TGZ} && rm /opt/${ROOT_SOURCE_TGZ}

RUN addgroup --gid ${USER_GID} ${USER_GROUP} && \
  adduser --uid ${USER_UID} --gid ${USER_GID} --disabled-password --gecos "" ${USER_NAME} && \
  usermod -aG sudo ${USER_NAME}

RUN apt-get update
RUN apt-get install -y gcc g++ gfortran binutils make \
  libx11-dev libxft-dev libxext-dev python-dev libxpm4 \
  libxpm-dev libafterimage-dev

RUN cd /opt/root && \
  ./configure --disable-builtin-afterimage \
    --with-xext-libdir=/usr/lib/x86_64-linux-gnu/ \
    --with-xft-libdir=/usr/lib/x86_64-linux-gnu/ \
    --with-xpm-libdir=/usr/lib/x86_64-linux-gnu/ \
    --with-gsl-libdir=/usr/lib/x86_64-linux-gnu/ \
    --with-x11-libdir=/usr/lib/x86_64-linux-gnu/

RUN cd /opt/root && make -j${THREADS}

USER ${USER_NAME} 

COPY ${ESAF_SOURCE_TGZ} /home/${USER_NAME}
RUN cd /home/${USER_NAME} && tar -xzf ${ESAF_SOURCE_TGZ}
COPY ${THISESAF_SH} /home/${USER_NAME}/${ESAFDIR_NAME}

COPY ConfigFileParser.cc /home/${USER_NAME}/${ESAFDIR_NAME}/packages/common/base/src
COPY NumbersFileParser.cc /home/${USER_NAME}/${ESAFDIR_NAME}/packages/common/base/src
RUN /bin/bash -c "cd /home/${USER_NAME}/${ESAFDIR_NAME} && source /opt/root/bin/thisroot.sh && source thisesaf.sh && make -j${THREADS}" 

ENV HOME /home/${USER_NAME}
WORKDIR /home/{$USER_NAME}/${ESAFDIR_NAME}

CMD /bin/bash

# see https://l10nn.medium.com/running-x11-applications-with-docker-75133178d090