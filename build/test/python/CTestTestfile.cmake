# CMake generated Testfile for 
# Source directory: /home/macke241/Documents/code/Higra/test/python
# Build directory: /home/macke241/Documents/code/Higra/build/test/python
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Test_python "/usr/bin/python3" "-c" "import sys;sys.path.insert(0, '/home/macke241/Documents/code/Higra/build/test/python/../..');import unittest;result=unittest.TextTestRunner().run(unittest.defaultTestLoader.discover('/home/macke241/Documents/code/Higra/build/test/python'));
exit(0 if result.wasSuccessful() else 1)")
set_tests_properties(Test_python PROPERTIES  _BACKTRACE_TRIPLES "/home/macke241/Documents/code/Higra/test/python/CMakeLists.txt;32;add_test;/home/macke241/Documents/code/Higra/test/python/CMakeLists.txt;0;")
subdirs("resources")
subdirs("test_accumulator")
subdirs("test_algo")
subdirs("test_assessment")
subdirs("test_attribute")
subdirs("test_hierarchy")
subdirs("test_image")
subdirs("test_interop")
subdirs("test_io_utils")
subdirs("test_structure")
