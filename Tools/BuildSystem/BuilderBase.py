class StepBase:

    def __init__(self):
        self.config = None

    def setup(self, config):
        self.config = config

    def run(self):
        pass

    def setup_and_run(self, config):
        self.setup(config)
        self.run()


class BuilderBase:

    def __init__(self):
        self.arguments = None
        self.config = None
        self.parser = None
        self.steps = []

        self.setup_argument_parser()
        self.setup_steps()

    def setup_argument_parser(self):
        pass

    def setup_steps(self):
        pass

    def setup_sependencies(self):
        pass

    def setup(self, arguments):
        self.arguments = arguments
        self.config = self.parser.parse_args(args=arguments)
        self.setup_sependencies()

    def run(self):
        for step in self.steps:
            step.setup_and_run(self.config)

    def setup_and_run(self, arguments):
        self.setup(arguments)
        self.run()
