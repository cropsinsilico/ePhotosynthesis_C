#!/usr/bin/env python3
"""
   Copyright (2020)   Douglas N Friedel

 University of Illinois at Urbana Champaign
 National Center for Supercomputing Applications
 Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.


   This file is part of e-photosynthesis.

    e-photosynthesis is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation;

    e-photosynthesis is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License (GPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.




    This module is designed to convert ePhotosynthesis Matlab code to C++. The conversion
    is not 100% accurate, but gets 95% of the work done automatically.
"""
import glob
import re
import copy
from collections import OrderedDict

class Driver:
    """
        Class for handling the driver codes as they need to be converted to accept Sundials style arguments
    """
    def __init__(self, name, driveFunction, args=None, iniFunc=None, mbFunc=None):
        self.name = name
        self.function = driveFunction
        if args is not None:
            self.args = args
        else:
            self.args = []
        self.iniFunc = iniFunc
        self.mbFunc = mbFunc

    def __str__(self):
        st = f"Diver:\n  {self.name}\n  {self.function}\n  {str(self.args)}\n  {self.iniFunc}\n  {self.mbFunc}\n"
        return st

class FileData:
    """
        Class to hold the data for each matlab file
    """
    def __init__(self, orig):
        self.orig = orig      # the original file contents
        self.lines = []       # a list containing the file contents as it is converted to C++
        self.localVars = {}   # dict for any local variables
        self.localArrays = {} # dict for any local arrays
        self.constants = []   # list of constant variables
        self.timeVars = {}    # lisiting of time indicating variables
        self.func = None      # the function in the file (if any)
        self.calls = set()    # track what modules are called
        self.usesGlobals = False  # whether global variables are used
        self.usedGlobals = []  # what global variables are used

class Func:
    """
        Class for holding the function data
    """
    def __init__(self, name, ret, fname, inp=None):
        self.name = name       # the function name
        if inp is not None:    # the function inputs
            self.inputs = inp
        else:
            self.inputs = []
        self.inTypes = [None] * len(self.inputs)
        self.file = fname
        self.ret = ret         # the function return variable name
        self.rtype = None      # the function return type
        self.pattern = [re.compile(fr"^{name}[^a-zA-Z0-9_]"), re.compile(fr"[^a-zA-Z0-9_]{name}[^a-zA-Z0-9_]")]
        self.needGlobals = False
        self.calls = set()

    def __str__(self):
        st = f"Function\n  {self.name}\n  ret: {self.ret}\n  rtype: {self.rtype}\n  inputs: {str(self.inputs)}\n  called by: {str(self.calls)}\n  in: {self.file}"
        return st

class MyVar:
    """
        Class to hold the data on a variable
    """
    def __init__(self, name, files=None, count=0, const=False, asgn=None):
        self.name = name        # the name of the variable
        if files is None:       # list of what files it appears in (global variables)
            self.files = []
        else:
            self.files = files
        self.count = count      # the number of times a variable is used (not assigned a value)
        self.const = const      # bool indicating if the variable is a constant
        if asgn is None:
            self.assign = []    # list of all assignment statements
        else:
            self.assign = asgn
        self.pattern = [re.compile(fr"^{name}[^a-zA-Z0-9_]"), re.compile(fr"[^a-zA-Z0-9_]{name}[^a-zA-Z0-9_]")]
        self.value = 0.         # the value of the variable, only used for constants.

    def __str__(self):
        st = f"{self.name}\n  count={self.count}\n  files={self.files}\n  const={self.const}"
        st += "\n  assign:\n"
        for i in self.assign:
            st += f"         {i}\n"
        return st

class Array:
    """
        Class for array variables
    """
    def __init__(self, var, init=False):
        self.var = var           # the variable base class
        self.length = 0          # the length of the array
        self.initialized = init  # whether it is explicitly instantiated in matlab

    def __str__(self):
        st = str(self.var)
        st += f"  length={self.length}\n"
        st += f"  init={self.initialized}\n"
        return st

    def assign(self, val):
        self.var.assign.append(val)

    @property
    def pattern(self):
        return self.var.pattern

    @property
    def name(self):
        return self.var.name

    @property
    def files(self):
        return self.var.files

    @property
    def count(self):
        return self.var.count

    @property
    def const(self):
        return self.var.const

    @name.setter
    def name(self, val):
        self.var.name = val

    @files.setter
    def files(self, val):
        self.var.files = val

    @count.setter
    def count(self, val):
        self.var.count = val

    @const.setter
    def const(self, val):
        self.var.const = val

    @const.setter
    def const(self, val):
        self.var.const = val

    @pattern.setter
    def pattern(self, val):
        self.var.pattern = val

### NOTE
#
#    The original conversion of this code used 2D matrices to hold intermediate data. This was
#    manually converted to use templated classes in C++, thus the below code will not
#    produce what is currently used.
#
###
class Matrix:
    """
        Class for matrix variables
    """
    def __init__(self, arry, length=None, init=False):
        self.array = arry         # the array base class
        if length is None:        # the length of each dimension
            self.length = []
        else:
            self.length = length
        self.initialized = init   # whether it is explicitly instantiated in matlab

    def assign(self, val):
        self.array.assign(val)

    @property
    def name(self):
        return self.array.name

    @property
    def files(self):
        return self.array.files

    @property
    def count(self):
        return self.array.count

    @property
    def const(self):
        return self.array.const

    @property
    def pattern(self):
        return self.array.pattern

    @name.setter
    def name(self, val):
        self.array.name = val

    @files.setter
    def files(self, val):
        self.array.files = val

    @count.setter
    def count(self, val):
        self.array.count = val

    @const.setter
    def const(self, val):
        self.array.const = val

    @pattern.setter
    def pattern(self, val):
        self.array.pattern = val

def report(item, i,  items):
    """ Basic reporting function, used to indicate progress of a task
    """
    out = f"  {item:25s}"
    lg = int(50.*i/len(items))
    out += f"[{'=' * lg + '>':51s}] {lg * 2}%"
    print(out, end='\r')

# constants used in the contains function
NO = 0            # variable is not in stmt
YES = 1           # variable is in stmt
STARTSWITH = 2    # variable is at the beginning of the stmt
MULTI = 3         # variable appears more than once in stmt

ARR = 'arr'
DOUBLE = 'double'
GLOBAL = 'myVars'

def contains(var, stmt):
    """
        search over each potential pattern for a variable to see if it is in the given statement
    """
    stmt = stmt.split('//')[0]
    m = var.pattern[0].search(stmt)
    if m is None:
        m = var.pattern[1].search(stmt)
        if m is None:
            return NO
        return YES
    else:
        ts = var.pattern[0].sub("", stmt)
        m = var.pattern[1].search(ts)
        if m is None:
            return STARTSWITH
        return MULTI

knownFunctions = ['size']
operators = {'+': '',
             '-': '',
             '*': '',
             '/': ''
             }
cvars = ['x', 'y', 'z']
# precompile regex searches that are used a lot
deq = re.compile(r"\s*==\s*")  # pattern for equality operator surrounded by zero or more whitespace characters
seq = re.compile(r"\s*=\s*")   # pattern for assignment operator surrounded by zero or more whitespace characters

par = re.compile(r"\s*\(\s*")  # pattern for openin bracket surrounded by zero or more whitespace characters
epar = re.compile(r"\s*\)")    # pattern for closing bracket preceded by zero or more whitespace characters
# patterns for ^ to pow() conversion
pow1 = re.compile(r"(.*)(\(.*\))\s*\^\s*(\S*)\s+(.*)")
pow2 = re.compile(r"(.*\s+)(\S+)\s*\^\s*(\S*)\s+(.*)")

