Dmitry Schelkunov's white-box implementation of a Rijndael-like SPN-cipher.

An underlying mathematical approach allows to transfom a randomly generated Rijndael-like SPN-cipher to the ultra-fast asymmetric scheme. And not only...
Read http://eprint.iacr.org/2010/419 for more.

Use wb_creator.exe number_of_rounds min_number_of_mixes max_number_of_mixes to create white-box encryption and decryption tables (public and private keys).

number_of_mixes determines a resistance to known attacks.

Sample of usage: wb_creator.exe 10 200 300



