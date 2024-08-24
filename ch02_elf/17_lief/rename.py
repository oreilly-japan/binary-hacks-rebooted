import lief

libhoge = lief.parse("./libadd.so")
for lsym in libhoge.dynamic_symbols:
    if lsym.name == "add":
        lsym.name = "add_renamed"
libhoge.write("./libadd_renamed.so")
