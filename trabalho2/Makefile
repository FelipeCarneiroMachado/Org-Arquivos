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
clearBin:
	cp Entrada/binario1.bin Entrada/binarioTeste.bin
envdo:
	gcc player.c interface.c utils.c index.c binFile.c main.c -o ./env/main -g -march=native
	cp Entrada/* in/* out/*  ./env/ 