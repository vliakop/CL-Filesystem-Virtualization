#ifndef COMMANDS_H
#define COMMANDS_H

#include "../Structures/Superblock.h"
#include "../Structures/Metadata.h"
#include "../Structures/DirData.h"

bool getCommand(char *inputStr, Superblock* superBlock, Metadata *currDirMetadata, DirData *currDirData, int *fd, bool &created);
void makeAcceptedStr(char *filename, char *argStr1, int file_name_size);

#endif
