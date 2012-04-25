#include "clips.h"
#include "zmq.h"
#include "hypnotoad.h"

void UserFunctions(void);
void EnvUserFunctions(void *);

void * remote_retr_file()
{
	DATA_OBJECT data;
	RtnUnknown(1, &data);

	char * filename = (char *) DOToString(data);

	printf("Fetching %s\n", filename);

    zmq_msg_t message;
    zmq_msg_init_size (&message, strlen(filename));
    memcpy (zmq_msg_data (&message), filename, strlen(filename));
    zmq_send (pub, &message, 0);
    zmq_msg_close (&message);

    void * result = AddSymbol(filename);
    
	return result;
}

void UserFunctions()
{   
     DefineFunction2("remote-retrieve-file",'s',PTIF remote_retr_file,"remote-retrieve-file","1s");
}

#if WIN_BTC
#pragma argsused
#endif
void EnvUserFunctions(
  void *theEnv)
{
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
}

