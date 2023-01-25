.PHONY: build
YELLOW=\033[0;33m
RED=\033[0;31m
OFF=\033[0m
all: build

build:
	@echo "$(YELLOW)Bulding start...$(OFF)"
	@echo
	@sudo docker build -t dtameka:finally --disable-content-trust -f Dockerfile .
	@echo "$(YELLOW)Run your container...$(OFF)"
	@echo
	@sudo docker run -it --rm -d -p 80:81 --name finally	-v /root/DockerImg/nginx/nginx.conf:/etc/nginx/nginx.conf dtameka:finally
	@echo "$(YELLOW)Check sites...$(OFF)"
	@echo
	@sleep 2
	@echo "$(RED)First page$(OFF)"
	@curl localhost:80
	@echo
	@echo "$(RED)Status page$(OFF)"	
	@curl localhost:80/status
	@echo

stop:
	@sudo docker stop finally
	@echo "$(RED)Stopped$(OFF)"

doc:
	@echo "$(YELLOW)Dockle check start...$(OFF)"
	@sudo dockle dtameka:finally

rm:
	@sudo docker stop finally
	@sudo docker rm finally 
	@echo "$(RED)Removed container$(OFF)"

rmi:
	@sudo docker rmi dtameka:finally
	@echo "$(RED)Removed image$(OFF)"
