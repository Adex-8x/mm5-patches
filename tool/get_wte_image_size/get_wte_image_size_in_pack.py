from skytemple_files.graphics.wte.handler import WteHandler
from skytemple_files.container.bin_pack.model import BinPack
import sys

pack_path = sys.argv[1]
file_id = sys.argv[2]

print("opening wte at " + str(pack_path) + " id " + str(file_id))

f = open(pack_path, "rb")
b = f.read()
f.close()

pack = BinPack(b)

wte = WteHandler.deserialize(pack[int(file_id)])

print("image size is {}".format(hex(len(wte.image_data))))