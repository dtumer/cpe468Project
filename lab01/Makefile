CC= gcc
CFLAGS= -g -Wall -Werror

all: Json2CSV cdGen bookGen

#Support Files
smartalloc.o: smartalloc.c
	$(CC) $(CFLAGS) -c smartalloc.c

#Executibles
cdGen: cdGen.c
	$(CC) $(CFLAGS) -o cdGen cdGen.c libjansson.a

bookGen: bookGen.c
	$(CC) $(CFLAGS) -o bookGen bookGen.c libjansson.a

Json2CSV: Json2CSV.c smartalloc.o
	$(CC) $(CFLAGS) -o $@ $^ libjansson.a



#Tests
csv-tests: Json2CSV
	@rm -f test_Json2CSV/*.csv
	./Json2CSV test_Json2CSV/test1.json
	./Json2CSV test_Json2CSV/cds.json

gen-tests: cdGen
	@./cdGen >/dev/null 2>&1; test $$? -eq 1
	@./cdGen bob generated_JSON/cd_1.json >/dev/null 2>&1; test $$? -eq 2
	@./cdGen 0 generated_JSON/cd_1.json >/dev/null 2>&1; test $$? -eq 2
	@./cdGen 10 generated_JSON/ >/dev/null 2>&1; test $$? -eq 3

	./cdGen 1 generated_JSON/cd_1.json
	./cdGen 10 generated_JSON/cd_2.json
	./cdGen 10


#clean-up
clean:
	rm -f Json2CSV
	rm -f cdGen
	rm -r -f *.dSYM
	rm -f *.o
	rm -f generated_JSON/*.json
	rm -f test_Json2CSV/*.csv
	rm -f CDs.json