# patterns for spacing
com = re.compile(r"\s*,\s*")
subsub = re.compile(r"\( - ")

# listing of operators and special symbols
rep = {'+': '',
       '-': '',
       '*': '',
       '/': '',
       '^': '',
       'exp': '',
       '(': '',
       ')': '',
       '[': '',
       ']': '',
       '.': '',
       ';': ''}

# make a special regex for the above
rep = dict((re.escape(k), v) for k, v in rep.items())
pattern = re.compile('|'.join(rep.keys()))

globalVars = {}          # dict for the global variables
globalArrays = {}        # dict for global arrays
globalMatrix = {}        # dict for global matrices
functions = {}           # dist for functions
timeArraySizes = {}      # dict for *_TIME_N array sizes
drivers = {}
modFuncs = {}
mbFuncs = []

def getVars(line, fname):
    """
        Generate a list of input variables from a function call
    """
    loc = line.find(fname + '(')
    loc = line.find('(', loc)
    eloc = find_balance(line, loc - 1)
    temp = line[loc + 1 : eloc - 1].replace(" ", '')
    if len(temp) == 0:
        return []
    return temp.split(',')

def find_balance(stmt, start=0):
    """ Function to find the outermost set of balanced parentheses.

        Parameters
        ----------
        stmt: str
            The string to look for the balanced parentheses in.

        start: int
            The index to start at. Default is 0.

        Returns
        -------
        int indicating the index + 1 of the location of the closing parentheses. Useful in slicing
        an array.
    """
    open_list = ['(']
    close_list = [')']
    stack = []
    found = False
    for i, val in enumerate(stmt[start:]):
        if val in open_list:
            stack.append(val)
            found = True
        elif val in close_list:
            pos = close_list.index(val)
            if (stack and open_list[pos] == stack[len(stack) - 1]):
                stack.pop()
            else:
                raise Exception("Unbalanced parentheses found")
            if found and not stack:
                return i + start + 1
    raise Exception("Unbalanced parentheses found")

def rfind_balance(stmt, start=-1):
    """ Function to find the outermost set of balanced parentheses, starting from the right.

        Parameters
        ----------
        stmt: str
            The string to look for the balanced parentheses in.

        start: int
            The index to start at. Default is -1.

        Returns
        -------
        int indicating the index of the location of the closing parentheses. Useful in slicing
        an array.
    """
    open_list = [')']
    close_list = ['(']
    stack = []
    found = False
    if start == -1:
        start = len(stmt) - 1

    for i in range(start, -1, -1):
        val = stmt[i]
        if val in open_list:
            stack.append(val)
            found = True
        elif val in close_list:
            pos = close_list.index(val)
            if (stack and open_list[pos] == stack[len(stack) - 1]):
                stack.pop()
            else:
                raise Exception("Unbalanced parentheses found")
            if found and not stack:
                return i
    raise Exception("Unbalanced parentheses found")

def spaceFormat(lines):
    """ Function to format the lines by removing extra spaces, convert comment markers from
        % (matlab) to // (C), and add spaces around operators to make reading the code
        easier

        Parameters
        ----------
        lines: list
            List of the lines of code to go through

        Returns
        -------
        List of the modified lines of code.

    """
    newLines = []
    for line in lines:
        line = line.strip()
        # convert comment markers
        line = line.replace('%', '//')
        line = line.strip()
        if line.startswith('//'):
            newLines.append(line)
            continue
        comment = ""
        loc = line.find('//')
        if loc > 0:
            comment = line[loc:]
            line = line[:loc]
        # get proper spacing around parentheses
        line = par.sub('(', line)
        line = epar.sub(')', line)

        # get proper spacing around operators
        if '==' in line:
            line = deq.sub(' == ', line)
        elif '=' in line and not(line.startswith('if') or line.startswith('for') or line.startswith("function")):
            line = seq.sub(' = ', line)
        for op in operators.keys():
            if op in line:
                temp = line.split(op)
                for i, v in enumerate(temp):
                    temp[i] = v.strip()
                line = f" {op} ".join(temp)
        # remove extra spaces before the ;
        line = re.sub(r"(.*\S)\s+;", r"\1;", line)
        # remove extra spaces before commas
        line = re.sub(r"(.*)\s* ,\s*(\S+.*)", r"\1, \2", line)
        # ensure there is only one space after commas
        line = com.sub(', ', line)
        # correct the spacing for negative numbers
        line = subsub.sub('(-', line)
        newLines.append(line + comment)
    return newLines

def powFormat(line):
    """ Convert the matlab exponential ^, to C's pow()

        Parameters
        ----------
        line - str
            The line of code to convert

        Returns
        -------
        Str containing the converted code

    """
    # remove any end comments from the line
    if '//' in line:
        temp = line.split('//', 1)
        comments = '  //' + temp[1]
        line = temp[0].strip()
    # we can only properly handle lines with only one ^
    multi = False
    if line.count('^') > 1:
        #line += "  ----- COMPLEX"
        multi = True
    newline = ""
    comments = ""
    haveEnd = False
    if line.startswith(indent):
        newline += indent
    # remove the ;
    if line.endswith(';'):
        line = line.replace(';', '')
        comments = ';' + comments
        haveEnd = True
    # parse the code
    parts = line.split('^', 1)
    parts[0] = parts[0].strip()
    # determine what the base of the exponent is
    #  is it a multi-part calculation surrounded by ()
    if parts[0].endswith(')'):
        loc = rfind_balance(parts[0])
        newline = parts[0][:loc]
        parts[0] = parts[0][loc:]
    # or is it just a single item
    elif ' ' in parts[0]:
        temp = parts[0].rsplit(' ', 1)
        newline += temp[0]
        parts[0] = temp[1]
    if parts[0].startswith('('):
        if parts[0].count(')') < parts[0].count('('):
            while parts[0].count(')') < parts[0].count('(') and parts[0].startswith('('):
                newline += '('
                parts[0] = parts[0][1:].strip()

    # determine the exponent of the expression
    parts[1] = parts[1].strip()
    # is it enclosed by ()
    if parts[1].startswith('('):
        loc = find_balance(parts[1])
        if loc < len(parts[1]):
            comments = parts[1][loc:] + comments
            parts[1] = parts[1][:loc]
        else:
            parts[1] = parts[1][1:-1]
    # or just a single item
    else:
        if ' ' in parts[1]:
            temp = parts[1].split(' ', 1)
            # if it is a negative number
            if temp[0] == '-':
                if ' ' in temp[1]:
                    tt = temp[1].split(' ', 1)
                    parts[1] = '-' + tt[0]
                    comments = ' ' + tt[1] + comments
                else:
                    parts[1] = '-' + temp[1]
            else:
                comments = ' ' + temp[1] + comments
                parts[1] = temp[0]
        # handle any parts that are enclosed in ()
        if '(' in parts[1]:
            temp = parts[1].split('(', 1)
            comments = '(' + temp[1] + comments
            parts[1] = temp[0]
        if ')' in parts[1]:
            temp = parts[1].split(')', 1)
            comments = ')' + temp[1] + comments
            parts[1] = temp[0]
    # put it all back together
    newline += f" pow({parts[0]}, {parts[1]}){comments}"
    if multi:
        newline = powFormat(newline)
    return newline

