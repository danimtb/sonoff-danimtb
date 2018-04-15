import os
import shutil
from glob import glob

# Set platfomio envirnoments to build
envs = ["sonoff", "sonoff-button", "sonoff-switch", "sonoff-touch", "sonoff-pow",
        "sonoff-s20", "sonoff-touch-esp01", "ewelink-touch-double"]

# Get version of firmware from travis tag, otherwise set test-build
version = os.getenv("TRAVIS_TAG", "test-build")

# Iterate to generate firmware files and remove not needed ones
for env in envs:
    # Iterate to generate firmware files
    os.system("platformio run -e %s -t buildfs" % env)
    os.system("platformio run -e %s" % env)

    # Remove not needed files
    shutil.rmtree(os.path.abspath(os.path.join(".pioenvs", env, "FrameworkArduino")))
    shutil.rmtree(os.path.abspath(os.path.join(".pioenvs", env, "src")))
    os.remove(os.path.abspath(os.path.join(".pioenvs", env, "libFrameworkArduino.a")))
    os.remove(os.path.abspath(os.path.join(".pioenvs", env, "libFrameworkArduinoVariant.a")))

    for lib in glob(os.path.abspath(os.path.join(".pioenvs", env, "lib*"))):
         shutil.rmtree(os.path.abspath(os.path.join(".pioenvs", env, lib)))

    # Check built files exist, otherwise raise
    if (os.path.exists(os.path.abspath(os.path.join(".pioenvs", env, "firmware.bin"))) and
        os.path.exists(os.path.abspath(os.path.join(".pioenvs", env, "firmware.elf"))) and
        os.path.exists(os.path.abspath(os.path.join(".pioenvs", env, "spiffs.bin")))):
        print ("OK [%s]: All files created" % env)
    else:
        raise Exception("ERROR [%s]: Missing files" % env)

# Remove not needed external files
for element in glob(os.path.abspath(os.path.join(".pioenvs", "*"))):
    if os.path.isfile(element):
        os.remove(element)

# Create zip with firmware files for each platformio environment
shutil.make_archive(os.path.abspath("sonoff-danimtb_%s" % version),
                    'zip',
                    os.path.abspath(os.path.join(".", ".pioenvs")))
