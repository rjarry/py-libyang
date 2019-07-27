import os
import unittest
import libyang


YANG_DIR = os.path.join(os.path.dirname(__file__), 'yang')
YANG_MODULE = "minimal-integrationtest"
BASE_XPATH = "/" + YANG_MODULE


class test_libyangdata(unittest.TestCase):

    def setUp(self):
        self.ctx = libyang.Context(YANG_DIR)
        self.ctx.load_module("minimal-integrationtest")
        self.data = libyang.DataTree(self.ctx)

    def test_basic(self):
        # Act
        value = "this-is-a-string"
        self.data.set_xpath(BASE_XPATH + ":types/str", value);
        result = next(self.data.get_xpath(BASE_XPATH + ":types/str"))

        # Assert
        self.assertEqual(result, value)
