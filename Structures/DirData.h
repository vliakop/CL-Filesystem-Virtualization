#ifndef DIRDATA_H
#define DIRDATA_H

#include "Superblock.h"

struct DirData{

public:

    char* filename;
    int fileID;
    int metadataBlock;
    int type;

    DirData(Superblock *superBlock);
    DirData();
    ~DirData();

    void setFields(char* filename, int fileID ,int metadataBlock, int type, int filename_size);
    void write_DirData(int fd);
    char* getFilename();
    void readDirData(int fd, int filename_size);
    bool empty();
    void insert(char *filename, int fileID, int metadatablock, int simple_file);
    void readDirDataTest(Superblock *superBlock, int fd);

};

#endif
