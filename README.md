# ServerClientMult
## Operating Systems
This program has multiple functions

client.c, server.c, common.c, common.h
These files create a messaging system between a "server" and a "client" program through the use of message queues.

mult.c
This file computes a function f(x) a specified number of times and multiplies all the results together. When multiplying the results together, the work is split up evenly among a specified number of processes.

mult-thread.c
This file accomplishes the same task as mult.c but it uses threads instead of processes.

Mult.java
This file accomplishes the same task as mult-thread.c but in Java instead of C.

p1test.sh, p2test.sh
These files are bash scripts that are used to test the mult programs.
