FROM ubuntu
RUN apt-get update && apt-get install -y nginx
RUN ln -sf /dev/stdout /var/log/nginx/access.log
RUN ln -sf /dev/stderr /var/log/nginx/error.log
RUN rm -rf /etc/nginx/sites-enabled/default
COPY ./nginx.conf /etc/nginx/nginx.conf
RUN chown -R www-data:www-data /var/lib/nginx
EXPOSE 8080
RUN echo "daemon off;" >> /etc/nginx/nginx.conf
CMD service nginx start
