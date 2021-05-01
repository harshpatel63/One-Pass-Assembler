                                                                        PROJECT NAME    :- ONE PASS ASSEMBLER
                                                                   
                                                                        GROUP NUMBER    :- 4

                                                                      NAME OF MEMBER    :- 1.) Roshan Kumar (19CSE1027)
                                                                                           2.) Patel Harsh Rajesh (19CSE1019)

                                                                          SUPERVISOR    :- Dr. Veena Thenkanidiyoor
 
                                                                                DATE    :- May 1, 2021

                                                                          DEPARTMENT    :- Computer Science and Engineeing


Hello welcome to one pass assembler.

In this program we are are taking an input program and generating corresponding object program for it.

This program was written in C++14 standard and compiled using MINGW64(G++) compiler.

HOW TO RUN :-

      To run this program go to the  One-Pass-Assembler file directory and write the following

          ./assembler.exe input_file_name.txt

      If input file name is not given in the command line then by default the program considers "input1.txt" as  the file name.

      The welcome screen will ask to enter any key to continue
      
ASSUMPTIONS:-

Input1.txt file is written in three columns . The 1st column contains all the labels.
            
            In general column1 is followed by a tab and then column2 is written and after column2 there is a tab and then column3 
            is written
            
            <column1><tab><column2><tab><column3>
            
            If a label is not present in the column1 then a combination of one tab and space is used and then column2 is written.
            
            <space><tab><column2><tab><column3>
           
            Before the changes are made in the input file this combination should be considered , otherwise error will be generated in 
            object program.
            
EXPLANATION:-

Input1.txt file contains a source code. Ths source code is for a program that reads records from a input device and copies them to an output device. The main routine calls subroutine RDREC to read the records from input device and WRREC to write the records in the output device.


Each line is passed through the assembler and corresponding object code is generated  for it.




One pass assembler:-

This program helps us to generate corresponding object code from a input file. Program mimics a actual SIC one pass assembler. 

For this program to work and generate object code we use SYMTAB , OPTAB and LOCATION TABLE

SYMTAB in this program is applied through a vector defined as vSymtab . vSymtab is vector of structure Symtab . The structure Symtab contains the following data member.
1.) Symbol  :- This is the symbol ( label or operand) which is entered in Symtab.
2.) Address :- Address is the corresponding LOCATION where the label or operand is defined or  declared.
2.) Link        :- This link holds the address of a corresponding linked list which hold the address of a operand if it is only defined i.e the                                  operand does not have a previous declaration.

To the store Linkedlist a Structure defined as Node is made. This structure has the following data member.
1.) notDefinedAddress :- This stores the address of Symbols whose defination is and not available with us till the time this Symbol is
                                           declared. 
2.)*link                           :- It stores the address the next node

vSymtab helps us to solve the issue of forward referencing.





Next this program has a Optab to get the corresponding opcode to a mnemonic.
In this program Optab is realized through a vector defined as vOptab . vOptab is a vector of structure Optab. To enter the mnemonic and its corresponding opcode in vector we have made a .txt file called optab.
The vOptab reads the optab.txt and stores it.
Structure Optab has the following data member

1.) menmonic :- It store mnemonic
2.) opcode   :- It stores the opcode corresponding to a mnemonic





LOCATION TABLE is defined to read the file and store the correspoding LOCATION.

LOCATION TABLE in this program is realized through a vector of string called location. 



searchSymtab() function searches wheather a symbol is already in the symtab and return 1 if its already there and 0 when symbol is not there in symtab.


Further this program works on to generate object code and hence object program for the input file.



To make all this happen we have a function called onePassScan()

This function is where all of the work is happening and the input text file is read and passed to various functions to generate object program.

First the input file is opened and the input text file is read line by line. The main idea behind the program is to separate program into three columns.

1.)First column contains all the labels and white spaces.
2.)Second column contains all the mnenonic codes and assembler directive.
3.)Third column contains all the labels and symbols that are used .

This program extensively uses ostringstream to store and display strings.
After separating the input text file into different columns the locations for every line except the line that has START assembler directive is stored in vector location in hexadecimal format.

Then the program checks if the column2 contains assembler directives. According to which assembler directive it contains location is updates. If column2 contains no assembler directive then location is updated by 3.

After all this the object code is generated and stored in a ostringstream , and this object code is further put in textrecord and hence a
full object program is generated.
 

