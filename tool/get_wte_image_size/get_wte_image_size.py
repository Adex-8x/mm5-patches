from skytemple_files.graphics.wte.handler import WteHandler
import sys

path = sys.argv[1]

print("opening wte at " + str(path))

f = open(path, "rb")
b = f.read()
f.close()

wte = WteHandler.deserialize(b)

print("image size is {}".format(hex(len(wte.image_data))))