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
        return str( gdb.parse_and_eval( call ) )
    except:
        return "except"

class IStringPrinter:
    "Print an IString"
    
    def __init__ (self,val):
        self.val = val
    
    def to_string (self):
        return invoke( self.val, 'c_str()' )

def build_icp_printers():
    pp = gdb.printing.RegexpCollectionPrettyPrinter( "__PRJ_NAME__-__PRJ_BRANCH__" )
    pp.add_printer( '__PRJ_NAME__::IString', '^__PRJ_NAME__::IString$', IStringPrinter )
    return pp

gdb.printing.register_pretty_printer( gdb.current_objfile(), build_icp_printers() )
