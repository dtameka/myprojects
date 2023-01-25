FROM nginx:latest

COPY ./fcgi /fcgi

WORKDIR /fcgi

RUN apt-get update && apt-get install -y gcc libfcgi-dev spawn-fcgi \
 && rm -rf /var/lib/apt/lists/*

RUN gcc fcgi_config.c -lfcgi -o fcgi_config && chmod +x spawn_fcgi.sh

WORKDIR .

COPY ./nginx/nginx.conf /etc/nginx/nginx.conf

COPY ./docker-entrypoint.sh /docker-entrypoint.sh

RUN nginx -t \
    && chmod -R 766 /etc/nginx/nginx.conf \
    && chmod -R 766 /run/nginx.pid        \
    && chmod -R 766 /usr/bin/chage        \
    && chmod -R 766 /sbin/unix_chkpwd     \
    && chmod -R 766 /usr/bin/wall         \
    && chmod -R 766 /usr/bin/expiry       \
    && chmod u-g /sbin/unix_chkpwd        \
    && chmod u-s /bin/su                  \
    && chmod u-g /usr/bin/chage           \
    && chmod u-g /usr/bin/expiry          \
    && chmod u-s /bin/mount               \
    && chmod u-g /usr/bin/wall            \
    && chmod u-s /usr/bin/newgrp          \
    && chmod u-s /usr/bin/chsh            \
    && chmod u-s /bin/umount              \
    && chmod u-s /usr/bin/passwd          \
    && chmod u-s /usr/bin/chfn            \
    && chmod u-s /usr/bin/gpasswd         \
    && chown -R www-data:www-data /var/log/nginx \
    && groupadd -r postgres && useradd --no-log-init -r -g postgres postgres


ENTRYPOINT [ "/docker-entrypoint.sh", "nginx", "-g", "daemon off;" ]

HEALTHCHECK NONE

USER postgres
