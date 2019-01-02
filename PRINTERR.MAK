# make definition file for the printerr DLL

all:    printerr.dll printerr.lib

printerr.dll:   printerr.obj
    link /NOD /CO  printerr,printerr.dll,,llibcdll+os2,printerr.def

printerr.obj:   printerr.c
    cl -c -Alfu -G2s -Zip -W3 -Ox printerr.c

printerr.lib:   printerr.def
    implib printerr.lib printerr.def

