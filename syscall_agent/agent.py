import subprocess


class Agent:
    def __init__(self, host: str, target: str, path: str):
        self.host = host
        self.target = target
        self.path = path

    def launch(self, endpoint_str: str):
        proc = subprocess.Popen([self.path, endpoint_str])
        return proc
