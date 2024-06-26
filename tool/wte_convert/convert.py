from skytemple_files.graphics.wte.handler import WteHandler
from skytemple_files.graphics.wte.model import WteImageType
import sys
from PIL import Image

input_path = sys.argv[1]
output_path = sys.argv[2]

print("opening png at " + str(input_path) + " and converting it into " + str(output_path))

img = Image.open(input_path)


imgtype = None
if int(len(img.getpalette()) / 3) <= 4:
    imgtype = WteImageType.COLOR_2BPP
else:
    imgtype = WteImageType.COLOR_4BPP

wte = WteHandler.new(img, imgtype, False)

bytes = WteHandler.serialize(wte)

f = open(output_path, "wb")
f.write(bytes)
f.close()

print("wte conversion done")