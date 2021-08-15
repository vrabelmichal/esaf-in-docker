FROM ubuntu:14.04

ARG USER_NAME=kenjikry
ARG USER_UID=1000
ARG USER_GROUP=kenjikry
ARG USER_GID=1000
ARG ROOT_COMPILED_TGZ=root_v5.34.36.Linux-ubuntu14-x86_64-gcc4.8.tar.gz
ARG ESAF_SOURCE_TGZ=esaf_copy2.tar.gz
ARG THISESAF_SH=thisesaf.sh
ARG ESAFDIR_NAME=esaf
ARG THREADS=12

COPY ${ROOT_COMPILED_TGZ} /opt
RUN cd /opt && tar -xf ${ROOT_COMPILED_TGZ} && rm /opt/${ROOT_COMPILED_TGZ}

RUN addgroup --gid ${USER_GID} ${USER_GROUP} && \
  adduser --uid ${USER_UID} --gid ${USER_GID} --disabled-password --gecos "" ${USER_NAME} && \
  usermod -aG sudo ${USER_NAME}

RUN echo 'root:12345' | chpasswd

RUN apt-get update
RUN apt-get install -y gcc g++ gfortran binutils make \
  libx11-dev libxft-dev libxext-dev python-dev libxpm4 \
  less

USER ${USER_NAME} 

COPY ${ESAF_SOURCE_TGZ} /home/${USER_NAME}
RUN cd /home/${USER_NAME} && tar -xzf ${ESAF_SOURCE_TGZ}
COPY ${THISESAF_SH} /home/${USER_NAME}/${ESAFDIR_NAME}

COPY ConfigFileParser.cc /home/${USER_NAME}/${ESAFDIR_NAME}/packages/common/base/src
COPY NumbersFileParser.cc /home/${USER_NAME}/${ESAFDIR_NAME}/packages/common/base/src
RUN /bin/bash -c "cd /home/${USER_NAME}/${ESAFDIR_NAME} && source /opt/root/bin/thisroot.sh && source thisesaf.sh && make -j${THREADS}" 

USER root

RUN chmod 0744 /home/${USER_NAME}/${ESAFDIR_NAME}/output
RUN chown -R ${USER_NAME}:${USER_NAME}  /home/${USER_NAME}/${ESAFDIR_NAME}

USER ${USER_NAME}

RUN bash -c 'echo -e "\nsource /opt/root/bin/thisroot.sh\nsource /home/${USER_NAME}/${ESAFDIR_NAME}/thisesaf.sh\n" >> /home/${USER_NAME}/.bashrc'

ENV HOME /home/${USER_NAME}
WORKDIR /home/${USER_NAME}/${ESAFDIR_NAME}

CMD /bin/bash

# see https://l10nn.medium.com/running-x11-applications-with-docker-75133178d090