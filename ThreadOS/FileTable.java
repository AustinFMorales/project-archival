import java.util.*; // need this for vectors

public class FileTable {

    private Vector<FileTableEntry> table; // the actual entity of this file table
    private Directory dir; // the root directory

    public FileTable( Directory directory ) { // constructor
        table = new Vector<FileTableEntry>();  // instantiate a file (structure) table
        dir = directory;        // receive a reference to the Directory
    }                           // from the file system

    // major public methods
    public synchronized FileTableEntry falloc( String filename, String mode ) {
        // allocate a new file (structure) table entry for this file name
        // allocate/retrieve and register the corresponding inode using dir
        // increment this Inode's count
        // immediately write back this Inode to the disk
        // return a reference to this file (structure) table entry
        
        short iNumber = -1;
        Inode inode = null;
        while (true) {
            iNumber = (filename.equals("/") ? 0 : dir.namei(filename));
            if (iNumber >= 0) { // the file exists (check if Inode corresponding to
                                // fileName already exists and if so use its iNumber)
                inode = new Inode(iNumber);
                // check if mode is read
                if (mode.equals("r")) { // we want to read
                    // enter while loop if writing but keep (2) only
                    while (inode.flag == 2) { // while writing but keep (2) only (while not allowed to read and not delete)
                        try {
                            wait();
                        } catch (InterruptedException e) {}
                    }
                    
                    if (inode.flag >= 3) { // if delete
                        iNumber = -1;
                        return null;
                    }
                    /* inode.flag can be either reading but keep (1) or unused but keep (0) by time that reach this
                       line; inode.flag will be unused (0) if this thread happens to be the first thread to wake up
                       after calling notifyAll; however, if another thread wakes up first, it might change inode.flag
                       to reading but keep (1), which is still valid (and while loop will exit as expected); if
                       another thread wakes up first, it might instead change inode.flag to writing but keep (2),
                       which is invalid (and the while loop will continue as expected, calling wait) */
                    inode.flag = 1; // if unused but keep (0), set to reading but keep (1); if reading but keep (1), keep as is
                    break; // must break out of loop because can read now
                } else if (mode.equals("w") || mode.equals("w+") || mode.equals("a")) { // we want to write
                    // if 0, can write, so proceed
                    // if 1 or 2, cannot write, so while
                    // if 3, 4, or 5, cannot write, but delete so proceed to if and return null
                    while (inode.flag == 1 || inode.flag == 2) { // while reading but keep (1) or writing but keep (2) (while not allowed to write and not delete)
                        try {
                            wait();
                        } catch (InterruptedException e) {}
                    }
                    if (inode.flag >= 3) {
                        iNumber = -1; // unnecessary because returning null anyway and iNumber is a local variable
                        return null;
                    }
                    /* inode.flag can be only unused but keep (0) by time that reach this line; inode.flag will be
                       unused (0) if this thread happens to be the first thread to wake up after calling notifyAll;
                       however, if another thread wakes up first, it might change inode.flag to reading but keep (1)
                       or writing (2), which are invalid (and the while loop will continue as expected, calling wait) */
                    inode.flag = 2; // if unused but keep (0), set to writing but keep (2)
                    break; // must break out of loop because can write now
                } else { // if file exists and mode is left empty, return null to indicate error (eventually return -1 in Kernel.OPEN because myTcb.getFd returns -1 if FileTableEntry is null)
                    iNumber = -1;
                    return null;
                }
            } else {
                // if Inode with given fileName has not been found (file does not exist), get new iNumber and make a new Inode (create new file); do not have to call wait here because file does not exist (similar to unused)
                if (mode.equals("w") || mode.equals("w+") || mode.equals("a")) { // only create file if it does not exist when intending to write (not when intending to read or when mode is left empty)
                    iNumber = dir.ialloc(filename); /* only one iNumber per file (applies to all threads,
                                                    parent and child threads and entirely different threads),
                                                    because only FileTableEntry objects keep track of seekptr (which is only shared
                                                    by parent and child threads and not entirely different threads)
                                                    and mode (which is not maintained in an Inode -- the same one
                                                    is used by all FileTableEntry objects pointing to a specific file) */
                    inode = new Inode();
                    inode.flag = 2; // set flag
                    break; // must break from loop
                } else { // if file does not exist and mode is either read or left empty, return null to
                         // indicate error (eventually return -1 in Kernel.OPEN because myTcb.getFd
                         // returns -1 if FileTableEntry is null)
                    iNumber = -1;
                    return null;
                }
            }
        }
        // increment number of FileTableEntry objects pointing to this inode since we're freeing the given FileTableEntry
        inode.count++; 
        // save the corresponding inode to the disk using inode's toDisk()
        inode.toDisk(iNumber);;
        FileTableEntry e = new FileTableEntry(inode, iNumber, mode);
        table.add(e);
        return e;
    }

    public synchronized boolean ffree( FileTableEntry e ) {
        // receive a file table entry reference
        // save the corresponding inode to the disk
        // free this file table entry.
        // return true if this file table entry found in my table
        // given FileTableEntry will be closed
        
        // if this reference is found in the table, remove it
        if (table.remove(e)) {
            // decrement number of FileTableEntrys pointing to this
            // inode since freeing the given FileTableEntry
            e.inode.count--;
            // change flag to unused
            e.inode.flag = (short)(e.inode.flag >= 3 ? 3 : 0);
            // save the corresponding inode to the disk using Inode's toDisk()
            e.inode.toDisk(e.iNumber);
            // notify all threads
            notifyAll();
            return true;
        }
        // otherwise, we haven't found the table entry
        return false;
    }

    public synchronized boolean fempty( ) {
        return table.isEmpty( ); // return if table is empty 
    }                            // should be called before starting a format

}
