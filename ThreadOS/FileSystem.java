public class FileSystem {
    private SuperBlock superblock;
    private Directory directory;
    private FileTable filetable;

    public FileSystem(int diskBlocks) {
        superblock = new SuperBlock(diskBlocks);
        directory = new Directory (superblock.totalInodes);
        filetable = new FileTable(directory);
    
        // read the "/" file from disk
        FileTableEntry dirEnt = open("/", "r");
        int dirSize = fsize(dirEnt);
        if (dirSize > 0) {
            // the directory has some data
            byte[] dirData = new byte[dirSize];
            read(dirEnt, dirData);
            directory.bytes2directory(dirData);
        }
        close(dirEnt);
    }

    // an additional method that the professor described that we need to implement
    // synchronize directory data in this filesystem by writing it back to the disk
    void sync() {
        // open the root directory as a filetableentry
        FileTableEntry dirEnt = open("/", "w");
        // put all of this directory's contents into a buffer
        byte[] directoryBuffer = directory.directory2bytes();
        // then write it back all to disk
        write(dirEnt, directoryBuffer);
        // afterwards, close it
        close(dirEnt);
        // sync the superblock
        superblock.sync();
    }
    
    boolean format(int files) {
        // call format on the superblock
        superblock.format(files);

        // then create a new directory and filetable after we just formatted
        // the superblock
        directory = new Directory(superblock.totalInodes);
        filetable = new FileTable(directory);
        return true;
    }

    // implement the corresponding methods for FileSystem.java
    FileTableEntry open(String filename, String mode) {
        FileTableEntry ftEnt = filetable.falloc(filename, mode);
        if (mode.equals("w")) { // would like to delete file only when w
            if (deallocAllBlocks(ftEnt) == false) {
                return null;
            }
        }
        return ftEnt;
    }

    boolean close(FileTableEntry ftEnt) {
        if (ftEnt != null && --ftEnt.count == 0 && filetable.ffree(ftEnt)) {
            // deallocate all blocks associated with Inode if it is unused and if flag is in delete set
            if (ftEnt.inode.flag == 3) {
                deallocAllBlocks(ftEnt);
            }
            return true;
        }
        return false; // return false if ftEnt could not be found in table
    }

    int fsize(FileTableEntry ftEnt) {
        // size in bytes of file to which ftEnt corresponds is not unique to
        // ftEnt; it is shared by all FTEs pointing to Inode pointed by ftEnt
        return ftEnt.inode.length;
    }

    int read(FileTableEntry ftEnt, byte[] buffer) {
        // if the entry doesn't exist or the mode is incorrect for read
        // return -1 for error
        if ( ftEnt == null || (!ftEnt.mode.equals("r") && !ftEnt.mode.equals("w+")) ) {
            return -1;
        }

        int readSize = 0, sourceBlock = ftEnt.inode.findTargetBlock(ftEnt.seekPtr), remForBuffer = 0, fileSize = fsize(ftEnt);

        // once call findTargetBlock and get -1, no more left to read
        for (remForBuffer = buffer.length; remForBuffer > 0 && sourceBlock != -1 && ftEnt.seekPtr <= fileSize; remForBuffer -= readSize, sourceBlock = ftEnt.inode.findTargetBlock(ftEnt.seekPtr)) {
            // get a new buffer and send a rawread call to disk
            byte[] block = new byte[Disk.blockSize];
            SysLib.rawread(sourceBlock, block);

            // find where we start first
            int offsetWithinBlock = ftEnt.seekPtr % Disk.blockSize;
            int blockRemaining = Disk.blockSize - offsetWithinBlock;
                
            // choose the least amount of bytes to read into
            readSize = Math.min(remForBuffer, blockRemaining);

            // copy this block's contents to the buffer we just passed in
            System.arraycopy(block, offsetWithinBlock, buffer, buffer.length - remForBuffer, readSize);

            // adjust the ptrs 
            ftEnt.seekPtr += readSize;
        }

        // return the number of bytes read
        return buffer.length - remForBuffer;
    }

    int write(FileTableEntry ftEnt, byte[] buffer) {
        // check if ftEnt is null or the mode is read
        // if so, then return -1 for failure (allows w+)
        if (ftEnt == null || ftEnt.mode.equals("r") || buffer.length == 0) {
            return -1;
        }

        int writeSize = 0, targetBlock = 0, remInBuffer = 0, fileSize = fsize(ftEnt);

        for (remInBuffer = buffer.length; remInBuffer > 0; remInBuffer -= writeSize) {
            // if target does not exist
            if ((targetBlock = ftEnt.inode.findTargetBlock(ftEnt.seekPtr)) == -1) {
                int newBlockNumber = superblock.getFreeBlock();
                int test = ftEnt.inode.registerTargetBlock(ftEnt.seekPtr, (short)newBlockNumber);
                
                // use preceding blocks if available
                while (test == Inode.ErrorPrecBlockUnused) {
                    test = ftEnt.inode.registerTargetBlock(ftEnt.seekPtr -= Disk.blockSize, (short)newBlockNumber);
                }

                if (test == Inode.ErrorBlockRegistered || test == Inode.ErrorIndirectNull) {
                    // must call registerTargetBlock after registerIndexBlock
                    // must get new block number for target block (one obtained earlier will be used for index block)
                    int newBlockNumber2 = superblock.getFreeBlock();
                    if (!(ftEnt.inode.registerIndexBlock((short)newBlockNumber) &&
                          ftEnt.inode.registerTargetBlock(ftEnt.seekPtr, (short)newBlockNumber2) == Inode.NoError)) return -1; // should never happen
                }

                targetBlock = ftEnt.inode.findTargetBlock(ftEnt.seekPtr); // this works for both indirect and direct pointers   
            }

            // only overwrite the necessary parts, keep everything else
            byte[] block = new byte[Disk.blockSize];
            SysLib.rawread(targetBlock, block);

            int offsetWithinBlock = ftEnt.seekPtr % Disk.blockSize;
            int blockRemaining = Disk.blockSize - offsetWithinBlock;

            writeSize = Math.min(remInBuffer, blockRemaining);

            System.arraycopy(buffer, buffer.length - remInBuffer, block, offsetWithinBlock, writeSize);
            SysLib.rawwrite(targetBlock, block);
            
            ftEnt.seekPtr += writeSize;
        }
        
        // verify if the filesize is correct for this inode
        // at the end of a write, the seekptr should be the same as the filesize
        // if the seekPtr is greater, then reassign the inode (file) length to
        // the correct size.
        if (ftEnt.seekPtr > fileSize) {
            ftEnt.inode.length = ftEnt.seekPtr;
        } // this is for append (change filesize)

        // when this is done, we have modified the inode
        // so we have to call toDisk();
        ftEnt.inode.toDisk(ftEnt.iNumber);
        byte[] block = new byte[Disk.blockSize];

        return buffer.length - remInBuffer;
    }

    boolean delete(String filename) {
        short iNumber = directory.namei(filename); // get iNumber for given filename
        // construct Inode from disk using this iNumber (know that iNumber is valid)
        Inode inode = new Inode(iNumber);
        if (inode.count > 0) { // check if 3, 4, or 5 and do not add 2 in those cases (already marked as "to be
                               // "deleted")
            inode.flag += 2; // to place in delete set
        } else {
            // if inode.count is 0, all opens have been closed, and thus the flag will have been set to unused
            // (0 or 3)
            // free the file from the directory
            directory.ifree(iNumber);
            // then we deallocate all the blocks associated with this file (inode)
            deallocAllBlocks(new FileTableEntry(inode, iNumber, "w"));
        }

        return false;
    }

    int seek(FileTableEntry ftEnt, int offset, int whence) {
        // if this file table entry doesn't exist, then 
        // return a negative value indicating error
        if (ftEnt == null) {
            return -1;
        }

        // whence = 0, go offset from beginning; whence = 1, go offset from
        // current position; whence = 2, go offset from end of file
        ftEnt.seekPtr = whence == 0 ? offset : (whence == 1
                                    ? ftEnt.seekPtr + offset
                                    : fsize(ftEnt) + offset);
        
        // deal with clamping down here
        // if our seekPtr is negative
        if (ftEnt.seekPtr < 0) {
            // clamp it back to zero
            ftEnt.seekPtr = 0;
            // if our seekPtr is exceeding the filesize of this entry
        } else if (ftEnt.seekPtr > fsize(ftEnt)) {
            // clamp it down to the actual filesize
            ftEnt.seekPtr = fsize(ftEnt);
        }

        // return our new seekPtr on success.
        return ftEnt.seekPtr;
    }

    boolean deallocAllBlocks(FileTableEntry ftEnt) {
        if (ftEnt == null) {
            return false;
            // block doesn't exist
        }

        //otherwise, lets deallocate all the direct blocks first
        for (int i = 0; i < ftEnt.inode.direct.length; i++) {
            if (ftEnt.inode.direct[i] != -1) {
                superblock.returnBlock(ftEnt.inode.direct[i]);
                ftEnt.inode.direct[i] = -1;
            }
        }
        // then we need to handle the indirect block for deallocation
        byte[] freeBuffer = ftEnt.inode.unregisterIndexBlock();

        if (freeBuffer != null) {
            short blockId = 0;
            int block = 0;
            // while we don't have an invalid block
            while ( blockId != -1) {
                // return all of these blocks back to the superblock;
                blockId = SysLib.bytes2short(freeBuffer, block);
                superblock.returnBlock(blockId);
                block += 2;
            }
        } 
        // because we modified this inode's contents, we should write this back towards the disk
        ftEnt.inode.toDisk(ftEnt.iNumber);
        return true;
    }
}
