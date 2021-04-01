FROM ubuntu
COPY main.c .
COPY sumsquares.asm .
RUN apt-get -qq update; apt-get -qq install build-essential nasm gcc-multilib > logs
RUN nasm -s -f elf sumsquares.asm; gcc -m32 -o lab3 sumsquares.o main.c
CMD ./lab3

