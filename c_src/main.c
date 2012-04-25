#include <stdio.h>
#include "zmq.h"
#include "clips.h"

static void *zmq;

int main(int argc, char **argv) {
	   InitializeEnvironment();
       IncrementGCLocks();
       Load("src/main.clp");
       void * module = FindDefmodule("Hypnotoad");
       SetCurrentModule(module);

       UserFunctions();

       zmq = zmq_init(1);
       void * sub = zmq_socket(zmq, ZMQ_SUB);
       zmq_bind(sub, "tcp://*:5555");
       zmq_setsockopt(sub, ZMQ_SUBSCRIBE, "", 0);

	   while (1) {
			char * msg;
		    zmq_msg_t request;
		    zmq_msg_init (&request);
		    zmq_recv (sub, &request, 0);
		    msg = zmq_msg_data (&request);
		    LoadFactsFromString(msg, zmq_msg_size(&request));
		    Focus(module);
		    Run(-1);
		    zmq_msg_close (&request);
	   }


       zmq_term(zmq);

       DecrementGCLocks();
}