def processGlobals(fobj, stmt):
    """ Function to process global variables. This will prepend the myVars. object name to the
        variables in the code. Time variables (those ending in TIME_N) are tracked to find their
        array dimensions.

        Parameters
        ----------
        fobj - Files type object
            The current Files object

        stmt - str
            The currentl line of code

        Returns
        -------
        Str of the current line of code with modifications

    """
    # loop over the global variables
    for var in fobj.usedGlobals:#globalVars.keys():
        if 'ode15s' in stmt:
            continue
        m = re.match(fr".*(\[|\s|\(|=|\+|-|\/|\*|^){var}(\[|\]|\s|\(|=|\+|-|\/|\*|;|\)).*", stmt)
        # prepend myVars. as needed
        if m is not None:
            stmt = stmt.replace(var, f"{GLOBAL}.{var}")
            #stmt = re.sub(re.escape(m.group(1)) + var + re.escape(m.group(2)), m.group(1) + GLOBAL + "." + var + m.group(2), stmt)
            # keep track of TIME_N array sizes
            if var.endswith("TIME_N") and var + ']' in stmt:
                setVar = stmt.split('[')[0].replace(GLOBAL + '.', '')
                pvar = stmt.split('[')[0].strip().replace(GLOBAL + '.', '')
                if pvar not in timeArraySizes.keys():
                    timeArraySizes[pvar] = 0
                m = re.match(fr".*\[{GLOBAL}\.{var}\]\[(\d+)\].*", stmt)
                # determine if the format is [TIME_N][m] or [m][TIME_N]
                if m is not None:
                    num = int(m.group(1))
                    timeArraySizes[pvar] = max(globalArrays[pvar].length, num + 1)
                    stmt = re.sub(fr"\[{GLOBAL}\.{var}\]\[\d+]", f"[{GLOBAL}.{var} - 1][{num}]", stmt)
                    if setVar not in fobj.timeVars.keys():
                        fobj.timeVars[setVar] = 0
                    continue
                m = re.match(fr".*\[(\d+)\]\[{GLOBAL}\.{var}\].*", stmt)
                if m is not None:
                    num = int(m.group(1))
                    timeArraySizes[pvar] = max(globalArrays[pvar].length, num + 1)
                    stmt = re.sub(fr"\[\d+\]\[{GLOBAL}\.{var}\]", f"[{num}][{GLOBAL}.{var} - 1]", stmt)
                    if setVar not in fobj.timeVars.keys():
                        fobj.timeVars[setVar] = 1

    return stmt


def processArrays(lines, idx, arrays):
    """ Function to format the code for array variables and track the size of each array.

        Parameters
        ----------
        lines - list
            List of the lines of code

        idx - int
            The index of the current line

        arrays - dict
            Dict containing the Array objects to use

        Returns
        -------
        Int indicating the number of lines of code that were added

    """
    ret = 0
    stmt = lines[idx]
    keys = arrays.keys()
    # loop over the given Array objects
    for ary in keys: #ary, v in arrays.items():
        if contains(arrays[ary], stmt) == NO:
            continue
        if ary + '(' in stmt or ary + '[' in stmt:
            substmt = []
            multi = False
            if contains(arrays[ary], stmt) == MULTI:
                multi = True

            offset = -1
            # find the array items
            m = re.search(fr"{ary}\s*\(\s*(\S+(?:\s*(?:\+|-|,)\s*\S+){{0,2}})\s*\)", stmt)
            if m is None:
                m = re.search(fr"{ary}\s*\[\s*(\S+(?:\s*(?:\+|-|,)\s*\S+){{0,2}})\s*\]", stmt)
                offset = 0
            val = m.group(1)
            matches = []
            if offset == 0:
                matches = re.findall(fr"{ary}\s*\[\s*(\S+(?:\s*(?:\+|-|,)\s*\S+[^)]\s*){{0,2}})\s*\]", stmt)
            else:
                if '))' in stmt:
                    matches = re.findall(fr"{ary}\s*\(\s*(\S+(?:\s*(?:\+|-|,)\s*[^)]\s*){{0,2}})\s*\)", stmt)
                else:
                    matches = re.findall(fr"{ary}\s*\(\s*(\S+(?:\s*(?:\+|-|,)\s*\S+[^)]\s*){{0,2}})\s*\)", stmt)
                    if not matches:
                        matches = re.findall(fr"{ary}\s*\(\s*(\S+(?:\s*(?:\+|-|,)\s*\S*[^)]\s*){{0,2}})\s*\)", stmt)
            # if this is a 2D array
            for z, val in enumerate(matches):
                if ',' in val:
                    if ary not in globalMatrix.keys():
                        globalMatrix[ary] = Matrix(arrays[ary], [0] * (val.count(',') + 1))
                    nums = val.split(',')
                    for i, va in enumerate(nums):
                        nums[i] = va.strip()

                    onums = copy.deepcopy(nums)
                    isVar = [False] * len(nums)
                    count = 0
                    # if this is in (:), (x:), (:x), or (x:y) notation the code needs to be
                    #  changed to a for loop for C++
                    for i, n in enumerate(nums):
                        if ':' in n:
                            stin = 1
                            enin = -1
                            if n == ':':
                                enin = ary + f"_SIZE{i+1}"
                            else:
                                if n.startswith(':'):
                                    enin = n.split(':')[1].strip()
                                elif n.endswith(':'):
                                    stin = n.split(':')[0].strip()
                                    enin = ary + f"_SIZE{i+1}"
                                else:
                                    t = n.split(':')
                                    stin = t[0]
                                    enin = t[1]
                            onums[i] = cvars[i]
                            if z == 0:
                                newline = f"for {cvars[i]} = {stin}:{enin}"
                                lines.insert(idx, newline)
                                ret += 1
                                idx += 1
                                count += 1
                                isVar[i] = True

                        else:
                            try:
                                onums[i] = int(n)
                                globalMatrix[ary].length[i] = max(globalMatrix[ary].length[i], onums[i] - offset)
                                onums[i] = str(onums[i] + offset)
                            except ValueError:
                                isVar[i] = True
                    # if this is an assignment then track the size of the array by storing the
                    # highest index found

                    if '=' in stmt and z == 0:
                        loc = stmt.find(ary)
                        if stmt.find('=') < loc:
                            lhs = stmt.split('=')[0].strip()
                            tonum = copy.deepcopy(onums)
                            if '(' not in lhs and '[' not in lhs:
                                if any(isVar) and not all(isVar):
                                    if isVar[0]:
                                        tonum = onums[:-1]
                                    elif isVar[-1]:
                                        tonum = onums[1:]
                                    stmt = re.sub(fr"{lhs} =", fr"{lhs}[{']['.join(tonum)}] =", stmt)
                    stmt = re.sub(fr"{ary}\s*(?:\(|\[)\s*{val}\s*(?:\)|\]){{1}}", f"{ary}[{']['.join(onums)}]", stmt)

                    lines[idx] = stmt
                    for i in range(count):
                        if z != 0:
                            break
                        lines.insert(idx + 1, "end")
                        ret += 1
                # if this is a 1D array
                else:
                    val = val.strip()
                    # if this is in (:), (x:), (:x), or (x:y) notation the code needs to be
                    #  changed to a for loop for C++
                    if ':' in val:
                        stin = 1
                        enin = -1
                        if val == ':':
                            enin = ary + f"_SIZE"
                        else:
                            if val.startswith(':'):
                                enin = val.split(':')[1].strip()
                            elif val.endswith(':'):
                                stin = val.split(':')[0].strip()
                            else:
                                t = val.split(':')
                                stin = t[0]
                                enin = t[1]
                        if z == 0:
                            newline = f"for {cvars[0]} = {stin}:{enin}"
                            onum = cvars[0]
                            lines.insert(idx, newline)
                            ret += 2
                            idx += 1
                            if '=' in stmt:
                                loc = stmt.find(ary)
                                if stmt.find('=') < loc:
                                    lhs = stmt.split('=')[0].strip()
                                    if '(' not in lhs and '[' not in lhs:
                                        stmt = re.sub(fr"{lhs} =", fr"{lhs}[{onum}] =", stmt)
                        # convert from () to [] format
                        stmt = re.sub(fr"{ary}\s*(?:\(|\[)\s*{val}\s*(?:\)|\])", f"{ary}[{onum}]", stmt)
                        lines[idx] = stmt
                        if z == 0:
                            lines.insert(idx + 1, 'end')
                    else:
                        # track the highest index for the array
                        nval = val
                        try:
                            nval = int(val)
                            arrays[ary].length = max(arrays[ary].length, nval + offset + 1)
                            nval += offset
                        except ValueError:
                            pass
                        # convert from () to [] format
                        stmt = re.sub(fr"{ary}\s*(?:\(|\[)\s*{re.escape(val)}\s*(?:\)|\])", f"{ary}[{nval}]", stmt)
                        lines[idx] = stmt
    return ret

