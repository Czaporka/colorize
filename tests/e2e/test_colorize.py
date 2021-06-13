import os
from subprocess import Popen, PIPE
import unittest


DUT_PATH = os.environ["DUT_PATH"]
assert DUT_PATH is not None


class TestColorize(unittest.TestCase):

    def _test_sanity_generic(self, cmd):
        input_ = (
            "A a 1 12 12 0xFF 0b01\n"
            "Z z 0 -1 0xGG 0b02\n"
        ).encode()

        p = Popen(cmd, stdout=PIPE, stdin=PIPE, stderr=PIPE)
        out, err = p.communicate(input_)
        print("--------------------")
        print("stdout:", out)
        print("stderr:", err)
        print("--------------------")
        out = out.decode(errors="replace")
        err = err.decode(errors="replace")
        print("stdout:", out)
        print("stderr:", err)
        print("--------------------")

        self.assertEqual(p.returncode, 0)
        self.assertEqual(out, (
            "A a \x1b[96m1\x1b[0m \x1b[96m12\x1b[0m \x1b[96m12\x1b[0m \x1b"
            "[32m0xFF\x1b[0m \x1b[32m0b01\x1b[0m\n"
            "Z z \x1b[95m0\x1b[0m -\x1b[96m1\x1b[0m 0xGG \x1b[36m0b02\x1b[0m\n"
        ))

    def test_sanity_cxx(self):
        self._test_sanity_generic([DUT_PATH])


    def test_sanity_posix(self):
        self._test_sanity_generic([DUT_PATH, "--posix"])
