import gdb
import gdb.printing
import itertools
import re


def invoke( val, method ):
    if not val.address or not val.type.name:
        return None
    addr = str( val.address ).split( ' ' )[0]
    call = "(*(%s*)(%s)).%s" % ( val.type.name, addr, method )
    try:
        return gdb.parse_and_eval( call )
    except:
        return "except1"

def invokeDeref( val, method ):
    if not val.address or not val.type.name:
        return None
    addr = str( val.address ).split( ' ' )[0]
    call = "*(*(%s*)(%s)).%s" % ( val.type.name, addr, method )
    try:
        return gdb.parse_and_eval( call )
    except:
        return "except2"

class IStringPrinter:
    "Print an IString"
    
    def __init__ (self,val):
        self.val = val
    
    def to_string (self):
        return str( invoke( self.val, 'c_str()' ) )

class IPtrIterator:
    "An iterator for smart pointer types with a single 'child' value"

    def __init__(self, idx,  val):
        self.idx = idx
        self.val = val
        self.typename = val.dereference().type
        self.count = 0

    def __iter__(self):
        return self

    def next(self):
        self.count = self.count + 1
        if self.count == 3:
            raise StopIteration
        if self.count == 1:
            return ( 'idx', self.idx )
        if self.count == 2:
            return ( str(self.typename), self.val.dereference() )

class IPtrPrinter:
    "Print an IPtr"
    
    def __init__ (self,val):
        self.val = val
    
    def to_string (self):
        # return '{ idx = ' + str( self.val["_idx"] ) + ', ' + str(self.val.type) + ' = ' + str( invokeDeref( self.val, 'get()' ) ) + ' }'
        return ''

    def children (self):
        return IPtrIterator( self.val["_idx"], invoke( self.val, 'get()' ) )

def build_icp_printers():
    pp = gdb.printing.RegexpCollectionPrettyPrinter( "__PRJ_NAME__-__PRJ_BRANCH__" )
    pp.add_printer( '__PRJ_NAME__::IPtr'   , '^__PRJ_NAME__::IPtr.*$', IPtrPrinter )
    pp.add_printer( '__PRJ_NAME__::IString', '^__PRJ_NAME__::IString$', IStringPrinter )
    return pp

gdb.printing.register_pretty_printer( gdb.current_objfile(), build_icp_printers() )
