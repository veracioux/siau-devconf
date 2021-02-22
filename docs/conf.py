# -- Project information -----------------------------------------------------

project = 'SIAU konfigurator uređaja'
copyright = '2021, Haris Gušić, Anes Hadžić, Džana Aldžić, Medina Gračo'
author = 'Haris Gušić, Anes Hadžić, Džana Aldžić, Medina Gračo'

# -- General configuration ---------------------------------------------------

extensions = [
        'sphinx.ext.todo', 'breathe', 'sphinx.ext.autosectionlabel',
        'sphinx_tabs.tabs'
]

default_role = 'envvar'

breathe_projects = { "dev-conf": "_build/doxygen/xml/",
        "devlib": "_intermediate/devlib/_build/doxygen/xml/" }
breathe_default_project = "dev-conf"
breathe_default_members = ('members', 'protected-members', 'undoc-members')

primary_domain = 'cpp'
highlight_language = 'cpp'

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
exclude_patterns = ['_build', 'man', 'inc', 'Thumbs.db', '.DS_Store']

todo_include_todos = True

exec(open('../devlib/docs/include.py').read())

tags.add('dev_conf')

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'

html_static_path = ['_static']
html_css_files = ['custom.css']

# -- Custom build for ReadTheDocs --------------------------------------------

import os

if os.environ.get('READTHEDOCS', False):
    import subprocess

    subprocess.call('chmod -R 777 ./', shell=True)
    subprocess.call('umask 000', shell=True)

    subprocess.call('make doxygen', shell=True)
    subprocess.call('make prepare-man', shell=True)
    subprocess.call('make mv-files', shell=True)
    subprocess.call('make devlib', shell=True)
