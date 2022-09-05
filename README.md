# Program Explanation
In this program, we are assuming we are the leader of the Glorious People’s Republic of Santa Cruz. As the leader, I have decided that the Internet content must be filtered so that children are not corrupted through the use of unfortunate, hurtful, offensive, and far too descriptive language. Therefore, the purpose of this program is to immediately filter the bad words that some people in my republic use in an efficient way. We will use a Bloom Filter (a Bit Vector) and a Hash Table (an array of Binary Search Trees) to set up a database of bad words in order to be able to search up quickly/efficiently if a bad word is in the database or not. We will make use of lexical analysis with regular expressions in order to split the files that contain text into individual words so the task of filtering the bad words used can be accomplished.

## Formatting

To format all the source code including header files:

...

$ make format

...

## Building 

To build this program:

...

$ make all

...

## Running 

To run the executable of banhammer.c:

$ ./banhammer

The program accepts the following command-line options for banhammer:

• -h: prints out the program usage.

• -t size: specifies that the hash table will have size entries (the default will be 2^16).

• -f size: specifies that the Bloom filter will have size entries (the default will be 2^20).

• -s: will enable the printing of statistics to stdout. The statistics include:

	*Average binary search tree size

	*Average binary search tree height

	*Average branches traversed

	*Hash table load

	*Bloom filter load

## Cleaning

To remove all files that are compiler generated:

...

$ make clean

...
