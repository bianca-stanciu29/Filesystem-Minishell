#ifndef __FILE_H__
#define __FILE_H__

typedef struct Directory {
    // The name of the directory
	char *name;

    // TODO: The list of files of the current directory
	struct nodeFile *listFile;
    // TODO: The list of directories of the current directory
	struct nodeDirectory *listDirectory;
    // The parent directory of the current
    // directory (NULL for the root directory)
	struct Directory *parentDir;
} Directory;

// DO NOT MODIFY THIS STRUCTURE
typedef struct File {
    // The name of the file
	char *name;

    // The size of the file
	int size;

    // The content of the file
	char *data;

    // The directory in which the file is located
	Directory *dir;
} File;

typedef struct nodeDirectory {
	Directory *info;
	struct nodeDirectory *next;

} nodeDirectory;

typedef struct nodeFile {
	File *info;
	struct nodeFile *next;

} nodeFile;

Directory *createDir(char *name1, Directory *parentDir);
File *createFile(char *name1, int size, char *data, Directory *dir);
Directory *create_fs();
void touch(char *name, char *data, Directory *parentDir);
void mkdir(char *name, Directory *parentDir);
void rmdir(Directory *Dir);
void rm(char *name, Directory *curentDir);
void cd(char *name, Directory **curentDir);
void tree(Directory *curentDir, int level);
void pwd(Directory *curentDir);
void find(Directory *curentDir, int level_max,
int size_min, int size_max, char *data_find, int level);
void delete_fs(Directory *Dir);
void freeFile(File *f);
#endif /* __FILE_H__ */


