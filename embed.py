from io import TextIOWrapper
import pathlib
import cv2

output_file = "include/asa/game/embedded.h"
assets = pathlib.Path("assets")


def conv_files(folder: pathlib.Path, stream: TextIOWrapper):
    for file in folder.iterdir():
        if ".ico" in file.name:
            continue
        stream.write(f"constexpr uint8_t {file.stem}_data[] = {{\n")
        image = cv2.imread(str(file))

        height, width, _ = image.shape
        image_data = image.flatten().astype("uint8")

        with open(file, "rb") as resource:
            for byte in image_data:
                stream.write(f"0x{byte:02X}, ")

        stream.write("\n};\n")

        stream.write(
            f"const cv::Mat {file.stem} = cv::Mat({height}, {width}, CV_8UC3, (uchar*){file.stem}_data);\n"
        )


def run():
    with open(output_file, "w") as f:
        f.write("// Automatically generated\n")

        f.write("#pragma once\n")
        f.write("#include <opencv2/opencv.hpp>")
        f.write("#include <cstdint>\n\n")

        f.write(f"namespace asa::embedded {{\n")

        for folder in assets.iterdir():
            print(folder)

            f.write(f"namespace {folder.name} {{\n")
            if folder.name == "items":
                for sub in folder.iterdir():
                    conv_files(sub, f)
            else:
                conv_files(folder, f)

            f.write("\n}\n")
        f.write("\n}\n")


run()
