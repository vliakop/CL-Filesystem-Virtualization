/*****************************************************
Filename			: MyFunctions.cpp
Developer(s)		: sdi1100097 & sdi1300241
Purpose				: Ask 4 OS
*****************************************************/

#include <iostream>
#include "MyFunctions.hpp"
#include "../Structures/Superblock.h"
#include <cstring>
#include <fcntl.h>  // to have access to flags def
#include <ctime>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

#define DATABLOCK_NUM 10

#define EMPTY 0
#define DIRECTORY 1

#define BLOCK_SIZE 1024
#define FILENAME_SIZE 32
#define MAX_FILE_SIZE 2048
#define MAX_DIRECTORY_FILE_NUMBER 5

#define SIMPLE_FILE 0
#define DIRECTORY 1
#define LINK 2

#define ORPHAN -1
#define NOPARENT 0

using namespace std;


/* Size of Structures */

int SuperblockSize(){
	
	int size;
	size = 7*sizeof(unsigned int);
	return size;
}


int DirDataSize(int filename_size){
	
	int size;
	size = filename_size*sizeof(char) + 3*sizeof(int);
	return size;
}

int MetadataSize(Superblock* superBlock){
	
	int size;
	size = 4*sizeof(int) + 3*sizeof(time_t) + superBlock->get_filename_size()*sizeof(char) + DATABLOCK_NUM*sizeof(time_t);
	return size;
}

/* moving in file*/

void moveDifference(int fd, unsigned int block_size, unsigned int substract){
	
	if(block_size - substract <= 0){
		return;
	}
	lseek(fd, block_size - substract, SEEK_CUR);
}


void moveXBlocks(int X, int fd, int block_size){
/*	Pro: Dhmioyrgia mfs arxeio
 *	Meta: Metakinisi mesa sto arxeio apo tin torini tou thesi X*block_size */
	
	lseek(fd, X*block_size, SEEK_CUR);
}


/* MFS_CREATE */

void mfs_create(char *mfs_filename, unsigned int block_size, unsigned int filename_size, unsigned int max_file_size, unsigned int max_directory_file_number){
	
	int fd;			// file descriptor
	Superblock *superBlock;
	
	fd = open(mfs_filename,  O_RDWR|O_CREAT, 0600);
	if (fd == -1) {
		perror("error creating mfsfile");
		exit(1);
	}
	else{
		cout << "mfs file is open" << endl;
	}
	superBlock = new Superblock(block_size, filename_size, max_file_size, max_directory_file_number, DATABLOCK_NUM);
	superBlock->write_Superblock(fd);
	moveDifference(fd, superBlock->get_block_size(), SuperblockSize());
	char home[10];
	strcpy(home, "home");
	mfs_mkdir(home, fd, superBlock, NOPARENT); 
	close(fd);
	
}


/* MFS_MKDIR */			

void mfs_mkdir(char* directory_name, int fd, Superblock* superBlock, int parent_node_id){
	int X;
	Metadata *metaData;
	DirData* dirData;
	
	
	lseek(fd, 0, SEEK_SET);
	lseek(fd, superBlock->get_block_size(), SEEK_CUR);
	X = superBlock->tellBlockNumber();
	
	metaData = new Metadata(superBlock->getNodeID(), directory_name, superBlock->get_filename_size(), EMPTY, DIRECTORY, parent_node_id);
	int current = superBlock->getBlockNumber(); 					// in which block we currently are 
	metaData->insertDatastream(superBlock->getBlockNumber());
	// Move to the first empty spot 
	moveXBlocks(X, fd, superBlock->get_block_size());
	metaData->write_Metadata(fd, superBlock->get_filename_size());	// write the folder's metadata 
	
	moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));	// move enough to be on the next block
	dirData = new DirData[superBlock->get_max_directory_file_number()];								// create an array holding the folders data 
	for(int i = 0; i < (int)superBlock->get_max_directory_file_number(); i++){	// write it on the next block 
		dirData[i].write_DirData(fd);
	}
	
	moveDifference(fd, superBlock->get_block_size(), superBlock->get_max_directory_file_number()*DirDataSize(superBlock->get_filename_size()));
	//add to parent
	if(parent_node_id > 0){
		cout<<"adding to parent mkdir"<<endl;
		Metadata *mtDt = new Metadata(superBlock->get_filename_size());
		DirData *drDt = new DirData[superBlock->get_max_directory_file_number()];
		bool flag = false;
		
		lseek(fd, 0, SEEK_SET);
		lseek(fd, superBlock->get_block_size(), SEEK_CUR);
		
		while(flag == false){
			mtDt->readMetadata(superBlock, fd);
			if(mtDt->getType() == DIRECTORY){
				if(mtDt->getNodeID() == parent_node_id){
					flag = true;
					moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
					for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
						drDt[i].readDirData(fd, superBlock->get_filename_size());
					}
					moveDifference(fd, superBlock->get_block_size(), superBlock->get_max_directory_file_number()*DirDataSize(superBlock->get_filename_size()));
					for(int i = 0; i < superBlock->get_max_directory_file_number();  i++){
						if(drDt[i].empty() == true){
							drDt[i].setFields(directory_name, metaData->getNodeID(), current, DIRECTORY,superBlock->get_filename_size());
							break;
						}
					}
					lseek(fd, -superBlock->get_filename_size(), SEEK_CUR);
					for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
						drDt[i].write_DirData(fd);
					}
					lseek(fd, -(superBlock->get_max_directory_file_number()*DirDataSize(superBlock->get_filename_size())), SEEK_CUR);
					for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
						drDt[i].readDirData(fd, superBlock->get_filename_size());
					}
				}
				else{
					int X = 1;
					moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
					moveXBlocks(X, fd, superBlock->get_block_size());
				}
			}
			else{ /* if it's not a directory */
				int X = mtDt->getOccupiedDatablocks();
				moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
				moveXBlocks(X, fd, superBlock->get_block_size());
			}
		}
	}
}

