from distutils.core import setup
setup(name='btcspendfrom',
      version='1.0',
      description='Command-line utility for aurumcoin "coin control"',
      author='Gavin Andresen',
      author_email='gavin@aurumcoinfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
