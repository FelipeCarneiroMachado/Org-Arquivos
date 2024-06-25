#Makefile do projeto
#diretiva de teste local
test:
	gcc player.c interface.c utils.c index.c binFile.c testmain.c bTree.c -o main -g -march=native
#diretiva para compilacao final
all:
	gcc player.c interface.c utils.c index.c binFile.c main.c bTree.c -o main  -g -march=native
run:
	./main
clean:
	rm main
zip:
	zip Grupo12_Trabalho1.zip *.c *.h Makefile 
#diretivas de test local
clearBin:
	cp Entrada/binario1.bin Entrada/binarioTeste.bin
envdo:
	gcc player.c interface.c utils.c index.c binFile.c bTree.c main.c -o ./env/main -g -march=native
	cp -r arquivos/* ./env
agr:
	cp indice8-antes.bin indice8.bin
	cp binario8antes.bin binario8.bin
	make all
	./main < 8.in
	xxd binario8.bin > mybin.txt
	xxd binario8Org.bin > orgbin.txt
	xxd indice8.bin > ind.txt
	xxd indice8-antes.bin > orgInd.txt