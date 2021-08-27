import os
import sys

project = 'devconf manual' # Displayed on top of a manpage when viewed in man

extensions = [ 'sphinx.ext.todo' ]

# Provides the function get_description to load descriptions for man pages
sys.path.insert(0, os.path.dirname(__file__))
from man_descriptions import *

man_pages = [
        (
            'devconf', # source file (no extension)
            'devconf', # output file (under output dir)
            man_descriptions['devconf'],  # description
            'SIAU tim 1', # author
            1, # section
        )
]

