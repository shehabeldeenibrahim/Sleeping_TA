# Sleeping_TA

Use pthreading to imitate a real life scenario of a TA who’s sleeping in his room, waiting for students to ask questions.

The TA remains sleeping until a student comes and wake him up. There are 3 waiting chairs outside his room. If the student came and found the 3 chairs are taken, then they should come back in a later time, else he will wait for the TA to finish with the one inside.

Each student and the TA is represented by a separate thread. Also, there are 3 semaphores representing each chair, a semaphore representing the state of the TA whether sleeping or not, a mutex representing whether the TA chair is being used by a student or not, and a semaphore representing whether the TA is done helping a student or not.

Each student goes to do the programming assignment for a random amount of time then comes to ask the TA questions. 

HOW TO RUN FOR TESTING?

//go to directory where the c file is located

//compile c code

gcc -pthread Sleeping_TA.c

//run executable 

./a.out <numberofstudents>
