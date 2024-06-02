import os

from pco_model import *


class PcoDirectory:
    def __init__(self, build_config: PreCompiledTargetBuildConfiguration):
        self.build_config = build_config

    def type_index(self):
        return os.path.join(self.build_config.base_dir_path, "pco", "typeindex")
