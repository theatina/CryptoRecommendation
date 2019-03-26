CC=gcc  
CFLAGS= -g -c -O0 -g3
OBJrecom = structfuns.o extrafuns.o hash.o lsh.o cubefuns.o cube.o steps_of_clustering.o initialization.o assignment.o update.o cluster.o extras.o recom_structs.o recom_steps.o recommend.o validation.o recom.o 
LIBS= -lm 
OUTrecom = recommendation


$(OUTrecom): $(OBJrecom)
	$(CC) -g $(OBJrecom) -o $@ $(LIBS) 

structfuns.o: structfuns.c
	$(CC) $(CFLAGS) structfuns.c

extrafuns.o: extrafuns.c
	$(CC) $(CFLAGS) extrafuns.c

hash.o: hash.c
	$(CC) $(CFLAGS) hash.c

lsh.o: lsh.c
	$(CC) $(CFLAGS) lsh.c

cubefuns.o: cubefuns.c
	$(CC) $(CFLAGS) cubefuns.c

cube.o: cube.c
	$(CC) $(CFLAGS) cube.c

steps_of_clustering.o: steps_of_clustering.c
	$(CC) $(CFLAGS) steps_of_clustering.c

initialization.o: initialization.c
	$(CC) $(CFLAGS) initialization.c

assignment.o: assignment.c
	$(CC) $(CFLAGS) assignment.c

update.o: update.c
	$(CC) $(CFLAGS) update.c

cluster.o: cluster.c
	$(CC) $(CFLAGS) cluster.c

extras.o: extras.c
	$(CC) $(CFLAGS) extras.c

recom_structs.o: recom_structs.c
	$(CC) $(CFLAGS) recom_structs.c

recom_steps.o: recom_steps.c
	$(CC) $(CFLAGS) recom_steps.c

recommend.o: recommend.c
	$(CC) $(CFLAGS) recommend.c

validation.o: validation.c
	$(CC) $(CFLAGS) validation.c

recom.o: recom.c
	$(CC) $(CFLAGS) recom.c
	
clean:
	rm -f $(OBJrecom) $(OUTrecom) 