indent = "    "

def initialPass(names):
    """ Function that performs the initial pass over the files. I reads in the files,
        calls the spacing format function, gathers information on functions and global variables,
        and does initial identification of arrays (those instantiated with calls to zeros or ones).

        Parameters
        ----------
        names - list
            List of the file names to process

        Returns
        -------
        Dict containing the Files objects

    """
    files = {}
    drv = {}
    for i, f in enumerate(names):
        report(f, i, names)
        currentGlobals = {}
        newData = []
        # read in the file
        data = open(f, 'r').readlines()
        files[f] = FileData(data)
        # format the spacings
        lines = spaceFormat(data)
        for line in lines:
            if not line or line.startswith('//'):
                newData.append(line)
            # gather data on functions, their return values, and arguments
            elif line.startswith('function'):
                rval = ''
                fname = ''
                args = ''
                line = line.replace(';', '')
                if '=' in line:
                    temp = line.split('=')
                    rval = temp[0].replace('function', '').strip()
                    if '(' in temp[1]:
                        tt = temp[1].split('(')
                        fname = tt[0].strip()
                        args = tt[1].strip()
                    else:
                        fname = temp[1].strip()
                else:
                    if '(' in line:
                        temp = line.split('(')
                        fname = temp[0].strip()
                        args = temp[1].strip()
                    else:
                        fname = line
                fname = fname.replace(';', '')
                fname = fname.replace('function', '')
                fname = fname.strip()
                files[f].func = fname
                if len(args) > 1:
                    args = args.replace(')', '')
                    args = args.replace(' ', '')
                    args = args.split(',')
                functions[fname] = Func(fname, rval, f, args)
                for a in args:
                    files[f].localVars[a] = MyVar(a, count=2)
                newData.append(line)
            # gather information on global variables
            elif line.startswith('global'):
                files[f].usesGlobals = True
                if files[f].func is not None:
                    functions[files[f].func].needGlobals = True
                var = line.split()[1]
                var = var.replace(';', '')
                if var not in globalVars.keys():
                    globalVars[var] = MyVar(var)
                    currentGlobals[var] = False
                if var not in files[f].usedGlobals:
                    files[f].usedGlobals.append(var)
                newData.append('//' + line)
            else:
                if 'ode15s' in line:
                    tmp = line.split("ode15s")[1]
                    ifunc = tmp.split(',')[0].replace('(', '').replace('@', '')
                    drv[f] = ifunc
                for g in currentGlobals.keys():
                    if contains(globalVars[g], line) != NO:
                        currentGlobals[g] = True
                # find the obvious arrays
                if "zeros" in line or "ones" in line:
                    found = False
                    for gv, val in globalVars.items():
                        if contains(val, line) == STARTSWITH:
                            if gv not in globalArrays.keys():
                                globalArrays[gv] = Array(globalVars[gv])
                            found = True
                            line = '// ' + line
                            break
                    if not found:
                        temp = line.split('=')[0].strip()
                        temp = temp.split('(')[0].strip()
                        v = MyVar(temp)
                        files[f].localVars[temp] = v
                        files[f].localArrays[temp] = Array(v, init=True)

                newData.append(line)

        if files[f].func is None:
            del files[f]
            if f in drv.keys():
                del drv[f]
        else:
            # assemble the line of code
            files[f].lines = newData

            for g, v in currentGlobals.items():
                if v:
                    globalVars[g].files.append(f)
        report("", i + 1, names)

    for d in drv.keys():
        print(d)
        bname = d.split("Drive")[0]
        ini = None
        fname = files[d].func
        for name in names:
            if re.search(fr"{bname}Ini.m", name, re.IGNORECASE) is not None:
                ini = files[name].func
        if bname.endswith('_'):
            bname = bname[:-1]
        if fname is not None:
            drivers[d] = Driver(bname, fname, functions[fname].inputs, ini, drv[d])
            modFuncs[fname] = bname
            if ini is not None:
                modFuncs[ini] = bname
            modFuncs[drv[d]] = bname

    return files

