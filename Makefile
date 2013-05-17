#Jonathan An
#860986899
#Lab Section 022
#Homework 9

E = multiClient server1

multiClient: multiClient.cpp
	g++ -ggdb multiClient.cpp -lpthread -o multiClient

server1: server1.cpp
	g++ -ggdb server1.cpp -lpthread -o server1

clean:
	/bin/rm -f $E
	/bin/rm -rf Thread_*
