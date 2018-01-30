import os
import shutil
from glob import glob

envs = ["sonoff", "sonoff-s20", "sonoff-touch", "sonoff-touch-esp01", "sonoff-pow", "sonoff-switch",
        "sonoff-button"]

for env in envs:
    os.system("platformio run -e %s" % env)
    os.system("platformio run -e %s -t buildfs" % env)
    shutil.rmtree(os.path.abspath(os.path.join(".pioenvs", env, "FrameworkArduino")))
    shutil.rmtree(os.path.abspath(os.path.join(".pioenvs", env, "src")))
    os.remove(os.path.abspath(os.path.join(".pioenvs", env, "libFrameworkArduino.a")))
    os.remove(os.path.abspath(os.path.join(".pioenvs", env, "libFrameworkArduinoVariant.a")))

    for lib in glob(os.path.abspath(os.path.join(".pioenvs", env, "lib*"))):
         shutil.rmtree(os.path.abspath(os.path.join(".pioenvs", env, lib)))

for element in glob(os.path.abspath(os.path.join(".pioenvs", "*"))):
    if os.path.isfile(element):
        os.remove(element)

shutil.make_archive(os.path.abspath("sonoff-danimtb_%s" % os.environ['TRAVIS_TAG']),
                    'zip',
                    os.path.abspath(os.path.join(".", ".pioenvs")))