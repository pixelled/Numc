from utils import *
import dumbpy as dp

def f(*args, **kwd):
    return dp.Matrix(*args, **kwd)

f([[1]])

