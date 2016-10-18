# Description of the test data

Every folder is a test case. The files found in each one are:

* `input.txt` with the input to be given to the program from stdin.
  The file structure is one object per line as follows:
  
  - First the degree of the generating polynomial (natural number)
  
  - Then for each message we have the first line is the polynomial
    message and the second line is the CRC (calculated with the to be
    found polynomial)
	
* `output.txt` containing the generating polynomial of the codes. It has
  to match one line of the output of the script.
  
