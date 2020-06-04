from os import unlink
from os.path import isfile, join
from shutil import copy
Import("pio_lib_builder")

# enable GCC ASM
if isfile(
        join(pio_lib_builder.path, "source", "asm",
             "CortexContextSwitch.s.gcc")):
    if isfile(
            join(pio_lib_builder.path, "source", "asm",
                 "CortexContextSwitch.s")):
        unlink(
            join(pio_lib_builder.path, "source", "asm",
                 "CortexContextSwitch.s"))
    copy(
        join(pio_lib_builder.path, "source", "asm",
             "CortexContextSwitch.s.gcc"),
        join(pio_lib_builder.path, "source", "asm", "CortexContextSwitch.S"))

# fix TWI API

twi_file_path = join(pio_lib_builder.path, "source", "drivers",
                     "MicroBitI2C.cpp")
if isfile(twi_file_path):
    content = ""
    with open(twi_file_path) as fp:
        content = fp.read()
    if "twi_master_init_and_clear()" in content:
        with open(twi_file_path, "w") as fp:
            fp.write(
                content.replace("twi_master_init_and_clear()",
                                "twi_master_init_and_clear(NRF_TWI1)"))
