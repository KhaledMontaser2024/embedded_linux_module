SUMMARY = "Install a custom Python script"
DESCRIPTION = "This recipe installs a Python script into the image"
LICENSE = "CLOSED"


SRC_URI = "file://state.py"

inherit allarch

# Specify the install location
do_install() {
    install -d ${D}/home/root
    install -m 0755 ${WORKDIR}/state.py ${D}/home/root/state.py
}

FILES:${PN} += "/home/root/state.py"
RDEPENDS:${PN} += "python3-core"



