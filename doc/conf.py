# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------


project = 'C++-Pricer'
html_title = 'C++Pricer'
copyright = '2023, Charles-Auguste GOURIO'
author = 'Charles-Auguste GOURIO, Grégoire Ounnoughene'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autosectionlabel',
    'sphinx_inline_tabs',
    'sphinx_last_updated_by_git',
    'sphinxcontrib.mermaid',
]
autosectionlabel_prefix_document = True

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', 'old_doc']



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'furo'
html_static_path = ['_static']

# -- Options for LaTeX output --------------------------------
# title: warning latex syntax e.g. "my\_text" escape "_"
# mermaid warning: to have mermaid on pdf add mmdc
#   https://github.com/mermaid-js/mermaid-cli


title = "C++ Project - Monte Carlo Pricing Engine"
latex_documents = [('index', 'pricer.tex', title, author, 'howto')]
latex_elements = {
  'preamble': r'\usepackage{unicode-math}',
  'papersize': 'a4paper',  # 'letterpaper' or 'a4paper'
  'pointsize': '10pt',     # global fontsize, possible values are 10pt, 11pt and 12pt
  'sphinxsetup': 'hmargin={1.5cm,1.5cm}, vmargin={2cm,2cm}',
  # 'classoptions': ',twocolumn',    # to have two columns
  'tableofcontents': '',           # To remove the TOC
  }
latex_theme = 'manual'  # two values: 'manual' to make a book, 'howto' to have an article
latex_engine = 'xelatex'


#latex_logo = '_static/images/enpc.eps'
latex_logo = '_static/images/enpc2.png'

# https://sphinx-panels.readthedocs.io/en/latest/
# sudo apt-get install texlive-extra-utils texlive-full

