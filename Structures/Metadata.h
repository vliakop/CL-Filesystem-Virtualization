#ifndef METADATA_H
#define METADATA_H

#include "Datastream.h"
#include <time.h>

class Metadata{

private:

    unsigned int nodeid;			/* id of file, directory, link */
    char *filename;
    unsigned int size;				/* size of file, directory or link in bytes */
    unsigned int type;				/* type of file:0, directory:1, link:2 */
    unsigned int parent_nodeid;		/* id of parent directory or directory of a file */
    time_t creation_time;			/* time of creation of a file, directory, link */
    time_t access_time;				/* time of last access of a file, directory, link */
    time_t modification_time;		/* time of last modification of a file, directory or link */
    Datastream data;				/* keep the number of blocks of a file, directory or link */

public:

    Metadata(unsigned int nodeid, char const* filename, unsigned int filename_size, unsigned int size, unsigned int type, unsigned int parent_nodeid);
    Metadata(int filename_size);
    ~Metadata();

    /*Mutators*/

    void set_filename(char const* filename, unsigned int filename_size);
    void write_Metadata(int mfs_fileDescriptor, int filename_size);

    unsigned int getNodeID();
    int getType();
    int getOccupiedDatablocks();
    int getParentID();
    char* getFilename();
    int getSize();
    void insertDatastream(int blockNumber);
    void changeSize(int size);
    void readMetadata(Superblock *superBlock, int fd);
};

#endif
