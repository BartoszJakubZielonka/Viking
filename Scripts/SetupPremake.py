import os
import sys
import subprocess
from pathlib import Path

import Utils

from io import BytesIO
from urllib.request import urlopen

class PremakeConfiguration:
    requiredPremakeVersion = "5.0.0-alpha16"
    premakeDirectory = "./Vendor/premake/bin"

    @classmethod
    def Validate(cls):
        if (not cls.CheckPremake()):
            print("Premake not installed correctly.")
            return

    @classmethod
    def CheckPremake(cls):
        PremakeURLlist = [
            f"https://github.com/premake/premake-core/releases/download/v{cls.requiredPremakeVersion}/premake-{cls.requiredPremakeVersion}-windows.zip"
        ]

        premakePath = os.path.join(cls.premakeDirectory, "premake5.exe")
        if os.path.exists(premakePath):
            print(f"\nPremake located at {os.path.abspath(cls.premakeDirectory)}")
            return True
        else:
            packageName = f"premake-{cls.requiredPremakeVersion}-windows.zip"
            premakePath = os.path.join(cls.premakeDirectory, packageName)
            Utils.DownloadFile(PremakeURLlist, premakePath)
            print("Extracting", premakePath)
            Utils.UnzipFile(premakePath, deleteZipFile=True)
            print(f"Premake installed at {os.path.abspath(cls.premakeDirectory)}")
            return True