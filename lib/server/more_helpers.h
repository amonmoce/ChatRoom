#ifndef 	CHATSERVER_H_
#define 	CHATSERVER_H_

#include <netinet/in.h>

// Substring function: it returns a pointer to the substring
char *substring(char *string, int position, int length) {
   char *pointer;
   int c;
   pointer = malloc(length+1);
   for (c = 0 ; c < position -1 ; c++)
      string++;
   for (c = 0 ; c < length ; c++){
      *(pointer+c) = *string;
      string++;
   }
   *(pointer+c) = '\0';
   return pointer;
}

// check if the username exists already
int name_exist(char *string){
	int i;
	for(i=0;i<MAX_ONLINE_USER;i++){
		if(strcmp(usersList[i].name, string)==0)
			return 1;
	}
	return 0;
}

// check string length, has to be between 2 and 12
int length_check(char *string){
	char *p;
	int count=0;
	for(p=string; *p!='\0'; p++)
			count++;
	if(count < 2 || count > 12)
		return 0;
	else
		return 1;
}

// check if string characters are english alphabet
int is_english_alphabet(char *string){
	char *p;
	for(p=string; *p!='\0'; p++){
		if(!isalpha(*p))
			return 0;
	}
	return 1;
}

// check the validity of the string and return 0, 1, 2, 3, 4
int name_validity(char *string){
	if(strcmp(string, "anonymous") == 0)
		return 1;
	else if(name_exist(string))
		return 2;
	else if(!length_check(string))
		return 3;
	else if(!is_english_alphabet(string))
		return 4;
	return 0;
}

// add user and return user id if succeed or return -1 if failed
int addUser(ConnectedUser* usersList, int socket, char *ip_address, int port){
	int i;
	for(i=0;i<MAX_ONLINE_USER;i++){
		if(!usersList[i].isOnline){
			usersList[i].socket = socket;
			strcpy(usersList[i].name, "anonymous");
			strcpy(usersList[i].ip_address, ip_address);
			usersList[i].port = port;
			usersList[i].isOnline = 1;
			return i;
		}
	}
	return -1;
}

#endif
