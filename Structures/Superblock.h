#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

class Superblock{

private:

    unsigned int block_size;					/* megethos block se bytes */
    unsigned int filename_size;					/* megethos onomatos arxeiou se bytes */
    unsigned int max_file_size;					/* megisto megethos arxeiwn se bytes */
    unsigned int max_directory_file_number;		/* megistos arithmos arxeiwn se kathe directory */
    unsigned int Datablock_Num;					/* to megethos tou datablocks array */
    unsigned int nodeCount;						/*afksanate me kathe directory, link h arxeio kai xreisimopoihte gia nodeid*/
    unsigned int blockCount;

public:

    Superblock(unsigned int block_size, unsigned int filename_size, unsigned int max_file_size, unsigned int max_directory_file_number, unsigned int Datablock_Num);
    Superblock();
    ~Superblock();

    /*Accessors*/

    unsigned int get_block_size();
    unsigned int get_filename_size();
    unsigned int get_max_file_size();
    unsigned int get_max_directory_file_number();
    unsigned int get_Datablock_Num();
    unsigned int getBlockNumber();
    unsigned int tellBlockNumber();

    /*Mutators*/

    unsigned int getNodeID();
    void increaseBlockCount();
    void write_Superblock(int mfs_fileDescriptor);
    void readSuperblock(int fd);

};

#endif
