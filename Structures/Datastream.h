#ifndef DATASTREAM_H
#define DATASTREAM_H

class Superblock; // Class forwarding

class Datastream{

private:

    int occupiedDatablocks;			/* number of occupied array elements */
    int *datablocks;				/* int array that keeps the id of the blocks of a file, directory or link */

public:

    Datastream();
    ~Datastream();

    void write_Datastream(int fd);
    int getOccupiedDatablocks();
    void insertDatastream(int blockNumber);
    void readDatastream(Superblock *superBlock, int fd);
};


#endif
