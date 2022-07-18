import java.util.*;

public class SuperBlock {
    // explain this and use this *
    private final int defaultInodeBlocks = 64;
    public int totalBlocks; // the number of disk blocks
    public int totalInodes; // the number of inodes
    public int freeList; // the block number of the free list's head
    // the freeList is where you start storing Inodes
    // when you add a new Inode, you change the end of the freeList's head

    // block 0: first 512 bytes reserved into superblock
    // block 1 (may also include blocks 2, 3, and 4): first 32 bytes is reserved for Directory Inode 
    //          and remaining 480 bytes are used for other Inode objects; 16 Inode objects, each of size 32 bytes,
    //          can be stored in a block; each block takes 16 Inodes
    // block 2 and beyond (may start at block 5 depending on Inodes): store actual data

    public SuperBlock( int diskSize ) {
        // read the superblock from disk. 
        // check disk contents are valid.
        // if invalid, call format( ). 
        byte[] superBlock = new byte[Disk.blockSize];
        // extract data from disk and put into superblock's 
        // byte array
        SysLib.rawread(0, superBlock);
        // allocate totalBlocks from 0-4 in memory
        totalBlocks = SysLib.bytes2int(superBlock, 0);
        // allocate totalInodes 4-8 in memory 
        totalInodes = SysLib.bytes2int(superBlock, 4);
        // allocate freeList 8-12 in memory
        freeList = SysLib.bytes2int(superBlock, 8);

        // check if the totalBlocks are equal to the diskSize
        if (totalBlocks == diskSize && totalInodes > 0 && freeList >= 2) {
            // disk contents are valid
            return;
        } else {
            // need to format disk 
            totalBlocks = diskSize;
            format(defaultInodeBlocks);
        }
    }
    
    void sync( ) {
        // write back in-memory superblock to disk
        byte[] superBlock = new byte[Disk.blockSize];

        SysLib.int2bytes(totalBlocks, superBlock, 0);
        SysLib.int2bytes(totalInodes, superBlock, 4);
        SysLib.int2bytes(freeList, superBlock, 8);

        SysLib.rawwrite(0, superBlock);
    }
    
    void format( int files ) {
        // initialize the superblock 
        // initialize each inode and immediately write it back to disk
        // initialize free blocks

        byte[] superBlock = new byte[Disk.blockSize];
        freeList = 2 + (files - 1) / 16; // 16 bytes per block
        totalInodes = files;
        SysLib.int2bytes(totalBlocks, superBlock, 0);
        SysLib.int2bytes(files, superBlock, 4);
        SysLib.int2bytes(freeList, superBlock, 8);
        SysLib.rawwrite(0, superBlock);

        // initialize inodes by clearing data between start of block 1
        // and end of last block that currently contains inodes; then set
        // totalInodes to files because number of inodes has been updated
        for (int i = 1; i < freeList; i++) {
            byte[] block = new byte[Disk.blockSize]; // creating empty buffer
            SysLib.rawwrite(i, block);
        }

        // use freeList because that is first block where there exists data
        for(int i = freeList; i < totalBlocks; i++) {
            byte[] block = new byte[Disk.blockSize];
            SysLib.short2bytes((short)(i + 1), block, 0);
            SysLib.rawwrite(i, block);
        }
    }
    
    public int getFreeBlock( ) {
        // get a new free block from the freelist
        int freeBlockNumber = freeList;
        byte[] block = new byte[Disk.blockSize];
        SysLib.rawread(freeList, block);
        freeList = SysLib.bytes2short(block, 0);
        return freeBlockNumber;
    }
    
    public boolean returnBlock( int oldBlockNumber ) {
        // return this old block to the free list. The list can be a stack. 
        // return true or false;
        // block number is 0-indexed so maximum is totalBlocks - 1
        if (oldBlockNumber < 0 || oldBlockNumber >= totalBlocks) {
            return false;
        }

        int tmp = freeList;
        freeList = oldBlockNumber;
        oldBlockNumber = tmp;
        byte[] block = new byte[Disk.blockSize];
        SysLib.rawread(freeList, block);
        SysLib.short2bytes((short)oldBlockNumber, block, 0);
        SysLib.rawwrite(freeList, block);
        
        return true;
    }
}
