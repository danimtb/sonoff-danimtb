import os

os.system("platformio run -e sonoff")
os.system("platformio run -e sonoff -t buildfs")

os.system("platformio run -e sonoff-s20")
os.system("platformio run -e sonoff-s20 -t buildfs")

os.system("platformio run -e sonoff-touch")
os.system("platformio run -e sonoff-touch -t buildfs")

os.system("platformio run -e sonoff-touch-esp01")
os.system("platformio run -e sonoff-touch-esp01 -t buildfs")

os.system("platformio run -e sonoff-pow")
os.system("platformio run -e sonoff-pow -t buildfs")

os.system("platformio run -e sonoff-switch")
os.system("platformio run -e sonoff-switch -t buildfs")

os.system("platformio run -e sonoff-button")
os.system("platformio run -e sonoff-button -t buildfs")