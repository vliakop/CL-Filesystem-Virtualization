#include <iostream>
#include <stdio.h>
#include "Structures/Metadata.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Functions/MyFunctions.hpp"
#include "Functions/Commands.h"
#include "Structures/DirData.h"
#include <cstring>
#include <unistd.h>

#define inputSize 512


using namespace std;


int main(){
	
	bool end = false, created = false, once = false;
	int filenameSize, fd;
	char *inputStr;
	
	Superblock *superBlock = new Superblock();
	Metadata *currDirMetadata ;
	DirData *currDirData = NULL;
	
	inputStr = new char[inputSize];

	
	while(end == false){
		cout<<endl<<"Please, give a command:"<<endl;
		fgets(inputStr, inputSize, stdin);
		end = getCommand(inputStr, superBlock, currDirMetadata, currDirData, &fd, created);
		if(created == true && once == false){
			lseek(fd, 0, SEEK_SET);
			lseek(fd, superBlock->get_block_size(), SEEK_CUR);
			currDirMetadata->readMetadata(superBlock , fd);
			moveDifference(fd, superBlock->get_block_size(), MetadataSize(superBlock));
			currDirData = new DirData[superBlock->get_max_directory_file_number()];
			for(int i = 0; i < superBlock->get_max_directory_file_number(); i++){
				currDirData[i].readDirData(fd, superBlock->get_filename_size());
			}
			once = true;
		}
		if(end == false){
			cout<<"Command Executed"<<endl<<"Now exiting..."<<endl;
			close(fd);
			return 0;
		}
	}
	
}



