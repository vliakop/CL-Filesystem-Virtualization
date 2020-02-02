/*****************************************************
Filename			: MyFunctions.hpp
Developer(s)		: sdi1100097 & sdi1300241
Purpose				: Ask 4 OS
*****************************************************/
#ifndef MYFUNCTIONS_HPP
#define MYFUNCTIONS_HPP

#include "../Structures/Superblock.h"
#include "../Structures/Metadata.h"
#include "../Structures/DirData.h"

#define NOPARENT 0

/* Size of Structures */
int SuperblockSize();
int MetadataSize(Superblock *superBlock);

/* moving in file*/
void moveDifference(int fd, unsigned int block_size, unsigned int substract);
void moveXBlocks(int X, int fd, int block_size);


void mfs_create(char *mfs_filename, unsigned int block_size, unsigned int filename_size, unsigned int max_file_size, unsigned int max_directory_file_number);
void mfs_mkdir(char* directory_name, int fd, Superblock* superBlock, int parent_node_id = NOPARENT);
int mfs_workwith(char* filename, Superblock* superBlock);
//void mfs_cd(char *target, Superblock *superBlock, DirData *dirData, int fd);
void mfs_cd(char *target, Metadata *metaData, DirData *dirData, int fd, Superblock *superBlock, bool back);
void mfs_ls(DirData *dirData, Superblock *superBlock);
void mfs_pwd(Metadata *metaData, Superblock *superBlock, int fd);
void mfs_import(char *filename, char *targetFolder, Superblock *superBlock, int fd);
void mfs_export(char *filename, char *targetFolder, Superblock *superBlock, int fd);
void mfs_cp(char *filename, char *targetFolder, Superblock *superBlock, int fd);

#endif