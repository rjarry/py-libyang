from _libyang import ffi
from _libyang import lib

from .util import c2str
from .util import str2c


class DataNode(object):

    INT_TYPES = (
        lib.LY_TYPE_INT8,
        lib.LY_TYPE_INT16,
        lib.LY_TYPE_INT32,
        lib.LY_TYPE_INT64,
        lib.LY_TYPE_UINT8,
        lib.LY_TYPE_UINT16,
        lib.LY_TYPE_UINT32,
        lib.LY_TYPE_UINT64,
    )

    BOOL_TYPES = (
        lib.LY_TYPE_BOOL,
    )

    DECIMAL_TYPES = (
        lib.LY_TYPE_DEC64,
    )

    EMPTY_TYPES = (
        lib.LY_TYPE_EMPTY,
    )

    def __init__(self, context, lyd_node, xpath=None):
        self.value = self._get_value_from_lyd_node(lyd_node,xpath)

    @staticmethod
    def convert_python_value(value):
        if isinstance(value, bool):
            if value is True:
                return 'true'
            return 'false'

        if value is None:
            return ""

        return str(value)

    @staticmethod
    def _get_value_from_lyd_node(lyd_node, xpath=None):
        """
        When data is set /got from a lyd_node things come back as strings instead of their
        proper types.

        This method gets the value from a lyd_node and converts it to proper python types.
        """
        if lyd_node.schema.nodetype  in (lib.LYS_LEAF, lib.LYS_LEAFLIST):
            leaf = ffi.cast('struct lyd_node_leaf_list *', lyd_node)
            sleaf = ffi.cast('struct lys_node_leaf *', lyd_node.schema)
            type = ffi.addressof(sleaf.type).base
            if type in DataNode.INT_TYPES:
                return int(c2str(leaf.value_str))
            elif type in DataNode.BOOL_TYPES:
                if c2str(leaf.value_str) == 'true':
                    return True
                return False
            elif type in DataNode.DECIMAL_TYPES:
                return float(c2str(leaf.value_str))
            elif type in DataNode.EMPTY_TYPES:
                return None
            return c2str(leaf.value_str)

        raise ValueError("Didnt manage to set a vlue")
        return None

    def __repr__(self):
        cls = self.__class__
        return '<%s.%s: %s>' % (cls.__module__, cls.__name__, str(self))