/* MFS_WORKWITH*/

int mfs_workwith(char* filename, Superblock* superBlock){ /* Open file requested , reads superBlock and returns the fd, if file doesn't exist, it created one */
	
	int fd;
	
	
	fd = open(filename, O_RDONLY, 0660);
	if(fd == -1){
		cout << "Request to open non-existing file, now creating..." << endl;
		mfs_create(filename, BLOCK_SIZE, FILENAME_SIZE, MAX_FILE_SIZE, MAX_DIRECTORY_FILE_NUMBER); //DEFINE THEM
		fd = open(filename, O_RDONLY, 0660);
	}
	
	//superBlock = new Superblock();
	superBlock->readSuperblock(fd);
	lseek(fd, 0, SEEK_SET);
	return fd;
	
}


/* MFS_CD*/
/*
// CHAOS cd apo edw p eimai mono se epomeno mono fakelo + na epistrefei metadata kai na grafei ta palia
void mfs_cd(char *target, Superblock *superBlock, DirData *dirData, int fd){
	
	int i = 0, X;
	bool flag = false;
	
	while(flag == false){
		if(strcmp(target, dirData[i].filename) == 0){	// found the target-directory
			X = dirData[i].metadataBlock;
			flag = true;
		}
		else{			 // searching for the target-directory 
			i++;
			if((unsigned int)i == superBlock->get_max_directory_file_number()){
				cout<<"Heaven Shall Burn"<<endl;
				return;
			}
		}
	}
	lseek(fd, 0, SEEK_SET);
	lseek(fd, superBlock->get_block_size(), SEEK_CUR);
	moveXBlocks(X+1, fd, superBlock->get_block_size());
	for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){ // diabasma pinaka DirData 
		dirData[i].readDirData(fd, superBlock->get_filename_size());
	}
}

*/


