M = number of Buffers
B = size of Buffers

(M - 1)*B data is used for storing input data.
B data is used for storing output data.

As soon as the input buffers gets full or all input records gets fetched. We start the processing for records, in both Btree and linear hashing.

Similarly, As soon as the output buffer gets filled , we store it into the output.txt file and flush the output buffer.


--> In Btree; B is used for calculating the order of tree.
	
	For calculating degree of b+tree. Assuming that block size = buffer size. If each block has space for n keys and n+1 pointers then size occupied = 4*(n) + 8*(n+1) if keys are integers.
    selecting the maximum n such that 4*n+8*(n+1) <= block size

--> In Linear Hashing; B is used to calculate block size.

	Block size = Buffer size(B)
	Let there be n blocks and r records then the occupancy = (r/(n*r1))*100 , where r1 is maximum records in a block(b/4).    