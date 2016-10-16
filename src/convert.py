#!/bin/python

import argparse, sys, itertools, functools

# Extracts all bits from a file string
def file2bits (string):
    return "".join([bin(ord(char))[2:].zfill(8) for char in string])
        
# Reverse the string
def str_reverse (string):
    return string[::-1]

# Chunks string in equal width pieces
def chunkstring (string, length):
    return (string[i:length+i] for i in range(0, len(string), length))

# Function to change endianness of the string
def str_change_endianness (endianbit, string):
    # We split the string at multiples of bitlength and change byte order in these
    return "".join(["".join([byte for byte in chunkstring(word, 8)][::-1]) for word in chunkstring(string, endianbit)])

# We create the one complement of the string of bits
def str_complement (string):
    complement = lambda char: '1' if char == '0' else '0' if char == '1' else False
    return "".join(map(complement, string))

# Returns the crc of the input stream
def crc_part (stream, crc_start, crc_width):
    return stream[crc_start:crc_start+crc_width]

# Returns the content part of the input stream
def content_part (stream, content_start, content_end):
    if content_end == 'end':
        return stream[content_start:]
    else:
        return stream[content_start:content_end]

# Opens the filename if it is a filename, otherwise it is a stream and just returns it
class open_if_filename:
    def __init__ (self, filename, mode):
        if type(filename) is file:
            self.fhandle = filename
            self.has_opened = False
        else:
            self.fhandle = open(filename, mode)
            self.has_opened = True
            
    def __enter__ (self):
        return self.fhandle
    
    def __exit__ (self, typel, value, traceback):
        if self.has_opened:
            self.fhandle.close()

# Function composition
def fn_compose (*functions):
    return functools.reduce (lambda f, g: lambda x: g(f(x)), functions, lambda x: x)

def log_print(verb_level, *largs, **kwargs):
    if args.verbose >= verb_level:
        sys.stderr.write(*largs, **kwargs)

class AfterCRCAction (argparse.Action):
    def __call__ (self, parser, namespace, values, option_string=None):
        if values:
            setattr(namespace, self.dest, values)
        else:
            setattr(namespace, self.dest, getattr(namespace, 'crc_start') + getattr(namespace, 'crc_width'))

### ___ START OF SCRIPT ___
# The command line arguments specify which actions to take
parser = argparse.ArgumentParser(
    prog='Universal CRC compute program',
    description='Generates all type of CRC challenges from given list of files',
    epilog='Please be aware that all numbers specified are bit lengths',
    fromfile_prefix_chars='@')
parser.add_argument('--crc-start', '-Cs', nargs='?', dest='crc_start', type=int, default=0, help='number of CRC start bit (from 0). Default: 0')
parser.add_argument('--crc-width', '-Cw', nargs='?', dest='crc_width', type=int, default=16, help='width in bit of CRC. Default: 16')
parser.add_argument('--content-start', '-cs', nargs='?', dest='content_start', action=AfterCRCAction, help='start of content. Default: after CRC')
parser.add_argument('--content-end', '-ce', nargs='?', dest='content_end', default='end', help='end of content. Default: end of stream')
parser.add_argument('--files', '-f', nargs='+', dest='files', default=[sys.stdin], help='filenames to process. Default: stdin')
parser.add_argument('--out-file', '-o', nargs='?', dest='out_file', default=sys.stdout, help='output file name. Default: stdout')
parser.add_argument('--verbose', '-v', action='count', dest='verbose', default=0)
parser.add_argument('--version', '-V', action='version', version='%(prog)s 1.0')
args = parser.parse_args()

# Now we should read all the files and do all conversions
initialstrings = [];
for filename in args.files:
    with open_if_filename(filename, "rb") as f:
        thisstring = file2bits(f.read())
        log_print(2, "Stringa di input: " + thisstring + "\n") 
        initialstrings.append(thisstring)

# We define the extractor functions and we open the file
extract_crc = lambda string: crc_part(string, args.crc_start, args.crc_width)
extract_content = lambda string: content_part(string, args.content_start, args.content_end)
extract_parts = lambda string: (extract_content(string), extract_crc(string))

# We then calculate the variant of all the strings and output them in the file
# First we consider the endianness: Std End, Rev End 16, Rev End 32
# Then we extract the crc and the content parts
# Then we apply: CRC normal or reverse
#                Content normal or reverse
#                CRC standard or bit complement
endianness = [
    (lambda x: x, 'standard endianness'),
    (lambda x: str_change_endianness(16, x), 'changed endianness 16'),
    (lambda x: str_change_endianness(32, x), 'changed endianness 32')
];

# After extracting crc and content we have a list of couples: (Content, CRC)

crc_reverse = [
    (lambda (x, y): (x, y), 'crc untouched'),
    (lambda (x, y): (x, str_reverse(y)), 'crc reversed')
];

content_reverse = [
    (lambda (x, y): (x, y), 'content untouched'),
    (lambda (x, y): (str_reverse(x), y), 'content reversed')
];

crc_complement = [
    (lambda (x, y): (x, y), 'crc uncomplemented'),
    (lambda (x, y): (x, str_complement(y)), 'crc complemented')
];

# We generate all function composition of applying the given function in the order (its reverse of classical order composition)
variants = map(lambda (x, y): (fn_compose(*(list(x))), ", ".join(list(y))),
               map(lambda x: zip(*x),
                   map(list, itertools.product(endianness, [(extract_parts, '')], crc_reverse, content_reverse, crc_complement))))

with open_if_filename(args.out_file, "w") as f:
    for (variant, description) in variants:
        f.write("# Begin Variant: " + description + "\n")
        for string in initialstrings:
            (lambda (cont, crc): f.write(cont + "\n" + crc + "\n"))(variant(string))
        
