#include <unistd.h>
#include "Superblock.h"

Superblock::Superblock(unsigned int block_size, unsigned int filename_size, unsigned int max_file_size, unsigned int max_directory_file_number, unsigned int Datablock_Num){

    this->block_size = block_size;
    this->filename_size = filename_size;
    this->max_file_size = max_file_size;
    this->max_directory_file_number = max_directory_file_number;
    this->Datablock_Num = Datablock_Num;
    nodeCount = blockCount = 0;

}

Superblock::Superblock(){

    block_size = filename_size = max_file_size = max_directory_file_number = Datablock_Num = -1;
    nodeCount = blockCount = 0;
}

Superblock::~Superblock(){
}

/* Accessors */


unsigned int Superblock::get_block_size(){

    return block_size;
}

unsigned int Superblock::get_filename_size(){

    return filename_size;
}

unsigned int Superblock::get_max_file_size(){

    return max_file_size;
}

unsigned int Superblock::get_max_directory_file_number(){
    return max_directory_file_number;
}


unsigned int Superblock::get_Datablock_Num(){

    return Datablock_Num;
}

unsigned int Superblock::getNodeID(){

    nodeCount++;
    return nodeCount;
}

unsigned int Superblock::tellBlockNumber(){

    return blockCount;
}

/* Mutators */

unsigned int Superblock::getBlockNumber(){

    blockCount++;
    return blockCount;
}



void Superblock::write_Superblock(int mfs_fileDescriptor){
/*	Pro: Dhmioyrgia mfs arxeio
 *	Meta: Engrafi tou superBlock sto mfs arxeiou*/

    write(mfs_fileDescriptor, &block_size, sizeof(int));
    write(mfs_fileDescriptor, &filename_size, sizeof(int));
    write(mfs_fileDescriptor, &max_file_size, sizeof(int));
    write(mfs_fileDescriptor, &max_directory_file_number, sizeof(int));
    write(mfs_fileDescriptor, &Datablock_Num, sizeof(int));
    write(mfs_fileDescriptor, &nodeCount, sizeof(int));
    write(mfs_fileDescriptor, &blockCount, sizeof(int));

}



void Superblock::readSuperblock(int fd){

    //cout<<"READING SUPERBLOCK"<<endl;

    read(fd, &block_size, sizeof(int));
    //cout << "block_size " << block_size << endl;

    read(fd, &filename_size, sizeof(int));
//	cout << "filename_size " << filename_size << endl;

    read(fd, &max_file_size, sizeof(int));
    //cout << "max_file_size " << filename_size << endl;

    read(fd, &max_directory_file_number, sizeof(int));
    //cout << "max_directory_file_number " << max_directory_file_number << endl;

    read(fd,  &Datablock_Num, sizeof(int));
    //cout << "Datablock_Num " << Datablock_Num << endl;

    read(fd, &nodeCount, sizeof(int));
    //cout << "nodeCount " << nodeCount << endl;

    read(fd, &blockCount, sizeof(int));
    //cout << "blockCount " << blockCount << endl<<endl;
}