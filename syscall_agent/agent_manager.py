import platform
import os
from typing import List, Optional
import logging

from .agent import Agent

_l = logging.getLogger(__name__)


class AgentManager:
    def __init__(self):
        self.agents: List[Agent] = [ ]

    @staticmethod
    def get_local_host() -> str:
        return platform.machine().lower()

    @staticmethod
    def get_local_system() -> str:
        system = platform.system()
        if system == "Darwin":
            return "macos"
        elif system == "Linux":
            return "linux"
        elif system == "Windows":
            return "windows"
        raise NotImplementedError("Unsupported system %s" % system)

    @staticmethod
    def compare_arch(arch_0: str, arch_1: str) -> bool:
        if arch_0 == arch_1:
            # fast path
            return True

        try:
            import archinfo
        except ImportError:
            _l.warning("archinfo is not installed. Fall back to exact matching.")
            return arch_0 == arch_1

        arch0 = archinfo.arch_from_id(arch_0)
        arch1 = archinfo.arch_from_id(arch_1)
        return arch0.name == arch1.name and arch0.memory_endness == arch1.memory_endness

    def register(self, agent: Agent):
        self.agents.append(agent)

    def get_agent(self, target: str, host=None, system=None) -> Optional[Agent]:
        if host is None:
            host = self.get_local_host()
        if system is None:
            system = self.get_local_system()

        if system == "linux":
            for agent in self.agents:
                if self.compare_arch(agent.host, host) and self.compare_arch(agent.target, target):
                    return agent
        return None

    def discover_agents(self) -> None:
        """
        List all executables under /native and create Agent objects for them.
        """

        self.agents.clear()
        basedir = os.path.join(os.path.dirname(os.path.realpath(__file__)), "..", "native")

        for f in os.listdir(basedir):
            full_path = os.path.join(basedir, f)
            if os.path.isfile(full_path) and os.access(full_path, os.X_OK):
                if f.startswith("syscall-agent-"):
                    target = f[len("syscall-agent-"): ]
                    agent = Agent(self.get_local_host(), target, full_path)
                    self.agents.append(agent)