void mfs_cd(char *target, Metadata *metaData, DirData *dirData, int fd, Superblock *superBlock, bool back){
	
	bool flag = false; 
	bool flag2 = false;
	Metadata *mtDt = new Metadata(superBlock->get_filename_size());
	int id = metaData->getParentID();
//	DirData *dd = new DirData[superBlock->get_max_directory_file_number()];

	lseek(fd, 0, SEEK_SET);
	lseek(fd, superBlock->get_block_size(), SEEK_CUR);
	
	if(back == true){
		while(flag == false){ /* update the existing directory datastream and directory data */
			mtDt->readMetadata(superBlock, fd);
			if(mtDt->getType() == DIRECTORY){
				if(mtDt->getNodeID() == metaData->getNodeID()){
					flag = true;
					lseek(fd, -MetadataSize(superBlock), SEEK_CUR);
					metaData->write_Metadata(fd, superBlock->get_filename_size());
					moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
					for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
						dirData[i].write_DirData(fd);
					}
					moveDifference(fd, superBlock->get_block_size(), (superBlock->get_max_directory_file_number())*DirDataSize(superBlock->get_filename_size()));
				}
				else{
					int X = 1;
					moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
					moveXBlocks(X, fd, superBlock->get_block_size());
				}
			}
			else{
				int X = mtDt->getOccupiedDatablocks();
				moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
				moveXBlocks(X, fd, superBlock->get_block_size());
			}
		}
		/* Time to update the current directory*/
		while(flag2 == false){
			metaData->readMetadata(superBlock, fd);
			if(metaData->getType() == DIRECTORY){
				if(metaData->getNodeID() == id) {
					flag2 = true;
					moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
					for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
						dirData[i].readDirData(fd, superBlock->get_filename_size());
					}
					moveDifference(fd, superBlock->get_block_size(), (superBlock->get_max_directory_file_number())*DirDataSize(superBlock->get_filename_size()));
				}
				else{
					int X = 1;
					moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
					moveXBlocks(X, fd, superBlock->get_block_size());
				}
			}
			else{
				int X = mtDt->getOccupiedDatablocks();
				moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
				moveXBlocks(X, fd, superBlock->get_block_size());
			}
		}
	} // NEVER GO BACK ( oxi allo karvouno )
	else{
		while(flag == false){ /* update the existing directory datastream and directory data */
			mtDt->readMetadata(superBlock, fd);
			if(mtDt->getType() == DIRECTORY){
				if(mtDt->getNodeID() == metaData->getNodeID()){
					flag = true;
					lseek(fd, -MetadataSize(superBlock), SEEK_CUR);
					metaData->write_Metadata(fd, superBlock->get_filename_size());
					moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
					for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
						dirData[i].write_DirData(fd);
					}
					moveDifference(fd, superBlock->get_block_size(), (superBlock->get_max_directory_file_number())*DirDataSize(superBlock->get_filename_size()));
				}
				else{
					int X = 1;
					moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
					moveXBlocks(X, fd, superBlock->get_block_size());
				}
			}
			else{
				int X = mtDt->getOccupiedDatablocks();
				moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
				moveXBlocks(X, fd, superBlock->get_block_size());
			}
		}
			/* Time to update the current */
			while(flag2 == false){
				metaData->readMetadata(superBlock, fd);
				if(metaData->getType() == DIRECTORY){
					if(strcmp(metaData->getFilename(), target) == 0){
						flag2 = true;
						moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
						for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
							dirData[i].readDirData(fd, superBlock->get_filename_size());
						}
						moveDifference(fd, superBlock->get_block_size(), (superBlock->get_max_directory_file_number())*DirDataSize(superBlock->get_filename_size()));
					}
					else{
						int X = 1;
						moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
						moveXBlocks(X, fd, superBlock->get_block_size());
					}
				}
				else{
					int X = mtDt->getOccupiedDatablocks();
					moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
					moveXBlocks(X, fd, superBlock->get_block_size());
				}
			}
		}
}


/* MFS_LS */

 // LS mono ston current directory
void mfs_ls(DirData *dirData, Superblock *superBlock){
	bool flag = true;
	
	for(int i = 0; i < superBlock->get_max_directory_file_number() && flag == true; i++){
		if(dirData[i].empty() == false){	/* an dn einai adeio */
			cout << dirData[i].filename << endl;
		}
		else{
			flag = false;
		}
	}
	cout << endl;
}


void mfs_pwd(Metadata *metaData, Superblock *superBlock, int fd){
	
	int parentID = metaData->getParentID();
	bool flag = false;
	DirData *dirData = new DirData[superBlock->get_max_directory_file_number()];
	Metadata *mtDt = new Metadata(superBlock->get_filename_size());
	
	
	if(parentID > 0){
		lseek(fd, 0, SEEK_SET);
		lseek(fd, superBlock->get_block_size(), SEEK_CUR);
		while(flag == false){
			mtDt->readMetadata(superBlock, fd);
			if(mtDt->getType() == DIRECTORY){
				if(mtDt->getNodeID() == parentID){
					mfs_pwd(mtDt, superBlock, fd);
					flag = true;
				}
				else{
					int X = 1;
					moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
					moveXBlocks(X, fd, superBlock->get_block_size());
				}
			}
			else{ /* if it is a simple file */
				int X = mtDt->getOccupiedDatablocks();
				moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
				moveXBlocks(X, fd, superBlock->get_block_size());
			}
		}
	}
	else{ /* it is the home folder */
		cout << "/" << metaData->getFilename() << endl;
		return;
	}
	cout << "/" << metaData->getFilename() << endl;
	return;
}



