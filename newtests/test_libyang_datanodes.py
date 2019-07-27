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
        xpath = BASE_XPATH + ":types/str1"
        value = "this-is-a-string"
        self.data.set_xpath(xpath, value);
        result = next(self.data.get_xpath(xpath)).value

        # Assert
        self.assertEqual(result, value)

    def test_multiple(self):
        # Act
        self.data.set_xpath(BASE_XPATH + ":types/str1", "A");
        self.data.set_xpath(BASE_XPATH + ":types/str2", "B");

        result = list(self.data.get_xpath(BASE_XPATH + ":types/*"))

        # Assert
        self.assertEqual(len(result), 2)