def locateVars(files):
    """ This function looks for variable assignment statements, tracking information on any that are
        found.

        Parameters
        ----------
        files - dict
            Dict of Files objects to go through

    """
    count = 0
    # loop over the inputs
    for f, v in files.items():
        report(f, count, files)
        for i, line in enumerate(v.lines):
            funcFound = False
            # skip comment lines
            if not line or line.startswith('//'):
                continue
            # skip function, if and for statements
            elif line.startswith('function') or line.startswith("if") or line.startswith("for"):
                continue
            for fn, fun in functions.items():
                if contains(fun, line) != NO:
                    fun.calls.add(f)
                    files[f].calls.add(fn)
                    m = re.search(r"(\(|\s)begin,?", line, re.IGNORECASE)
                    line = re.sub(r"(\(|\s)begin,?", r"\1", line, flags=re.IGNORECASE)
                    funcFound = True
                    if fun.name + ';' in line:
                        line = line.replace(fun.name + ';', f"{fun.name}();")
                    v.lines[i] = line

            if line.startswith('['):
                continue
            # find assignment statements
            if '=' in line and '==' not in line:
                m = re.match(r"(.*)(.*//.*)", line)
                temp = None
                if m is not None:
                    if '=' in m.group(1):
                        temp = m.group(1).split('=')
                else:
                    temp = line.split('=')
                if len(temp) > 1:
                    var = temp[0].strip()
                    var = var.split('(')[0].strip()
                    # remove inline comments
                    loc = temp[1].find('//')
                    if loc >= 0:
                        temp[1] = temp[1][:loc]
                    # look for any function input parameters (used to determine parameter type)
                    if funcFound:
                        if var not in files[f].localVars.keys():
                            files[f].localVars[var] = MyVar(var, count=2)
                        files[f].localVars[var].assign.append(temp[1].strip())
                        continue
                    if v.func is not None:
                        for inp in functions[v.func].inputs:
                            if inp + '(' in temp[1]:
                                if inp not in files[f].localArrays.keys():
                                    files[f].localArrays[inp] = Array(files[f].localVars[inp], init=True)
                                break
                    # if a local variable is found
                    if var not in files[f].usedGlobals:#globalVars.keys():
                        if var not in files[f].localVars.keys():
                            files[f].localVars[var] = MyVar(var)
                        files[f].localVars[var].assign.append(temp[1].strip())
                        # look for indications of it being an array
                        if '(' in temp[0]:
                            if var not in files[f].localArrays.keys():
                                files[f].localArrays[var] = Array(files[f].localVars[var])
                            if v.func is not None and var in functions[v.func].inputs:
                                files[f].localArrays[var].initialized = True
                    # if it is a global variable
                    else:
                        tline = line.replace(' ', '')
                        globalVars[var].assign.append(temp[1].strip())
                        if var.endswith('_com'):
                            line = line.replace('0', 'false')
                            line = line.replace('1', 'true')
                            files[f].lines[i] = line
                        for gv in globalVars.keys():
                            # look to see if it is an array
                            if contains(globalVars[gv], tline) != NO and gv + '(' in tline:
                                if gv not in globalArrays.keys():
                                    globalArrays[gv] = Array(globalVars[gv])
                                break

        if v.func in functions.keys():
            theFunc = functions[v.func]
            if functions[v.func].inputs:
                if re.match('begin', theFunc.inputs[0], re.IGNORECASE) is not None:
                    del functions[v.func].inputs[0]
                elif re.match('be', theFunc.inputs[0], re.IGNORECASE) is not None:
                    del functions[v.func].inputs[0]

            if functions[v.func].ret is not None:
                if functions[v.func].ret in v.localArrays:
                    functions[v.func].rtype = ARR
                else:
                    functions[v.func].rtype = DOUBLE
        count += 1
        report("", count, files)
    print("\n   Pass 2...")
    count = 0
    for f, v in files.items():
        report(f, count, files)
        for line in v.lines:
            if not line or line.startswith('//'):
                continue
            # skip function, if and for statements
            elif line.startswith('function') or line.startswith("if") or line.startswith("for") or line.startswith('['):
                continue
            # find assignment statements
            if '=' in line and '==' not in line:
                temp = line.split('=')
                pf = temp[1].strip()
                pv = temp[0].strip()
                for fun in functions.values():
                    if contains(fun, pf) == STARTSWITH:
                        if fun.rtype == ARR:
                            if pv not in files[f].localArrays:
                                files[f].localArrays[pv] = Array(files[f].localVars[pv])
                            files[f].localArrays[pv].length = -1
                        break

        count += 1
        report("", count, files)

def traceCalls(files, start, i=0):
    callMap = OrderedDict()
    callMap[start] = []
    for func in files[start].calls:
        callMap[start].append({'calls':func, 'trace':traceCalls(files, functions[func].file, i+1)})
    return callMap

def consistencyCheck(files):
    inconsistent = {}
    for name, f in files.items():
        for i, line in enumerate(f.lines):
            if not line or line.startswith('//') or '@' in line or line.startswith('function'):
                continue
            subl = line.split('//')[0]
            for fname in f.calls:
                if contains(functions[fname], line) != NO:
                    if functions[fname].rtype in [None, 'void'] and ' = ' in line:
                        temp = line.split('=', 1)[1].strip()
                        f.lines[i] = temp
                    v = getVars(subl, fname)
                    if len(v) != len(functions[fname].inputs):
                        if name not in inconsistent:
                            inconsistent[name] = []
                        inconsistent[name].append([fname, len(v), len(functions[fname].inputs), v, functions[fname].inputs])

    if inconsistent:
        print("\n\nThe following inconsistencies were found:")
        remove = set()
        for name, val in inconsistent.items():
            remove.add(name)
            for item in val:
                print(f"  {name} calls {item[0]} with {item[1]} arguments, {item[2]} are expected.")
                print(f"          {str(item[3])}")
                print(f"          {str(item[4])}")

        print("\nAttempting to reconcile")
        last = 0
        while len(remove) != last:
            last = len(remove)
            rmv = list(remove)
            for i in range(last):
                for name, f in files.items():
                    if files[rmv[i]].func in f.calls:
                        remove.add(name)
        print("Removing the following files:")
        for i in remove:
            print(f"    {i}")
            del functions[files[i].func]
            del files[i]



def processIf(stmt):
    """ Function to convert the format of if statements

        Parameters
        ----------
        stmt - str
            The code to convert

        Returns
        -------
        Str of the modified code

    """
    # strip off any outer ( or )
    if stmt.startswith('('):
        stmt = stmt[1:]
    if stmt.endswith(')'):
        stmt = stmt[:-1]
    # get proper spacing around operators
    stmt = stmt.replace(';', '')
    for x in ['>=', '<=', '==', '!=', '&&', '||', '>', '<']:
        if x in stmt:
            stmt = re.sub(fr"\s*{x}\s*", f" {x} ", stmt)
            break
    # if this is an equality check
    if '==' in stmt:
        temp = stmt.split(' == ')
        # _com variables are booleans, so convert 0 to false and 1 to true
        if temp[0].endswith('_com'):
            temp[1] = temp[1].replace(';', '')
            if int(temp[1]) == 0:
                stmt = '!' + temp[0]
            else:
                stmt = temp[0]
    # reassemble the statement
    stmt = "if (" + stmt + ")"
    return stmt

def ifAndFor(item, idx):
    """ Function to convert if statements and for loop to the proper format.

        Parameters
        ----------
        item - Files object
            The object to process

        idx - int
            The line of code to process
    """
    adds = []
    end = -1
    i = idx + 1
    theLine = item.lines[idx]
    # strip off any inline comments and ;
    loc = theLine.find('//')
    if loc >= 0:
        theLine = item.lines[idx][:loc]
    theLine.replace(';', '')
    count = 0
    # see how many line the if/for block encompasses
    while i < len(item.lines) and end < idx:
        # the end of the block
        if item.lines[i].startswith("end"):
            count += 1
            end = i
        # handle else and elseif statements
        elif item.lines[i].startswith("else"):
            if item.lines[i].startswith("elseif"):
                temp = item.lines[i].replace("else", "")
                item.lines[i] = "} else " + processIf(temp) + "{"
            else:
                item.lines[i] = "} else {"
            count += 1
        # handle any if/for blocks inside the current one
        elif item.lines[i].startswith("if") or item.lines[i].startswith("for"):
            adds += ifAndFor(item, i)
            count += 1
        elif item.lines[i].startswith('//') or not item.lines[i]:
            pass
        else:
            if ' = ' in item.lines[i]:
                temp = item.lines[i].split(' = ', 1)[0].strip()
                if '(' in temp:
                    temp = temp.split('(')[0].strip()
                elif'[' in temp:
                    temp = temp.split('[')[0].strip()
                if temp in item.localVars.keys():
                    fnd = False
                    for z in range(i):
                        if contains(item.localVars[temp], item.lines[z]) != NO:
                            fnd = True
                            break
                    if not fnd:
                        adds.append(temp)
            count += 1
        i += 1
    # format the if statement
    if theLine.startswith("if"):
        stmt = processIf(theLine.replace("if", "").strip())
    # format the for statement
    elif theLine.startswith("for"):
        stmt = theLine.replace("for", '').strip()
        temp = stmt.split('=')
        theVar = temp[0].strip()
        temp = temp[1].split(':')
        try:
            st = int(temp[0].strip()) - 1
        except ValueError:
            st += " - 1"
        en = temp[1].strip()
        stmt = f"for (int {theVar} = {st}; {theVar} < {en}; {theVar}++)"

    else:
        raise Exception("Bad call to ifAndFor")

    # if the block os more than 2 lines (including the if/for statement) then add {} around it
    for i in range(idx + 1, end):
        if ' else' not in item.lines[i]:
            item.lines[i] = indent + item.lines[i]
    if count > 2:
        stmt += ' {'
        item.lines[end] = '}'
    else:
        item.lines[end] = ''
    item.lines[idx] = stmt
    return adds

