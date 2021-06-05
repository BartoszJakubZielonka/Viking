import platform
from pathlib import Path

from io import BytesIO
from urllib.request import urlopen
from zipfile import ZipFile

gPremakeZipUrls = {
    "Windows": 'https://github.com/premake/premake-core/releases/download/v5.0.0-alpha16/premake-5.0.0-alpha16-windows.zip'
}

gPremakelocation = 'Vendor/premake/bin'

def InstallPremake():
    platformName = platform.system()
    if (platformName != "Windows"):
        print("Viking currently supports Windows only.")
        return False

    premakeExe = ""
    if (platformName == 'Windows'):
        premakeExe = Path(f"{gPremakelocation}/premake5.exe")
    else:
        premakeExe = Path(f"{gPremakelocation}/premake5")

    if (not premakeExe.exists()):
        print('Premake not found.')
        print('   Downloading premake...')
        with urlopen(gPremakeZipUrls[platformName]) as zipresp:
            with ZipFile(BytesIO(zipresp.read())) as zfile:
                zfile.extractall(gPremakelocation)
        print(f"   Premake (v.5.0-alpha16) has been downloaded to '{gPremakelocation}'")

    return True