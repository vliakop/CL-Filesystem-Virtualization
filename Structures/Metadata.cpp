#include <cstring>
#include "Metadata.h"
#include <iostream>
#include <unistd.h>

#define ORPHAN -1

using namespace std;

Metadata::Metadata(unsigned int nodeid, char const* name, unsigned int filename_size, unsigned int size, unsigned int type, unsigned int parent_nodeid):data(){

    this->nodeid = nodeid;
    filename = new char[filename_size];
    strcpy(filename, name);
    for(int i = strlen(filename); i < (int)filename_size-1; i++){
        this->filename[i] = ' ';
    }
    this->filename[filename_size-1] = '\0';
    this->size = size;
    this->type = type;
    this->parent_nodeid = parent_nodeid;
    time(&creation_time);   	/*get current time*/
    time(&access_time);   		/*get current time*/
    time(&modification_time);   /*get current time*/
}

Metadata::Metadata(int filename_size):data(){
    nodeid = -1;
    filename = new char[filename_size];
    memset(filename , ' ', filename_size);
    filename[filename_size-1]='\0';
    size = -1;
    type = -1;
    parent_nodeid = ORPHAN;
    time(&creation_time);   	/*get current time*/
    time(&access_time);   		/*get current time*/
    time(&modification_time);   /*get current time*/
}

Metadata::~Metadata(){

    delete filename;
    cout << "Metadata is destroyed" << endl;
}

void Metadata::set_filename(char const *filename, unsigned int filename_size){

    strcpy( this->filename, filename);
    for(int i = strlen(filename); i < (int)filename_size-1; i++){
        this->filename[i] = ' ';
    }
    this->filename[filename_size-1] = '\0';
}

void Metadata::write_Metadata(int mfs_fileDescriptor, int filename_size){
/*	Pro: Dhmioyrgia mfs arxeio
 *	Meta: Engrafi tou Metadata sto mfs arxeiou*/

    write(mfs_fileDescriptor, &nodeid, sizeof(int));
    write(mfs_fileDescriptor, filename, filename_size*sizeof(char));
    write(mfs_fileDescriptor, &size, sizeof(int));
    write(mfs_fileDescriptor, &type, sizeof(int));
    write(mfs_fileDescriptor, &parent_nodeid, sizeof(int));
    write(mfs_fileDescriptor, &creation_time, sizeof(time_t));
    write(mfs_fileDescriptor, &access_time, sizeof(time_t));
    write(mfs_fileDescriptor, &modification_time, sizeof(time_t));
    data.write_Datastream(mfs_fileDescriptor);

}

unsigned int Metadata::getNodeID(){

    return nodeid;
}

int Metadata::getType(){

    return type;
}

int Metadata::getOccupiedDatablocks(){

    return data.getOccupiedDatablocks();
}


int Metadata::getParentID(){

    return parent_nodeid;
}

char* Metadata::getFilename(){

    return filename;
}

int Metadata::getSize(){

    return size;
}

void Metadata::insertDatastream(int blockNumber){

    data.insertDatastream(blockNumber);
}


void Metadata::changeSize(int size){

    this->size += size;
}


void Metadata::readMetadata(Superblock *superBlock, int fd){

    //cout<<"READING METADATA"<<endl;

    read(fd, &nodeid, sizeof(int));
//	cout<<"nodeid "<<nodeid<<endl;

    read(fd, filename, superBlock->get_filename_size()*sizeof(char));
//	cout<<"filename "<<filename<<endl;

    read(fd, &size, sizeof(int));
//	cout<<"size "<<size<<endl;

    read(fd, &type, sizeof(int));
//	cout<<"type "<<type<<endl;

    read(fd, &parent_nodeid, sizeof(int));
//	cout<<"parent_nodeid "<<parent_nodeid<<endl;

    read(fd, &creation_time, sizeof(time_t));
//	cout<<"creationg_time "<<creation_time<<endl;

    read(fd, &access_time, sizeof(time_t));
//	cout<<"access_time "<<access_time<<endl;

    read(fd, &modification_time, sizeof(time_t));
//	cout<<"modification_time "<<modification_time<<endl;

    data.readDatastream(superBlock, fd);
}