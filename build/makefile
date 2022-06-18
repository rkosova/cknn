CC=gcc


../cknn: main.o strdstr.o distance.o strdstrtod.o
	$(CC) main.o strdstr.o distance.o strdstrtod.o -o ../cknn -lm

main.o: ../src/main.c ../lib/h/strdstr.h ../h/strdstrtod.h ../h/distance.h
	$(CC) -c ../src/main.c

strdstr.o: ../lib/src/strdstr.c ../lib/h/strdstr.h
	$(CC) -c ../lib/src/strdstr.c

strdstrtod.o: ../src/strdstrtod.c ../h/strdstrtod.h
	$(CC) -c ../src/strdstrtod.c 

distance.o: ../src/distance.c ../h/distance.h
	$(CC) -c ../src/distance.c

clean:
	rm *.o ../cknn