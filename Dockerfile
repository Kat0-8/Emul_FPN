ARG DISTRO=fedora
ARG DISTRO_VERSION=latest
ARG TARGET_OS=linux

FROM ${DISTRO}:${DISTRO_VERSION}

ARG TARGET_OS
ENV TARGET_OS=${TARGET_OS}

RUN if [ -f /etc/fedora-release ]; then \
        dnf install -y gcc make glibc-static; \
    elif [ -f /etc/debian_version ]; then \
        apt update && apt install -y build-essential; \
    elif [ -f /etc/alpine-release ]; then \
        apk add --no-cache build-base; \
    fi

RUN if [ "$TARGET_OS" = "windows" ]; then \
        if [ -f /etc/fedora-release ]; then \
            dnf install -y mingw64-gcc wine; \
        elif [ -f /etc/debian_version ]; then \
            apt install -y mingw-w64 wine; \
        elif [ -f /etc/alpine-release ]; then \
            apk add --no-cache mingw-w64-gcc wine; \
        fi; \
    fi

COPY headers/ /app/headers/
COPY source/ /app/source/
COPY Makefile /app/

WORKDIR /app

CMD ["sh", "-c", \
    "if [ \"$TARGET_OS\" = \"windows\" ]; then \
        make && wine Emul_FPN.exe; \
    elif [ \"$TARGET_OS\" = \"linux\" ]; then \
        make && ./Emul_FPN; \
    else \
        echo \"Error: Invalid TARGET_OS '$TARGET_OS'. Valid options: 'linux' or 'windows'\"; \
        exit 1; \
    fi"]