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

    def __init__(self, context, lyd_node):
        self.value = self._get_value_type_from_lyd_node(lyd_node)

    @staticmethod
    def _get_value_type_from_lyd_node(lyd_node):
        """
        This would have seemed to be more complex that it really is

        for a node that's of type int8
           lyd_new_path(root_a, NULL, "/minimal-integrationtest:types/int_8", "55", 0, 1);
        rather than
           lyd_new_path(root_a, NULL, "/minimal-integrationtest:types/int_8", 55, 0, 1);

        I was thinking that we'd have to know if we should call c2tr or not.

        But of course we will have to turn things back into integers or booleans

        """
        if lyd_node.schema.nodetype  in (lib.LYS_LEAF, lib.LYS_LEAFLIST):
            # lyd_node_leaf_list is definetely a leaf or a leaf list
            leaf = ffi.cast('struct lyd_node_leaf_list *', lyd_node)
            sleaf = ffi.cast('struct lys_node_leaf *', lyd_node.schema)
            type = ffi.addressof(sleaf.type).base
            if type in DataNode.INT_TYPES:
                # TODO: should we int this or just look for the right type?
                return int(c2str(leaf.value_str))

            return c2str(leaf.value_str)

        return None

    def __repr__(self):
        cls = self.__class__
        return '<%s.%s: %s>' % (cls.__module__, cls.__name__, str(self))
