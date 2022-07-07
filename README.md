# Radiohead_testing_v4

Radiohead library implementation for the RMF95 radio module, specifically on the Sparkfun Pro RF SAMD21 board. In order for this code to work, we must interface it with the Radiohead Arduino Library as well as PlatformIO

INTERFACING INSTRUCTIONS:
1. Setup PlatformIO for VSCode, following the instruction [here](https://www.youtube.com/watch?v=JmvMvIphMnY). Though this is a bit of a long video, I recommend watching it in its entirely tp fully understand the nuance of PlatformIO.
2. Once PlatformIO is set up, navigate to to desired location for the repository and clone the Github repository as usual. Board information for the Sparkfun Pro RF is included in the repository.
3. Once the repository is cloned, open it in VSCode. After a few seconds, VSCode should automatically add two folders: .pio and .vscode. We will leave the .vscode folder alone but we must make changes to the .pio folder to include the Radiohead library.
4. At this stage, the .pio folder should contain a folder called build, which in turn contains a folder called sparkfun_samd21_proRF as well as a project.checksum file.
5. Add a folder called libdeps to the .pio folder.
6. Within the libdeps folder, add another folder called sparkfun_samd21_proRF.
7. Within this new sparkfun_samd21_proRF folder, we will add both the Radiohead library itself and an integrity.dat file. 
8. Navigate to the correct Radiohead version's Github page. For me it was [this one](https://github.com/mcauser/RadioHead) but the most current one can be found [here](http://www.airspayce.com/mikem/arduino/RadioHead/) by downloading the first link (which should be a link to download a zip file) on this page.
9. Either through the terminal or by dragging the file from your folder, insert the Radiohead library into the build>libdeps>sparkfun_samd21_proRF folder.
10. That's it! Any files in src that had undefined called to header files should now be resolved.

