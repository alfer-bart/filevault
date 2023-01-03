# FileVault
Encrypts and decrypts files using AES 128bit algorithm. It works well with smaller files but gets a bit slow with large files. It does not compress data. Feel free to send me suggestions.
## Usage Guide
To encrypt a file, use the following syntax: lock [input file path] [password] [output file path]
### Example:
`lock D:\mypic.jpg 1234 D:\jpg.locked`
### Decrypting:
`unlock D:\jpg.locked 1234 D:\unlocked.jpg`
<br /><br />*NOTE: this version does not support spaces in file paths!*
