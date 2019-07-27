from _libyang import ffi
from _libyang import lib

from .util import c2str
from .util import str2c


class DataNode(object):

    STR_TYPES = (
        lib.LY_TYPE_STRING,
    )

    def __init__(self, context, lyd_node):
        self.value = self._get_value_type_from_lyd_node(lyd_node)

    @staticmethod
    def _get_value_type_from_lyd_node(lyd_node):
        if lyd_node.schema.nodetype  in (lib.LYS_LEAF, lib.LYS_LEAFLIST):
            # lyd_node_leaf_list is definetely a leaf or a leaf list
            leaf = ffi.cast('struct lyd_node_leaf_list *', lyd_node)
            sleaf = ffi.cast('struct lys_node_leaf *', lyd_node.schema)
            type = ffi.addressof(sleaf.type)
            if type.base in DataNode.STR_TYPES:
                return c2str(leaf.value_str)

        return None

    def __repr__(self):
        cls = self.__class__
        return '<%s.%s: %s>' % (cls.__module__, cls.__name__, str(self))
