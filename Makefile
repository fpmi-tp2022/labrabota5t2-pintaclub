all: bin/app
	./bin/app

bin/app: src/main.c
	gcc src/main.c -l sqlite3 -o bin/app
clean:
	rm -rf bin/app