def secondPass(files):
    """ Function to do the second pass over all the Files objects. This will track variables usage
        arrays, ^, and if/for blocks.

        Parameters
        ----------
        files - dict
            Dict of the Files objects to process

        Returns
        -------
        Dict of the modified Files objects (this is not strictly necessary as the input
        and its contents are passed by reference)

    """
    counter = 0
    # loop over the objects
    for k, f in files.items():
        report(k, counter, files)
        i = 0
        # restart is used to indicate a change in the number of lines of code by the processing
        #  of arrays, this will trigger the loop to restart at the given index, and recalculate
        #  the number of lines of code, since Python does not like modifying loop objects
        restart = True
        while restart:
            restart = False
            idx = 0
            while i < len(f.lines):

                line = f.lines[i]
                if not line or line.startswith('//'):
                    i += 1
                    continue
                tline = line
                ending = ''
                loc = tline.find('//')
                if loc >= 0:
                    tline = line[:loc]
                # track local variables to see if they are actually used
                for var in f.localVars.keys():
                    if contains(f.localVars[var], tline) in [YES, MULTI]:
                        f.localVars[var].count += 1
                # track global variables to see if they are actually used
                for var in globalVars.keys():
                    if contains(globalVars[var], tline) in [YES, MULTI]:
                        globalVars[var].count += 1
                # process any arrays found, this can add lines to the code and cause the loop
                #  to restart
                if '(' in tline and 'function' not in tline:
                    idx = processArrays(f.lines, i, f.localArrays)
                    idx += processArrays(f.lines, i + idx, globalArrays)
                    #for q in range(idx):
                    tline = f.lines[i].split('//')[0]
                # process any lines with ^
                if '^' in tline:
                    f.lines[i] = powFormat(f.lines[i])
                # process global variables
                f.lines[i] = processGlobals(f, f.lines[i])
                # process for loops
                if tline.startswith('if') or tline.startswith('for'):
                    added = False
                    for z in ifAndFor(f, i):
                        added = True
                        idx += 1
                        f.lines.insert(i, f"{z};")
                    if added:
                        idx += 1
                if idx > 0:
                    i += idx
                    restart = True
                    break
                i += 1

        counter += 1
        report("", counter, files)

    return files


def traceBack(files, func, idx):
    if not list(func.calls):
        return False
    rv = False
    for fc in list(func.calls):
        if rv:
            return True
        f = files[fc]
        if f.func is None:
            continue
        for line in f.lines:
            if '@' in line or 'function' in line or line.startswith('//'):
                continue
            if contains(func, line) != NO:
                var = getVars(line, func.name)
                var = var[idx]
                if var in f.localArrays.keys():
                    return True
                if var in functions[f.func].inputs:
                    rv = traceBack(files, functions[f.func], functions[f.func].inputs.index(var))
    return False

def traceCall(files, func, idx):
    var = func.inputs[idx].replace('double ', '').replace('arr ', '').replace('&', '').strip()
    for line in files[func.file].lines:
        if line.startswith('//') or not line or line.startswith('function') or '@' in line:
            continue
        line = line.split('//')[0].strip()
        if contains(files[func.file].localVars[var], line) != NO:
            for ff in list(files[func.file].calls):
                if contains(functions[ff], line):

                    var2 = getVars(line, ff)
                    i = var2.index(func.inputs[idx])
                    if functions[ff].inputs[i] in files[functions[ff].file].localArrays.keys():
                        return True
                    return traceCall(files, functions[ff], i)
    return False


def thirdPass(files):
    """ Function to do the third pass over the Files objects. This will add variable instantiation
        statements to the initial instance of local variables, and comment out unused ones.

        Parameters
        ----------
        files - dict
            Dict of the Files objects

    """
    counter = 0
    # loop over the objects
    for x, f in files.items():
        report(x, counter, files)
        found = []
        # loop over the lines of code
        for i, line in enumerate(f.lines):

            if not line or line.startswith('//') or line.startswith(GLOBAL) or '@' in line:
                continue

            if line.startswith('function'):
                theFunc = functions[f.func]
                if theFunc.ret in globalVars.keys():
                    theFunc.ret = None
                    theFunc.rtype = 'void'

                continue

            for var, val in f.localVars.items():
                #    if contains(val, line) == STARTSWITH:
                # if this is the first time this local variable is encountered
                if contains(val, line) == STARTSWITH:
                    if var in found:
                        break
                    # if instantiating an array
                    if var in f.localArrays.keys():
                        if 'zeros' in line:
                            f.lines[i] = f"arr {var} = zeros({f.localArrays[var].length});"
                        found.append(var)
                        break
                    # if the variable is used in the code
                    if val.count > 0:
                        line = "double " + line
                        # if it is a constant
                        if val.const:
                            line = "const " + line

                    elif ' = ' in line and 'function' in line:
                        temp = line.split('=')
                        # if this is the function definition then add the myVars.
                        for name, func in functions.items():
                            if name in temp[1]:
                                val.count = 2
                                if func.rtype is None:
                                    line = temp[1].strip()
                                else:
                                    line = func.rtype + ' ' + line
                                if func.needGlobals:
                                    if func.inputs:
                                        line = line.replace(');', ', ')
                                    func.inputs.append(GLOBAL);
                                    line = line.replace(');', f"{GLOBAL});")
                                break
                    else:
                        line = '// ' + line + '// --unused'
                    f.lines[i] = line
                    found.append(var)
                    break

        counter += 1
        report("", counter, files)


def eval2(line, localConst=[]):
    """ Function to determine if a variable assignment is to a constant value

    """
    # remove all symbols, leaving variables and numbers
    exp = pattern.sub(lambda m: rep[re.escape(m.group(0))], line)
    # remove all numbers, leaving only variables
    exp = re.sub(r'\d', '', exp)
    # loop through known constants
    exp = exp.strip()
    if exp:
        return (False, None)
    return (True, line.replace(';', ''))


def findConstants(files):
    """ Function to find constant variables

        Parameters
        ----------
        files - dict
            Dict of Files objects to loop over

    """
    # find definite constants
    # loop over global variables, looking for those that have only one assignment
    for var, val in globalVars.items():
        if var in globalArrays.keys() or '_TIME' in var:
            continue
        if len(val.assign) == 1:
            (val.const, val.value) = eval2(val.assign[0])
            if val.const:
                if '^' in val.value:
                    val.value = powFormat(val.value)
    counter = 0
    for x, f in files.items():
        report(x, counter, files)
        # loop over local variables looking for those with only one assignment
        for var, val in f.localVars.items():
            if len(val.assign) == 1:
                val.const = True
                f.constants.append(var)
        counter += 1
        report("", counter, files)

