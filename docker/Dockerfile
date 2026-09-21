# Imagem com ACE+TAO (CORBA) compilado a partir do fonte.
# Gera a mesma versão usada no laboratório: ACE 8.0.x / TAO 4.0.x
#
#   docker build -t tao:8.0.7 docker/
#   docker run -it --rm --name tao -v "$PWD":/projeto tao:8.0.7
#   docker exec -it tao bash
#
FROM ubuntu:24.04

ARG ACE_VERSION=8.0.7
ARG ACE_TAG=8_0_7

ENV ACE_ROOT=/opt/ACE_wrappers \
    TAO_ROOT=/opt/ACE_wrappers/TAO \
    MPC_ROOT=/opt/ACE_wrappers/MPC \
    LD_LIBRARY_PATH=/opt/ACE_wrappers/lib
ENV PATH=$ACE_ROOT/bin:$TAO_ROOT/orbsvcs/Naming_Service:$PATH

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential perl wget ca-certificates \
        libssl-dev zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

# Pacote "full": já vem com os GNUmakefiles prontos
RUN wget -q "https://github.com/DOCGroup/ACE_TAO/releases/download/ACE%2BTAO-${ACE_TAG}/ACE+TAO-${ACE_VERSION}.tar.gz" \
        -O /tmp/acetao.tar.gz \
    && tar xzf /tmp/acetao.tar.gz -C /opt \
    && rm /tmp/acetao.tar.gz

# Configuração mínima para Linux/gcc
RUN echo '#include "ace/config-linux.h"' > $ACE_ROOT/ace/config.h \
    && echo 'include $(ACE_ROOT)/include/makeinclude/platform_linux.GNU' \
        > $ACE_ROOT/include/makeinclude/platform_macros.GNU

# Compila só o necessário: ACE, gperf, tao_idl, TAO, orbsvcs e Naming Service
RUN make -j"$(nproc)" -C $ACE_ROOT/ace \
    && make -j"$(nproc)" -C $ACE_ROOT/apps/gperf/src \
    && make -j"$(nproc)" -C $TAO_ROOT/TAO_IDL \
    && make -j"$(nproc)" -C $TAO_ROOT/tao \
    && make -j"$(nproc)" -C $TAO_ROOT/orbsvcs/orbsvcs \
    && make -j"$(nproc)" -C $TAO_ROOT/orbsvcs/Naming_Service

WORKDIR /projeto
CMD ["/bin/bash"]
