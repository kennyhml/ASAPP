from io import TextIOWrapper
import json
import pathlib
import cv2

output_file = "include/asa/game/embedded.h"
assets = pathlib.Path("assets")


def conv_files(folder: pathlib.Path, stream: TextIOWrapper):
    for file in folder.iterdir():
        if ".ico" in file.name:
            continue
        stream.write(f"inline constexpr uint8_t {file.stem}_data[] = {{\n")
        image = cv2.imread(str(file))

        height, width, _ = image.shape
        image_data = image.flatten().astype("uint8")

        for byte in image_data:
            stream.write(f"0x{byte:02X}, ")

        stream.write("\n};\n")

        stream.write(
            f"inline const cv::Mat {file.stem} = cv::Mat({height}, {width}, CV_8UC3, (uchar*){file.stem}_data);\n"
        )


def run():
    with open(output_file, "w") as f:
        f.write("// Automatically generated\n")

        f.write("#pragma once\n")
        f.write("#include <opencv2/opencv.hpp>\n")
        f.write("#include <map>\n")
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

        with open("itemdata.json", "r") as itemdata:
            json_data: dict[str, dict] = json.load(itemdata)

        json_string = json.dumps(json_data).strip()

        # Split into chunks
        chunk_size = 1024  # Adjust the size to suit your needs
        chunks = [
            json_string[i : i + chunk_size]
            for i in range(0, len(json_string), chunk_size)
        ]

        # Write the C++ code
        f.write("inline const char* embedded_json = ")
        for chunk in chunks:
            f.write(f'R"({chunk})"\n')
        f.write(";")

        # f.write(f'const char* embedded_json = R"({json.dumps(json_data).strip()})";\n')

        f.write("inline std::map<std::string, cv::Mat> item_icon_map = {")

        for k, v in json_data.items():
            path = v["icon"][:-4]
            split = path.split("\\")
            path = f"items::{split[-1]}"

            f.write("{" + f'"{k}", {path}' + "},")

        f.write("\n};")

        f.write("\n}\n")


run()
