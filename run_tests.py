import subprocess

tests = ["asmtest", "cachedTimer"]

for test in tests:
   make = ["make"]
   make.append(test + ".c")
   make.append(test)
   print "Making: " + test
   subprocess.call(make)
   print "Running: " + test
   subprocess.call(["./" + test])
