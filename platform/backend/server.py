import json
import socket

import config
import tornado.ioloop
import tornado.tcpserver
import tornado.web
import threading

socket_pool = {}
job_pool = {}
job_pool_lock = threading.Lock()
job_counter = 0
job_counter_lock = threading.Lock()


class ApiHandler(tornado.web.RequestHandler):

    @tornado.gen.coroutine
    def post(self):
        global job_counter_lock
        global socket_pool
        global job_pool
        global job_counter
        data = {k: self.get_argument(k) for k in self.request.arguments}
        if data['studentId'] not in socket_pool:
            self.write('FAIL')
            return
        job_counter_lock.acquire()
        now_job_counter = job_counter
        job_counter += 1
        job_counter_lock.release()

        if data['aiType'] == 'solver':
            data['aiType'] = 'play'
        elif data['aiType'] == 'evil':
            data['aiType'] = 'evil'
        socket_pool[data['studentId']].send_message(
            "genmove %s %s jid:%d\r\n" % (
                data['aiType'],
                data['serializedTiles'],
                now_job_counter
            )
        )
        times = 0
        while times < 1000:
            job_pool_lock.acquire()
            if now_job_counter in job_pool:
                self.write(job_pool[now_job_counter])
                job_pool.pop(now_job_counter)
                job_pool_lock.release()
                return
            job_pool_lock.release()
            yield tornado.gen.sleep(0.001)
            times += 1
        self.write("TIMEOUT")


class RoomHandler(tornado.web.RequestHandler):

    def get(self):
        self.write(json.dumps([x for x in socket_pool]))


def make_app():
    return tornado.web.Application([
        (r"/api/", ApiHandler),
        (r"/room/", RoomHandler),
        ("/(.*)", tornado.web.StaticFileHandler,
         {'path': '../frontend', "default_filename": "index.html"})
    ])


class TcpConnection(object):

    def __init__(self, stream, address):
        self.stream = stream
        self.address = address
        self.stream.set_close_callback(self.on_close)
        self.stream.read_until(b'\n', self.on_read_line)
        self.name = ""
        print("new connection", address, stream)
        self.line_counter = 0

    def on_read_line(self, data):
        try:
            data = data.decode()
        except:
            return
        if len(self.name) == 0:
            name = data.split()[1]
            global socket_pool
            if name not in socket_pool:
                self.name = name
                socket_pool[self.name] = self
                self.send_message("Register Successed.\n")
            else:
                self.send_message("Register Failed.\n")
                self.stream.close()
                return
        else:
            if data.find("jid:") != -1:
                data = data.split()
                response = data[0]
                jobid = int(data[1].split(':')[1])
                global job_pool
                global job_pool_lock
                job_pool_lock.acquire()
                job_pool[jobid] = response
                job_pool_lock.release()
                print(jobid, response)
        self.stream.read_until(b'\n', self.on_read_line)

    def send_message(self, data):
        self.stream.write(data.encode())

    def on_close(self):
        if self.name in socket_pool:
            socket_pool.pop(self.name)
        print(self.name, self.address, "left")


class TcpServer(tornado.tcpserver.TCPServer):

    def handle_stream(self, stream, address):
        TcpConnection(stream, address)

def tick():
    global socket_pool
    for x in socket_pool:
        socket_pool[x].send_message("tick\n")


if __name__ == "__main__":
    web_server = make_app()
    web_server.listen(config.web_port)
    client_server = TcpServer()
    client_server.listen(config.socket_port)
    tornado.ioloop.PeriodicCallback(tick, 60000).start()
    tornado.ioloop.IOLoop.current().start()
