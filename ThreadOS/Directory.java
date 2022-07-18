
public class Directory {
    private static int maxChars = 30; // max characters of each file name
    // Directory entries
    private int fsizes[];        // each element stores a different file size.
    private char fnames[][];    // each element stores a different file name.

    public Directory( int maxInumber ) { // directory constructor
        fsizes = new int[maxInumber]; // maxInumber = max files
        for ( int i = 0; i < maxInumber; i++ ) {
            fsizes[i] = 0; // all file size initialized to 0
        }
        fnames = new char[maxInumber][maxChars];
        String root = "/";          // entry(inode) 0 is "/"
        fsizes[0] = root.length( );  // fsizes[0] is the size of "/".
        root.getChars( 0, fsizes[0], fnames[0], 0 ); // fnames[0] includes "/"
    }

    public void bytes2directory( byte data[] ) {
        // assumes data[] contains directory information retrieved from disk 
        // initialize the directory fsizes[] and fnames[] with this data[] 
        int offset = 0;
        for ( int i = 0; i < fsizes.length; i++, offset += 4 ) {
            fsizes[i] = SysLib.bytes2int( data, offset );
        }
        
        for ( int i = 0; i < fnames.length; i++, offset += maxChars * 2 ) {
            String fname = new String( data, offset, maxChars * 2 );
            fname.getChars( 0, fsizes[i], fnames[i], 0 );
        }
    }

    public byte[] directory2bytes( ) {
        // converts and return directory information into a plain byte array 
        // this byte array will be written back to disk 
        byte[] data = new byte[fsizes.length * 4 + fnames.length * maxChars * 2];
        int offset = 0;
        for ( int i = 0; i < fsizes.length; i++, offset += 4 ) {
            SysLib.int2bytes( fsizes[i], data, offset );
        }

        for ( int i = 0; i < fnames.length; i++, offset += maxChars * 2 ) {
            String tableEntry = new String( fnames[i], 0, fsizes[i] );
            byte[] bytes = tableEntry.getBytes( );
            System.arraycopy( bytes, 0, data, offset, bytes.length );
        }
        return data;
    }

    // implement the following below:
    public short ialloc( String filename ) {
        // filename is the one of a file to be created.
        // allocates a new inode number for this filename

        // start at 1 because directory is at 0
        for (int i = 1; i < fsizes.length; i++) {
            // if we see there's a spot that hasn't been allocated yet
            // we've found a spot to allocate this filename
            if (fsizes[i] == 0) {
                // put the filename's size here
                fsizes[i] = filename.length(); // is this correct? (or should it be number of bytes)
                // copy the filename to the fnames array
                filename.getChars(0, fsizes[i], fnames[i], 0);
                // return the iNumber, cast it back to short
                return (short)i;
            }
        }
        // return -1 if failed, cast it back to short
        return (short)-1;
    }

    public boolean ifree( short iNumber ) {
        // deallocates this inumber (inode number)
        // the corresponding file will be deleted.
        // if iNumber goes out of boundaries or tries to free root (assumed to be invalid)
        // return false
        if (iNumber >= fsizes.length || iNumber <= 0) {
            return false;
        } else {
            fsizes[iNumber] = 0; // reset to initialized int values
            fnames[iNumber] = new char[maxChars]; // reset to initialized char values
            return true;
        }
    }

    public short namei( String filename ) {
        // returns the inumber corresponding to this filename
        for (int i = 0; i < fsizes.length; i++) {
            // convert the fnames char array into a string
            // that will be used to compared without manipulating
            // fnames contents
            String fnamesString = new String(fnames[i]);
            // don't worry about allocating new strings, JVM
            // cleans up allocated memory afterwards
            if (filename.trim().equals(fnamesString.trim())) {
                // if we found it, return the inumber in this filename
                return (short)i;
            }
        }
        // otherwise, return -1 for failure
        return (short)-1;
    }
}
