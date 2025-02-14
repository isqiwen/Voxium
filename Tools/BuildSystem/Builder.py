
import sys

from Tools.BuildSystem.ProjectConfig import ProjectConfig
from Tools.Utilities.Logger import Logger
from Tools.Utilities.FileUtils import clean
from Tools.BuildSystem.ConanHelper import run_conan_command
from Tools.BuildSystem.BuilderBase import StepBase, BuilderBase


class StepClean(StepBase):
    def run(self):
        if not self.config.clean:
            return

        Logger.Info("Builder: @@@ Cleaning @@@")

        clean(ProjectConfig.BUILD_DIR)


class StepInstallDP(StepBase):
    def run(self):
        if not self.config.install_dp:
            return

        Logger.Info("Builder: @@@ Installing dependencies @@@")

        if self.config.debug:
            run_conan_command(f"conan install {ProjectConfig.PROJECT_ROOT} --build=missing -s build_type=Debug")
        else:
            run_conan_command(f"conan install {ProjectConfig.PROJECT_ROOT} --build=missing -s build_type=Release")


class StepBuild(StepBase):
    def run(self):
        if not self.config.build:
            return

        Logger.Info("Builder: @@@ Building @@@")

        if self.config.debug:
            run_conan_command(f"conan build {ProjectConfig.PROJECT_ROOT} -s build_type=Debug")
        else:
            run_conan_command(f"conan build {ProjectConfig.PROJECT_ROOT} -s build_type=Release")


class StepPack(StepBase):
    def run(self):
        if not self.config.pack:
            return

        Logger.Info("Builder: @@@ Packing @@@")

        if self.config.debug:
            run_conan_command(f"conan install {ProjectConfig.PROJECT_ROOT} --deployer=runtime_deploy -s build_type=Debug")
        else:
            run_conan_command(f"conan install {ProjectConfig.PROJECT_ROOT} --deployer=runtime_deploy -s build_type=Release")


class StepTest(StepBase):
    def run(self):
        if not self.config.test:
            return

        Logger.Info("Builder: @@@ Testing @@@")


class Builder(BuilderBase):
    def setup_steps(self):
        self.steps.append(StepClean())
        self.steps.append(StepInstallDP())
        self.steps.append(StepBuild())
        self.steps.append(StepPack())
        self.steps.append(StepTest())

    def setup(self, config):
        self.config = config

    def setup_and_run(self, config):
        self.setup(config)
        self.run()


if '__main__' == __name__:
    builder = Builder()
    builder.setup_and_run(sys.argv[1:])
