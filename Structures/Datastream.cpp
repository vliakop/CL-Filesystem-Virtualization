#include "Datastream.h"
#include <iostream>
#include <unistd.h>

#define DATABLOCK_NUM 10

using namespace std;

Datastream::Datastream(){

    occupiedDatablocks = 0;
    datablocks = new int(DATABLOCK_NUM);
    for(int i=0;i<DATABLOCK_NUM;i++){
        datablocks[i] = -1;
    }
}

Datastream::~Datastream(){

    delete datablocks;
    cout << "Datastream is destroyed" << endl;
}

//Accessors

int Datastream::getOccupiedDatablocks(){

    return occupiedDatablocks;
}

//Mutators

void Datastream::write_Datastream(int fd){

    write(fd, &occupiedDatablocks, sizeof(int));
    write(fd, datablocks, DATABLOCK_NUM*sizeof(int));
}

void Datastream::insertDatastream(int blockNumber){

    if(occupiedDatablocks == DATABLOCK_NUM){
        cout << "Datastream::insertDatastream problem" << endl;
        return;
    }
    occupiedDatablocks++;
    datablocks[occupiedDatablocks - 1] = blockNumber;
}


void Datastream::readDatastream(Superblock *superBlock, int fd){

//	cout<<endl<<"READING DATASTREAM"<<endl;

    read(fd, &occupiedDatablocks, sizeof(int));
//	cout<<"occupiedDatablocks "<<occupiedDatablocks<<endl;

    read(fd, datablocks, DATABLOCK_NUM*sizeof(int));
//	for(int i = 0; i < DATABLOCK_NUM; i++){
//		cout<<"datablocks["<<i+1<<"] "<<datablocks[i]<<endl;
//	}

}