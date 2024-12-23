import pathlib
import cv2

output_file = "include/loa/game/assets.h"

assets = pathlib.Path("assets")
with open(output_file, "w") as f:
    f.write("// Automatically generated\n")
    f.write("#pragma once\n")
    f.write("#include <cstdint>\n\n")

    f.write(f"namespace embedded {{\n")

    for folder in assets.iterdir():
        print(folder)

        f.write(f"namespace {folder.name} {{\n")
        for file in folder.iterdir():
            if ".ico" in file.name:
                continue
            f.write(f"constexpr uint8_t {file.stem}[] = {{\n")
            if folder.name != "graphics":
                image = cv2.imread(str(file))
            else:
                image = cv2.imread(str(file), cv2.IMREAD_UNCHANGED)
            height, width, _ = image.shape
            image_data = image.flatten().astype("uint8")

            with open(file, "rb") as resource:
                for byte in image_data:
                    f.write(f"0x{byte:02X}, ")

            f.write("\n};\n")
            f.write(f"constexpr size_t {file.stem}_size = sizeof({file.stem});\n")

            f.write(f"constexpr size_t {file.stem}_width = {width};\n")
            f.write(f"constexpr size_t {file.stem}_height = {height};\n")

        f.write("\n}\n")
    f.write("\n}\n")