def write(files):
    """ Function to write out the code to cpp files

        Parameters
        ----------
        files - dict
            Dict of the Files objects to write out

    """
    funcs = []
    counter = 0
    # loop over the Files objects
    for f, val in files.items():
        print(f)
        isDrive = False
        if f in drivers.keys():
            isDrive = True
        haveODE = False
        report(f, counter, files)
        theFunc = None
        sz = False
        o = open(f"test/{f.replace('.m', '.cpp')}", 'w')
        ind = 0
        if f == "SimEphotosynthesis.m":
            o.write("""#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <ctime>
#include <boost/array.hpp>
//#include <boost/numeric/odeint.hpp>
#include <algorithm>
#include <math.h>
""")

        # add default include
        o.write("#include \"globals.hpp\"\n\n")
        if f == "SimEphotosynthesis.m":
            o.write("""using namespace std;

arr zeros(const int length){
    return arr(length, 0.);
}
void readFile(const string &filename, map<string, string> &mapper) {
    vector<string> tempVec;
    string input;
    ifstream inputfile(filename);
    while (getline(inputfile, input)) {
        if (input.empty())
            return;
        boost::split(tempVec, input, boost::is_any_of("\t "));
        mapper.insert(pair<string, string>(tempVec[0], tempVec[1]));
    }

}

int main(int argc, char **argv){
    //std::cout << "Starting" << endl;
    map<string, string> inputs;
    //cout << "Init" << endl;
    readFile("/home/friedel/crops_in_silico/ODE-FBA/ePhotosynthesis/C++/InputEvn.txt", inputs);
    readFile("/home/friedel/crops_in_silico/ODE-FBA/ePhotosynthesis/C++/InputATPCost.txt", inputs);
    //cout << "Done read" << endl;
    varptr myVars;

    myVars.TestCa = stof(inputs.at("CO2"), nullptr);
    myVars.TestLi = stof(inputs.at("PAR"), nullptr);
    myVars.TestSucPath = stoi(inputs.at("SucPath"), nullptr);
    myVars.TestATPCost = stoi(inputs.at("ATPCost"), nullptr);
    TrDyna *myDyna = new TrDyna(myVars);
    //cout << "Calling" << endl;
    vector<double> ResultRate = myDyna->trDynaPS_Drive(1, 1);

    ofstream outFile("output.dat");
    outFile << myVars.TestLi << "," << ResultRate[0] << "," << ResultRate[1] << "," << ResultRate[2] << "," << ResultRate[3] << "," << ResultRate[4] << "," << ResultRate[5] << "," << ResultRate[6] << endl;
    outFile.close();
}

""")
            o.close()
            continue
        for line in val.lines:
            # write out the function definition line
            if line.startswith('function'):
                line = line.replace('function ', '')
                line = line.replace('=', ' ')
         ###       line = line[:-1] + ", varptr &myVars) {"
                if val.func in functions.keys():
                    theFunc = functions[val.func]
                    if theFunc.inputs and re.match('begin', theFunc.inputs[0], re.IGNORECASE) is not None:
                        #theFunc.ret = None
                        #theFunc.rtype = "void"
                        del theFunc.inputs[0]
                    elif theFunc.inputs and re.match('be', theFunc.inputs[0], re.IGNORECASE) is not None:
                        del theFunc.inputs[0]
                    # if the function has a return type
                    if theFunc.ret in globalVars.keys():
                        theFunc.ret = None
                        theFunc.rtype = 'void'
                    if theFunc.ret is not None:
                        if theFunc.ret in val.localArrays:
                            o.write("arr ")
                            theFunc.rtype = "arr"
                        else:
                            o.write("double ")
                            theFunc.rtype = "double"
                    else:
                        o.write("void ")
                        theFunc.rtype = 'void'
                    if val.func in modFuncs.keys():
                        o.write(f"{modFuncs[val.func]}::")
                    o.write(f"{val.func}(")
                    if val.func in mbFuncs:
                        o.write(f"""realtype t, N_Vector u, N_Vector u_dot, void *user_data) {{
realtype *{theFunc.ret} = N_VGetArrayPointer(u_dot);
realtype *{theFunc.inputs[1]} = N_VGetArrayPointer(u);
""")
                        theFunc.ret = 0
                    else:
                        for i in range(len(theFunc.inputs)):
                            # output the function arguments along with their type
                            if theFunc.inputs[i] in val.localArrays.keys():
                                theFunc.inTypes[i] = "arr &"# + theFunc.inputs[i]
                            else:
                                if traceCall(files, theFunc, i):
                                    theFunc.inTypes[i] = "arr &"# + theFunc.inputs[i]
                                    val.localArrays[theFunc.inputs[i]] = val.localVars[theFunc.inputs[i]]
                                else:
                                    theFunc.inTypes[i] = "double "# + theFunc.inputs[i]
                        x = list(zip(theFunc.inTypes, theFunc.inputs))
                        for i, q in enumerate(x):
                            x[i] = q[0] + q[1]
                        o.write(', '.join(x))
                        if theFunc.needGlobals and not isDrive:
                            if theFunc.inputs:
                                o.write(', ')
                            o.write("varptr &myVars")
                        o.write(") {\n")
                funcs.append(line)
                ind = 1
                continue
            if isDrive:
                line = line.replace(f"{GLOBAL}.", f"{GLOBAL}->")
            # write out the TIME_N lines with proper indexing
            if 'TIME_N' in line and '[' in line and not sz:
                m = re.search(r"\[myVars\.(\S+_TIME_N) - 1\]", line)
                if m is None:
                    m = re.search(r"\[myVars->(\S+_TIME_N) - 1\]", line)
                tvar = m.group(1)
                fc = []
                sc = []
                for ks, col in val.timeVars.items():
                    if col == 0:
                        fc.append(ks)
                    else:
                        sc.append(ks)
                # write out the expansion of the matrices
                if fc:
                    o.write(ind * indent + f"if (myVars.{fc[0]}.shape()[0]")
                else:
                    o.write(ind * indent + f"if (myVars.{sc[0]}.shape()[1]")
                o.write(f" < myVars.{tvar}) {{\n")
                for vv in fc:
                    o.write((ind + 1) * indent + f"myVars.{vv}.resize(boost::extents[myVars.{tvar}][{vv}_SIZE]);\n")
                for vv in sc:
                    o.write((ind + 1) * indent + f"myVars.{vv}.resize(boost::extents[{vv}_SIZE][myVars.{tvar}]);\n")
                o.write(ind * indent + "}\n\n")
                sz = True
            found = False
            # process any function calls
            for n, v in functions.items():
                if contains(v, line) != NO and 'ode15s' not in line:
                    if v.needGlobals:
                        l1 = line.find(n)
                        loc = line.find('(', l1)
                        eloc = find_balance(line, loc - 1)
                        stmt = ""
                        if v.inputs:
                            stmt += ', '
                        stmt += "myVars"
                        line = line[:eloc - 1] + stmt + line[eloc - 1:]
            # process any local variables found
            for n, v in val.localVars.items():
                if contains(v, line) == STARTSWITH and v.count == 0:
                    line = '//' + line + '  // --unused'
                    found = True
                    break
            # process any global variables found
            if not found:
                for n in val.usedGlobals:
                    v = globalVars[n]
                    if contains(v, line.replace(GLOBAL + '.', '')) == STARTSWITH:
                        if v.count == 0:
                            line = '//' + line + '  // --unused'
                        elif v.const:
                            line = line = '//' + line + '   // constant set in globals.hpp'
                        break
            # process any arrays found
            for n, v in val.localArrays.items():
                if contains(v, line) == STARTSWITH and not v.initialized:
                    if v.length < 0:
                        line = ARR + " " + line
                    else:
                        o.write(ind * indent + f"arr {n} = zeros({v.length});\n")
                    v.initialized = True
            if not line.startswith('//') and 'ode15s' in line and theFunc is not None:
                haveODE = True
                o.write(f"//{line}\n")
                temp = line.split("ode15s")
                ivar = temp[1].split(',')[3]
                o.write(f"""
    UserData *data = alloc_user_data();
    int flag;
    realtype abstol = 1e-5;
    realtype reltol = 1e-4;
    sunindextype N = {ivar}.size();
    N_Vector y;
    y = N_VNew_Serial(N);

    for (int i = 0; i < N; i++)
        NV_Ith_S(y, i) = {ivar}[i];

    void *cvode_mem = NULL;
    cvode_mem = CVodeCreate(CV_BDF);
    realtype t0 = 0;
    flag = CVodeInit(cvode_mem, {drivers[f].mbFunc}, t0, y);

    if (flag != 0)
        std::cout << "FAIL" << std::endl;
    flag = CVodeSStolerances(cvode_mem, reltol, abstol);
    if (flag != 0)
        std::cout << "FAIL" << std::endl;
    flag = CVodeSetUserData(cvode_mem, data);

    SUNMatrix A = SUNDenseMatrix(N, N);


    SUNLinearSolver LS = SUNDenseLinearSolver(y, A);

    flag = CVDlsSetLinearSolver(cvode_mem, LS, A);
    if (flag != 0)
        std::cout << "FAIL" << std::endl;
    realtype tout;
    realtype end_time = 250;
    realtype step_length = 1.;
    realtype t = 0;

    for (tout = step_length; tout <= end_time; tout += step_length)
        flag = CVode(cvode_mem, tout, y, &t, CV_NORMAL);

""")
            else:
                o.write(ind * indent + line + '\n')
        if haveODE:
            o.write("""
    N_VDestroy(y);
    CVodeFree(&cvode_mem);
    SUNLinSolFree(LS);

            """)
        if theFunc is not None and theFunc.ret is not None:
            o.write(indent + "return " + str(theFunc.ret) + ';\n')
        o.write('}\n')
        o.close()
        counter += 1
        report("", counter, files)

    # write out the global header
    o = open("test/globals.hpp", 'w')
    o.write("""#pragma once
#include <vector>
#include <boost/multi_array.hpp>
#include <boost/smart_ptr/atomic_shared_ptr.hpp>
#include <iostream>

#include <cvode/cvode.h>
#include <nvector/nvector_serial.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <cvode/cvode_direct.h>
#include <sundials/sundials_types.h>
#include <sundials/sundials_math.h>

#define NV_Ith_S(v,i) ( NV_DATA_S(v)[i] )

//static int f(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
//static int jtv(N_Vector v, N_Vector Jv, realtype t, N_Vector u, N_Vector fu, void *user_data, N_Vector tmp);
//static int check_flag(void *flagvalue, const char *funcname, int opt);

//typedef N_Vector myvec2;

struct UserData {
  std::vector < realtype > coeffs;
};

UserData* alloc_user_data() {
    UserData *data;
    data = new UserData();
    return data;
}


typedef std::vector<double> arr;
typedef boost::multi_array<double, 2> Matrix;
inline Matrix init(const int length, const int width, double value) {
    Matrix mat(boost::extents[length][width]);
    std::iota(mat.data(), mat.data() + mat.num_elements(), value);
    return mat;
}
inline Matrix ones(const int length, const int width) {
    return init(length, width, 1.);
}
inline Matrix zeros(const int length, const int width) {
    return init(length, width, 0.);
}

inline arr ones(const int length){
    return arr(length, 1.);
}

arr zeros(const int length);

    """)
    # write out the time array sizes
    keys = list(timeArraySizes.keys())
    keys.sort()
    o.write('\n')
    for k in keys:
        o.write(f"const int {k}_SIZE = {timeArraySizes[k]};\n")
    # write out the main global object
    o.write('\nstruct Variables {\n')
    bools = []
    longs = []
    double = []
    arr = []
    mtx = []
    # separate the global variables by type
    for var in globalVars.keys():
        if globalVars[var].count == 0:
            continue
        if var.endswith("_com"):
            bools.append(var)
        elif var.endswith("_N") or var.endswith("_TIME"):
            longs.append(var)
        elif var in globalMatrix.keys():
            continue
            #mtx.append(var)
        elif var in globalArrays.keys():
            continue
            #arr.append(var)
        else:
            double.append(var)
    # write out booleans
    bools.sort()
    for b in bools:
        o.write(f"    bool {b} = false;\n")
    o.write('\n')
    longs.sort()
    # write out longs
    for l in longs:
        o.write(f"    unsigned long {l} = 0;\n")
    o.write('\n')
    double.sort()
    # write out doubles
    for d in double:
        if globalVars[d].const:
            o.write(f"    double {d} = {globalVars[d].value};\n")
        else:
            o.write(f"    double {d} = 0.;\n")
    o.write('\n')

    arr.sort()
    # write out arrays
    for a in sorted(globalArrays.keys()):
        if a in globalMatrix.keys():
            continue
        if a.endswith("Ratio"):
            o.write(f"    arr {a} = ones({globalArrays[a].length});\n")
        else:
            o.write(f"    arr {a} = zeros({globalArrays[a].length});\n")
    o.write('\n')
    mtx.sort()
    for m in sorted(globalMatrix.keys()):
        o.write(f"    Matrix {m} = Matrix();\n")
    o.write('};\n')

    o.write("typedef Variables varptr;\n\n")
    # write out function signatures
    func = list(functions.keys())
    func.sort()
    for f in func:
        if f in modFuncs:
            continue
        ff = functions[f]
        x = list(zip(ff.inTypes, ff.inputs))
        for i, q in enumerate(x):
            x[i] = q[0] + q[1]

        o.write(f"{ff.rtype} {ff.name}({', '.join(x)}")
        if ff.inputs:
            o.write(", ")
        o.write(f"varptr &myVars);\n\n")
    k = sorted(drivers.keys())
    for d in k:
        x = list(zip(functions[drivers[d].function].inTypes, functions[drivers[d].function].inputs))
        for i, q in enumerate(x):
            x[i] = q[0] + q[1]

        o.write('\n')
        o.write(f"""class {drivers[d].name} {{
public:
    {drivers[d].name}(varptr *myVars) {{
        this->myVars = myVars;
    }}
    static varptr *myVars;
    {functions[drivers[d].function].rtype} {drivers[d].function}({', '.join(x)});

private:
    static int {drivers[d].mbFunc}(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
""")
        if drivers[d].iniFunc is not None:
            o.write(f"    {functions[drivers[d].iniFunc].rtype} {drivers[d].iniFunc}({', '.join(functions[drivers[d].iniFunc].inputs)});\n")
        o.write("};\n")

    o.close()



def rpt(d, ind):
    if not d:
        return f"{ind * indent}Nothing"
    st = ""
    for k, v in d.items():
        st += f"{ind * indent}{k} calls\n"
        if not v:
            st += f"{(ind + 1) * indent}Nothing\n"
        for item in v:
            st += f"{(ind + 1) * indent}{item['calls']} -> \n{rpt(item['trace'], ind+2)}"
    return st

def main():
    names = glob.glob("*.m")
    print("Gather initial data from files...")
    files = initialPass(names)
    print("\n\nLocating local variables...")
    locateVars(files)
    #tra = traceCalls(files, 'Sim_Ephotosynthesis.m')

    print("\n\nDetermining constant variables...")
    findConstants(files)
    print("\n\nConverting array and math notations...")
    files = secondPass(files)
    print("\n\nAssigning data types to local variables...")
    thirdPass(files)
    consistencyCheck(files)

    print("\n\nWriting output files...")
    write(files)
    print('\n')

if __name__ == "__main__":
    main()
