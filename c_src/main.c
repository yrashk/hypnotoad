#include <stdio.h>
#include <unistd.h>
#include "zmq.h"
#include "clips.h"
#include "hypnotoad.h"

void * zmq;
void * pub;

int main(int argc, char **argv) {
	   InitializeEnvironment();
       IncrementGCLocks();
       Load("src/main.clp");
       void * module = FindDefmodule("Hypnotoad");
       SetCurrentModule(module);

       UserFunctions();

       zmq = zmq_init(1);
       pub = zmq_socket(zmq, ZMQ_PUB);
       zmq_bind(pub, "tcp://*:5554");

       void * sub = zmq_socket(zmq, ZMQ_SUB);
       zmq_bind(sub, "tcp://*:5555");
       zmq_setsockopt(sub, ZMQ_SUBSCRIBE, "", 0);

	   while (1) {
            char * msg, * topic;
            int topic_size;
            char * remote_filename;
		    zmq_msg_t request;

		    zmq_msg_init (&request);
		    zmq_recv (sub, &request, 0);
		    topic = zmq_msg_data (&request);
            topic_size = zmq_msg_size(&request);

            int fact = !strncmp(topic, "fact", 4);
            if (!fact) {
              remote_filename = (char *) malloc(topic_size + 1);
              strncpy(remote_filename, topic, topic_size);
            }

		    zmq_msg_close (&request);

		    zmq_msg_init (&request);
		    zmq_recv (sub, &request, 0);
		    msg = zmq_msg_data (&request);

            if (fact) {
              LoadFactsFromString(msg, zmq_msg_size(&request));
              Focus(module);
              Run(-1);
            } else {
              char * filename = tmpnam(0);

              /* dump file */
              FILE * f = fopen(filename,"w+");
              fwrite(msg, zmq_msg_size(&request), 1, f);
              fclose(f);

              char * fact = (char *) malloc(1024);
              sprintf(fact, "(remote-file \"%s\" \"%s\")\n",remote_filename,filename);

              /* load file mapping */
              Focus(module);
              LoadFactsFromString(fact, -1);

              Focus(module);
              Run(-1);

#if 0
              DATA_OBJECT res;
              Eval("(facts)",&res);
              fflush(0);
#endif              
              /* cleanup */
              free(fact);
              free(remote_filename);
              unlink(filename);
              free(filename);
            }
		    zmq_msg_close (&request);
	   }

       zmq_close(pub);
       zmq_close(sub);
       zmq_term(zmq);

       DecrementGCLocks();
}
