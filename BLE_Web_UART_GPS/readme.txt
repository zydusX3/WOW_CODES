Here we create a HTML logger that will collect the GPS logs from the connect module via BLE.
WebLogger will show the device position and self position on the map. Provision are added to mark the location and various calculation based on the data received from it.
Device (Seeed Studio XIAO nRF52840 BLE) will transmit the GPS serial data over BLE UART.
WebLogger will collect the BLE UART data from the paired device. Note in this code, BLE devices are filtered on "BHG", so name the device as such.
