#!/usr/bin/python

"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import sys
from optparse import OptionParser

delimiter = "        "
EMPTY = ""

class comm:
    #Constructor for the comm class
    def __init__(self, flags, file):
        self.flags = flags
        self.file = file

    #Auxillary function to do actually comparison
    def comp(self):
        #Given by TA
        if self.file[0] == '-':
            lines1 = sys.stdin.readlines()
        else:
            file1 = open(self.file[0], 'r')
            lines1 = file1.readlines()
            file1.close()

        if self.file[1] == '-':
            lines2 = sys.stdin.readlines()
        else:
            file2 = open(self.file[1], 'r')
            lines2 = file2.readlines()
            file2.close()

        #Set indentation for each column
        if self.flags.only_file_1 == True:
            indent2 = "\t"
        else:
            indent2 = ""

        if (self.flags.only_file_1 == True) and (self.flags.only_file_2 == True):
            indent3 = "\t\t"
        elif (self.flags.only_file_1 == True) or (self.flags.only_file_2 == True):
            indent3 = "\t"
        else:
            indent3 = ""

        #The files have been sorted
        if self.flags.files_unsorted == False:
            index1 = 0
            index2 = 0
            len1 = len(lines1)
            len2 = len(lines2)

            while index1 < len1 and index2 < len2:
                #Check for uniqueness and print in first column
                if lines1[index1] < lines2[index2]:
                    if self.flags.only_file_1 == True:
                       sys.stdout.write(lines1[index1])
                    index1 += 1

                #Check for uniqueness and print in second column
                elif lines1[index1] > lines2[index2]:
                    if self.flags.only_file_2 == True:
                        sys.stdout.write(indent2 + lines2[index2])
                    index2 += 1

                #Check if word exists in both files
                else:
                    if self.flags.both == True:
                        sys.stdout.write(indent3 + lines2[index2])
                    index1 += 1
                    index2 += 1

            if index1 == len1:
                while index1 < len1:
                    if self.flags.only_file_1 == False:
                        sys.stdout.write(lines1[index1])
                    index1 += 1
            else:
                while index2 < len2:
                    if self.flags.only_file_2 == False:
                       sys.stdout.write(indent2 + lines2[index2])
                    index2 += 1

        #Using comm with files that are not sorted
        else:
            for string1 in lines1:
                inlist3 = False
                for string2 in lines2:
                    if string1 == string2:
                        inlist3 = True

            for string in lines2:
                if self.flags.only_file_2 == False:
                    sys.stdout.write(indent2 + string)            

            if inlist3 == True:
                lines2.remove(string1)
                if self.flags.both == False:
                    sys.stdout.write(indent3 + string1)
                else:
                    if self.flags.only_file_1 == False:
                        sys.stdout.write(string1)

            

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 [FILE2] 

Output randomly selected lines from FILE(S). 
Files must be non-empty.

OPTIONS: 
    -1: suppress 1st column (lines unique to first file)
    -2: suppress 2nd column (lines unique to the second file)
    -3: suppress 3rd column (lines contained in both the first and second file)
    -u: utilize unsorted files
"""

    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", "--nofile1", action="store_false", dest="only_file_1", default=True, help="suppress column 1 (lines unique to FILE1)")
    parser.add_option("-2", "--nofile2", action="store_false", dest="only_file_2", default=True, help="suppress column 2 (lines unique to FILE2)")
    parser.add_option("-3", "--nocommon", action="store_false", dest="both", default=True, help="suppress column 3 (lines that appear in both files)")
    parser.add_option("-u", "--unsorted", action="store_true", dest = "files_unsorted", default=False, help="work with unsorted files")
    options, args = parser.parse_args(sys.argv[1:])

    try:
        new_comparison = comm(options, args)
        new_comparison.comp()
    except IOError as err:
        parser.error("I/0 error({0}): {1}".format(errno, strerror))

if __name__ == "__main__":
    main()
