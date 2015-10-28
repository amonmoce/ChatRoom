#ifndef 	FINDUSER_H_
#define 	FINDUSER_H_


// find user id and return it or return -1 if failed
int findUser(ConnectedUser* usersList, int socket){
  int i;
  for(i=0;i<MAX_ONLINE_USER;i++){
    if(usersList[i].socket==socket)
      return i;
  }
  return -1;
}

//find user id by the name
int findUserByName(ConnectedUser* usersList, char *name){
  int i;
  for(i=0;i<MAX_ONLINE_USER;i++){
    if(strcmp(usersList[i].name, name)==0)
      return i;
  }
  return -1;
}

#endif
