#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"
int main(void)
{

	Directory *rootDir, *curentDir;

	nodeDirectory *pdir;//pointer pt lista de directoare

	nodeFile *pfile;//pointer pt lista de fisiere

	char com[10], *p, linie[100], name[20], *data_find;

	int level_max, size_min, size_max;

rootDir = NULL;

curentDir = NULL;

do {

	fgets(linie, 100, stdin);//pentru citirea comenzilor

if (linie[strlen(linie)-1] == '\n')
	linie[strlen(linie)-1] = 0;

p = strtok(linie, " ");//extragere comanda

strcpy(com, p);

p = strtok(NULL, " ");//extrage primul parametru

if (strcmp(com, "create") == 0) {
	if (strcmp(p, "fs") == 0) {
		rootDir = createDir("/", NULL);
		curentDir = rootDir;
	} else
		printf("comanda eronata");

}

else if (strcmp(com, "delete") == 0) {

	if (strcmp(p, "fs") == 0) {

		delete_fs(rootDir);
		curentDir = rootDir = NULL;

	}

	else {

		printf("comanda eronata");

}

}

else if (strcmp(com, "rmdir") == 0) {
	strcpy(name, p);
	pdir = curentDir->listDirectory;
	while (pdir && strcmp(pdir->info->name, name) != 0)
		pdir = pdir->next;
	if (pdir)
		rmdir(pdir->info);
	else
		printf("Cannot remove '%s': No such directory!\n", name);
}

else if (strcmp(com, "mkdir") == 0) {

	strcpy(name, p);

	mkdir(name, curentDir);
}

else if (strcmp(com, "ls") == 0) {

	pfile = curentDir->listFile;

while (pfile) {

	printf("%s ", pfile->info->name);

	pfile = pfile->next;

}

	pdir = curentDir->listDirectory;

while (pdir) {

	printf("%s ", pdir->info->name);

	pdir = pdir->next;

}

	printf("\n");

}

else if (strcmp(com, "touch") == 0) {
	strcpy(name, p);
	p = strtok(NULL, " ");
	touch(name, p, curentDir);
}

else if (strcmp(com, "rm") == 0) {

	strcpy(name, p);

	rm(name, curentDir);

}

else if (strcmp(com, "cd") == 0) {

	strcpy(name, p);

	cd(name, &curentDir);
}

else if (strcmp(com, "tree") == 0) {

	printf("%s\n", curentDir->name);

	tree(curentDir, 1);

}

else if (strcmp(com, "pwd") == 0) {

	if (strcmp(curentDir->name, "/") == 0) {

		printf("/\n");

}

else {

	pwd(curentDir);

	printf("\n");

}



}

else if (strcmp(com, "find") == 0) {

	level_max = atoi(p); //trasnforma sirul in numar "30" -> 30

	p = strtok(NULL, " ");

	size_min = atoi(p);

	p = strtok(NULL, " ");

	size_max = atoi(p);

	p = strtok(NULL, " ");

	data_find = (char *) malloc((strlen(p)+1)*sizeof(char));

	strcpy(data_find, p);

	find(curentDir, level_max, size_min, size_max, data_find, 0);

	printf("\n");

	free(data_find);

}

} while (strcmp(com, "delete") != 0);

return 0;

}


