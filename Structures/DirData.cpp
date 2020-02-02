#include <unistd.h>
#include <cstring>
#include "DirData.h"

#define FILENAME_SIZE 32

DirData::DirData(Superblock *superBlock){

    filename = new char[superBlock->get_filename_size()];
    memset(filename, ' ', superBlock->get_filename_size());
    filename[superBlock->get_filename_size()-1] = '\0';
    fileID = metadataBlock = 0;
    type = -1;
}

DirData::DirData(){

    filename = new char[FILENAME_SIZE];
    memset(filename, ' ', FILENAME_SIZE);
    filename[FILENAME_SIZE - 1]='\0';
    fileID = metadataBlock = 0;
    type = -1;
}

DirData::~DirData(){

    delete filename;
}


void DirData::setFields(char *filename, int fileID, int metadataBlock, int type, int filename_size){
/*	Pro: Dhmioyrgia mfs arxeio
 *	Meta: Anathetoume times sta orismata tou struct DirData*/

    strcpy(this->filename, filename);
    for(int i = strlen(filename); i < (int)filename_size-1; i++){
        this->filename[i] = ' ';
    }
    this->filename[filename_size-1] = '\0';
//	cout<<"filename "<<this->filename<<" size "<<strlen(this->filename)<<endl;
    this->fileID = fileID;
//	cout<<"file ID "<<this->fileID<<endl;
    this->metadataBlock = metadataBlock;
//	cout<<"metadataBlock "<<this->metadataBlock<<endl;
    this->type = type;
//	cout<<"type "<<this->type<<endl;
}

void DirData::write_DirData(int fd){


    write(fd, filename, (strlen(filename)+1)*sizeof(char));
//	cout<<"filenamesize "<<strlen(filename)<<"   filename "<<filename<<endl;

    write(fd, &fileID, sizeof(int));
//	cout<<"FILEID "<<fileID<<endl;

    write(fd, &metadataBlock, sizeof(int));
//	cout<<"metadatablock "<<metadataBlock<<endl;

    write(fd, &type, sizeof(int));
//	cout<<"type "<< type<<endl<<endl;

}

char* DirData::getFilename(){

    return filename;
}

void DirData::readDirData(int fd, int filename_size){


    read(fd, filename, filename_size*sizeof(char));
//	cout<<"filenamesize "<<strlen(filename)+1<<endl;
//	cout<<"filename "<<filename<<endl;

    read(fd, &fileID, sizeof(int));
//	cout<<"fileID "<<fileID<<endl;

    read(fd, &metadataBlock, sizeof(int));
//	cout<<"metadataBlock "<<metadataBlock<<endl;

    read(fd, &type, sizeof(int));
//	cout<<"type "<<type<<endl;

}

bool DirData::empty(){

    if(type < 0){
        return true;
    }
    return false;

}

/*
void DirData::insert(char *filename, int fileID, int metadatablock, int type){

	strcpy(this->filename, filename);
	for(int i = strlen(filename); i < (int)filename_size-1; i++){
		this->filename[i] = ' ';
	}
	this->fileID = fileID;
	this->metadataBlock = metadataBlock;
	this->type = type;

}
*/

void DirData::readDirDataTest(Superblock *superBlock, int fd){

    int i;
    char array [superBlock->get_filename_size()];

    read(fd,array, superBlock->get_filename_size()*sizeof(char));
    // cout << "filename " << i << endl;

    read(fd,&i, sizeof(int));
    // cout << "fileID " << i << endl;

    read(fd,&i, sizeof(int));
    // cout << "metadataBlock " << i << endl;

    read(fd,&i, sizeof(int));
    // cout << "type " << i << endl;

}