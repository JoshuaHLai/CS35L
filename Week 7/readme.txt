Looking at the homework initially, it seemed very daunting and made me feel very demotivated to get this done.
But after reading on Piazza about the various comments and solutions to problems other people had.  It did not seem
that bad.  To start with this part of the lab, I had to read up on some documentation of how "pthread_create" and
"pthread_join" work in general and get a better understanding of what is contained in the "pthread.h" library.

First, I looked at the "Makefile" file since it was one of the files I had to edit and it was smaller than the 
"main.c" file.  I had to do some extra research into what exactly "CC" "CFLAGS" and "LDLIBS" were in order to know where
to add the "-lpthread" flag.  After reading, I still did not really have a clue of where to put it, so I put it arbitrarily
in the "CFLAGS" section.

Next, I looked at the "main.c" file.  At first, it seemed really daunting since it was huge file.  But after reading through it
and the comments, I realized that it really is just some copying and pasting and then writing a few lines of code.  After moving
the section of code that plots the image and named it "helper_function" and made some changes to incorporate multithreading based
on the number of threads utilized (i.e. change some paramters of "for" loops).  Afterwards, I added the code for "pthread_create"
and "pthread_join" to the main function in the style that was based on the slide from my TA.

Once I tried running "make check clean" I immediately got an error in that nothing was printed out.  After reading through my TA
slides, I saw that the "-lpthread" flag was in the wrong spot and should be placed with "LDLIBS" so I made that change.  Afterwards,
I did get some compiler warnings about some type-casting but I still got a results in terms of the time, which can be seen in the
file "make-log.txt"

# of Threads 	Real Time
      1		0m45.297s
      2		0m27.127s
      4		0m14.086s
      8		0m5.766s

Looking at the performance times, it shows clearly that the more threads that are utilized, the better the performance is. Of course,
from the lab, I realized that there is a certain cap at to how much the program can be improved with the number of threads, but I
did not really test it.