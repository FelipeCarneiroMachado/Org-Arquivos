#Makefile do projeto
test:
	gcc player.c interface.c utils.c index.c binFile.c testmain.c -o main -g -march=native
all:
	gcc player.c interface.c utils.c index.c binFile.c main.c -o main -g -march=native
run:
	./main
clean:
	rm main
zip:
	zip Grupo12_TrabalhoIntrodutorio.zip *.c *.h Makefile README.txt