void mfs_import(char *filename, char *targetFolder, Superblock *superBlock, int fd){ 
	
	int count = 0, last;
	FILE *fp;
	bool flag = false;
	int filesize, blocksize, X, fileID = superBlock->getNodeID(), metadatablock = superBlock->getBlockNumber();
	char* buffer;
	Metadata *metaData, *mtDt;
	DirData *dirData;
	
	fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	rewind(fp);
	
	blocksize = superBlock->get_block_size();
	buffer = new char[blocksize];
	
	mtDt = new Metadata(superBlock->get_filename_size()); /* Metadara Directory */
	dirData = new DirData[superBlock->get_max_directory_file_number()]; /* Directory Data */ 
	
	/* Find the directory in which the new file will be imported and update it , then write it again */
	
	lseek(fd, 0, SEEK_SET);
	lseek(fd, blocksize, SEEK_CUR); /* skip the superblock */
	while(flag == false){
			mtDt->readMetadata(superBlock, fd);
			if(mtDt->getType() == DIRECTORY){
				if(strcmp(mtDt->getFilename(), targetFolder) == 0){
					moveDifference(fd, blocksize, MetadataSize(superBlock));
					for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){ // diabasma pinaka DirData 
						dirData[i].readDirData(fd, superBlock->get_filename_size());
					}
					flag = true;
				}
				else{
					X = 1;
					moveDifference(fd, blocksize, MetadataSize(superBlock));
					moveXBlocks(X, fd, superBlock->get_block_size());
				}

			}
			else{
				int X = mtDt->getOccupiedDatablocks();
				moveDifference(fd, blocksize, MetadataSize(superBlock));
				moveXBlocks(X, fd, superBlock->get_block_size());
			}
	}
	if(flag == false){
		cout<< "Problem finding parent folder in import" << endl;
		return;
	}
	
	mtDt->changeSize(filesize);  /* Metadata for Directory */
	for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
		if(dirData[i].empty() == true){
			dirData[i].setFields(filename, fileID, metadatablock, SIMPLE_FILE, superBlock->get_filename_size());
		}
	}
	
	lseek(fd, -(superBlock->get_max_directory_file_number()*DirDataSize(superBlock->get_filename_size())), SEEK_CUR); /* go back at the beginning of the block */
	lseek(fd, -superBlock->get_block_size(), SEEK_CUR); /* go back one block to write the metadata */
	
	mtDt->write_Metadata(fd, superBlock->get_filename_size());
	moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
	
	for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
		dirData[i].write_DirData(fd);
	}
	
	/* go at the end of the .mfs file and after you create the metadata for the file, move the difference and write its data, when go back to write its metadata */
	
	lseek(fd, 0, SEEK_SET);
	lseek(fd, superBlock->get_block_size(), SEEK_CUR);
	
	moveXBlocks(superBlock->tellBlockNumber(), fd, superBlock->get_block_size());
	
	metaData = new Metadata(fileID, filename, superBlock->get_filename_size(), filesize, SIMPLE_FILE, ORPHAN);
	
	lseek(fd, superBlock->get_block_size(), SEEK_CUR);

	/* Writing the file's data */
	
	while(last = fread(buffer, blocksize, 1, fp) > 0){
		write(fd, buffer, blocksize*sizeof(char));
		metaData->insertDatastream(superBlock->getBlockNumber());
		count++;
	}
	/* Going back enough to write the file's metadata */
	
	moveDifference(fd, superBlock->get_block_size(), last);
	lseek(fd, -(count +1)*superBlock->get_block_size(), SEEK_CUR);
	metaData->write_Metadata(fd, superBlock->get_block_size());
	fclose(fp);
}

/* MFS_ EXPORT */
void mfs_export(char *filename, char *targetFolder, Superblock *superBlock, int fd){
	
	/* filename: the file to search for, targetFolder: linux folder */
	
	bool flag = false;
	int blocksize, filesize, X, blockNumber, extraB;
	char *buffer;
	FILE *fp;
	Metadata *metaData;
	
	blocksize = superBlock->get_block_size();
	buffer = new char[blocksize];
	fp = fopen(targetFolder, "wb");
	
	lseek(fd, 0, SEEK_SET);
	lseek(fd, blocksize, SEEK_CUR);
	
	while(flag == false){ /* search for the file */
		metaData->readMetadata(superBlock, fd);
		if(metaData->getType() == SIMPLE_FILE){
			if(strcmp(metaData->getFilename(), filename) == 0){ /* processed filename */
				moveDifference(fd, blocksize, MetadataSize(superBlock));
				flag = true;
			}
			else{
				X = metaData->getOccupiedDatablocks();
				moveDifference(fd, blocksize, MetadataSize(superBlock));
				moveXBlocks(X, fd, blocksize);
			}
		}
		else{
			X = 1;
			moveDifference(fd, blocksize, MetadataSize(superBlock));
			moveXBlocks(X, fd, blocksize);
		}
	}
	if(flag == false){
		cout << "File to export not found" << endl;
		return;
	}
	blockNumber = metaData->getSize()/blocksize;
	extraB = metaData->getSize() - blockNumber*blocksize;
	
	for(int i = 0; i < blockNumber; i++){
		read(fd, buffer, blocksize);
		fwrite(buffer, blocksize, 1, fp);
	}
	if(extraB > 0){
		read(fd, buffer, extraB);
		fwrite(buffer, extraB, 1, fp);
	}
	fclose(fp);
}


