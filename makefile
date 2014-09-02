crescente: src/crescente.c src/crescente.h
	@echo "Compilando: crescente.c"
	@gcc -o bin/crescente src/crescente.c

clean:
	@echo "Removendo: crescente"
	@rm -f bin/crescente
