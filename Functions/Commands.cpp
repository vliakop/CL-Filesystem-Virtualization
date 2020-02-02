#define commandSize 32
#define argSize 256

#include <cstring>
#include <cstdlib>
#include <iostream>
#include "Commands.h"
#include "MyFunctions.hpp"

using namespace std;

bool getCommand(char *inputStr, Superblock* superBlock, Metadata *currDirMetadata, DirData *currDirData, int *fd, bool &created){

    char  *commandStr, *temp, *argStr1, *argStr2, *filename1, *filename2;
    commandStr = new char[commandSize];
    argStr1 = new char[argSize];
    argStr2 = new char[argSize];


    temp = strtok(inputStr, " \n");
    strcpy(commandStr, temp);
    if(strcmp(commandStr, "mfs_workwith") == 0){ /* mfs_workwith */
        temp = strtok(NULL, " ");
        strcpy(argStr1, temp);
        *fd = mfs_workwith(argStr1, superBlock);

        currDirMetadata = new Metadata(superBlock->get_filename_size());
        created = true;


    }

    else if(strcmp(commandStr, "mfs_mkdir") == 0){ /* mfs_mkdir */
        temp = strtok(NULL, " ");
        strcpy(argStr1, temp);
        filename1 = new char[superBlock->get_filename_size()];
        makeAcceptedStr(filename1, argStr1, superBlock->get_filename_size());
        mfs_mkdir(filename1, *fd, superBlock, currDirMetadata->getNodeID());
        delete filename1;
    }
    else if(strcmp(commandStr, "mfs_create") == 0){ /* mfs_create */
        int bs = -1, fns = -1, mfs = -1, mdfn = -1;
        bool flag = true;
        temp = strtok(NULL, "\n ");
        strcpy(argStr1, temp);
        while(flag == true){
            if(strcmp(argStr1, "-bs") == 0){
                temp = strtok(NULL, "\n ");
                strcpy(argStr1, temp);
                bs = atoi(argStr1);
                temp = strtok(NULL, "\n ");
            }
            else if(strcmp(argStr1, "-fns") == 0){
                temp = strtok(NULL, "\n ");
                strcpy(argStr1, temp);
                fns = atoi(argStr1);
                temp = strtok(NULL, "\n ");
            }
            else if(strcmp(argStr1, "-mfs") == 0){
                temp = strtok(NULL, "\n ");
                strcpy(argStr1, temp);
                mfs = atoi(argStr1);
                temp = strtok(NULL, "\n ");
            }
            else if(strcmp(argStr1, "-mdfn") == 0){
                temp = strtok(NULL, "\n ");
                strcpy(argStr1, temp);
                mdfn = atoi(argStr1);
                temp = strtok(NULL, "\n ");
            }
            else{
                flag = false;
            }
            strcpy(argStr1, temp);
        }
        strcpy(argStr1, temp);
        if(bs == -1){
            bs = BLOCK_SIZE;
        }
        if(fns == -1){
            fns = FILENAME_SIZE;
        }
        if(mfs == -1){
            mfs =  MAX_FILE_SIZE;
        }
        if(mdfn == -1){
            mdfn = MAX_DIRECTORY_FILE_NUMBER;
        }
        filename1 = new char[fns];
        makeAcceptedStr(filename1, argStr1, fns);
        mfs_create(filename1, bs, fns, mfs, mdfn);
        delete filename1;
    }
    else if(strcmp(commandStr, "mfs_cd") == 0){
        bool back = false;
        temp = strtok(NULL, "\n ");
        strcpy(argStr2, temp);
        if(strcmp(argStr2, "..") == 0){
            back = true;
        }
        while(temp != NULL && back == false){
            strcpy(argStr1, temp);
            temp = strtok(NULL, "\n /");
        }
        filename1 = new char[superBlock->get_filename_size()];
        makeAcceptedStr(filename1, argStr1, superBlock->get_filename_size());
        mfs_cd(filename1, currDirMetadata, currDirData, *fd, superBlock, back);
        delete filename1;
    }
    else if(strcmp(commandStr, "mfs_ls") == 0){
        mfs_ls(currDirData, superBlock);
    }
    else if(strcmp(commandStr, "mfs_pwd") == 0){
        mfs_pwd(currDirMetadata, superBlock, *fd);
    }
    else if(strcmp(commandStr, "mfs_import") == 0){
        temp = strtok(NULL, " \n");
        strcpy(argStr1, temp);
        filename1 = new char[superBlock->get_filename_size()];
        makeAcceptedStr(filename1, argStr1, superBlock->get_filename_size());

        temp = strtok(NULL, " \n");
        while(temp != NULL){
            strcpy(argStr2, temp);
            temp = strtok(NULL, "\n /");
        }
        filename2 = new char[superBlock->get_filename_size()];
        makeAcceptedStr(filename2, argStr2, superBlock->get_filename_size());
        mfs_import(filename1, filename2, superBlock, *fd);
        delete filename1;
        delete filename2;
    }
    else if(strcmp(commandStr, "mfs_export") == 0){
        temp = strtok(NULL, " \n");
        strcpy(argStr1, temp);
        filename1 = new char[superBlock->get_filename_size()];
        makeAcceptedStr(filename1, argStr1, superBlock->get_filename_size());
        temp = strtok(NULL, " \n");
        strcpy(argStr2, temp);
        mfs_export(filename1, argStr2, superBlock, *fd);
        delete filename1;
    }
    else if(strcmp(commandStr, "mfs_cp") == 0){

        char *temp2 , *kati;
        kati = new char[argSize];

        temp = strtok(NULL, " \n");
        strcpy(argStr1, temp);
        temp = strtok(NULL, " \n");
        strcpy(argStr2, temp);

        temp2 = strtok(argStr1, "/ \n");
        while(temp2 != NULL){
            strcpy(kati, temp2);
            temp2 = strtok(NULL, "/ \n");
        }
        filename1 = new char[superBlock->get_filename_size()];
        makeAcceptedStr(filename1, kati, superBlock->get_filename_size());

        temp2 = strtok(argStr2, "/ \n");
        while(temp2 != NULL){
            strcpy(kati, temp2);
            temp2 = strtok(NULL, "/ \n");
        }
        filename2 = new char[superBlock->get_filename_size()];
        makeAcceptedStr(filename2, kati, superBlock->get_filename_size());

        mfs_cp(filename1, filename2, superBlock, *fd);
        delete filename1;
        delete filename2;
        delete kati;
    }
    else if(strcmp(commandStr, "mfs_mv") == 0){
        cout<<"Command not yet ready for implementation"<<endl;
    }
    else if(strcmp(commandStr, "mfs_touch") == 0){
        cout<<"Command not supported"<<endl;
    }
    else if(strcmp(commandStr, "mfs_cat") == 0){
        cout<<"Command not supported"<<endl;
    }
    else if(strcmp(commandStr, "mfs_rm") == 0){
        cout<<"Command not supported"<<endl;
    }
    else{
        delete commandStr;
        delete argStr1;
        delete argStr2;
        return true;
    }
    delete commandStr;
    delete argStr1;
    delete argStr2;
    return false;

}


void makeAcceptedStr(char *filename ,char *argStr1, int file_name_size){

    strcpy(filename, argStr1);
    if(filename[strlen(filename)-1] == '\n'){
        filename[strlen(filename)-1] = ' ';
    }
    for(int i = strlen(argStr1); i < file_name_size; i++){
        filename[i] = ' ';
    }
    filename[file_name_size - 1] = '\0';
}