void mfs_cp(char *filename, char *targetFolder, Superblock *superBlock, int fd){
	
	
	bool flag = false;
	int filesize, blocksNeeded, extra = 0, fileID, metadatablock;
	char **buffer;
	
	Metadata *source = new Metadata(superBlock->get_filename_size());
	Metadata *target;
	
	lseek(fd, 0, SEEK_SET);
	lseek(fd, superBlock->get_block_size(), SEEK_CUR);
	
	while(flag == false){ /* find the source's metadata */
		source->readMetadata(superBlock, fd);
		if(source->getType() == SIMPLE_FILE){
			if(strcmp(source->getFilename(), filename) == 0){
				flag = true;
				moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
			}
			else{
				int X = source->getOccupiedDatablocks();
				moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
				moveXBlocks(X, fd, superBlock->get_block_size());
			}
		}
		else{
			int X = 1;
			moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
			moveXBlocks(X, fd, superBlock->get_block_size());
		}
	}
	
	// source found
	
	filesize = source->getSize();
	blocksNeeded = filesize/superBlock->get_block_size();
	if(filesize%superBlock->get_block_size() != 0){
		blocksNeeded++;
		extra = filesize - blocksNeeded*superBlock->get_block_size();
	}
	
	/* copy the file at the end of the .mfs file */
	buffer = new char*[blocksNeeded];
	for(int i = 0; i < blocksNeeded; i++){
		buffer[i] = new char[superBlock->get_block_size()];
		read(fd, buffer[i], superBlock->get_block_size());
	}
	
	lseek(fd, 0, SEEK_SET);
	lseek(fd, superBlock->get_block_size(), SEEK_CUR);
	moveXBlocks(superBlock->tellBlockNumber(), fd, superBlock->get_block_size());
	
	
	fileID = superBlock->getNodeID();
	metadatablock = superBlock->getBlockNumber();
	target = new Metadata(fileID, filename, superBlock->get_filename_size(), filesize, SIMPLE_FILE, ORPHAN);
	moveXBlocks(1, fd, superBlock->get_block_size());
	
	for(int i = 0; i < blocksNeeded; i++){
		write(fd, buffer[i], superBlock->get_block_size());
		target->insertDatastream(superBlock->getBlockNumber());
	}
	moveDifference(fd, superBlock->get_block_size(), extra);
	lseek(fd, -(blocksNeeded+1)*superBlock->get_block_size(), SEEK_CUR);
	
	/* find and update the directory's metadata */
	
	lseek(fd, 0, SEEK_SET);
	lseek(fd, superBlock->get_block_size(), SEEK_CUR);
	flag = false;
	Metadata *mtDt = new Metadata(superBlock->get_filename_size());
	DirData *dd = new DirData[superBlock->get_max_directory_file_number()];
	
	while(flag == false){ /* find the directory */
		mtDt->readMetadata(superBlock, fd);
		if(mtDt->getType() == DIRECTORY){
			if(strcmp(mtDt->getFilename(), targetFolder) == 0){
				flag = true;
				moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
				for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
					dd[i].readDirData(fd, superBlock->get_filename_size());
				}
			}
			else{
				int X = 1;
				moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
				moveXBlocks(X, fd, superBlock->get_block_size());
			}
		}
		else{
			int X = mtDt->getOccupiedDatablocks();
			moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
			moveXBlocks(X, fd, superBlock->get_block_size());
		}
	} /* Directory Found */
	
	mtDt->changeSize(filesize);
	bool exit = false;
	for(int i = 0; i < superBlock->get_max_directory_file_number() && exit == false; i++){
		if(dd[i].empty() == true){
			dd[i].setFields(filename, fileID, metadatablock, SIMPLE_FILE, superBlock->get_filename_size());
			exit = true;
		}
	}
	lseek(fd, -(superBlock->get_max_directory_file_number()*DirDataSize(superBlock->get_filename_size())), SEEK_CUR);
	lseek(fd, -superBlock->get_block_size(), SEEK_CUR);
	
	mtDt->write_Metadata(fd, superBlock->get_filename_size());
	moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
	for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
		dd[i].write_DirData(fd);
	}
}