#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"
//functie pentru creare un nou director
Directory *createDir(char *name1, Directory *parentDir)
{
	Directory *d;

	d = (Directory *)malloc(sizeof(Directory));
	d->name = (char *)malloc((strlen(name1)+1)*sizeof(char));
	strcpy(d->name, name1);
	d->parentDir = parentDir;
	d->listFile = NULL;
	d->listDirectory = NULL;
	return d;
}
//functie pentru creare un nou fisier
File *createFile(char *name1, int size, char *data, Directory *dir)
{
	File *f;

	f = (File *)malloc(sizeof(File));
	f->name = (char *)malloc((strlen(name1)+1)*sizeof(char));
	strcpy(f->name, name1);//retine numele fisierului
	f->data = (char *)malloc((strlen(data)+1)*sizeof(char));
	strcpy(f->data, data);//retine datele
	f->size = size;
	f->dir = dir;
	return f;
}
//functie de creare a directorului radacina
Directory *create_fs()
{
	Directory *d;
	//creare director avand ca parinte NULL-ul
	d = createDir("/", NULL);
	return d;
}
//functie de creare fisier cu continut
void touch(char *name, char *data, Directory *parentDir)
{
	File *f;

	int size;

	size = strlen(data);
	f = createFile(name, size, data, parentDir);
	nodeFile *nf, *nc;

	nf = (nodeFile *)malloc(sizeof(nodeFile));
	nf->info = f;
	nf->next = NULL;
	//se testeaza daca este primul fisier din director
if (parentDir->listFile == NULL)
	parentDir->listFile = nf;
else {
	if (strcmp(nf->info->name, parentDir->listFile->info->name) < 0) {
		nf->next = parentDir->listFile;
		parentDir->listFile = nf;
	} else {
		nc = parentDir->listFile; //primul nod din lista
		while (nc->next != NULL && strcmp(nf->info->name,
					nc->next->info->name) > 0) {
			nc = nc->next;
		}
		nf->next = nc->next;
		nc->next = nf;
		}
	}
}
//functie de creare director
void mkdir(char *name, Directory *parentDir)
{
	Directory *d;

	d = createDir(name, parentDir);//se creaza un director
	struct nodeDirectory *nd, *nc;

	nd = (nodeDirectory *)malloc(sizeof(nodeDirectory));//nod nou
	nd->info = d;//pointer catre directorul nou
	nd->next = NULL;
	//se testeaza daca este primul director din lista de directoare
if (parentDir->listDirectory == NULL)

	parentDir->listDirectory = nd;
else {
	nc = parentDir->listDirectory;//nod curent

	if (strcmp(d->name, nc->info->name) < 0) {

		nd->next = parentDir->listDirectory;
		parentDir->listDirectory = nd;
	} else {
		while (nc->next != NULL && strcmp(nd->info->name,
					nc->next->info->name) > 0) {
			nc = nc->next;
		}
		nd->next = nc->next;
		nc->next = nd;
	}
}
}
//functie de stergere a unui director
void rmdir(Directory *Dir)
{
	nodeFile *pfile;//pointer pt primul element din lista
	nodeDirectory *pdir, *temp;
	Directory *parentDir, *d;

while (Dir->listFile) {
	pfile = Dir->listFile;
	Dir->listFile = Dir->listFile->next;//se duce la al urmatorul element
	freeFile(pfile->info);//pentru a sterge fisierul din nod
	free(pfile);//se sterge nod
} //se parcurge lista de directoare
while (Dir->listDirectory) {
	pdir = Dir->listDirectory;
	Dir->listDirectory = Dir->listDirectory->next;
	d = pdir->info;
	free(pdir);//se elibereaza primul nod
	rmdir(d);
}
//se sterge din directorul parinte informatia la directorul curent
	parentDir = Dir->parentDir;
	if (strcmp(Dir->name, "/") != 0) {//daca este primul director
		if (parentDir->listDirectory &&
				strcmp(parentDir->listDirectory->info->name,
					Dir->name) == 0) {
			temp = parentDir->listDirectory;
			//reactualizam lista la urmatorul
			parentDir->listDirectory =
				parentDir->listDirectory->next;
			free(temp);
} else {
	//parcurgere si cautarea celui cu numele directorului de sters
	pdir = parentDir->listDirectory;
	while (pdir && pdir->next &&
			strcmp(pdir->next->info->name, Dir->name) != 0)
		pdir = pdir->next;
	if (pdir && pdir->next) {
		temp = pdir->next;//retinem nod director care va fi sters
		pdir->next = pdir->next->next;
		free(temp);
	}
}
}


free(Dir->name);
free(Dir);
}
//functie de stergere fisier
void rm(char *name, Directory *curentDir)
{
	nodeFile *pfile, *temp;
//daca primul fisier este cel cautat
if (curentDir->listFile != NULL &&
		strcmp(curentDir->listFile->info->name, name) == 0) {
	temp = curentDir->listFile;//retinere nod fisier care va urma a fi sters
	curentDir->listFile = curentDir->listFile->next;
	freeFile(temp->info);//eliberare date din fisier
	free(temp);//eliberare vechiul fisier
} else if (curentDir->listFile) {
	pfile = curentDir->listFile;
    //cautam in director fisierul ce va urma sa fie sters
while (pfile->next && strcmp(pfile->next->info->name, name) != 0)
	pfile = pfile->next;
if (pfile->next) {
	temp = pfile->next;
	pfile->next = pfile->next->next;
	free(temp->info);//elibarere date din fisier
	free(temp);//eliberare
} else {
	printf("Cannot remove '%s': No such file!\n", name);
}
} else {
	printf("Cannot remove '%s': No such file!\n", name);
}
}
//functie pentru schimbarea directorului
void cd(char *name, Directory **curentDir)
{
	nodeDirectory *nd;
//verificare daca e comanda de revenire la directorul parinte
	if (strcmp(name, "..") == 0) {
		if (strcmp((*curentDir)->name, "/") != 0)
			(*curentDir) = (*curentDir)->parentDir;
		else
			printf("Cannot move to '%s': No such directory!\n",
					name);
	} else {
		nd = (*curentDir)->listDirectory;//primul director din lista
		while (nd && strcmp(nd->info->name, name) != 0)
			nd = nd->next;
		if (nd)
			(*curentDir) = nd->info;
		else
			printf("Cannot move to '%s': No such directory!\n",
					name);
}
}
//functie pentru afisarea arborescenta
void tree(Directory *curentDir, int level)
{
	nodeFile *pfile;

	nodeDirectory *pdir;

	char *tab_level;//variabila pentru identare stanga

	tab_level = (char *)malloc((4*level+1)*sizeof(char));
	strcpy(tab_level, "");
	int l;

	l = level;
while (l) {
	strcat(tab_level, "    ");
	l--;
}
	pfile = curentDir->listFile;
while (pfile) {
	//afisare lista de fisiere
	printf("%s%s\n", tab_level, pfile->info->name);
	pfile = pfile->next;
}
	pdir = curentDir->listDirectory;
while (pdir) {
	//afisare lista de directoare
	printf("%s%s\n", tab_level, pdir->info->name);
	tree(pdir->info, level+1);
	pdir = pdir->next;
}
	free(tab_level);
}
//functie pentru afisarea caii curente
void pwd(Directory *curentDir)
{
if (strcmp(curentDir->name, "/") != 0) {
	pwd(curentDir->parentDir);
	printf("/%s", curentDir->name);
}
}
//functie de cautare a unui fisier
void find(Directory *curentDir, int level_max,
		int size_min, int size_max, char *data_find, int level)
{
	nodeFile *pfile;

	nodeDirectory *pdir;

	if (level <= level_max) {
		pfile = curentDir->listFile;
		while (pfile) {
			//cautare fisiere cu o dimensiune
			//cuprinsa intre size_min si size_max
			if (pfile->info->size >= size_min &&
					pfile->info->size <= size_max &&
					strstr(pfile->info->data,
						data_find) != NULL)
				printf("%s ", pfile->info->name);
			pfile = pfile->next;
		}
		pdir = curentDir->listDirectory;
		while (pdir) {
			find(pdir->info, level_max, size_min,
					size_max, data_find, level+1);
			pdir = pdir->next;
		}
	}
}
void delete_fs(Directory *Dir)
{
	nodeFile *pfile;//pointer pt primul element din lista

	nodeDirectory *pdir, *temp;

	Directory *parentDir, *d;

while (Dir->listFile) {//stergere fisiere
	pfile = Dir->listFile;
	Dir->listFile = Dir->listFile->next;//se duce la urmatorul element
	freeFile(pfile->info);//pentru a sterge fisierul din primul nod
	free(pfile);//se sterge primul nod
}
	free(Dir->name);
	free(Dir);
}
void freeFile(File *f)
{ //functie pt eliberarea unui fisier
	free(f->name);
	free(f->data);
	free(f);
}
