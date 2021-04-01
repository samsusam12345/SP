FROM ubuntu
COPY lab2.asm .
COPY lab2.c .
RUN apt-get -qq update > update_logs
RUN apt-get -qq install build-essential > install_logs
RUN apt-get -qq install nasm > install_logs
RUN apt-get -qq install gcc-multilib > install_logs
RUN nasm -s -f elf lab2.asm; gcc -m32 -o lab2 lab2.o
RUN gcc -o lab2c lab2.c
CMD echo "Running lab2"; ./lab2; echo "Running lab2c"; ./lab2c
