from svgelements import SVG
from svgelements.svgelements import Path
import scipy # svgelements have some issue issuing an appropriate error if scipy is missing.
import sys

svg = SVG.parse(sys.argv[1])

ratio = (1.0/1000.0)*256.0

commands = []

#TODO: segments?

for element in list(svg.elements()):
    if isinstance(element, Path):
        x = None
        y = None
        last_valid_x = None
        last_valid_y = None
        color_rgb = element.stroke.rgb
        commands.append({
            "command": "set_color",
            "r": color_rgb >> 16,
            "g": (color_rgb >> 8) & 0xFF,
            "b": color_rgb & 0xFF
        })
        for i in range(int(element.length())):
            p = i / float(int(element.length()))
            point = element.point(p)
            x = int(int(point[0] * ratio))
            y = int(int(point[1] * ratio))
            if x == last_valid_x and y == last_valid_y:
                pass
            elif x > 254 or x < 2 or y > 254 or y < 2:
                pass
            else:
                last_valid_x = x
                last_valid_y = y
                commands.append({
                    "command": "pen_down",
                    "x": x,
                    "y": y
                })
        if last_valid_x != None and last_valid_y != None:
            commands.append({
                "command": "pen_up",
                "x": last_valid_x,
                "y": last_valid_y
            })

out = open(sys.argv[2], "wb")

for command in commands:
    if command["command"] == "pen_down":
        out.write(b"\x00")
        out.write(bytes([command["x"], command["y"], 0]))
    elif command["command"] == "pen_up":
        out.write(b"\x01")
        out.write(bytes([command["x"], command["y"], 0]))
    elif command["command"] == "set_color":
        out.write(b"\x03")
        out.write(bytes([command["r"], command["g"], command["b"]]))
    else:
        raise

out.write(b"\x02\x00\x00\x00" * 20)
            
    