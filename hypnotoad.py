#! /usr/bin/env python
"""
Installation instructions:

1. Install pyzmq: 
   
   easy_install pyzmq

"""

import os.path
import string, uuid
from optparse import OptionParser
import zmq
import threading
from urlparse import urlparse
import base64

class FileServer(threading.Thread):
    def __init__(self, command):
        self.command = command
        super(FileServer, self).__init__()

    def run(self):
        while True:
            data = self.command.receive()
            url = urlparse(data)
            if url.scheme == 'hypnotoad' and url.netloc == self.command.id:
                self.serve(data, url.path.lstrip('/'))

    def serve(self, url, path):
        print "Sending %s" % path
        f = open(path, 'r')
        content = f.read()
        self.command.publish(url, content)

class Command:
    def __init__(self, args):
        # zmq 
        self.ctx = zmq.Context()
        self.sub = self.ctx.socket(zmq.SUB)
        self.sub.connect("tcp://localhost:5554")
        self.sub.setsockopt(zmq.SUBSCRIBE, "")
        self.pub = self.ctx.socket(zmq.PUB)
        self.pub.connect("tcp://localhost:5555")
        self.args = args
        self.id = str(uuid.uuid4())

    def publish(self, topic, data):
        self.pub.send_multipart([topic, data])

    def receive(self):
        return self.sub.recv()

    
class Apply(Command):
    def run(self):
        print "Applying", self.args
        include_stmt = string.join([ '(include "hypnotoad://%s/%s")' % (self.id, toadie) for toadie in self.args ])
        self.publish("facts", include_stmt)

Commands = {'apply': Apply}

def main():
    parser = OptionParser(version='Hypnotoad Console')
    (options,args) = parser.parse_args()
    parser.destroy()

    command = Commands[args[0]](args[1:])

    file_server = FileServer(command)
    file_server.start()

    command.run()    


if __name__ == "__main__":
    main()
